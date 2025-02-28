#include <memory>
#include <stdint.h>
#include "unique_fd.hpp"
#include "unique_mmap_ptr.hpp"

class display
{
public:
    int width;
    int height;
    std::unique_ptr<uint8_t[]> pbuffer;

    display(int width, int height);
    virtual ~display();

    virtual void write_buffer() = 0;
};

class console_display : public display
{
    bool cleared;

public:
    console_display(int width, int height);
    virtual ~console_display() override;

    void write_buffer() override;
};

struct led_segment
{
    int cs_pin;
};

class led_display : public display
{
    int csegments;
    std::unique_ptr<led_segment[]> psegments;
    unique_fd fdspi;
    unique_mmap_ptr pgpio;

public:
    led_display(int segments, led_segment *psegments);
    virtual ~led_display() override;

    void write_buffer() override;

    void write_to_segment(int segment, uint8_t address, uint8_t data);
};
