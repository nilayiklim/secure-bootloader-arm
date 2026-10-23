#include "../inc/uart.h"
#include <stdint.h>
#define UART0_BASE 0x4000C000
#define UART0_TX   (*(volatile uint32_t *)(UART0_BASE + 0x00))

void uart_print(const char *str) {
    while (*str != '\0') {
        UART0_TX = (uint32_t)(*str);
        str++;
    }
}