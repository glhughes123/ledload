#include <memory>
#include <stdint.h>
#include <stdlib.h>
#include "Display.hpp"

Display::Display(int width, int height)
{
    this->width = width;
    this->height = height;
    this->pbuffer = std::unique_ptr<uint8_t[]>(new uint8_t[width * height]);
}

Display::~Display()
{
}
