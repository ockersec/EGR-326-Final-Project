/*
* filename: photoresistor.h
* purpose: Driver for Light Dependent Resistor (LDR) on PA1
* author: Connor Ockerse
* date: 11/26/2025
* note: Depends on RccConfig.h for clock speed calculations
*/

#ifndef PHOTORESISTOR_H
#define PHOTORESISTOR_H

#include <stm32f446xx.h>
#include <stdint.h>

/**
 * @brief Initializes ADC1 on Pin PA1 (ADC1_IN1)
 * @details Automatically configures prescalers based on system clock
 */
void PHOTO_INIT(void);

/**
 * @brief Performs a single read of the light level
 * @return 12-bit value (0 = light, 4095 = dark) 
 * assuming 3.3 V -> 10k -> PA1 -> Photoresistor -> GND wiring
 */
uint16_t PHOTO_read(void);

#endif