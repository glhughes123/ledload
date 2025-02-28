#include <arpa/inet.h>
#include <time.h>
#include "unique_fd.hpp"

#ifndef _CPU_LOAD_H_
#define _CPU_LOAD_H_

#define PACKET_HEADER_BYTES 5

class cpu_load
{
public:
    time_t time;
    int ccpu;
    int *pcpu;

    cpu_load(cpu_load &load);
    cpu_load(time_t time, int ccpu);
    virtual ~cpu_load();
};

class cpu_load_reader
{
public:
    cpu_load_reader();
    virtual ~cpu_load_reader();

    virtual cpu_load read_load() = 0;
};

class local_cpu_load_reader : public cpu_load_reader
{
    int ccpu;
    long *ptotals;
    long *pbusys;

public:
    local_cpu_load_reader();
    virtual ~local_cpu_load_reader() override;

    cpu_load read_load() override;
};

class remote_cpu_load_reader : public cpu_load_reader
{
    unique_fd fdsock;

public:
    remote_cpu_load_reader(int port);
    virtual ~remote_cpu_load_reader() override;

    cpu_load read_load() override;
};

class remote_cpu_load_writer
{
    unique_fd fdsock;
    in_addr_t addr;
    int port;

public:
    remote_cpu_load_writer(const char *pszaddr, int port);
    virtual ~remote_cpu_load_writer();

    void write_load(cpu_load load);
};

#endif // _CPU_LOAD_H_
