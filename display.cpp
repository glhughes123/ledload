#include <memory>
#include <stdint.h>
#include "display.hpp"

display::display(int width, int height)
{
    this->width = width;
    this->height = height;
    this->pbuffer = std::unique_ptr<uint8_t[]>(new uint8_t[width * height]);
}

display::~display()
{
}
