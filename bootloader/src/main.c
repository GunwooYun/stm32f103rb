

//typedef unsigned int uint32_t;
//typedef unsigned short uint16_t;
//typedef unsigned char uint8_t;
//

#include <stdint.h>
#include "uart.h"

#define RCC_BASE        0x40021000 // Reset and clock control RRC
#define RCC_CR          (*(volatile uint32_t *)(RCC_BASE + 0x00))
#define RCC_CFGR        (*(volatile uint32_t *)(RCC_BASE + 0x04))
#define RCC_APB2ENR     (*(volatile uint32_t *)(RCC_BASE + 0x18)) // APB2 peripheral clock enable register (RCC_APB2ENR)
#define RCC_APB1ENR     (*(volatile uint32_t *)(RCC_BASE + 0x1C)) // APB1 peripheral clock enable register (RCC_APB1ENR)

/* FLASH */
#define FLASH_BASE      0x40022000 // Flash memory register
#define FLASH_ACR       (*(volatile uint32_t *)(FLASH_BASE + 0x00))

/* SYSTICK (Timer) */
#define SYSTICK_BASE    0xE000E010
#define STK_CTRL        (*(volatile uint32_t *)(SYSTICK_BASE + 0x00))
#define STK_LOAD        (*(volatile uint32_t *)(SYSTICK_BASE + 0x04))
#define STK_VAL         (*(volatile uint32_t *)(SYSTICK_BASE + 0x08))

volatile uint32_t cur_ticks = 0;
void SysTick_Handler(void)
{
    cur_ticks++;
}

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


    uart_init();
    usart2_write_string("USART2 Init OK");

    STK_LOAD = 72000 - 1; // 72MHz -> 1ms
    STK_VAL = 0; // Current count value
    // Bit 0: Count enable
    // Bit 1: SysTick exception request enable
    // Bit 2: Clock source -> Processor clock AHB
    STK_CTRL = 0x07;

    uint32_t prev_ticks = 0;
    uint32_t second = 0;

    // Test
    while(1)
    {
        //while(!(USART2_SR  & (0x01 << 7))){}
        //USART2_DR = 'a';

        //for(volatile int i = 0; i < 10000000; i++);
        //while(!(USART2_SR  & (0x01 << 5))){} // RXNE: Read data register not empty
        //usart2_write_char(USART2_DR & 0xFF);
        if( cur_ticks - prev_ticks >= 1000)
        {
            usart2_write_string("hello,world");
            prev_ticks = cur_ticks;

        }
        //prev_ticks = cur_ticks;
    }

    while(1){}
    
    return 0;

}
