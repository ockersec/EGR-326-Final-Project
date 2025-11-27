/*
* filename: encoder.h
* purpose: Driver for Rotary Encoder using TIM4 (PB6/PB7)
* author: Connor Ockerse
* date: 11/26/2025
* * Connections:
* CLK (A) -> PB6 (TIM4_CH1)
* DT  (B) -> PB7 (TIM4_CH2)
*/

#ifndef ENCODER_H
#define ENCODER_H

#include <stm32f446xx.h>
#include <stdint.h>

/**
 * @brief Initializes TIM4 in Encoder Mode (PB6 & PB7)
 */
void ENCODER_INIT(void);

/**
 * @brief Reads the current raw encoder count
 * @return 0 to 65535 (16-bit timer value)
 */
uint16_t ENCODER_read(void);

/**
 * @brief Knows if enoder was turned CW or CCW
 * @returns 1 for CCW, 0 for CW
 */
uint8_t ENCODER_raw_direction(void);

#endif