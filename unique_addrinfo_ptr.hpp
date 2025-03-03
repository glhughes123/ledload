#include <arpa/inet.h>
#include <memory>

#ifndef _UNIQUE_ADDRINFO_PTR_H_
#define _UNIQUE_ADDRINFO_PTR_H_

class addrinfo_ptr_deleter
{
public:
    void operator()(struct addrinfo *pai)
    {
        if (pai)
        {
            freeaddrinfo(pai);
        }
    }
};

typedef std::unique_ptr<struct addrinfo, addrinfo_ptr_deleter> unique_addrinfo_ptr;

#endif // _UNIQUE_ADDRINFO_PTR_H_
