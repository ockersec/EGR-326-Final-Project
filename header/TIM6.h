/*
* filename: TIM6.h
* purpose: Basic 1ms timer driver using TIM6
* author: Connor Ockerse
* date: 11/27/2025
* note: TIM6 is an APB1 Basic Timer.
*/

#ifndef TIM6_H
#define TIM6_H

#include <stm32f446xx.h>
#include <stdint.h>

/**
 * @brief Initializes TIM6 to generate an interrupt every 1ms
 * @details Calculates Prescaler based on CLOCK_FREQUENCY from RccConfig
 */
void TIM6_INIT(void);

/**
 * @brief Blocking delay using the TIM6 counter
 * @param ms: Milliseconds to wait
 */
void TIM6_delay(uint32_t ms);

/**
 * @brief get current ms count of TIM6
 * @return millisecond count that is incremented by interrupt
 */
uint32_t TIM6_get_count(void);


/**
 * @brief reset millisecond count to prevent overflow issues
 */
void TIM6_reset_count(void);
#endif