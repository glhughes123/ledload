#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cpu_load.hpp"
#include "digits.h"
#include "display.hpp"
#include "gpio.h"
#include "util.h"

#define DEFAULT_PORT 2020

#define NUM_CPUS 56
#define ROW_CPUS 28

#define UPDATE_DELAY_US (100 * 1000)

static int coremap[4][7] =
{
    {0, 0, 0, 0, 0, 1, 1},
    {0, 0, 0, 0, 1, 1, 1},
    {0, 0, 0, 1, 1, 1, 1},
    {0, 0, 1, 1, 1, 1, 1},
};

enum display_kind
{
    dk_none,
    dk_console,
    dk_led
};

void display_test_random(cpu_load_reader *preader, remote_cpu_load_writer *pwriter, display *pdisplay, int delay);
void display_test_ants(cpu_load_reader *preader, remote_cpu_load_writer *pwriter, display *pdisplay, int delay);
void display_test_digits(cpu_load_reader *preader, remote_cpu_load_writer *pwriter, display *pdisplay, int delay);

void display_load(cpu_load_reader *preader, remote_cpu_load_writer *pwriter, display *pdisplay, int delay);

int main(int argc, char **argv)
{
    bool local = true;
    display_kind dk_display = dk_console;
    char *pszserver = NULL;
    int port = DEFAULT_PORT;
    void (*pfn_display)(cpu_load_reader *preader, remote_cpu_load_writer *pwriter, display *pdisplay, int delay) = display_load;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp("-s", argv[i]) == 0)
        {
            local = false;
            continue;
        }

        if (strcmp("-l", argv[i]) == 0)
        {
            dk_display = dk_led;
            continue;
        }

        if (strcmp("-c", argv[i]) == 0)
        {
            if (++i >= argc)
            {
                return -1;
            }
            dk_display = dk_none;
            pszserver = argv[i];
            continue;
        }

        if (strcmp("-p", argv[i]) == 0)
        {
            if (++i >= argc)
            {
                return -1;
            }
            port = atoi(argv[i]);
            continue;
        }

        if (strcmp("-tr", argv[i]) == 0)
        {
            pfn_display = display_test_random;
            continue;
        }

        if (strcmp("-ta", argv[i]) == 0)
        {
            pfn_display = display_test_ants;
            continue;
        }

        if (strcmp("-td", argv[i]) == 0)
        {
            pfn_display = display_test_digits;
            continue;
        }

        return -1;
    }

    int delay = UPDATE_DELAY_US;
    cpu_load_reader *preader = NULL;
    if (local)
    {
        preader = new local_cpu_load_reader();
    }
    else
    {
        delay = 0; // NOTE: timing driven off of remote end
        preader = new remote_cpu_load_reader(port);
    }

    remote_cpu_load_writer *pwriter = NULL;
    if (pszserver != NULL)
    {
        pwriter = new remote_cpu_load_writer(pszserver, port);
    }

    display *pdisplay = NULL;
    if (dk_display == dk_console)
    {
        pdisplay = new console_display(13 * 8, 8);
    }
    else if (dk_display == dk_led)
    {
        led_segment rgsegments[13];
        for (int i = 0; i < 5; i++) { rgsegments[i].cs_pin = GPIO_PIN_25; }
        for (int i = 5; i < 9; i++) { rgsegments[i].cs_pin = GPIO_PIN_24; }
        for (int i = 9; i < 13; i++) { rgsegments[i].cs_pin = GPIO_PIN_23; }
        pdisplay = new led_display(13, rgsegments);
    }

    srand(time(NULL));

    (*pfn_display)(preader, pwriter, pdisplay, delay);

    delete preader;
    delete pwriter;
    delete pdisplay;

    return 0;
}

void display_test_random(cpu_load_reader *preader, remote_cpu_load_writer *pwriter, display *pdisplay, int delay)
{
    srand(0); // NOTE: for stable sequence
    while (true)
    {
        for (int y = 0; y < pdisplay->height; y++)
        {
            for (int x = 0; x < pdisplay->width; x++)
            {
                pdisplay->pbuffer[y * pdisplay->width + x] = rand() % 2;
            }
        }
        pdisplay->write_buffer();
    }
}

void display_test_ants(cpu_load_reader *preader, remote_cpu_load_writer *pwriter, display *pdisplay, int delay)
{
    while (true)
    {
        time_t t = time(NULL);
        for (int y = 0; y < pdisplay->height; y++)
        {
            for (int x = 0; x < pdisplay->width; x++)
            {
                pdisplay->pbuffer[y * pdisplay->width + x] = (t + y + x) % 2;
            }
        }
        pdisplay->write_buffer();
    }
}

void display_test_digits(cpu_load_reader *preader, remote_cpu_load_writer *pwriter, display *pdisplay, int delay)
{
    while (true)
    {
        time_t t = time(NULL);
        for (int y = 0; y < pdisplay->height; y++)
        {
            for (int x = 0; x < pdisplay->width; x++)
            {
                int s = x / DIGIT_WIDTH;
                pdisplay->pbuffer[y * pdisplay->width + x] = (digits[(s + t) % 16][y % DIGIT_HEIGHT][x % DIGIT_WIDTH] == '*');
            }
        }
        pdisplay->write_buffer();
    }
}

void display_load(cpu_load_reader *preader, remote_cpu_load_writer *pwriter, display *pdisplay, int delay)
{
    int lupd[NUM_CPUS];
    int dcpu[NUM_CPUS];
    int cpus0[NUM_CPUS];
    int cpus1[NUM_CPUS];
    int *pcpus0 = cpus0;
    int *pcpus1 = cpus1;
    for (int i = 0; i < NUM_CPUS; i++)
    {
        lupd[i] = 1000;
        dcpu[i] = 0;
        cpus0[i] = 0;
        cpus1[i] = 0;
    }

    int init = true;
    while (true)
    {
        cpu_load load = preader->read_load();

        if (pwriter != NULL)
        {
            pwriter->write_load(load);
        }

        if (pdisplay != NULL)
        {
            if (init)
            {
                init = false;
                continue;
            }

            for (int i = 0; i < NUM_CPUS && i < load.ccpu; i++)
            {
                pcpus0[i] = load.pcpu[i] * 10;
            }

            // cpu load
            for (int ci = 0; ci < NUM_CPUS; ci++)
            {
                int thresh = pcpus0[ci];
                int thresh2 = (thresh > pcpus1[ci]) ? thresh : pcpus1[ci];

                lupd[ci] -= (thresh2 < 100) ? 100 : thresh2;
                if (lupd[ci] > 0) { continue; }
                lupd[ci] += 1000;

                /*
                  0/28 -> 0
                  1/29 -> 0
                  2/30 -> 1
                  3/31 -> 1
                */
                /*
                  0/28 -> 0
                  1/29 -> 1
                  2/30 -> 0
                  3/31 -> 1
                */
                int vc = ci / ROW_CPUS;
                int r = ci % 2;
                int c = (ci % ROW_CPUS) / 2;

                int l0 = pcpus0[ci] / 71;
                int delta = l0 - dcpu[ci];
                dcpu[ci] = l0;
                int flicker = rand() % 2;
                if (l0 == 14 && flicker)
                {
                    dcpu[ci]--;
                    delta--;
                }
                for (int d = 0; d < abs(delta); d++)
                {
                    int rnd = rand() % ROW_CPUS;
                    for (int o = 0; o < ROW_CPUS; o++)
                    {
                        int xy = (rnd + o) % ROW_CPUS;
                        int y = xy / 7;
                        int x = xy % 7;
                        if (coremap[y][x] == vc)
                        {
                            int sr = (r % 2) * 4;
                            int sc = c * 7;
                            int idx = (sr + y) * pdisplay->width + sc + x;
                            char n = (delta > 0) ? 1 : 0;
                            if (pdisplay->pbuffer[idx] != n)
                            {
                                pdisplay->pbuffer[idx] = n;
                                break;
                            }
                        }
                    }
                }
            }

            // unix clock
            for (int y = 0; y < 8; y++)
            {
                for (int x = 0; x < 4; x++)
                {
                    int oy = (y + (load.time / 16)) % 8;
                    int b = oy * 4 + x;
                    pdisplay->pbuffer[(7 - y) * pdisplay->width + pdisplay->width - 1 - x] = (load.time & (1 << b)) ? 1 : 0;
                }
            }

            pdisplay->write_buffer();
        }

        microsleep(delay);
    }
}
