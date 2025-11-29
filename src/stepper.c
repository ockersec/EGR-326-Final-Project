/*
* filename: stepper.c
* purpose: Non-Blocking Implementation of Stepper Motor driver
* author: Connor Ockerse
* date: 11/28/2025
*/

#include "stepper.h"
#include "TIM6.h"

// === PRIVATE STATE VARIABLES ===
static uint32_t steps_remaining = 0;   // How many steps left to go
static uint32_t step_delay = 0;        // How long to wait between steps
static uint32_t last_step_time = 0;    // Timestamp of the previous step
static uint8_t  current_direction = 0; // CW or CCW
static uint8_t  step_index = 0;        // Current position in the 0-1-2-3 sequence

// The 4-step sequence lookup table
static const uint8_t step_sequence[4] = {STEP1, STEP2, STEP3, STEP4};

// Helper to write pattern to GPIO
static void STEPPER_write_pattern(uint8_t pattern) {
    // Clear PB0-PB3 (upper 16 bits of BSRR) then set new pattern
    GPIOB->BSRR = (0xF << 16) | (pattern & 0xF);
}

void STEPPER_INIT(void){
    // 1. Enable GPIOB Clock
    RCC->AHB1ENR |= (1 << 1); 
    
    // 2. Configure PB0-PB3 as Outputs
    GPIOB->MODER &= ~(0xFF << 0); // Clear
    GPIOB->MODER |= 0x55;         // Set Output
    GPIOB->OSPEEDR |= 0x55;       // High Speed

}

void STEPPER_set_target(uint32_t steps, uint8_t direction, uint32_t delay_ms) {
    // Set the state variables
    steps_remaining = steps;
    current_direction = direction;
    step_delay = delay_ms;
    
    // Reset timer so movement starts immediately
    last_step_time = TIM6_get_count() - delay_ms; 
}

uint8_t STEPPER_update(void) {
    // 1. Check if we have work to do
    if (steps_remaining == 0) {
        return 0; // Idle
    }

    // 2. Check if enough time has passed (Non-Blocking Check)
    uint32_t now = TIM6_get_count();
    
    if ((now - last_step_time) >= step_delay) {
        // Time to take a step!
        
        // A. Update the Step Index (0-3)
        if (current_direction == STEP_CW) {
            step_index++;
            if (step_index > 3) step_index = 0;
        } else {
            if (step_index == 0) step_index = 3;
            else step_index--;
        }
        
        // B. Write the hardware pins
        STEPPER_write_pattern(step_sequence[step_index]);
        
        // C. Update State
        steps_remaining--;
        last_step_time = now;
    }
    
    return 1; // Still busy moving
}

void wag(void) {
    // === STEP 1: Move CW ===
    // Set target: 50 steps, CW direction, 2ms speed
    STEPPER_set_target(65, STEP_CW, 2);
    
    // "Blocking" Loop: Wait for this specific move to finish
    // We must call STEPPER_update() here so the motor actually moves!
    while(STEPPER_update() == 1) {
    }

    // === STEP 2: Move CCW ===
    // Set target: 50 steps back, CCW direction
    STEPPER_set_target(65, STEP_CCW, 2);
    
    // Wait for the return trip to finish
    while(STEPPER_update() == 1) {
    }
}