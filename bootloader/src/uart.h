#include <stdint.h>

// GPIOA
#define GPIOA_BASE      0x40010800 // GPIO A
#define GPIOA_CRL       (*(volatile uint32_t *)(GPIOA_BASE + 0x00))

// USART2
#define USART2_BASE     0x40004400 // USART2
#define USART2_SR       (*(volatile uint32_t *)(USART2_BASE + 0x00)) // Status register (USART_SR)
#define USART2_BRR      (*(volatile uint32_t *)(USART2_BASE + 0x08)) // Baud rate register (USART_BRR)
#define USART2_CR1      (*(volatile uint32_t *)(USART2_BASE + 0x0C)) // Control register 1 (USART_CR1)
#define USART2_DR      (*(volatile uint32_t *)(USART2_BASE + 0x04)) // Data register (USART_DR)


void uart_init(void);

void uart_write_char(char c);
void uart_write_string(char *str);
void uart_write_line(char *str);
void uart2_write_hex(uint32_t val);
void uart_write_dec(int val);

void uart_printf(const char *format, ...);
