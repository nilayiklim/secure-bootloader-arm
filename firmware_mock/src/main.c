#include <stdint.h>

#define RAM_END 0x20010000
#define UART0_BASE 0x4000C000
#define UART0_TX   (*(volatile uint32_t *)(UART0_BASE + 0x00))

void reset_handler(void);
int main(void);

__attribute__((section(".isr_vector"), used))
uint32_t *vector_table[] = {
    (uint32_t *)RAM_END,
    (uint32_t *)reset_handler,
};

void reset_handler(void) {
    main();
    while(1);
}

void app_print(const char *str) {
    while (*str != '\0') {
        UART0_TX = (uint32_t)(*str);
        str++;
    }
}

int main(void) {
    app_print("\n[FIRMWARE] Application successfully started!\n");
    app_print("[FIRMWARE] System running in secure mode...\n");
    
    volatile uint32_t counter = 0;
    while(1) {
        counter++;
    }
    
    return 0;
}