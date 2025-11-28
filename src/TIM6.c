/*
* filename: TIM6.c
* purpose: implementation of 1ms system timer
* author: Connor Ockerse
* date: 11/27/2025
*/

#include "TIM6.h"
#include "RccConfig.h" // Needed for CLOCK_FREQUENCY

volatile uint32_t ms_counter = 0;

void TIM6_INIT(void){
    // 1. Enable TIM6 Clock (APB1 Bus)
    // TIM6 is Bit 4 in APB1ENR
    RCC->APB1ENR |= (1 << 4);
    
    // 2. Configure Prescaler (PSC)
    // We want the timer to tick at 1 MHz (1 microsecond)
    // Formula: PSC = (TimerClock / 1,000,000) - 1
    // Note: Assuming TimerClock = CLOCK_FREQUENCY (True for Prescaler=1)
    uint16_t psc_val = (CLOCK_FREQUENCY / 1000000) - 1;
    TIM6->PSC = psc_val;
    
    // 3. Configure Auto-Reload (ARR)
    // We want an interrupt every 1ms (1000 microseconds)
    TIM6->ARR = 1000 - 1;
    
    // 4. Enable Update Interrupt source (UIE)
    TIM6->DIER |= (1 << 0);
    
    // 5. Enable NVIC Interrupt
    // TIM6_DAC_IRQn is the interrupt line for TIM6
    NVIC_EnableIRQ(TIM6_DAC_IRQn);

    // 6. Enable Timer
    TIM6->CR1 |= (1 << 0); // CEN
}

// The Interrupt Service Routine
void TIM6_DAC_IRQHandler(void){
    // Check if Update Interrupt Flag is set
    if (TIM6->SR & (1 << 0)) {
        
        // Clear the flag immediately
        TIM6->SR &= ~(1 << 0);
        
        // Increment global millisecond counter
        ms_counter++;
    }
}


// blocking delay
void TIM6_delay(uint32_t delay){
    uint32_t start = ms_counter;
    while ((ms_counter - start) < delay){}
}

// getter
uint32_t TIM6_get_count(void){
    return ms_counter;
}

// setter
void TIM6_reset_count(void){
    ms_counter = 0;
}