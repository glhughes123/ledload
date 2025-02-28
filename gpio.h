#include <stdint.h>

#define GPIO_MEM_BASE  0
#define GPIO_MEM_SIZE  0x01800000

#define GPIO_SETFUNC_OFFSET 0x00
#define GPIO_SET_OFFSET     0x1C
#define GPIO_RESET_OFFSET   0x28

#define GPIO_PIN_MOSI 19
#define GPIO_PIN_MISO 21
#define GPIO_PIN_CLK  23
#define GPIO_PIN_CE0  24
#define GPIO_PIN_CE1  26

#define GPIO_PIN_23   23
#define GPIO_PIN_24   24
#define GPIO_PIN_25   25

#define GPIO_FUNC_IN   0b000
#define GPIO_FUNC_OUT  0b001
#define GPIO_FUNC_ALT0 0b100

void gpio_set_func(volatile void *pgpio, int pin, uint8_t mode);

void gpio_set(volatile void *pgpio, int pin);
void gpio_reset(volatile void *pgpio, int pin);
