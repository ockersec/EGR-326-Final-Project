/*
* filename: buzzer.c
* purpose: implementation of simple buzzer control
* author: Connor Ockerse
* date: 11/26/2025
*/

#include "buzzer.h"
#include "RccConfig.h"

// PWM initialization for TIM1_CH1 (PA8)
void BUZZER_INIT(void){
    // 1. Enable clocks
    RCC->AHB1ENR |= (1 << 0);          // GPIOA clock
    RCC->APB2ENR |= (1 << 0);          // TIM1 clock (TIM1 is on APB2)
    
    // 2. Configure PA8 as alternate function
    GPIOA->MODER &= ~(3 << (8 * 2));   // Clear mode bits for PA8
    GPIOA->MODER |= (2 << (8 * 2));    // Alternate function mode
    GPIOA->AFR[1] &= ~(0xF << ((8-8) * 4)); // Clear AF bits for PA8
    GPIOA->AFR[1] |= (1 << ((8-8) * 4)); // AF1 for TIM1
    
    // 3. Configure TIM1 for PWM
    TIM1->PSC = 0;                     // No prescaler
    TIM1->ARR = 0;                     // Auto-reload: 0 Hz initially (off)
    
    // 4. Configure Channel 1 for PWM Mode 1
    TIM1->CCMR1 &= ~(0x7 << 4);        // Clear OC1M bits
    TIM1->CCMR1 |= (6 << 4);           // PWM Mode 1 (OC1M = 110)
    TIM1->CCMR1 |= (1 << 3);           // Output compare preload enable (OC1PE)
    
    // 5. Enable output (but don't start timer yet)
    TIM1->CCER |= (1 << 0);            // Capture/Compare 1 output enable
    TIM1->BDTR |= (1 << 15);           // Main output enable (MOE) - required for TIM1
    TIM1->CR1 |= (1 << 7);             // Auto-reload preload enable (ARPE)
    
    // 6. Set initial duty cycle to 0 (buzzer off)
    TIM1->CCR1 = 0;
}

/**
 * @brief Updates buzzer to desired frequency
 * @param frequency: frequency in Hz
 */
void update_buzzer_freq(uint32_t frequency) {
    if (frequency == 0) {
        TIM1->CR1 &= ~(1 << 0);        // Counter disable
        TIM1->CCR1 = 0;                // 0% duty cycle
        return;
    }
    
    // Use the GLOBAL clock variable from RccConfig.h
    // TIM1 is on APB2, so we use APB2_CLOCK_FREQ
    uint32_t total_ticks = CLOCK_FREQUENCY / frequency;
    uint32_t prescaler = 1;            
    uint32_t period;
    
    // If total ticks exceeds 16-bit register limit (65535), use prescaler
    if (total_ticks > 65535) {
        prescaler = (total_ticks / 65535) + 1;
    }
    
    // Calculate final period (ARR)
    period = (CLOCK_FREQUENCY / (prescaler * frequency)) - 1;
    
    // Stop timer before reconfiguration
    TIM1->CR1 &= ~(1 << 0);            
    
    // Update timer configuration
    TIM1->PSC = prescaler - 1; 
    TIM1->ARR = period;
    TIM1->CCR1 = period / 2; // 50% duty cycle
    
    TIM1->BDTR |= (1 << 15);           // MOE bit
    TIM1->EGR |= (1 << 0);             // Force update of shadow registers
    TIM1->CR1 |= (1 << 0);             // Counter enable
}