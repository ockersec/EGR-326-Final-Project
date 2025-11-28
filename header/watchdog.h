/*
* filename: watchdog.h
* purpose: Driver for Independent Watchdog (IWDG)
* author: Connor Ockerse
* date: 11/28/2025
* * NOTE: The IWDG runs on the 32 kHz LSI clock. 
* It is independent of the main system PLL.
*/

#ifndef WATCHDOG_H
#define WATCHDOG_H

#include <stm32f446xx.h>
#include <stdint.h>

// === CONFIGURATION ===
// Set the desired timeout in milliseconds
// Max timeout with Prescaler 256 is ~32000 ms (32 seconds)
#define WDT_TIMEOUT_MS 30*1000 

/**
 * @brief Initializes the Watchdog timer with the WDT_TIMEOUT_MS period.
 * @details Enables LSI, sets Prescaler to 256, and calculates Reload value.
 */
void WDT_INIT(void);

/**
 * @brief "Kicks" the dog (Reloads the counter).
 * @details Call this frequently in main loop to prevent system reset.
 */
void WDT_kick(void);

#endif