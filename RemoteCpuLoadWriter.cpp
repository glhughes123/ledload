#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#include "CpuLoad.hpp"

#define PACKET_HEADER_BYTES 5

#define GET_BYTE(x, n) (((size_t)(x) >> (n * 8)) & 0xFF)

RemoteCpuLoadWriter::RemoteCpuLoadWriter(const char *pszaddr, int port)
{
    this->fdsock = socket(AF_INET, SOCK_DGRAM, 0);
    if (this->fdsock < 0)
    {
        std::ostringstream message;
        message << "unable to open socket: (" << errno << ") " << strerror(errno);
        throw std::runtime_error(message.str());
    }

    this->addr = inet_addr(pszaddr);
    this->port = port;
}

RemoteCpuLoadWriter::~RemoteCpuLoadWriter()
{
    close(this->fdsock);
}

void RemoteCpuLoadWriter::WriteLoad(CpuLoad load)
{
    int cbbuffer = PACKET_HEADER_BYTES + load.ccpu;
    uint8_t *pbuffer = (uint8_t *)calloc(cbbuffer, sizeof(uint8_t));

    pbuffer[0] = GET_BYTE(load.time, 0);
    pbuffer[1] = GET_BYTE(load.time, 1);
    pbuffer[2] = GET_BYTE(load.time, 2);
    pbuffer[3] = GET_BYTE(load.time, 3);
    pbuffer[4] = (uint8_t)load.ccpu;
    for (int i = 0; i < load.ccpu; i++)
    {
        pbuffer[PACKET_HEADER_BYTES + i] = (uint8_t)load.pcpu[i];
    }

    struct sockaddr_in server;
    bzero((char *)&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = this->addr;
    server.sin_port = htons(this->port);

    int result = sendto(this->fdsock, pbuffer, cbbuffer, 0, (struct sockaddr *)&server, sizeof(server));
    if (result < 0)
    {
        std::ostringstream message;
        message << "unable to open socket: (" << errno << ") " << strerror(errno);
        throw std::runtime_error(message.str());
    }

    free(pbuffer);
}
