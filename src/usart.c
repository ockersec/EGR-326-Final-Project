/*
* filename: usart.c
* purpose: implementation of uart communication to terminal
* author: Connor Ockerse
* date: 11/26/2025
*/

#include "usart.h"
#include "RccConfig.h" // <--- Added to get CLOCK_FREQUENCY



// USART initialization function
void USART_INIT(void){
    // 1. Enable Clocks
    RCC->AHB1ENR |= 1;             // Enable GPIOA clock
    RCC->APB1ENR |= 0x20000;       // Enable USART2 clock

    // 2. Configure PA2 as Alternate Function (USART2_TX)
    GPIOA->AFR[0] |= 0x0700;       // Set PA2 to AF7
    GPIOA->MODER |= 0x0020;        // Set PA2 to Alternate Function Mode
    
    // 3. Configure Baud Rate Dynamically
    // Formula: BRR = APB1_Clock / Baud_Rate
    // We add (BAUD_RATE / 2) to round to the nearest integer
    uint32_t brr_val = (CLOCK_FREQUENCY + (BAUD_RATE / 2)) / BAUD_RATE;
    USART2->BRR = brr_val;          
    
    // 4. Configure Control Registers
    USART2->CR1 = 0x0008;          // Enable Transmitter (TE)
    USART2->CR2 = 0x0000;          // 1 Stop bit
    USART2->CR3 = 0x0000;          // No flow control
    USART2->CR1 |= 0x2000;         // Enable USART (UE)
}

// Send char over UART
void USART2_write_char (uint8_t ch){
    while(!(USART2->SR & 0x0080)) {} 
    USART2->DR = (ch & 0xFF);
}

// Print string over UART
void USART2_write(char *line){
    for(uint8_t u8_string_inc = 0; line[u8_string_inc] != '\0'; u8_string_inc++){
        USART2_write_char(line[u8_string_inc]);
    }
}