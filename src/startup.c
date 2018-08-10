#include "defines.h"
#include <stdint.h>

extern uint32_t _data_loadaddr;
extern uint32_t _data;
extern uint32_t _edata;
extern uint32_t _ebss;
extern uint32_t _stack;

void entry(void) {
    uint32_t *src = &_data_loadaddr, *dst = &_data;
    while (dst < &_edata)
        *dst++ = *src++;
    while (dst < &_ebss)
        *dst++ = 0;
    main();
}

uint32_t interrupt_vector_table[] __attribute__((section(".vectors"))) = {
    (uint32_t)&_stack,
    (uint32_t)entry,
};
