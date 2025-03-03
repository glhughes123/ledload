#include <arpa/inet.h>
#include <errno.h>
#include <memory>
#include <netdb.h>
#include <netinet/in.h>
#include <sstream>
#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include "cpu_load.hpp"
#include "unique_addrinfo_ptr.hpp"

#define GET_BYTE(x, n) (((size_t)(x) >> (n * 8)) & 0xFF)

remote_cpu_load_writer::remote_cpu_load_writer(const char *pszaddr, int port)
{
    this->fdsock = unique_fd(socket(AF_INET, SOCK_DGRAM, 0));
    if (this->fdsock == -1)
    {
        std::ostringstream message;
        message << "unable to open socket: (" << errno << ") " << strerror(errno);
        throw std::runtime_error(message.str());
    }

    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    struct addrinfo *paddr;
    int status = getaddrinfo(pszaddr, nullptr, &hints, &paddr);
    if (status != 0)
    {
        std::ostringstream message;
        message << "unable to resolve " << pszaddr << ": (" << status << ") " << gai_strerror(status);
        throw std::runtime_error(message.str());
    }
    unique_addrinfo_ptr spaddr = unique_addrinfo_ptr(paddr);

    this->addr = 0;
    for (struct addrinfo *paddrCur = spaddr.get(); paddrCur != nullptr; paddrCur = paddrCur->ai_next)
    {
        if (paddrCur->ai_family == AF_INET)
        {
            this->addr = ((struct sockaddr_in *)paddrCur->ai_addr)->sin_addr.s_addr;
            break;
        }
    }
    if (this->addr == 0)
    {
        std::ostringstream message;
        message << "unable to resolve " << pszaddr;
        throw std::runtime_error(message.str());
    }

    this->port = port;
}

remote_cpu_load_writer::~remote_cpu_load_writer()
{
}

void remote_cpu_load_writer::write_load(cpu_load load)
{
    int cb = PACKET_HEADER_BYTES + load.ccpu;
    uint8_t rgb[cb] = {0};

    rgb[0] = GET_BYTE(load.time, 0);
    rgb[1] = GET_BYTE(load.time, 1);
    rgb[2] = GET_BYTE(load.time, 2);
    rgb[3] = GET_BYTE(load.time, 3);
    rgb[4] = (uint8_t)load.ccpu;
    for (int i = 0; i < load.ccpu; i++)
    {
        rgb[PACKET_HEADER_BYTES + i] = (uint8_t)load.pcpu[i];
    }

    struct sockaddr_in server;
    bzero((char *)&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = this->addr;
    server.sin_port = htons(this->port);

    int result = sendto(this->fdsock.get(), rgb, cb, 0, (struct sockaddr *)&server, sizeof(server));
    if (result < 0)
    {
        std::ostringstream message;
        message << "unable to open socket: (" << errno << ") " << strerror(errno);
        throw std::runtime_error(message.str());
    }
}
