#include <memory>
#include <errno.h>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <string.h>
#include <sys/sysinfo.h>
#include <time.h>
#include <unistd.h>
#include "cpu_load.hpp"
#include "unique_file_ptr.hpp"

local_cpu_load_reader::local_cpu_load_reader()
{
    this->ccpu = get_nprocs();
    this->ptotals = std::unique_ptr<long[]>(new long[this->ccpu]());
    this->pbusys = std::unique_ptr<long[]>(new long[this->ccpu]());
}

local_cpu_load_reader::~local_cpu_load_reader()
{
}

cpu_load local_cpu_load_reader::read_load()
{
    unique_file_ptr pf = unique_file_ptr(fopen("/proc/stat", "r"));

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
    cpu_load cpuLoad = cpu_load(t, this->ccpu);
    if (fgets(buf, sizeof(buf), pf.get()) != NULL)
    {
        for (int ic = 0; ic < this->ccpu; ic++)
        {
            if (fgets(buf, sizeof(buf), pf.get()) == NULL)
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
    return cpuLoad;
}
