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

RemoteCpuLoadReader::RemoteCpuLoadReader(int port)
{
    this->fdsock = unique_fd(socket(AF_INET, SOCK_DGRAM, 0));
    if (this->fdsock == -1)
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
    if (bind(this->fdsock.get(), (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        std::ostringstream message;
        message << "unable to bind to socket: (" << errno << ") " << strerror(errno);
        throw std::runtime_error(message.str());
    }
}

RemoteCpuLoadReader::~RemoteCpuLoadReader()
{
}

CpuLoad RemoteCpuLoadReader::ReadLoad()
{
    uint8_t rgb[2048];

    struct sockaddr_in client;
    uint cbclient = sizeof(client);
    int status = recvfrom(this->fdsock.get(), rgb, sizeof(rgb), 0, (struct sockaddr *)&client, &cbclient);
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

    time_t t = rgb[0] | (rgb[1] << 8) | (rgb[2] << 16) | (rgb[3] << 24);
    int ccpu = rgb[4];

    CpuLoad load = CpuLoad(t, ccpu);
    for (int i = 0; i < ccpu; i++)
    {
        load.pcpu[i] = rgb[PACKET_HEADER_BYTES + i];
    }

    return load;
}
