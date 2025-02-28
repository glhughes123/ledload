#include <memory>
#include <stdexcept>
#include <sys/mman.h>

#ifndef _UNIQUE_MMAP_PTR_H_
#define _UNIQUE_MMAP_PTR_H_

class unique_mmap_ptr
{
    volatile void *pv;
    int cbsize;

public:
    unique_mmap_ptr()
    {
        this->pv = nullptr;
        this->cbsize = -1;
    }

    explicit unique_mmap_ptr(volatile void *pv, int cbsize)
    {
        this->pv = pv;
        this->cbsize = cbsize;
    }

    unique_mmap_ptr(const unique_mmap_ptr &other)
    {
        throw std::runtime_error("not supported");
    }

    virtual ~unique_mmap_ptr()
    {
        if (this->pv != nullptr)
        {
            munmap((void *)this->pv, this->cbsize);
            this->pv = nullptr;
            this->cbsize = -1;
        }
    }

    unique_mmap_ptr& operator=(const unique_mmap_ptr& other)
    {
        throw std::runtime_error("not supported");
    }

    unique_mmap_ptr& operator=(unique_mmap_ptr&& other)
    {
        if (this->pv != nullptr)
        {
            throw std::runtime_error("cannot replace valid descriptor");
        }
        this->pv = other.release(&this->cbsize);
        return *this;
    }

    bool operator==(const unique_mmap_ptr& other)
    {
        return this->pv == other.pv;
    }

    bool operator==(volatile void *pvother)
    {
        return this->pv == pvother;
    }

    bool operator!=(const unique_mmap_ptr& other)
    {
        return this->pv != other.pv;
    }

    bool operator!=(volatile void *pvother)
    {
        return this->pv != pvother;
    }

    volatile void *get()
    {
        return this->pv;
    }

    volatile void *release(int *poutcbsize)
    {
        *poutcbsize = this->cbsize;
        volatile void *oldpv = this->pv;
        this->pv = nullptr;
        this->cbsize = -1;
        return oldpv;
    }
};

#endif // _UNIQUE_MMAP_PTR_H_
