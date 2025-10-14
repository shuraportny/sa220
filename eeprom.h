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
#if !defined(EEProm_H) /* EEProm_H */
#define EEProm_H

#include <project.h>
#include "i2c_master.h"
#include "runtime.h"
    
/***************************************
*            Variables
****************************************/

/***************************************
*            Constants
****************************************/
#ifndef EEPROM_I2C_ADDRESS
#define EEPROM_I2C_ADDRESS 0x50
#endif

#ifndef EEPROM_I2C_SN_ADDRESS
#define EEPROM_I2C_SN_ADDRESS 0x58
#endif



#define EEPROM_WRITE_DELAY     5  // ms
#define LOG_ENTRY_SIZE         7




// EEPROM REGISTERS
#define EEPROM_MY_ADDRESS_REG           0
#define EEPROM_I2C_PERIOD_MSB_REG       1
#define EEPROM_I2C_PERIOD_LSB_REG       2
#define EEPROM_SLEEP_PERIOD1_REG        3
#define EEPROM_SLEEP_PERIOD2_REG        4
#define EEPROM_SLEEP_PERIOD3_REG        5
#define EEPROM_SLEEP_PERIOD4_REG        6
#define EEPROM_CH2_ACTIVE_REG           7

#define EEPROM_RELAY1_ENERG_REG         8
#define EEPROM_RELAY1_LATCH_REG         9
#define EEPROM_RELAY1_ACK_REG           10
#define EEPROM_RELAY2_ENERG_REG         11
#define EEPROM_RELAY2_LATCH_REG         12
#define EEPROM_RELAY2_ACK_REG           13
#define EEPROM_RELAY3_ENERG_REG         14
#define EEPROM_RELAY3_LATCH_REG         15
#define EEPROM_RELAY3_ACK_REG           16
#define EEPROM_RELAY4_ENERG_REG         17
#define EEPROM_RELAY4_LATCH_REG         18
#define EEPROM_RELAY4_ACK_REG           19

#define EEPROM_LATTITUDE_REG            20 // 4 bytes
#define EEPROM_LONGITUTE_REG            24 // 4 bytes

#define LOG_PAGE_LENGTH                 0x80 // This chip has 1024 pages
#define LOG_INDEX_ADDR                  0x0040
#define LOG_DATA_START_ADDR             0x0050 // ch number(1byte)+ time stamp (2 bytes)+ sysreadnig(4 bytes) = 7 bytes total 
#define LOG_MAX_ENTRIES                 10 // 6 x 100=700 bytes?

#define LOG_TXT_LENGTH                  40
typedef struct {
    uint8_t  channel;
    uint16_t timestamp;
    uint32_t reading;
} LogEntry;


/***************************************
*         Function Prototypes
****************************************/
void EEPROM_WriteBytes(uint16_t memAddr, uint8_t *data, uint16_t len);

uint8_t EEPROM_GetMyAddress(void);
void EEPROM_SetMyAddress(uint8_t* addr);
uint16_t EEPROM_GetI2CPeriod(void);
void EEPROM_SetI2CPeriod(uint16_t period);
uint32_t EEPROM_GetSleepPeriod(void);
void EEPROM_SetSleepPeriod(uint32_t period);
uint8_t EEPROM_GetCh2Activity(void);
void EEPROM_SetCh2Activity(uint8_t active);
void EEPROM_GetRelaySetting( uint8_t* data);
void EEPROM_SetRelaySetting(uint8_t reg ,uint8_t val );


void EEPROM_SaveGPSLat(float lattitude);
void EEPROM_SaveGPSLong(float longitude);
void EEPROM_SaveGPS(float lattitude, float longitude );
void EEPROM_GetGPS(float* lattitude,  float* longitude);
void EEPROM_GPSLat(float lattitude);
void EEPROM_GPSLong(float longitude);

void SaveLogIndex(uint8_t index);
uint8_t LoadLogIndex(void);
void SaveLogEntry(uint8_t* idx, char* txt);
void GetLog(char* log_record);
void GetLogEntry(uint8_t idx, char* txt);
void EraseLog(void);

#endif /* EEProm_H */
/* [] END OF FILE */
