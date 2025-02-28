#include <memory>
#include <stdint.h>
#include "unique_fd.hpp"
#include "unique_mmap_ptr.hpp"

class Display
{
public:
    int width;
    int height;
    std::unique_ptr<uint8_t[]> pbuffer;

    Display(int width, int height);
    virtual ~Display();

    virtual void WriteBuffer() = 0;
};

class ConsoleDisplay : public Display
{
    bool cleared;

public:
    ConsoleDisplay(int width, int height);
    virtual ~ConsoleDisplay() override;

    void WriteBuffer() override;
};

struct LedSegment
{
    int cs_pin;
};

class LedDisplay : public Display
{
    int csegments;
    std::unique_ptr<LedSegment[]> psegments;
    unique_fd fdspi;
    unique_mmap_ptr pgpio;

public:
    LedDisplay(int segments, LedSegment *psegments);
    virtual ~LedDisplay() override;

    void WriteBuffer() override;

    void WriteToSegment(int segment, uint8_t address, uint8_t data);
};
