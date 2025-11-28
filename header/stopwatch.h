/*
* filename: stopwatch.h
* purpose: Long-duration stopwatch using TIM2 (32-bit)
* author: Connor Ockerse
* date: 11/27/2025
*/

#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <stm32f446xx.h>
#include <stdint.h>

/**
 * @brief Initializes TIM2 as a free-running 32-bit millisecond counter.
 * @details Uses CLOCK_FREQUENCY to set 1 tick = 1 millisecond.
 */
void STOPWATCH_INIT(void);

/**
 * @brief Resets the timer to 0 and starts counting.
 */
void STOPWATCH_start(void);

/**
 * @brief Reads the current elapsed time in milliseconds.
 * @return Time in ms (0 to ~4.2 billion ms / ~49 days)
 */
uint32_t STOPWATCH_read(void);

/**
 * @brief Pauses the timer (stops counting).
 */
void STOPWATCH_stop(void);

#endif