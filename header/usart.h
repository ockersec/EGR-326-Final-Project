#ifndef USART_H
#define USART_H

/*
* filename: usart.h
* purpose: header file for uart communication to terminal
* author: Connor Ockerse
* date: 11/26/2025
*/

/* Dependencies */
#include <stm32f446xx.h>
#include <stdint.h>

/* Function declarations */

/** * 
 * @brief Initializes USART2 peripheral for serial communication
 * @details Enables GPIOA/USART2 clocks, configures PA2(TX)/PA3(RX), and sets baud rate
 */
void USART_INIT(void);

/**
 * @brief Sends a single character over USART2
 * @param ch: The 8-bit character/data to transmit
 */
void USART2_write_char(uint8_t ch);

/**
 * @brief Sends a full string over USART2
 * @param line: Pointer to the null-terminated string/char array to transmit
 */
void USART2_write(char *line);

#endif