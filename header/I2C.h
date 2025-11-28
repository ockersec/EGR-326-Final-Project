/*
* filename: I2C.h
* purpose: Driver for I2C1 communication (e.g., for LCD or Sensors)
* author: Connor Ockerse
* date: 11/27/2025
* * Connections:
* I2C1_SCL -> PB8
* I2C1_SDA -> PB9
*/

#ifndef I2C_H
#define I2C_H

/* Dependencies */
#include <stm32f446xx.h>
#include <stdint.h>

/**
 * @brief Initializes I2C1 Peripheral on PB8 (SCL) and PB9 (SDA)
 * @details Configures clocks, GPIO AF4, and I2C timing (Standard Mode 100kHz)
 * Uses clock frequency from RccConfig for timing calculations.
 */
void I2C_INIT(void);

/**
 * @brief Writes a single byte to a specific register on an I2C slave device
 * @param saddr: Slave Address (7-bit address shifted left by 1)
 * @param maddr: Memory/Register Address inside the slave device to write to
 * @param data: The 8-bit data byte to write
 */
void I2C1_byteWrite(uint8_t saddr, uint8_t maddr, uint8_t data);

/**
 * @brief Reads a single byte from a specific register on an I2C slave device
 * @param saddr: Slave Address (7-bit address shifted left by 1)
 * @param maddr: Memory/Register Address inside the slave device to read from
 * @param data: Pointer to variable where the read data will be stored
 */
void I2C1_byteRead(uint8_t saddr, uint8_t maddr, uint8_t* data);


#endif