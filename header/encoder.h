/*
* filename: encoder.h
* purpose: Driver for Rotary Encoder using TIM4 (PB6/PB7)
* author: Connor Ockerse
* date: 11/26/2025
* * Connections:
* CLK -> PB6 (TIM4_CH1)
* DT  -> PB7 (TIM4_CH2)
* SW  -> PB10 (encoder button)
*/

#ifndef ENCODER_H
#define ENCODER_H

/* Dependencies */
#include <stm32f446xx.h>
#include <stdint.h>


/**
 * @brief Initializes TIM4 in Encoder Mode (PB6 & PB7)
 * and the button on PB10 as an interrupt
 */
void ENCODER_INIT(void);

/**
 * @brief Reads the current raw encoder count
 * @return 0 to 65535 (16-bit timer value)
 */
uint16_t ENCODER_read(void);

/**
 * @brief Knows if encoder was turned CW or CCW
 * @returns 1 for CCW, 0 for CW
 */
uint8_t ENCODER_raw_direction(void);

/**
 * @brief Simple debounce routine for the encoder that utilizes TIM6 as way to track valid press 
 * @returns 1 for valid debounce and 0 for invalid debounce 
 */
uint8_t ENCODER_debounce(void);

#endif