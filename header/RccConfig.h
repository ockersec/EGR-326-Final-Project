/* Author: Jacob Clarey
 * Date: 8/14/2023
 * Description: A header to contain function relating to
 * system clock configuration settings for the F446RE
 * Notes: Using 8MHz Crystal as HSE
 */

#ifndef __RCCCONFIG_H__
#define __RCCCONFIG_H__

#include <stm32f446xx.h>

/********************************************************************************
Equation Sheet for Clock Sourcing:

HSE / PLL_M * PLL_N / PLL_P = SYSCLK (SysTick Clock)
SYSCLK / AHB_PSC = HCLK (AHB1ENR Clock) (180 MHz maximum)

HCLK / APB1_PSC = PCLK1 (APB1ENR Clock) (45 MHz maximum)
PCLK1 * 2 = APB1_Timer_Clocks (90 MHz maximum)

HCLK / APB2_PSC = PCLK2 (APB2ENR Clock) (90 MHz maximum)
PCLK2 * 2 = APB2_Timer_Clocks (180 MHz maximum)
********************************************************************************/

// === GLOBAL CLOCK VARIABLE ===
// Referenced by main.c, buzzer.c, usart.c, etc.
#define CLOCK_FREQUENCY 45000000U

/*******************************************************************************/
/* USER PREPROCESSOR DEFS */
/*******************************************************************************/
/*
1: 0b0xxx   0
2: 0b1000   8
4: 0b1001   9
8: 0b1010   10
16: 0b1011  11
64: 0b1100  12
128: 0b1101 13
256: 0b1110 14
512: 0b1111 15
*/
/// @brief AHB prescaler value (Divide by 1 = 0)
#define AHB_PSC 0

/*
1: 0b0xx  0
2: 0b100  4
4: 0b101  5
8: 0b110  6
16: 0b111 7
*/
/// @brief APB1 prescaler value (Divide by 1 = 0)
// 45 MHz is safe for APB1, so we don't need to divide
#define APB1_PSC 0

/// @brief APB2 prescaler value (Divide by 1 = 0)
#define APB2_PSC 0

/*
2: 0b00 0
4: 0b01 1
6: 0b10 2
8: 0b11 3
*/
/// @brief PLL_P division value
// We use 3 (Binary 11) to divide by 8. 
// Calculation: 360MHz / 8 = 45MHz
#define PLL_P 3

/* direct conversion for these two values */
/// @brief PLL_M division value
// 8MHz / 4 = 2MHz Input
#define PLL_M 4

/// @brief PLL_N multiplier value
// 2MHz * 180 = 360MHz VCO Output
#define PLL_N 180

/*******************************************************************************/
/*******************************************************************************/

/*******************************************************************************/
/* USER FUNCTION PROTOTYPE */
/*******************************************************************************/
/// @brief Enables the SysClock to be at desired frequency.
/// @brief Without calling this, the default is 16MHz using
/// @brief the internal RC oscillator.
void SysClockConfig(void);
/*******************************************************************************/
/*******************************************************************************/

#endif //__RCCCONFIG_H__

/* EOF */