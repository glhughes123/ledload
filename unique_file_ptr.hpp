#include <memory>

#ifndef _UNIQUE_FILE_PTR_H_
#define _UNIQUE_FILE_PTR_H_

class file_ptr_deleter
{
public:
    void operator()(FILE *pfile)
    {
        if (pfile)
        {
            fclose(pfile);
        }
    }
};

typedef std::unique_ptr<FILE, file_ptr_deleter> unique_file_ptr;

#endif // _UNIQUE_FILE_PTR_H_
