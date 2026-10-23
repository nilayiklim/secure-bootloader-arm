#include <stdint.h>

extern uint32_t _estack; 
extern int main(void);


void Reset_Handler(void) {
    main();
    while(1);
}

__attribute__((used, section(".isr_vector")))
void (*const g_pfnVectors[])(void) = {
    (void (*)(void))(&_estack), 
    Reset_Handler,             
};