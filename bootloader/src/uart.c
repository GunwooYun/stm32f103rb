#include <stdarg.h>
#include "uart.h"

void uart_init(void)
{
    // GPIOA Config
    GPIOA_CRL &= ~(0x0F << 8); // clear
    GPIOA_CRL |= (0x0B << 8); // Bit[11:10]: Alternate function output, Bit[9:8]: Output mode, max 50MHz

    // Set buadrate 115200
    USART2_BRR = 0x139;

    // Set USART2
    USART2_CR1 = (1 << 13) | (1 << 3); // USART enable, Transmiiter enable
    USART2_CR1 |= (1 << 2); // Reciever enable
}


void uart_write_char(char c)
{
    // Wait for buffer empty
    while(!(USART2_SR  & (0x01 << 7))){}

    USART2_DR = (c & 0xFF);

}

void uart_write_string(char *str)
{
    while(*str) // until null
    {
        uart_write_char(*str++);
    }
}

void uart_write_line(char *str)
{
    uart_write_string(str);
    uart_write_char('\r');
    uart_write_char('\n');
}

void uart_write_hex(uint32_t val)
{
    char *hex_digits = "0123456789ABCDEF";
    uart_write_string("0x");

    // 32비트 숫자를 4비트씩 끊어서 8번 반복
    for (int i = 28; i >= 0; i -= 4)
    {
        uart_write_char(hex_digits[(val >> i) & 0x0F]);
    }
}

void uart_write_dec(int val)
{
    char buf[12];
    int i = 0;

    if (val == 0)
    {
        uart_write_char('0');
        return;
    }

    if (val < 0)
    {
        uart_write_char('-');
        val = -val;
    }

    // 일의 자리부터 역순으로 추출
    while (val > 0)
    {
        buf[i++] = (val % 10) + '0';
        val /= 10;
    }

    // 역순으로 저장된 배열을 다시 뒤집어서 출력
    while (i > 0)
    {
        uart_write_char(buf[--i]);
    }
}


void uart_printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    while (*format)
    {
        if (*format == '%')
        {
            format++;
            switch (*format)
            {
                case 'd':
                {
                        int i = va_arg(args, int);
                        uart_write_dec(i);
                        break;
                }
                case 'p':
                case 'x':
                {
                    uint32_t p = va_arg(args, uint32_t);
                    uart_write_hex(p);
                    break;
                }
                case 's':
                {
                    char *s = va_arg(args, char *);
                    uart_write_string(s);
                    break;
                }
                case 'c':
                {
                    char c = (char)va_arg(args, int);
                    uart_write_char(c);
                    break;
                }
                default:
                    uart_write_char('%');
                    uart_write_char(*format);
                    break;
            }
        }
        else
        {
            uart_write_char(*format);
        }
        format++;
    }

    va_end(args);
}
