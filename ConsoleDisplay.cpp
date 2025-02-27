#include <stdio.h>
#include "Display.hpp"

#define ESC 27

ConsoleDisplay::ConsoleDisplay(int width, int height)
    : Display(width, height)
{
    this->cleared = false;
}

ConsoleDisplay::~ConsoleDisplay()
{
}

void ConsoleDisplay::WriteBuffer()
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
