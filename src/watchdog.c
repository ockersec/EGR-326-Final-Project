/*
* filename: watchdog.c
* purpose: Implementation of IWDG driver
* author: Connor Ockerse
* date: 11/28/2025
*/

#include "watchdog.h"

void WDT_INIT(void) {
    // 1. Enable IWDG (Starts the LSI clock automatically)
    // Write 0xCCCC to Key Register (KR)
    IWDG->KR = 0xCCCC;

    // 2. Enable Register Access
    // Write 0x5555 to KR to unlock PR and RLR registers
    IWDG->KR = 0x5555;

    // 3. Configure Prescaler (PR)
    // 0x06 = Divide by 256
    // LSI (32kHz) / 256 = 125 Hz tick rate (8ms per tick)
    IWDG->PR = 0x06;

    // 4. Calculate and Set Reload Value (RLR)
    // Formula: Counts = Target_MS / (1000 / Freq)
    // Counts = Target_MS / (1000 / 125) = Target_MS / 8
    uint32_t reload_val = WDT_TIMEOUT_MS / 8;
    
    IWDG->RLR = reload_val;

    // 5. Wait for flags to clear (Hardware requires this)
    // SR Bit 1: PVU (Prescaler Value Update)
    // SR Bit 0: RVU (Reload Value Update)
    while (IWDG->SR & 0x03); 

    // 6. Initial Kick (Reloads counter with RLR value)
    IWDG->KR = 0xAAAA;
}

void WDT_kick(void) {
    // Write 0xAAAA to reload the counter
    IWDG->KR = 0xAAAA;
}
