/*
* filename: RTC.h
* purpose: Driver for DS3231 Real Time Clock (I2C)
* author: Connor Ockerse
* date: 11/28/2025
* * Connections:
* SCL -> PB8
* SDA -> PB9
* VCC -> 3.3V or 5V
* GND -> GND
*/

#ifndef RTC_H
#define RTC_H

#include <stm32f446xx.h>
#include <stdint.h>

// I2C Address (7-bit)
#define RTC_ADDRESS 0x68

// Register Map (BCD Format)
#define SECOND_ADDRESS 0x00
#define MINUTE_ADDRESS 0x01
#define HOUR_ADDRESS   0x02
#define DAY_ADDRESS    0x03
#define DATE_ADDRESS   0x04
#define MONTH_ADDRESS  0x05
#define YEAR_ADDRESS   0x06

// Time Structure to hold all data
typedef struct {
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t day;
    uint8_t date;
    uint8_t month;
    uint8_t year;
}Clock;

// === READ FUNCTIONS (Return the value) ===

/**
 * @brief Reads the seconds register
 * @return Seconds in Decimal (Converted from BCD)
 */
uint8_t RTC_read_second(void);

/**
 * @brief Reads the minutes register
 * @return Minutes in Decimal
 */
uint8_t RTC_read_minute(void);

/**
 * @brief Reads the hours register (24-hour format)
 * @return Hours in Decimal
 */
uint8_t RTC_read_hour(void);

/**
 * @brief Reads the day of the week
 * @return 1-7 (1=Sunday usually)
 */
uint8_t RTC_read_day(void);

/**
 * @brief Reads the day of the month
 * @return Date (1-31)
 */
uint8_t RTC_read_date(void);

/**
 * @brief Reads the month
 * @return Month (1-12)
 */
uint8_t RTC_read_month(void);

/**
 * @brief Reads the year (2-digit)
 * @return Year (0-99) representing 2000-2099
 */
uint8_t RTC_read_year(void);

/**
 * @brief Reads all time registers at once into a struct
 * @param time: Pointer to Clock struct to fill
 */
void RTC_read_clock(Clock* time);

// === WRITE FUNCTIONS (Set the value) ===

/**
 * @brief Sets the seconds
 * @param sec: 0-59
 */
void RTC_write_second(uint8_t sec);

/**
 * @brief Sets the minutes
 * @param min: 0-59
 */
void RTC_write_minute(uint8_t min);

/**
 * @brief Sets the hours
 * @param hour: 0-23 (24h format)
 */
void RTC_write_hour(uint8_t hour);

/**
 * @brief Sets the day of week
 * @param day: 1-7
 */
void RTC_write_day(uint8_t day);

/**
 * @brief Sets the date
 * @param date: 1-31
 */
void RTC_write_date(uint8_t date);

/**
 * @brief Sets the month
 * @param month: 1-12
 */
void RTC_write_month(uint8_t month);

/**
 * @brief Sets the year
 * @param year: 0-99
 */
void RTC_write_year(uint8_t year);

/**
 * @brief Sets the entire clock at once
 * @param time: Pointer to struct containing values
 */
void RTC_write_clock(Clock* time);

// === HELPER FUNCTIONS ===

/**
 * @brief Prints the current time to UART in format "HH:MM:SS DD/MM/YY"
 */
void RTC_print_clock(void);

/**
 * @brief Helper: Converts Decimal to Binary Coded Decimal (BCD)
 */
uint8_t decToBcd(int val);

/**
 * @brief Helper: Converts BCD to Decimal
 */
int bcdToDec(uint8_t val);

#endif