/*
* filename: i2c.c
* purpose: implementation of I2C1 driver
* author: Connor Ockerse
* date: 11/27/2025
*/

#include "I2C.h"
#include "RccConfig.h"

void I2C_INIT(void){
    // 1. Enable Clocks
    RCC->AHB1ENR |= (1 << 1);          // Enable GPIOB Clock (Bit 1)
    RCC->APB1ENR |= (1 << 21);         // Enable I2C1 Clock (Bit 21)

    // 2. Configure GPIO Pins (PB8=SCL, PB9=SDA)
    // Set to Alternate Function Mode (10)
    GPIOB->MODER &= ~((3 << 16) | (3 << 18));
    GPIOB->MODER |=  ((2 << 16) | (2 << 18));
    
    // Set Output Type to Open Drain (Critical for I2C)
    GPIOB->OTYPER |= ((1 << 8) | (1 << 9));
    
    // Set Speed to High Speed (optional but recommended)
    GPIOB->OSPEEDR |= ((3 << 16) | (3 << 18));
    
    // === EXTERNAL PULL-UPS USED ===
    // Disable Internal Pull-Ups/Pull-Downs (00)
    GPIOB->PUPDR &= ~((3 << 16) | (3 << 18));

    // Set Alternate Function to AF4 (I2C1)
    // AFR[1] handles pins 8-15
    GPIOB->AFR[1] &= ~((0xF << 0) | (0xF << 4)); // Clear PB8, PB9
    GPIOB->AFR[1] |=  ((4 << 0) | (4 << 4));     // Set AF4

    // 3. Reset I2C
    I2C1->CR1 |= (1 << 15);  // SWRST (Software Reset)
    I2C1->CR1 &= ~(1 << 15); // Clear Reset

    // 4. Configure I2C Timing (Standard Mode 100kHz)
    // CLOCK_FREQUENCY from RccConfig.h
    
    // FREQ[5:0]: Input Clock Frequency in MHz
    uint32_t pclk1_mhz = CLOCK_FREQUENCY / 1000000;
    I2C1->CR2 = pclk1_mhz; 

    // CCR: Clock Control Register
    // Formula: Thigh = Tlow = CCR * TPCLK1
    // Target: 100kHz (Period = 10us). Thigh + Tlow = 10us.
    // Thigh = 5us.
    // CCR = 5us / (1 / PCLK1_MHz) = 5 * PCLK1_MHz
    I2C1->CCR = 5 * pclk1_mhz; // Standard Mode

    // TRISE: Max Rise Time
    // Max rise time in SM is 1000ns.
    // TRISE = (1us / TPCLK1) + 1 = PCLK1_MHz + 1
    I2C1->TRISE = pclk1_mhz + 1;

    // 5. Enable I2C Peripheral
    I2C1->CR1 |= (1 << 0); // PE (Peripheral Enable)
}

void I2C1_byteWrite(uint8_t saddr, uint8_t maddr, uint8_t data){
    volatile int tmp;
    
    // 1. Wait until bus not busy
    while(I2C1->SR2 & (1 << 1)); 
    
    // 2. Generate START
    I2C1->CR1 |= (1 << 8); 
    while(!(I2C1->SR1 & (1 << 0))); // Wait for SB bit
    
    // 3. Send Slave Address
    I2C1->DR = saddr << 1; 
    while(!(I2C1->SR1 & (1 << 1))); // Wait for ADDR bit
    tmp = I2C1->SR2;                // Clear ADDR bit by reading SR2
    
    // 4. Send Memory Address
    while(!(I2C1->SR1 & (1 << 7))); // Wait for TXE
    I2C1->DR = maddr;
    
    // 5. Send Data
    while(!(I2C1->SR1 & (1 << 7))); // Wait for TXE
    I2C1->DR = data;
    
    // 6. Generate STOP
    while(!(I2C1->SR1 & (1 << 2))); // Wait for BTF (Byte Transfer Finished)
    I2C1->CR1 |= (1 << 9);          // Generate STOP
}

void I2C1_byteRead(uint8_t saddr, uint8_t maddr, uint8_t* data) {
    volatile int tmp;

    // 1. Generate START
    while(I2C1->SR2 & (1 << 1));    // Wait busy
    I2C1->CR1 |= (1 << 8); 
    while(!(I2C1->SR1 & (1 << 0))); // Wait SB

    // 2. Send Slave Address (Write Mode)
    I2C1->DR = saddr << 1; 
    while(!(I2C1->SR1 & (1 << 1))); // Wait ADDR
    tmp = I2C1->SR2;                // Clear ADDR

    // 3. Send Memory Address
    while(!(I2C1->SR1 & (1 << 7))); // Wait TXE
    I2C1->DR = maddr;
    while(!(I2C1->SR1 & (1 << 2))); // Wait BTF (Address sent)

    // 4. Generate Repeated START
    I2C1->CR1 |= (1 << 8);
    while(!(I2C1->SR1 & (1 << 0))); // Wait SB

    // 5. Send Slave Address (Read Mode)
    I2C1->DR = (saddr << 1) | 1; 
    while(!(I2C1->SR1 & (1 << 1))); // Wait ADDR

    // 6. Disable ACK (Single Byte Read)
    I2C1->CR1 &= ~(1 << 10);        // Clear ACK bit
    
    // 7. Clear ADDR Flag
    tmp = I2C1->SR2; 

    // 8. Generate STOP immediately after clearing ADDR (Sequence for 1 byte)
    I2C1->CR1 |= (1 << 9);          // Generate STOP

    // 9. Wait for Data
    while(!(I2C1->SR1 & (1 << 6))); // Wait RXNE
    *data = I2C1->DR;

    // 10. Re-enable ACK for future transfers (default state)
    I2C1->CR1 |= (1 << 10);
}