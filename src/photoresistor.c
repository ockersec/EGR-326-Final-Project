/*
* filename: photoresistor.c
* purpose: implementation of analog light sensor driver
* author: Connor Ockerse
* date: 11/26/2025
*/

#include "photoresistor.h"
#include "RccConfig.h" // Needed for CLOCK_FREQUENCY

void PHOTO_INIT(void){
    // 1. Enable Clocks
    RCC->AHB1ENR |= (1 << 0);          // GPIOA Clock
    RCC->APB2ENR |= (1 << 8);          // ADC1 Clock
    
    // 2. Configure PA1 as Analog Input
    GPIOA->MODER |= (3 << (1 * 2));    // Mode 11 (Analog)
    GPIOA->PUPDR &= ~(3 << (1 * 2));   // No pull-up/pull-down
    
    // 3. Configure ADC Clock Prescaler
    // The ADC runs off PCLK2. We need to ensure ADC clock is <= 30-36 MHz.
    // We use CLOCK_FREQUENCY to decide the prescaler.
    
    ADC123_COMMON->CCR &= ~(3 << 16);  // Clear ADCPRE bits
    
    // If System is fast (>60MHz), we generally need to divide by 4.
    // If System is slow (<=60MHz), divide by 2 is safe.
    if (CLOCK_FREQUENCY > 60000000) {
        // Example: 180MHz System -> 90MHz APB2. 
        // 90MHz / 4 = 22.5MHz (Safe)
        ADC123_COMMON->CCR |= (1 << 16); 
    } else {
        // Example: 45MHz System -> 45MHz APB2.
        // 45MHz / 2 = 22.5MHz (Safe)
        ADC123_COMMON->CCR |= (0 << 16);
    }
    
    // 4. Configure ADC1 CR1 (Control Register 1)
    ADC1->CR1 = 0;                     // Reset defaults
    ADC1->CR1 &= ~(1 << 8);            // SCAN mode disabled
    ADC1->CR1 &= ~(1 << 5);            // EOCIE (Interrupts) disabled
    
    // 5. Configure ADC1 CR2 (Control Register 2)
    ADC1->CR2 = 0;                     // Reset defaults
    ADC1->CR2 &= ~(1 << 1);            // CONT = 0 (Single Conversion)
    ADC1->CR2 &= ~(3 << 28);           // EXTEN = 00 (Software Trigger Only)
    ADC1->CR2 |= (1 << 10);            // EOCS = 1 (EOC bit set at end of conversion)
    
    // 6. Configure Sample Time for Channel 1
    // 84 Cycles provides stable reading
    ADC1->SMPR2 |= (1 << (1 * 3 + 2)); // Channel 1 -> 84 cycles
    
    // 7. Configure Sequence
    ADC1->SQR1 = 0;                    // Sequence length = 1 conversion
    ADC1->SQR3 = (1 << 0);             // 1st conversion = Channel 1
    
    // 8. Turn On ADC
    ADC1->CR2 |= (1 << 0);             // ADON = 1
}

uint16_t PHOTO_read(void){
    // 1. Start the Conversion
    ADC1->CR2 |= (1 << 30);            // SWSTART = 1
    
    // 2. Wait for conversion to complete
    while(!(ADC1->SR & (1 << 1))){}
    
    // 3. Read and Return Data
    return (uint16_t)ADC1->DR;
}