/*
* filename: sonar.c
* purpose: implementation of HC-SR04 driver
* author: Connor Ockerse
* date: 11/26/2025
*/

#include "sonar.h"
#include "RccConfig.h" // Needed for CLOCK_FREQUENCY

// Global variables for ISR to communicate with main
volatile uint16_t rise_time = 0;
volatile uint16_t fall_time = 0;
volatile uint16_t pulse_width = 0;
volatile uint8_t capture_edge = 0; // 0 = Looking for Rising, 1 = Looking for Falling

void SONAR_INIT(void){
    // 1. Enable Clocks
    RCC->AHB1ENR |= (1 << 1);          // GPIOB Clock (Bit 1)
    RCC->APB1ENR |= (1 << 1);          // TIM3 Clock (Bit 1)
    
    // 2. Configure Pins (PB4=Trig, PB5=Echo)
    // Set PB4 and PB5 to Alternate Function Mode (10)
    GPIOB->MODER &= ~((3 << 8) | (3 << 10)); // Clear bits
    GPIOB->MODER |=  ((2 << 8) | (2 << 10)); // Set AF mode
    
    // Set AF2 (TIM3) for both PB4 and PB5
    // AFR[0] handles pins 0-7. 
    // PB4: Bits 19:16 -> 0010 (2)
    // PB5: Bits 23:20 -> 0010 (2)
    GPIOB->AFR[0] &= ~((0xF << 16) | (0xF << 20)); // Clear
    GPIOB->AFR[0] |=  ((2 << 16) | (2 << 20));     // Set AF2
    
    // 3. Configure TIM3 Timebase
    // Goal: 1MHz Timer Clock (1 tick = 1 microsecond)
    // PSC = (Clock / Target) - 1
    // Note: If APB1 prescaler != 1, Timer Clock is 2x PCLK1. 
    // However, with your 45MHz config (prescalers=0), everything is 1:1.
    // We use CLOCK_FREQUENCY generic calculation.
    uint32_t psc_val = (CLOCK_FREQUENCY / 1000000) - 1;
    TIM3->PSC = psc_val;
    
    // 50 ms period
    TIM3->ARR = 50000 - 1;
    
    // 4. Configure Trigger (CH1 - PB4) - PWM Mode
    // We want a 10us pulse.
    TIM3->CCMR1 &= ~(3 << 0);          // CC1S = 00 (Output)
    TIM3->CCMR1 |= (6 << 4);           // OC1M = 110 (PWM Mode 1)
    TIM3->CCMR1 |= (1 << 3);           // OC1PE = 1 (Preload Enable)
    TIM3->CCR1 = 10;                   // High for 10us (10 ticks)
    TIM3->CCER |= (1 << 0);            // CC1E = 1 (Enable Output)
    
    // 5. Configure Echo (CH2 - PB5) - Input Capture Mode
    TIM3->CCMR1 &= ~(3 << 8);          // Clear CC2S bits
    TIM3->CCMR1 |= (1 << 8);           // CC2S = 01 (Input is TI2)
    TIM3->CCER &= ~(1 << 5 | 1 << 7);  // CC2P/CC2NP = 00 (Rising Edge initially)
    TIM3->CCER |= (1 << 4);            // CC2E = 1 (Enable Capture)
    
    // 6. Enable Interrupts
    TIM3->DIER |= (1 << 2);            // CC2IE (Enable Interrupt for Channel 2)
    NVIC_EnableIRQ(TIM3_IRQn);         // Enable NVIC
    
    // 7. Start Timer
    TIM3->CR1 |= (1 << 0);             // CEN = 1
}

// Interrupt Handler
void TIM3_IRQHandler(void){
    // Check if interrupt came from Channel 2 (Echo)
    if (TIM3->SR & (1 << 2)) {
        if (capture_edge == 0) {
            // We just caught the RISING edge
            rise_time = TIM3->CCR2;
            
            // Switch to Falling Edge Detection
            TIM3->CCER |= (1 << 5);    // CC2P = 1 (Falling)
            capture_edge = 1;
        } 
        else {
            // We just caught the FALLING edge
            fall_time = TIM3->CCR2;
            
            // Calculate Pulse Width
            if (fall_time >= rise_time) {
                pulse_width = fall_time - rise_time;
            } else {
                // Handle timer overflow (Counter wrapped around)
                pulse_width = (TIM3->ARR - rise_time) + fall_time;
            }
            
            // Switch back to Rising Edge Detection for next cycle
            TIM3->CCER &= ~(1 << 5);   // CC2P = 0 (Rising)
            capture_edge = 0;
        }
        
        // Clear Interrupt Flag
        TIM3->SR &= ~(1 << 2);
    }
}

/**
 * 
 */
uint16_t SONAR_get_distance(void){
    // Formula: Distance (cm) = Pulse_Width (us) / 58
    // Based on speed of sound 343m/s
    return (uint16_t)(pulse_width / 58);
}