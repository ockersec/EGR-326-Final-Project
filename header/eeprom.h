/*
* filename: eeprom.h
* purpose: Driver for 24C02C EEPROM
* author: Connor Ockerse
* date: 11/27/2025
*/

#ifndef EEPROM_H
#define EEPROM_H

#include <stm32f446xx.h>
#include <stdint.h>

// Default I2C Address for 24C02C (A0, A1, A2 grounded)
#define EEPROM_ADDRESS 0xA0 >> 1

// max storage on 2402C EEPROM
#define EEPROM_NUM_BYTES 256U

/**
 * @brief Writes a single byte to the EEPROM
 * @details Includes the required 5ms write cycle delay automatically.
 * @param address: Slave Address (usually 0b01010_0000)
 * @param memory_location: The address inside the EEPROM (0x00 to 0xFF)
 * @param data: The byte to write
 */
void EEPROM_write(uint8_t saddr, uint8_t memory_location, uint8_t data);

/**
 * @brief Reads a byte from a specific location via pointer
 * @param saddr: Slave Address
 * @param maddr: Memory Address to read from
 * @param data: Pointer to store the read value
 */
void EEPROM_random_read(uint8_t saddr, uint8_t maddr, uint8_t* data);

/**
 * @brief Reads a byte from a specific location and returns it directly
 * @param address: Slave Address
 * @param memory_location: Memory Address to read from
 * @return The data byte read from EEPROM
 */
uint8_t EEPROM_read_address(uint8_t saddr, uint8_t memory_location);

/**
 * @brief Wipes the entire EEPROM (Writes 0x00 to all addresses)
 * @param saddr: Slave Address
 */
void EEPROM_clear(uint8_t saddr);

#endif