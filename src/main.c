#include "defines.h"
#include <stdint.h>
#define PROGLEN 744

void send_data(char* data)
{
    volatile uint32_t* usart1_dr = (uint32_t*)(USART1 + 0x04);
    volatile uint32_t* usart1_sr = (uint32_t*)(USART1);

    for (int i = 0;  i < 1; ++i)
    {
        // check TXE=1, if transmitted
        *((char*)usart1_dr) = data[i];
        while (!(*usart1_sr & (1 << 7)))
            continue;
    }
}

char receive_data()
{
    volatile uint32_t* usart1_dr = (uint32_t*)(USART1 + 0x04);
    volatile uint32_t* usart1_sr = (uint32_t*)(USART1);
    char data_buffer;
    while (!(*usart1_sr & (1 << 5)))
        continue;
    data_buffer = *usart1_dr;
    return data_buffer;
}

void wait_busy()
{
    uint32_t *flash_sr = (uint32_t*)(FLASH_INTERFACE + 0x0C);
    while (*flash_sr & (1 << 16))
        continue;
}

void enable_write_on_flash_cr()
{
    volatile uint32_t *flash_keyr = (uint32_t*)(FLASH_KEYR);
    *flash_keyr = 0x45670123;
    *flash_keyr = 0xCDEF89AB;
    return;
}

void erase_flash()
{
    uint32_t *flash_cr = (uint32_t*)(FLASH_INTERFACE + 0x10);
    uint32_t *flash_sr = (uint32_t*)(FLASH_INTERFACE + 0x0C);
    wait_busy();
    //set SER
    SET_REG_BIT(flash_cr, 0, 1);
    //SET sector 2 to be erase
    RESET_REG_BIT(flash_cr, 0, 3);
    SET_REG_BIT(flash_cr, 0, 4);
    RESET_REG_BIT(flash_cr, 0, 5);
    RESET_REG_BIT(flash_cr, 0, 6);
    RESET_REG_BIT(flash_cr, 0, 7);
    // SET STRT bit
    SET_REG_BIT(flash_cr, 0, 16);
    wait_busy();
}

void setup_write_flash()
{
    enable_write_on_flash_cr();
    erase_flash();
}

void write_flash(char *data, int len)
{
    uint32_t *flash_cr = (uint32_t*)(FLASH_INTERFACE + 0x10);
    uint32_t *flash_sr = (uint32_t*)(FLASH_INTERFACE + 0x0C);
    enable_write_on_flash_cr();
    erase_flash();
    wait_busy();

    //SET PG (programming) bit of flash_cr also could
    SET_REG_BIT(flash_cr, 0, 0);
    char *flash = (char*)0x08008000;
    for (int i = 0; i < len; ++i)
    {
        flash[i] = data[i];
    }
    //RESET PG (programming) bit of flash_cr
    RESET_REG_BIT(flash_cr, 0, 0);
}
void receive_write_execute()
{
    // gpioA
    SET_REG_BIT(RCC, 0x30, 0);
    //RCC_APB2ENR set clock for usart1
    SET_REG_BIT(RCC, 0x44, 4);


    volatile uint32_t *pa = (uint32_t *)GPIOA;

    // GPIOA9_MODER 10 USART2_TX
    SET_REG_BIT(pa, 0x0, 19);
    RESET_REG_BIT(pa, 0x0, 18);
    // GPIOA10_MODER 10 USART2_RX
    SET_REG_BIT(pa, 0x0, 21);
    RESET_REG_BIT(pa, 0x0, 20);

    //GPIOx_AFRH, GPIOA in AF7
    SET_REG_BIT(pa, 0x24, 4);
    SET_REG_BIT(pa, 0x24, 5);
    SET_REG_BIT(pa, 0x24, 6);
    RESET_REG_BIT(pa, 0x24, 7);
    SET_REG_BIT(pa, 0x24, 8);
    SET_REG_BIT(pa, 0x24, 9);
    SET_REG_BIT(pa, 0x24, 10);
    RESET_REG_BIT(pa, 0x24, 11);

    //GPIOA_PUPDR set to pull up i.e 01
    SET_REG_BIT(pa, 0x0C, 18);
    RESET_REG_BIT(pa, 0x0C, 19);
    //GPIOA_PUPDR set to pull up i.e 01
    SET_REG_BIT(pa, 0x0C, 20);
    RESET_REG_BIT(pa, 0x0C, 21);

    volatile uint32_t *usart1 = (uint32_t *)USART1;

    RESET_REG_BIT(usart1, 0x0C, 7);
    RESET_REG_BIT(usart1, 0x0C, 8);
    *(uint32_t*)((char*)usart1 + 0x0C) = 0x00000000;

    //USART_CR1 UE to 1
    SET_REG_BIT(usart1, 0x0C, 13);
    //USART_CR1 M, word length is 8
    RESET_REG_BIT(usart1, 0x0C, 12);
    //USART_CR2 stop bit, 1 stop bit
    RESET_REG_BIT(usart1, 0x10, 12);
    RESET_REG_BIT(usart1, 0x10, 13);
    //USART_CR3, disable MAT (multiproces connection)
    RESET_REG_BIT(usart1, 0x14, 7);

    //Baudrate de 115200 -> BRR= 4,3125, frq clock 8Mhz
    //UARTDIV = DIVMANTISSA + DIVFRACTION/16 => 4 + 5/16
    volatile uint32_t* usart1_brr = (char*)USART1 + 0x08;
    *usart1_brr = (*usart1_brr & 0xFFFF0000) | (8 << 4) | 11;


    receiver_tranmit;
    //USART_CR1 TE to 1
    char data[PROGLEN];
    for (int i = 0; i < PROGLEN; ++i)
        data[i] = receive_data();
    write_flash(data, PROGLEN);
    __asm__ __volatile__ ("b 0x8008290");
}

int main()
{
    receive_write_execute();
    return 0;
}
