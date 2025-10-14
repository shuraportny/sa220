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


#define EEPROM_I2C_ADDRESS              0x50
#define I2C_RTC_SLAVE_ADDR              0x68
#define I2C_LED_SLAVE_ADDR              0x60
#define I2C_RELAY_SLAVE_ADDR            0x20 
#define I2C_DAC_SLAVE_ADDR              0x0F  
#define I2C_MUX_SLAVE_ADDR              0x73 
#define I2C_SENSOR_SLAVE_ADDR           0x61

//// EEPROM REGISTERS
//#define EEPROM_MY_ADDRESS_REG           0
//#define EEPROM_I2C_PERIOD_MSB_REG       1
//#define EEPROM_I2C_PERIOD_LSB_REG       2
//#define EEPROM_SLEEP_PERIOD_MSB_REG     3
//#define EEPROM_SLEEP_PERIOD_LSB_REG     4
//#define EEPROM_CH2_ACTIVE_REG           5
//
//#define EEPROM_RELAY1_ENERG_REG         6
//#define EEPROM_RELAY1_LATCH_REG         7
//#define EEPROM_RELAY1_ACK_REG           8
//#define EEPROM_RELAY2_ENERG_REG         9
//#define EEPROM_RELAY2_LATCH_REG         10
//#define EEPROM_RELAY2_ACK_REG           11
//#define EEPROM_RELAY3_ENERG_REG         12
//#define EEPROM_RELAY3_LATCH_REG         13
//#define EEPROM_RELAY3_ACK_REG           14
//#define EEPROM_RELAY4_ENERG_REG         15
//#define EEPROM_RELAY4_LATCH_REG         16
//#define EEPROM_RELAY4_ACK_REG           17
//
//#define EEPROM_LATTITUDE_REG            18 // 4 bytes
//#define EEPROM_LONGITUTE_REG            22 // 4 bytes
//
//#define LOG_DATA_START_ADDR             50 // ch number(1byte)+ time stamp (2 bytes)+ sysreadnig(4 bytes) = 7 bytes total 
//#define EEPROM_LOG_RECORDS_NUM          100 // 6 x 100=700 bytes?


#define I2C_SYS_READING_OFFSET      (1)
#define I2C_CALIBRATION_OFFSET      (11)
#define I2C_STATIC_DATA_OFFSET      (21)
#define I2C_CONFIG_DATA_OFFSET      (35)





#define I2C_NO_ERROR 0

#define I2CS_BUFF_SIZE          (100)





uint32_t i2c_status(uint8 slave);
uint8 i2c_Read_8( uint8 slave, uint8 address );
uint16 i2c_read_16( uint8 slave, uint8 address );
void i2c_write_8( uint8 slave, uint8 address, uint8 data );
void i2c_write_16( uint8 slave, uint8 address, uint16 data );

uint32 slave_ReadDataRaw(uint8 Reg, uint16 dataSize, uint8 *dataPtr);
uint32 slave_WriteDataRaw(uint8 Reg, uint16 dataSize, uint8 *dataPtr);

uint8_t SelectChannel(uint8_t channel);
void I2C_Recover();
/* [] END OF FILE */
