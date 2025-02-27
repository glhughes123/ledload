#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "CpuLoad.hpp"
#include "Display.hpp"
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

int main(int argc, char **argv)
{
    bool local = true;
    display_kind dk_display = dk_console;
    char *pszserver = NULL;
    int port = DEFAULT_PORT;
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

        return -1;
    }

    int delay = 0;
    CpuLoadReader *preader = NULL;
    if (local)
    {
        delay = UPDATE_DELAY_US;
        preader = new LocalCpuLoadReader();
    }
    else
    {
        preader = new RemoteCpuLoadReader(port);
    }

    RemoteCpuLoadWriter *pwriter = NULL;
    if (pszserver != NULL)
    {
        delay = UPDATE_DELAY_US;
        pwriter = new RemoteCpuLoadWriter(pszserver, port);
    }

    Display *pdisplay = NULL;
    if (dk_display == dk_console)
    {
        pdisplay = new ConsoleDisplay(13 * 8, 8);
    }
    else if (dk_display == dk_led)
    {
        LedSegment rgsegments[13];
        for (int i = 0; i < 5; i++) { rgsegments[i].cs_pin = GPIO_PIN_25; }
        for (int i = 5; i < 9; i++) { rgsegments[i].cs_pin = GPIO_PIN_24; }
        for (int i = 9; i < 13; i++) { rgsegments[i].cs_pin = GPIO_PIN_23; }
        pdisplay = new LedDisplay(13, rgsegments);
    }

    srand(time(NULL));

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
        CpuLoad load = preader->ReadLoad();

        if (pwriter != NULL)
        {
            pwriter->WriteLoad(load);
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

            pdisplay->WriteBuffer();
        }

        microsleep(delay);
    }

    delete preader;
    delete pwriter;
    delete pdisplay;

    return 0;
}
