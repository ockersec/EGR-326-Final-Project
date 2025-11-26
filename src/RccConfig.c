/*  Author: Jacob Clarey
 *  Date: 8/14/2023
 *  Description: A source to contain function relating to
 *  system clock configuration settings for the F446RE
 *  Notes: Using 8MHz Crystal as HSE, sets STM to run at 45 MHz
 */

#include "RccConfig.h"

/*******************************************************************************/
/*                          USER FUNCTION DEFINITIONS */
/*******************************************************************************/
void SysClockConfig(){
    RCC->CR |= 1 << 16; // enable HSE
    while (!(RCC->CR & (1 << 17)))
    {
    } // wait for HSE ready bit to set

    RCC->APB1ENR |= 1 << 28; // power enable HSE
    PWR->CR |= 3 << 14;      // set voltage regulator to scale by 1

    /* enable the Instruction Cache, Prefetch Buffer, and Data Cache */

    /* set Flash Latency to 5WS (6 CPU cycle) */
    FLASH->ACR = (1 << 8) | (1 << 9) | (1 << 10) | (5 << 0);

    /* clear and set AHB, APB1, APB2 prescalar config */
    RCC->CFGR &= ~0xFCF0U; // bit 8 and 9 are reserved/unused
    RCC->CFGR |= (AHB_PSC << 4) | (APB1_PSC << 10) | (APB2_PSC << 13);

    /* set PLL prescalar config */
    RCC->PLLCFGR = (1 << 22) | (PLL_M) | (PLL_N << 6) | (PLL_P << 16);

    RCC->CR |= 1 << 24; // enable PLL
    while (!(RCC->CR & (1 << 25)))
    {
    } // wait for PLL ready bit to set

    RCC->CFGR |= 2; // use PLLCLK as system clock

    /* wait for system clock source status to state PLLCLK in use */
    while (!(RCC->CFGR & (2 << 2)))
    {
    }
}
/*******************************************************************************/
/*******************************************************************************/

/* EOF */