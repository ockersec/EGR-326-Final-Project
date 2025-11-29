/*
* filename: encoder.c
* purpose: implementation of rotary encoder driver
* author: Connor Ockerse
* date: 11/26/2025
*/

#include "encoder.h"
#include "TIM6.h"

// volatile variables
volatile uint8_t encoder_button_flag = 0;
volatile uint32_t encoder_button_press_time = 0;

void ENCODER_INIT(void){
    // --- 1. ENCODER PINS (PB6, PB7) ---
    RCC->AHB1ENR |= (1 << 1);          // GPIOB Clock
    RCC->APB1ENR |= (1 << 2);          // TIM4 Clock
    
    // Configure PB6 & PB7 as AF2 (TIM4)
    GPIOB->MODER &= ~((3 << 12) | (3 << 14)); 
    GPIOB->MODER |=  ((2 << 12) | (2 << 14)); 
    GPIOB->AFR[0] &= ~((0xF << 24) | (0xF << 28)); 
    GPIOB->AFR[0] |=  ((0x2 << 24) | (0x2 << 28)); 
    
    // TIM4 Config
    TIM4->CR1 = 0;
    TIM4->SMCR |= (1 << 0); // Encoder Mode 1
    TIM4->CCMR1 |= (1 << 0) | (1 << 8); // Map TI1/TI2
    TIM4->ARR = 0xFFFF;
    TIM4->CR1 |= (1 << 0); // Enable Timer
    
    // --- 2. BUTTON PIN (PB10) ---
    // Configure PB10 as Input with Pull-Up
    GPIOB->MODER &= ~(3 << 20); // Input Mode (00)
    GPIOB->PUPDR &= ~(3 << 20); // Clear
    GPIOB->PUPDR |=  (1 << 20); // Pull-Up (01) - Button usually connects to GND
    
    // --- 3. EXTI INTERRUPT FOR BUTTON ---
    RCC->APB2ENR |= (1 << 14); // SYSCFG Clock
    
    // Map EXTI10 to Port B (0001)
    // EXTICR[2] controls EXTI 8-11. 
    // EXTI10 is bits 8-11 in that register.
    SYSCFG->EXTICR[2] &= ~(0xF << 8);
    SYSCFG->EXTICR[2] |=  (1 << 8);   // Set to Port B
    
    // Unmask EXTI10 and Set Falling Edge (Press)
    EXTI->IMR  |= (1 << 10);
    EXTI->FTSR |= (1 << 10); // Falling Edge Trigger
    
    // Enable NVIC (EXTI15_10 handles lines 10-15)
    NVIC_EnableIRQ(EXTI15_10_IRQn);
}

// IRQ handler
void EXTI15_10_IRQHandler(void) {
    // Check if interrupt came from Line 10 (PB10)
    if (EXTI->PR & (1 << 10)) {
        // Clear flag immediately
        EXTI->PR |= (1 << 10);
        
        // Set flag and record time
        encoder_button_flag = 1;
        encoder_button_press_time = TIM6_get_count();
    }
}

uint16_t ENCODER_read(void){
    // Return the raw counter value
    return (uint16_t)TIM4->CNT;
}

uint8_t ENCODER_raw_direction(void) {
    return (TIM4->CR1 & (1 << 4)) ? 1 : 0;
}


uint8_t ENCODER_debounce(void){
    if (encoder_button_flag){
        uint32_t current_time = TIM6_get_count();

        // 15 ms debounce
        if ((current_time - encoder_button_press_time) > 15){
            // check if button is still pressed
            if ((GPIOB->IDR & (1 << 10)) == 0){
                encoder_button_flag = 0;
                TIM6_reset_count();         // reset counter to 0
                return 1;   // valid button press
            }
            encoder_button_flag = 0;
        }
    }
    return 0;   // invalid button press
}