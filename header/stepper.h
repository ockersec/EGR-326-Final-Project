/*
* filename: stepper.h
* purpose: Non-Blocking Driver for 4-Phase Stepper Motor
* author: Connor Ockerse
* date: 11/28/2025
*/

#ifndef STEPPER_H
#define STEPPER_H

#include <stm32f446xx.h>
#include <stdint.h>

// Direction Definitions
#define STEP_CW  0
#define STEP_CCW 1

// Step Patterns
#define STEP1 0b1100
#define STEP2 0b0110
#define STEP3 0b0011
#define STEP4 0b1001

/**
 * @brief Initializes GPIO Port B Pins 0-3 as Outputs.
 */
void STEPPER_INIT(void);

/**
 * @brief Sets a target for the motor to move towards.
 * @details This function returns INSTANTLY. It just sets variables.
 * @param steps: How many steps to take
 * @param direction: STEP_CW or STEP_CCW
 * @param delay_ms: Speed (ms between steps)
 */
void STEPPER_set_target(uint32_t steps, uint8_t direction, uint32_t delay_ms);

/**
 * @brief The "Worker" function. Call this constantly in your main while(1) loop.
 * @details Checks the time. If enough time has passed, it takes ONE step.
 * @return 1 if motor is moving, 0 if idle/finished.
 */
uint8_t STEPPER_update(void);

/**
 * @brief simulates the movement of wagging by moving stepper motor back and forth
 */
void wag(void);

#endif