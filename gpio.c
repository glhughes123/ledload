#include "gpio.h"

void gpio_set_func(volatile void *pgpio, int pin, uint8_t mode)
{
    __sync_synchronize();
    int iw = pin / 10;
    int ib = (pin % 10) * 3;
    volatile uint32_t *pwgpio = (uint32_t *)((uint8_t *)pgpio + GPIO_SETFUNC_OFFSET);
    uint32_t w = pwgpio[iw];
    uint32_t nw = (w & ~(0b111 << ib)) | ((mode & 0b111) << ib);
    pwgpio[iw] = (w & ~(0b111 << ib)) | ((mode & 0b111) << ib);
    __sync_synchronize();

#ifdef DEBUG
    printf("pgpio: 0x%08x offset: 0x%08x &pwgpio[iw]: 0x%08x old: 0x%08x new: 0x%08x\n", pgpio, GPIO_SETFUNC_OFFSET, &pwgpio[iw], w, pwgpio[iw]);
#endif
}

void gpio_set(volatile void *pgpio, int pin)
{
    __sync_synchronize();
    int iw = pin / 32;
    int ib = pin % 32;
    volatile uint32_t *pwgpio = (uint32_t *)((uint8_t *)pgpio + GPIO_SET_OFFSET);
    pwgpio[iw] = 1 << ib;
    __sync_synchronize();

#ifdef DEBUG
    printf("pgpio: 0x%08x offset: 0x%08x &pwgpio[iw]: 0x%08x new: 0x%08x\n", pgpio, GPIO_SET_OFFSET, &pwgpio[iw], 1 << ib);
#endif
}

void gpio_reset(volatile void *pgpio, int pin)
{
    __sync_synchronize();
    int iw = pin / 32;
    int ib = pin % 32;
    volatile uint32_t *pwgpio = (uint32_t *)((uint8_t *)pgpio + GPIO_RESET_OFFSET);
    pwgpio[iw] = 1 << ib;
    __sync_synchronize();

#ifdef DEBUG
    printf("pgpio: 0x%08x offset: 0x%08x &pwgpio[iw]: 0x%08x new: 0x%08x\n", pgpio, GPIO_RESET_OFFSET, &pwgpio[iw], 1 << ib);
#endif
}
