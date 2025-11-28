/*
* filename: stopwatch.c
* purpose: implementation of 32-bit stopwatch driver using TIM2
* author: Connor Ockerse
* date: 11/27/2025
*/

#include "stopwatch.h"
#include "RccConfig.h" // Needed for CLOCK_FREQUENCY

void STOPWATCH_INIT(void){
    // 1. Enable TIM2 Clock (APB1 Bus)
    RCC->APB1ENR |= (1 << 0); // TIM2EN is Bit 0
    
    // 2. Configure Prescaler
    // Formula: PSC = (TimerClock / TargetFreq) - 1
    // Target: 1000 Hz (1 ms)
    // Example: 45,000,000 / 1000 = 45,000. PSC = 44,999.
    uint32_t psc_val = (CLOCK_FREQUENCY / 1000) - 1;
    TIM2->PSC = psc_val;
    
    // 3. Set Auto-Reload to Max (32-bit)
    TIM2->ARR = 0xFFFFFFFF;
    
    // 4. Force Update Generation (UG)
    // CRITICAL FIX: This loads the PSC and ARR values from the shadow registers 
    // into the active registers immediately. Without this, PSC stays 0.
    TIM2->EGR |= (1 << 0); // Bit 0 is UG (Update Generation)
    
    // Clear the Update Interrupt Flag that gets set by setting UG
    // (We don't want an interrupt to fire immediately if interrupts were enabled)
    TIM2->SR &= ~(1 << 0);

    // 5. Ensure Timer is disabled initially
    TIM2->CR1 &= ~(1 << 0); // CEN = 0
    
    // 6. Reset Counter
    TIM2->CNT = 0;
}

void STOPWATCH_start(void){
    // Reset count to 0
    TIM2->CNT = 0;
    // Enable Counter (CEN)
    TIM2->CR1 |= (1 << 0);
}

uint32_t STOPWATCH_read(void){
    // Return current 32-bit counter value (in milliseconds)
    return TIM2->CNT;
}

void STOPWATCH_stop(void){
    // Disable Counter
    TIM2->CR1 &= ~(1 << 0);
}