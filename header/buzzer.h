#ifndef BUZZER_H
#define BUZZER_H

/*
*   filename: buzzer.h
*   purpose: header file for simple buzzer control
*   author: Connor Ockerse
*   date: 11/26/2025
*/

/* Dependencies */
#include <stm32f446xx.h>
#include <stdint.h>

/* Function declarations */
/**
 * @brief Initializes TIM1_CH1 on PA8 50% duty cycle
 */
void BUZZER_INIT(void);

/**
 * @brief Updates buzzer to desired frequency
 * @param frequency: frequency in Hz
 */
void update_buzzer_freq(uint32_t frequency);

#endif