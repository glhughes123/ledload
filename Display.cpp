#include <stdint.h>
#include <stdlib.h>
#include "Display.hpp"

Display::Display(int width, int height)
{
    this->width = width;
    this->height = height;
    this->pbuffer = (uint8_t *)calloc(width * height, sizeof(uint8_t));
}

Display::~Display()
{
    free(this->pbuffer);
}
