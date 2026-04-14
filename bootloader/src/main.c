

//typedef unsigned int uint32_t;
//typedef unsigned short uint16_t;
//typedef unsigned char uint8_t;
//

#include <stdint.h>

#define RCC_BASE        0x40021000 // Reset and clock control RRC
#define RCC_CR          (*(volatile uint32_t *)(RCC_BASE + 0x00))
#define RCC_CFGR        (*(volatile uint32_t *)(RCC_BASE + 0x04))
#define RCC_APB2ENR     (*(volatile uint32_t *)(RCC_BASE + 0x18)) // APB2 peripheral clock enable register (RCC_APB2ENR)
#define RCC_APB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x1C)) // APB1 peripheral clock enable register (RCC_APB1ENR)

/* FLASH */
#define FLASH_BASE      0x40022000 // Flash memory register
#define FLASH_ACR       (*(volatile uint32_t *)(FLASH_BASE + 0x00))

// GPIOA
#define GPIOA_BASE      0x40010800 // GPIO A
#define GPIOA_CRL       (*(volatile uint32_t *)(GPIOA_BASE + 0x00))


// USART2
#define USART2_BASE     0x40004400 // USART2
#define USART2_SR       (*(volatile uint32_t *)(USART2_BASE + 0x00)) // Status register (USART_SR)
#define USART2_BRR      (*(volatile uint32_t *)(USART2_BASE + 0x08)) // Baud rate register (USART_BRR)
#define USART2_CR1      (*(volatile uint32_t *)(USART2_BASE + 0x0C)) // Control register 1 (USART_CR1)
#define USART2_DR      (*(volatile uint32_t *)(USART2_BASE + 0x04)) // Data register (USART_DR)



int main()
{

    // HSE clock enable
    RCC_CR |= (1 << 16); // 
    
    // Check if HSE oscillator ready
    while(!((RCC_CR >> 17) & 0x01)){} // Bit 17 HSERDY: External high-speed clock ready flag

    // Flash memory wait state (flash latency)
    FLASH_ACR |= (0x02 << 0);

    // Flash memory prefetch buffer
    FLASH_ACR |= (0x01 << 4);

    // Check prefetch buffer status
    //printf("Prefetch buffer status: %u\r\n", ((FLASH_ACR >> 5) & 0x01));


    // PLL Setup
    // 0. initialize
    //RCC_CFGR &= ~((0x7 << 18) | (1 << 16) | (0x7 << 8)); // 초기화
    // 1. PLL multiplication factor x9 -> 8MHz x 9 = 72MHz
    RCC_CFGR |= (0x07 << 18);
    // 2. Source: HSE  (PLL entory clock source)
    RCC_CFGR |= (0x01 << 16);
    // 3. APB1 prescale  /2 -> 72MHz / 2 = 36MHz (APB1 max 36 MHz)
    RCC_CFGR |= (0x04 << 8);
                                     
    // PLL enable
    RCC_CR |= (1 << 24); // Bit 24 PLLON: PLL enable
    // Check if PLL ready
    while(!((RCC_CR >> 25) & 0x01)){} 

    // Switch system clock to PLL
    RCC_CFGR |= (0x2 << 0);
    // Check if system clock switched
    while(((RCC_CFGR >> 2) & 0x03) != 0x02){}

    // APB2 GPIOA Clock Enable
    RCC_APB2ENR |= (0x01 << 2);
    // APB1 USART2 Clock Enable
    RCC_APB1ENR |= (0x01 << 17);

    // GPIOA Config
    GPIOA_CRL &= ~(0x0F << 8); // clear
    GPIOA_CRL |= (0x0B << 8); // Bit[11:10]: Alternate function output, Bit[9:8]: Output mode, max 50MHz

    // Set buadrate 115200
    USART2_BRR = 0x139;

    // Set USART2
    USART2_CR1 = (1 << 13) | (1 << 3);

    // Test
    while(1)
    {
        while(!(USART2_SR  & (0x01 << 7))){}
        USART2_DR = 'a';

        for(volatile int i = 0; i < 10000000; i++);
    }




    while(1){}
    
    return 0;

}
