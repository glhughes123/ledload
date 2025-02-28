#include <stdio.h>
#include "display.hpp"

#define ESC 27

console_display::console_display(int width, int height)
    : display(width, height)
{
    this->cleared = false;
}

console_display::~console_display()
{
}

void console_display::write_buffer()
{
    if (!this->cleared)
    {
        printf("%c[2J", ESC);
        this->cleared = true;
    }

    printf("%c[H", ESC);
    for (int y = 0; y < this->height; y++)
    {
        for (int x = 0; x < this->width; x++)
        {
            printf("%c", (this->pbuffer[y * this->width + x] == 0) ? ' ' : 'o');
        }
        printf("\n");
    }
}
