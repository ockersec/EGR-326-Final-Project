/*
* filename: RTC.c
* purpose: Implementation of DS3231 Driver
* author: Connor Ockerse
* date: 11/28/2025
*/

#include "RTC.h"
#include "I2C.h"
#include "usart.h"
#include <stdio.h> // For sprintf

// === HELPER FUNCTIONS ===

// Convert Decimal to Binary Coded Decimal (e.g., 12 -> 0x12)
uint8_t decToBcd(int val) {
    return (uint8_t)((val / 10 * 16) + (val % 10));
}

// Convert BCD to Decimal (e.g., 0x12 -> 12)
int bcdToDec(uint8_t val) {
    return (int)((val / 16 * 10) + (val % 16));
}


// === READ FUNCTIONS ===

uint8_t RTC_read_second(void) {
    uint8_t data = 0;
    I2C1_byteRead(RTC_ADDRESS, SECOND_ADDRESS, &data);
    return (uint8_t)bcdToDec(data);
}

uint8_t RTC_read_minute(void) {
    uint8_t data = 0;
    I2C1_byteRead(RTC_ADDRESS, MINUTE_ADDRESS, &data);
    return (uint8_t)bcdToDec(data);
}

uint8_t RTC_read_hour(void) {
    uint8_t data = 0;
    I2C1_byteRead(RTC_ADDRESS, HOUR_ADDRESS, &data);
    // Mask out 12/24 mode bit (Bit 6) just in case, assuming 24h mode usage
    return (uint8_t)bcdToDec(data & 0x3F);
}

uint8_t RTC_read_day(void) {
    uint8_t data = 0;
    I2C1_byteRead(RTC_ADDRESS, DAY_ADDRESS, &data);
    return (uint8_t)bcdToDec(data);
}

uint8_t RTC_read_date(void) {
    uint8_t data = 0;
    I2C1_byteRead(RTC_ADDRESS, DATE_ADDRESS, &data);
    return (uint8_t)bcdToDec(data);
}

uint8_t RTC_read_month(void) {
    uint8_t data = 0;
    I2C1_byteRead(RTC_ADDRESS, MONTH_ADDRESS, &data);
    // Mask out Century bit (Bit 7)
    return (uint8_t)bcdToDec(data & 0x1F);
}

uint8_t RTC_read_year(void) {
    uint8_t data = 0;
    I2C1_byteRead(RTC_ADDRESS, YEAR_ADDRESS, &data);
    return (uint8_t)bcdToDec(data);
}

void RTC_read_clock(Clock* time) {
    // Read all fields and store in the struct
    time->seconds = RTC_read_second();
    time->minutes = RTC_read_minute();
    time->hours   = RTC_read_hour();
    time->day     = RTC_read_day();
    time->date    = RTC_read_date();
    time->month   = RTC_read_month();
    time->year    = RTC_read_year();
}

// === WRITE FUNCTIONS ===

void RTC_write_second(uint8_t sec) {
    I2C1_byteWrite(RTC_ADDRESS, SECOND_ADDRESS, decToBcd(sec));
}

void RTC_write_minute(uint8_t min) {
    I2C1_byteWrite(RTC_ADDRESS, MINUTE_ADDRESS, decToBcd(min));
}

void RTC_write_hour(uint8_t hour) {
    // Writes in 24-hour format
    I2C1_byteWrite(RTC_ADDRESS, HOUR_ADDRESS, decToBcd(hour));
}

void RTC_write_day(uint8_t day) {
    I2C1_byteWrite(RTC_ADDRESS, DAY_ADDRESS, decToBcd(day));
}

void RTC_write_date(uint8_t date) {
    I2C1_byteWrite(RTC_ADDRESS, DATE_ADDRESS, decToBcd(date));
}

void RTC_write_month(uint8_t month) {
    I2C1_byteWrite(RTC_ADDRESS, MONTH_ADDRESS, decToBcd(month));
}

void RTC_write_year(uint8_t year) {
    I2C1_byteWrite(RTC_ADDRESS, YEAR_ADDRESS, decToBcd(year));
}

void RTC_write_clock(Clock* time) {
    RTC_write_second(time->seconds);
    RTC_write_minute(time->minutes);
    RTC_write_hour(time->hours);
    RTC_write_day(time->day);
    RTC_write_date(time->date);
    RTC_write_month(time->month);
    RTC_write_year(time->year);
}

// === PRINT FUNCTION ===

void RTC_print_clock(void) {
    char buffer[50];
    Clock t;
    
    // Get current time
    RTC_read_clock(&t);
    
    // Format: HH:MM:SS DD/MM/YY
    sprintf(buffer, "%02d:%02d:%02d %02d/%02d/%02d\r\n", 
            t.hours, t.minutes, t.seconds, 
            t.date, t.month, t.year);
            
    USART2_write(buffer);
}