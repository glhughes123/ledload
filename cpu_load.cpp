#include <stdexcept>
#include <string.h>
#include "cpu_load.hpp"

cpu_load::cpu_load(cpu_load &load)
{
    this->time = load.time;
    this->ccpu = load.ccpu;
    this->pcpu = std::unique_ptr<int[]>(new int[this->ccpu]());
    memcpy(this->pcpu.get(), load.pcpu.get(), this->ccpu * sizeof(int));
}

cpu_load::cpu_load(time_t time, int ccpu)
{
    this->time = time;
    this->ccpu = ccpu;
    this->pcpu = std::unique_ptr<int[]>(new int[this->ccpu]());
}

cpu_load::~cpu_load()
{
}
