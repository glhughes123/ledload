#include <stddef.h>
#include <time.h>
#include "util.h"

void microsleep(int us)
{
    struct timespec duration;
    duration.tv_sec = 0;
    duration.tv_nsec = us * 1000;
    nanosleep(&duration, NULL);
}
