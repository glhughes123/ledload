#include <errno.h>
#include <stdexcept>
#include <sstream>
#include <string.h>
#include "CpuLoad.hpp"

CpuLoad::CpuLoad(CpuLoad &load)
{
    this->time = load.time;
    this->ccpu = load.ccpu;
    this->pcpu = (int *)calloc(this->ccpu, sizeof(int));
    if (this->pcpu == NULL)
    {
        throw std::runtime_error("could not allocate buffers");
    }
    memcpy(this->pcpu, load.pcpu, this->ccpu * sizeof(int));
}

CpuLoad::CpuLoad(time_t time, int ccpu)
{
    this->time = time;
    this->ccpu = ccpu;
    this->pcpu = (int *)calloc(this->ccpu, sizeof(int));
    if (this->pcpu == NULL)
    {
        throw std::runtime_error("could not allocate buffers");
    }
}

CpuLoad::~CpuLoad()
{
    free(this->pcpu);
}
