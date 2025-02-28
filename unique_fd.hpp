#include <memory>
#include <stdexcept>
#include <unistd.h>

#ifndef _UNIQUE_FD_H_
#define _UNIQUE_FD_H_

class unique_fd
{
    const int uninitialized = -1;

    int fd;

public:
    unique_fd()
    {
        this->fd = uninitialized;
    }

    explicit unique_fd(int fd)
    {
        this->fd = fd;
    }

    unique_fd(const unique_fd &other)
    {
        throw std::runtime_error("not supported");
    }

    virtual ~unique_fd()
    {
        if (this->fd != uninitialized)
        {
            close(this->fd);
        }
    }

    unique_fd& operator=(const unique_fd& other)
    {
        throw std::runtime_error("not supported");
    }

    unique_fd& operator=(unique_fd&& other)
    {
        if (this->fd != uninitialized)
        {
            throw std::runtime_error("cannot replace valid descriptor");
        }
        this->fd = other.release();
        return *this;
    }

    bool operator==(const unique_fd& other)
    {
        return this->fd == other.fd;
    }

    bool operator==(int other)
    {
        return this->fd == other;
    }

    bool operator!=(const unique_fd& other)
    {
        return this->fd != other.fd;
    }

    bool operator!=(int other)
    {
        return this->fd != other;
    }

    int get()
    {
        return this->fd;
    }

    int release()
    {
        int oldfd = this->fd;
        this->fd = uninitialized;
        return oldfd;
    }
};

#endif // _UNIQUE_FD_H_
