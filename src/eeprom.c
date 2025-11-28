

#include "eeprom.h"
#include "I2C.h"
#include "TIM6.h"


void EEPROM_write(uint8_t saddr, uint8_t memory_location, uint8_t data){
    I2C1_byteWrite(saddr, memory_location, data);
    TIM6_delay(2);
}

void EEPROM_random_read(uint8_t saddr, uint8_t maddr, uint8_t* data){
    // random read for EEPROM involves first writing to the word address
    // then reading the data that is sent

    volatile int tmp;
    
    // Phase 1: Write memory address to set pointer
    while(I2C1->SR2 & 2);  // Wait until bus not busy
    I2C1->CR1 |= 0x100;    // Generate start 
    while(!(I2C1->SR1 & 1));
    
    I2C1->DR = saddr << 1; // Slave address + write
    while(!(I2C1->SR1 & 2));
    tmp = I2C1->SR2;
    
    while(!(I2C1->SR1 & 0x80));
    I2C1->DR = maddr;      // Memory address
    
    // Phase 2: Repeated start and read
    while(!(I2C1->SR1 & 0x80));
    I2C1->CR1 |= 0x100;    // Repeated start
    while(!(I2C1->SR1 & 1));
    
    I2C1->DR = (saddr << 1) | 1; // Slave address + read
    while(!(I2C1->SR1 & 2));
    tmp = I2C1->SR2;
    
    // Disable ACK for single byte read
    I2C1->CR1 &= ~(1 << 10);
    
    // Wait for data and read it
    while(!(I2C1->SR1 & 0x40));
    *data = I2C1->DR;
    
    // Generate STOP after reading
    I2C1->CR1 |= 0x200;
    
    // Re-enable ACK
    I2C1->CR1 |= (1 << 10);
}

uint8_t EEPROM_read_address(uint8_t saddr, uint8_t memory_location){
    uint8_t data;
    EEPROM_random_read(saddr, memory_location, &data);
    return data;
}


void EEPROM_clear(uint8_t saddr){
    for (uint16_t i = 0;  i < EEPROM_NUM_BYTES; i++){
        EEPROM_write(saddr, i, 0);
    }
}