#include <arpa/inet.h>
#include <time.h>

class CpuLoad
{
public:
    time_t time;
    int ccpu;
    int *pcpu;

    CpuLoad(CpuLoad &load);
    CpuLoad(time_t time, int ccpu);
    virtual ~CpuLoad();
};

class CpuLoadReader
{
public:
    CpuLoadReader();
    virtual ~CpuLoadReader();

    virtual CpuLoad ReadLoad() = 0;
};

class LocalCpuLoadReader : public CpuLoadReader
{
    int ccpu;
    long *ptotals;
    long *pbusys;

public:
    LocalCpuLoadReader();
    virtual ~LocalCpuLoadReader() override;

    CpuLoad ReadLoad() override;
};

class RemoteCpuLoadReader : public CpuLoadReader
{
    int fdsock;

public:
    RemoteCpuLoadReader(int port);
    virtual ~RemoteCpuLoadReader() override;

    CpuLoad ReadLoad() override;
};

class RemoteCpuLoadWriter
{
    int fdsock;
    in_addr_t addr;
    int port;

public:
    RemoteCpuLoadWriter(const char *pszaddr, int port);
    virtual ~RemoteCpuLoadWriter();

    void WriteLoad(CpuLoad load);
};
