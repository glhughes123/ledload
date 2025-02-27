#include <errno.h>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <string.h>
#include <sys/sysinfo.h>
#include <time.h>
#include <unistd.h>
#include "CpuLoad.hpp"

LocalCpuLoadReader::LocalCpuLoadReader()
{
    this->ccpu = get_nprocs();
    this->ptotals = (long *)calloc(this->ccpu, sizeof(long));
    this->pbusys = (long *)calloc(this->ccpu, sizeof(long));
    if (this->ptotals == NULL || this->pbusys == NULL)
    {
        throw std::runtime_error("could not allocate buffers");
    }
}

LocalCpuLoadReader::~LocalCpuLoadReader()
{
    free(this->ptotals);
    free(this->pbusys);
}

CpuLoad LocalCpuLoadReader::ReadLoad()
{
    FILE *pf;
    pf = fopen("/proc/stat", "r");

    char buf[256];
    char cpu[8];
    long user;
    long nice;
    long system;
    long idle;
    long ioWait;
    long hardIrq;
    long softIrq;
    long steal;
    long guest;
    long guestNice;

    time_t t = time(NULL);
    CpuLoad cpuLoad = CpuLoad(t, this->ccpu);
    if (fgets(buf, sizeof(buf), pf) != NULL)
    {
        for (int ic = 0; ic < this->ccpu; ic++)
        {
            if (fgets(buf, sizeof(buf), pf) == NULL)
            {
                break;
            }

            int cf = sscanf(buf, "%s %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld\n", cpu, &user, &nice, &system, &idle, &ioWait, &hardIrq, &softIrq, &steal, &guest, &guestNice);
            if (cf != 11)
            {
                break;
            }
            long total = user + nice + system + idle + ioWait + hardIrq + softIrq + steal + guest + guestNice;
            long busy = total - idle;

            long lastTotal = this->ptotals[ic];
            long lastBusy = this->pbusys[ic];
            if (total == lastTotal) { total++; }

            int load = (int)(100 * (busy - lastBusy) / (total - lastTotal));
            if (load < 0) { load = 0; }
            if (load > 100) { load = 100; }
            cpuLoad.pcpu[ic] = load;

            this->ptotals[ic] = total;
            this->pbusys[ic] = total - idle;
        }
    }
    fclose(pf);
    return cpuLoad;
}
