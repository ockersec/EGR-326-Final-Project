/*
* filename: sonar.h
* purpose: Driver for HC-SR04 Ultrasonic Sensor using TIM3
* author: Connor Ockerse
* date: 11/26/2025
* * Pins: PB4 (TRIG), PB5 (ECHO)
* TRIG: PB4 (TIM3 Channel 1) - Generates PWM Pulse
* ECHO: PB5 (TIM3 Channel 2) - Captures Pulse Width
*/

#ifndef SONAR_H
#define SONAR_H

#include <stm32f446xx.h>
#include <stdint.h>

/**
 * @brief Initializes TIM3 for Sonar (PWM Trig + Input Capture Echo)
 * @details Configures PB4 as PWM Output, PB5 as Capture Input
 * Uses CLOCK_FREQUENCY to automatically set 1us timer tick.
 */
void SONAR_INIT(void);

/**
 * @brief Returns the most recently calculated distance
 * @return Distance in centimeters (cm)
 */
uint16_t SONAR_get_distance(void);

#endif