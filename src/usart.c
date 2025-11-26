#include "usart.h"


// Configures PA2 for USART2_TX @ 9600 baud (assuming 16MHz clock)
void USART_INIT(void){
    // 1. Enable Clocks
    RCC->AHB1ENR |= 1;             // Enable GPIOA clock (Bit 0)
    RCC->APB1ENR |= 0x20000;       // Enable USART2 clock (Bit 17)

    // 2. Configure PA2 as Alternate Function (USART2_TX)
    GPIOA->AFR[0] |= 0x0700;       // Set PA2 to AF7 (USART2) in Alternate Function Low Register
                                   // (0x7 << (2 * 4)) = 0x0700
    GPIOA->MODER |= 0x0020;        // Set PA2 to Alternate Function Mode
                                   // (2 << (2 * 2)) = 0x0020
    
    // 3. Configure Baud Rate
    // 0x0683 = 1667 decimal. 
    // Calculation: 16MHz / (1667) approx 9600 Baud
    USART2->BRR = 0x0683;          
    
    // 4. Configure Control Registers
    USART2->CR1 = 0x0008;          // Enable Transmitter (TE, Bit 3)
    USART2->CR2 = 0x0000;          // 1 Stop bit (Default)
    USART2->CR3 = 0x0000;          // No flow control
    
    // 5. Enable USART
    USART2->CR1 |= 0x2000;         // Set UE (USART Enable) bit 13
}

// Textbook function to send char over UART
void USART2_write_char (uint8_t ch){
    // Wait for TXE (Transmit Data Register Empty) bit to be set
    // Bit 7 in Status Register (SR) checks if the data register is ready for new data
    while(!(USART2->SR & 0x0080)) {} 
    
    // Write character to Data Register (DR)
    USART2->DR = (ch & 0xFF);
}

// Modified textbook function to print string over UART
void USART2_write(char *line){
    // Send characters one by one until null terminator ('\0') is found
    for(uint8_t u8_string_inc = 0; line[u8_string_inc] != '\0'; u8_string_inc++){
        USART2_write_char(line[u8_string_inc]);
    }
}