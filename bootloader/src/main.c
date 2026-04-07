

//typedef unsigned int uint32_t;
//typedef unsigned short uint16_t;
//typedef unsigned char uint8_t;
//

#include <stdint.h>

#define RCC_BASE        0x40021000 // Reset and clock control RRC
#define RCC_CR          (*(volatile uint32_t *)(RCC_BASE + 0x00))
#define RCC_CFGR        (*(volatile uint32_t *)(RCC_BASE + 0x04))


int main()
{
    //volatile uint32_t *rcc_cr = (volatile uint32_t) RCC_CR;
    RCC_CR |= (1 << 16); // Bit 16 HSEON: HSE clock enable
    
    // Check if HSE oscillator ready
    while(!((RCC_CR >> 17) & 0x01)){} // Bit 17 HSERDY: External high-speed clock ready flag
                                     
    // PLL enable
    RCC_CR |= (1 << 24); // Bit 24 PLLON: PLL enable
                         //
    // Check if PLL ready
    while(!((RCC_CR >> 25) & 0x01)){} 
    
    return 0;

}
