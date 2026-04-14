#include "uart.h"

void uart_init(void)
{
    // GPIOA Config
    GPIOA_CRL &= ~(0x0F << 8); // clear
    GPIOA_CRL |= (0x0B << 8); // Bit[11:10]: Alternate function output, Bit[9:8]: Output mode, max 50MHz

    // Set buadrate 115200
    USART2_BRR = 0x139;

    // Set USART2
    USART2_CR1 = (1 << 13) | (1 << 3);
}

void usart2_write_char(char c)
{
    // Wait for buffer empty
    while(!(USART2_SR  & (0x01 << 7))){}

    USART2_DR = (c & 0xFF);

}

void usart2_write_string(char *str)
{
    while(*str) // until null
    {
        usart2_write_char(*str++);
    }
}
//void bl_printf(
