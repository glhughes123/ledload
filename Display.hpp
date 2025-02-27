#include <stdint.h>

class Display
{
public:
    int width;
    int height;
    uint8_t *pbuffer;

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
    LedSegment *psegments;
    int fdspi;
    volatile void *pgpio;

public:
    LedDisplay(int segments, LedSegment *psegments);
    virtual ~LedDisplay() override;

    void WriteBuffer() override;

    void WriteToSegment(int segment, uint8_t address, uint8_t data);
};
