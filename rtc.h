/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
#include "i2c_master.h"

typedef struct {
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t dayOfWeek;   // 1 = Sunday
    uint8_t day;
    uint8_t month;
    uint8_t year;        // last two digits (e.g., 24 for 2024)
} S_DateTime;


void DS3231_Init(void);
void DS3231_SetDateTime(const S_DateTime* dt);
void DS3231_GetDateTime(S_DateTime* dt);


void DS3231_EnableDateTime(void);
void read_RTC( S_DateTime *rtc);
void write_RTC( S_DateTime *rtc);
uint8_t read_RTCReg( uint8_t reg);
/* [] END OF FILE */
