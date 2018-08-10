#ifndef DEFINE_H
#define DEFINE_H

#define FLASH_INTERFACE 0x40023C00
#define FLASH_KEYR 0x40023C04

#define RCC (uint32_t*)0x40023800
#define GPIOA 0x40020000
#define USART1 0x40011000
#define USART2 0x40004400
#define GPIOA_IDR (0x40020000 + 0x10)
#define GPIOG_MODER (0x40021800)
#define GPIOG_ODR (0x40021800 + 0x14)

#define SET_REG_BIT(BASE, OFFSET, SHIFT) *((volatile uint32_t*)((char*)BASE + OFFSET)) |= 1 << SHIFT
#define RESET_REG_BIT(BASE, OFFSET, SHIFT) *((volatile uint32_t*)((char*)BASE + OFFSET)) &= ~(1 << SHIFT)

#define set_transmitter  SET_REG_BIT(USART1, 0x0C, 3); \
                         RESET_REG_BIT(USART1, 0x0C, 2)

#define set_receiver     SET_REG_BIT(USART1, 0x0C, 2); \
                         RESET_REG_BIT(USART1, 0x0C, 3)

#define receiver_tranmit SET_REG_BIT(USART1, 0x0C, 2); \
                         SET_REG_BIT(USART1, 0x0C, 3)

#endif
