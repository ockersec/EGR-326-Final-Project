/*
* filename: encoder.c
* purpose: implementation of rotary encoder driver
* author: Connor Ockerse
* date: 11/26/2025
*/

#include "encoder.h"

void ENCODER_INIT(void){
    // 1. Enable Clocks
    RCC->AHB1ENR |= (1 << 1);          // GPIOB Clock (Bit 1)
    RCC->APB1ENR |= (1 << 2);          // TIM4 Clock (Bit 2)
    
    // 2. Configure PB6 and PB7 as Alternate Function
    // PB6 -> TIM4_CH1
    // PB7 -> TIM4_CH2
    GPIOB->MODER &= ~((3 << 12) | (3 << 14)); // Clear bits for Pin 6 & 7
    GPIOB->MODER |=  ((2 << 12) | (2 << 14)); // Set to AF Mode (10)
    
    // Set AF2 (TIM4) for PB6 and PB7
    // AFR[0] handles pins 0-7
    GPIOB->AFR[0] &= ~((0xF << 24) | (0xF << 28)); // Clear AF bits
    GPIOB->AFR[0] |=  ((0x2 << 24) | (0x2 << 28)); // Set AF2 (0010)
    
    // 3. Configure TIM4 for Encoder Interface Mode
    // Reset defaults first
    TIM4->CR1 = 0;
    TIM4->SMCR = 0;
    TIM4->CCMR1 = 0;
    
    // SMS (Slave Mode Selection) = 011 (Encoder Mode 3)
    // Counts on both TI1 and TI2 edges (Highest resolution)
    TIM4->SMCR |= (3 << 0); 
    
    // Configure Input Capture Filters (Optional debounce)
    // IC1F and IC2F bits in CCMR1.
    // 0x0 = No filter. 0xF = Max filter.
    // We leave it at 0 for now, but you can set (0xF << 4) | (0xF << 12) if it's jittery.
    TIM4->CCMR1 |= (1 << 0); // CC1S = 01 (Map TI1 to TI1)
    TIM4->CCMR1 |= (1 << 8); // CC2S = 01 (Map TI2 to TI2)
    
    // Set Polarity (0 = Rising Edge / Non-inverted)
    // To reverse direction, change one of these to 1
    TIM4->CCER &= ~((1 << 1) | (1 << 3) | (1 << 5) | (1 << 7));
    
    // Set Auto-Reload to Max (16-bit timer)
    TIM4->ARR = 0xFFFF;
    
    // 4. Start Timer
    TIM4->CR1 |= (1 << 0); // CEN = 1
}

// raw counter valued stored in TIM4
uint16_t ENCODER_read(void){
    // Return the raw counter value
    return (uint16_t)TIM4->CNT;
}


// Returns 1 for CCW, 0 for CW
uint8_t ENCODER_raw_direction(void) {
    return (TIM4->CR1 & (1 << 4)) ? 1 : 0;
}
