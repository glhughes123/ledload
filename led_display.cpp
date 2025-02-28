#include <errno.h>
#include <fcntl.h>
#include <linux/spi/spi.h>
#include <linux/spi/spidev.h>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>
#include "display.hpp"
#include "gpio.h"
#include "util.h"

#define SPI_BAUD         (1 * 1000 * 1000)
#define SPI_CS_DELAY_US  10

#define SEGMENT_WIDTH   8
#define SEGMENT_HEIGHT  8

led_display::led_display(int segments, led_segment *psegments)
    : display(segments * SEGMENT_WIDTH, SEGMENT_HEIGHT)
{
    this->csegments = segments;
    this->psegments = std::unique_ptr<led_segment[]>(new led_segment[segments]);
    memcpy(this->psegments.get(), psegments, segments * sizeof(led_segment));

    unique_fd fdgpio = unique_fd(open("/dev/gpiomem", O_RDWR | O_SYNC));
    if (fdgpio == -1)
    {
        std::ostringstream message;
        message << "unable to open /dev/gpiomem: (" << errno << ") " << strerror(errno);
        throw std::runtime_error(message.str());
    }

    this->pgpio = unique_mmap_ptr(mmap(NULL, GPIO_MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fdgpio.get(), GPIO_MEM_BASE), GPIO_MEM_SIZE);
    if (this->pgpio == MAP_FAILED)
    {
        std::ostringstream message;
        message << "unable to map /dev/gpiomem: (" << errno << ") " << strerror(errno);
        throw std::runtime_error(message.str());
    }

    gpio_set_func(this->pgpio.get(), GPIO_PIN_MOSI, GPIO_FUNC_ALT0);
    gpio_set_func(this->pgpio.get(), GPIO_PIN_MISO, GPIO_FUNC_ALT0);
    gpio_set_func(this->pgpio.get(), GPIO_PIN_CLK, GPIO_FUNC_ALT0);
    gpio_set_func(this->pgpio.get(), GPIO_PIN_CE0, GPIO_FUNC_OUT);
    gpio_set_func(this->pgpio.get(), GPIO_PIN_CE1, GPIO_FUNC_OUT);
    for (int i = 0; i < this->csegments; i++)
    {
        gpio_set_func(this->pgpio.get(), this->psegments[i].cs_pin, GPIO_FUNC_OUT);
    }

    this->fdspi = unique_fd(open("/dev/spidev0.0", O_RDWR));
    if (this->fdspi == -1)
    {
        std::ostringstream message;
        message << "unable to open /dev/spidev0.0: (" << errno << ") " << strerror(errno);
        throw std::runtime_error(message.str());
    }

    uint32_t spi_mode = SPI_MODE_0;
    uint32_t spi_bpw = 8;
    uint32_t spi_hz = SPI_BAUD;
    if (ioctl(this->fdspi.get(), SPI_IOC_WR_MODE, &spi_mode) < 0 ||
        ioctl(this->fdspi.get(), SPI_IOC_RD_MODE, &spi_mode) < 0 ||
        ioctl(this->fdspi.get(), SPI_IOC_WR_BITS_PER_WORD, &spi_bpw) < 0 ||
        ioctl(this->fdspi.get(), SPI_IOC_RD_BITS_PER_WORD, &spi_bpw) < 0 ||
        ioctl(this->fdspi.get(), SPI_IOC_WR_MAX_SPEED_HZ, &spi_hz) < 0 ||
        ioctl(this->fdspi.get(), SPI_IOC_RD_MAX_SPEED_HZ, &spi_hz) < 0)
    {
        std::ostringstream message;
        message << "unable to configure /dev/spidev0.0: (" << errno << ") " << strerror(errno);
        throw std::runtime_error(message.str());
    }

    // initalize display
    for (int i = 0; i < this->csegments; i++)
    {
        this->write_to_segment(i, 0x09, 0x00);
        this->write_to_segment(i, 0x0a, 0x00); // brightness; 0-7
        this->write_to_segment(i, 0x0b, 0x07);
        this->write_to_segment(i, 0x0c, 0x01);
        this->write_to_segment(i, 0x0f, 0x00); // test (1 = all on)
    }
    for (int i = 0; i < this->csegments; i++)
    {
        for (int y = 1; y <= SEGMENT_HEIGHT; y++)
        {
            this->write_to_segment(i, y, 0);
        }
    }
}

led_display::~led_display()
{
}

void led_display::write_buffer()
{
    uint8_t rgbtx[this->csegments * 2];

    struct spi_ioc_transfer spitfer = { 0 };
    spitfer.tx_buf = (unsigned long)rgbtx;
    spitfer.rx_buf = (unsigned long)rgbtx;
    spitfer.delay_usecs = 0;
    spitfer.speed_hz = SPI_BAUD;
    spitfer.bits_per_word = 8;
    spitfer.cs_change = 0;

    for (int y = 0; y < SEGMENT_HEIGHT; y++)
    {
        int last_segment = this->csegments - 1;
        int last_cs_pin = this->psegments[last_segment].cs_pin;
        for (int s = last_segment; s >= 0; s--)
        {
            // add this segment to the line buffer
            uint8_t line = 0;
            for (int x = 0; x < 8; x++)
            {
                if (this->pbuffer[this->width * y + SEGMENT_WIDTH * s + x] != 0)
                {
                    line |= 1 << x;
                }
            }
            rgbtx[(last_segment - s) * 2] = SEGMENT_HEIGHT - y;
            rgbtx[(last_segment - s) * 2 + 1] = line;

            // write out the line buffer when we're at the end of the chain
            if (s == 0 || this->psegments[s - 1].cs_pin != last_cs_pin)
            {
                gpio_reset(this->pgpio.get(), last_cs_pin);
                microsleep(SPI_CS_DELAY_US);

                spitfer.len = (last_segment - s + 1) * 2 * sizeof(uint8_t);
                ioctl(this->fdspi.get(), SPI_IOC_MESSAGE(1), &spitfer);

                microsleep(SPI_CS_DELAY_US);
                gpio_set(this->pgpio.get(), last_cs_pin);

                last_cs_pin = this->psegments[s - 1].cs_pin;
                last_segment = s - 1;
            }
        }
    }
}

void led_display::write_to_segment(int segment, uint8_t address, uint8_t data)
{
    // figure out the chain we're writing to
    int chain_start_cs_pin = this->psegments[segment].cs_pin;
    int chain_start_segment = segment;
    for (int i = segment; i > 0; i--)
    {
        if (this->psegments[i - 1].cs_pin != chain_start_cs_pin)
        {
            chain_start_segment = i;
            break;
        }
    }
    int segments = segment - chain_start_segment + 1;
    segment -= chain_start_segment;

    // disable chip select for the chain
    gpio_reset(this->pgpio.get(), chain_start_cs_pin);
    microsleep(SPI_CS_DELAY_US);

    // set up the transfer buffer
    uint8_t rgbtx[segments * 2];
    memset(rgbtx, 0, sizeof(uint8_t) * segments * 2);
    rgbtx[(segments - segment - 1) * 2] = address;
    rgbtx[(segments - segment - 1) * 2 + 1] = data;

    struct spi_ioc_transfer spitfer = { 0 };
    spitfer.tx_buf = (unsigned long)rgbtx;
    spitfer.rx_buf = (unsigned long)rgbtx;
    spitfer.len = sizeof(rgbtx);
    spitfer.delay_usecs = 0;
    spitfer.speed_hz = SPI_BAUD;
    spitfer.bits_per_word = 8;
    spitfer.cs_change = 0;

    // transfer data
    ioctl(this->fdspi.get(), SPI_IOC_MESSAGE(1), &spitfer);

    // enable chip select to load the data
    microsleep(SPI_CS_DELAY_US);
    gpio_set(this->pgpio.get(), chain_start_cs_pin);
}
