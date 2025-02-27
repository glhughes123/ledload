#include <errno.h>
#include <netinet/in.h>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#include "CpuLoad.hpp"

#define PACKET_HEADER_BYTES 5

RemoteCpuLoadReader::RemoteCpuLoadReader(int port)
{
    this->fdsock = socket(AF_INET, SOCK_DGRAM, 0);
    if (this->fdsock < 0)
    {
        std::ostringstream message;
        message << "unable to open socket: (" << errno << ") " << strerror(errno);
        throw std::runtime_error(message.str());
    }

    struct sockaddr_in server;
    bzero((char *)&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);
    if (bind(this->fdsock, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        std::ostringstream message;
        message << "unable to bind to socket: (" << errno << ") " << strerror(errno);
        throw std::runtime_error(message.str());
    }
}

RemoteCpuLoadReader::~RemoteCpuLoadReader()
{
    close(this->fdsock);
}

CpuLoad RemoteCpuLoadReader::ReadLoad()
{
    uint8_t netbuffer[2048];

    struct sockaddr_in client;
    uint cbclient = sizeof(client);
    int status = recvfrom(this->fdsock, netbuffer, sizeof(netbuffer), 0, (struct sockaddr *)&client, &cbclient);
    if (status < 0)
    {
        std::ostringstream message;
        message << "unable to read from socket: (" << errno << ") " << strerror(errno);
        throw std::runtime_error(message.str());
    }
    else if (status < PACKET_HEADER_BYTES)
    {
        throw std::runtime_error("read invalid packet data");
    }

    time_t t = netbuffer[0] | (netbuffer[1] << 8) | (netbuffer[2] << 16) | (netbuffer[3] << 24);
    int ccpu = netbuffer[4];

    CpuLoad load = CpuLoad(t, ccpu);
    for (int i = 0; i < ccpu; i++)
    {
        load.pcpu[i] = netbuffer[PACKET_HEADER_BYTES + i];
    }

    return load;
}
