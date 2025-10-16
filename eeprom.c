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
#include "eeprom.h"
/***************************************
*            Variabeles
****************************************/

/* RAM variables */


void EEPROM_WriteBytes(uint16_t memAddr, uint8_t *data, uint16_t len)
{
    uint8_t buf[2 + len];
    buf[0] = (uint8_t)(memAddr >> 8);  // High byte of memory address
    buf[1] = (uint8_t)(memAddr & 0xFF); // Low byte of memory address
    memcpy(&buf[2], data, len);

    I2C_I2CMasterWriteBuf(EEPROM_I2C_ADDRESS, buf, sizeof(buf), I2C_I2C_MODE_COMPLETE_XFER);
    while (I2C_I2CMasterStatus() & I2C_I2C_MSTAT_XFER_INP);

    // Wait for internal write cycle (~5 ms for EEPROM)
    CyDelay(5);
}


// Read a sequence of bytes starting at memory address `memAddr`
void EEPROM_ReadBytes(uint16_t memAddr, uint8_t *data, uint16_t len)
{
    uint8_t addrBuf[2];
    addrBuf[0] = (uint8_t)(memAddr >> 8);
    addrBuf[1] = (uint8_t)(memAddr & 0xFF);

    // Send memory address
    I2C_I2CMasterWriteBuf(EEPROM_I2C_ADDRESS, addrBuf, 2, I2C_I2C_MODE_COMPLETE_XFER);
    while (I2C_I2CMasterStatus() & I2C_I2C_MSTAT_XFER_INP);

    // Read data
    I2C_I2CMasterReadBuf(EEPROM_I2C_ADDRESS, data, len, I2C_I2C_MODE_COMPLETE_XFER);
    while (I2C_I2CMasterStatus() & I2C_I2C_MSTAT_XFER_INP);
}



uint8_t EEPROM_GetMyAddress(void){
    uint8_t myaddr;
    EEPROM_ReadBytes(EEPROM_MY_ADDRESS_REG, &myaddr, 1);
    if((myaddr==0xFF)||(myaddr==0))
        myaddr = MODBUS_MY_DEFAULT_ADDRESS;
    return myaddr;
}

void EEPROM_SetMyAddress(uint8_t* addr){
    
    if(*addr > MODBUS_MAXIMAL_ADDRESS)
      *addr = MODBUS_MY_DEFAULT_ADDRESS;
    EEPROM_WriteBytes(EEPROM_MY_ADDRESS_REG, addr, 1);
    
}

uint16_t EEPROM_GetI2CPeriod(void){
    uint16_t i2c_period;
    uint8_t data[2];
    
    EEPROM_ReadBytes(EEPROM_I2C_PERIOD_MSB_REG, data, sizeof(data));
    i2c_period =data[0];
    i2c_period = (i2c_period<<8)+data[1];
    if((i2c_period==0xFFFF)||((i2c_period<MINIMAL_DEFAULT_I2C_PERIOD)))
        i2c_period = MINIMAL_DEFAULT_I2C_PERIOD;
    else if (i2c_period>MAXIMAL_DEFAULT_I2C_PERIOD)
        i2c_period=MAXIMAL_DEFAULT_I2C_PERIOD;
    return i2c_period;
}

void EEPROM_SetI2CPeriod(uint16_t period){
    
    
    uint8_t data[2];
    data[0]= (period>>8)&0xFF;
    data[1]= period&0xFF;
    EEPROM_WriteBytes(EEPROM_I2C_PERIOD_MSB_REG, data, sizeof(data));
}


uint32_t EEPROM_GetSleepPeriod(void){
    uint8_t data[4];
    uint32_t sleep_period;
    uint8_t reg = EEPROM_SLEEP_PERIOD1_REG;
    
    EEPROM_ReadBytes(EEPROM_SLEEP_PERIOD1_REG, data, sizeof(data));
    
    sleep_period =data[0];
    sleep_period = (sleep_period<<8)+data[1];
    sleep_period = (sleep_period<<8)+data[2];
    sleep_period = (sleep_period<<8)+data[3];
    
    if(sleep_period==0xFFFFFFFF)
        sleep_period = DEFAULT_SLEEP_PERIOD;
    else if (sleep_period > MAXIMAL_SLEEP_PERIOD)
        sleep_period = MAXIMAL_SLEEP_PERIOD;
    else if (sleep_period < MINIMAL_SLEEP_PERIOD)
        //sleep_period = MINIMAL_SLEEP_PERIOD;
        sleep_period = 0;
    return sleep_period;
}

void EEPROM_SetSleepPeriod(uint32_t period){
    uint8_t data[4];
    data[0]= (period>>24)&0xFF;
    data[1]= (period>>16)&0xFF;
    data[2]= (period>>8)&0xFF;
    data[3]= period&0xFF;
    EEPROM_WriteBytes(EEPROM_SLEEP_PERIOD1_REG, data, sizeof(data));
    
    
    
}

uint8_t EEPROM_GetCh2Activity(void){
    uint8_t ch2Active;
    EEPROM_ReadBytes(EEPROM_CH2_ACTIVE_REG, &ch2Active, 1);
    
    if(ch2Active>1)
        ch2Active = CHANNEL2_ACTIVE_DEFAULT;
    return ch2Active;

}

void EEPROM_SetCh2Activity(uint8_t active){
    uint8_t data = active;
    EEPROM_WriteBytes(EEPROM_CH2_ACTIVE_REG, &data, 1);
}

void EEPROM_GetRelaySetting(uint8_t* data){
    EEPROM_ReadBytes(EEPROM_RELAY1_ENERG_REG, data, 12 );
}

void EEPROM_SetRelaySetting(uint8_t reg ,uint8_t val ){
    EEPROM_WriteBytes(reg, &val, 1);
}


void EEPROM_SaveGPSLat(float lattitude){
   uint8_t data[4];
   uint8_t inv[4];
    memcpy(data,&lattitude, sizeof(data) );    
    for(uint8_t i=0 ;i<4;i++)
      inv[3-i]=data[i];
   EEPROM_WriteBytes(EEPROM_LATTITUDE_REG, (uint8_t*)&inv, sizeof(int32_t));
}

void EEPROM_SaveGPSLong(float longitude){
    uint8_t data[4];
    uint8_t inv[4];
    memcpy(data,&longitude, sizeof(data) );  
    for(uint8_t i=0 ;i<4;i++)
      inv[3-i]=data[i];
    EEPROM_WriteBytes(EEPROM_LONGITUTE_REG, (uint8_t*)&inv, sizeof(int32_t));
}


void EEPROM_SaveGPS(float lattitude, float longitude ){
    EEPROM_WriteBytes(EEPROM_LATTITUDE_REG, (uint8_t*)&lattitude, sizeof(int32_t));
    EEPROM_WriteBytes(EEPROM_LONGITUTE_REG, (uint8_t*)&longitude, sizeof(int32_t));
}





void EEPROM_GetGPS(float* lattitude,  float* longitude){
    uint8_t tmp[4];
    uint8_t inv[4];
    
    EEPROM_ReadBytes(EEPROM_LATTITUDE_REG, (uint8_t*)tmp, sizeof(int32_t) );
    for(uint8_t i=0 ;i<4;i++)
        inv[3-i]=tmp[i];
    memcpy(lattitude,inv,4 );
    EEPROM_ReadBytes(EEPROM_LONGITUTE_REG, (uint8_t*)tmp, sizeof(int32_t) );
    for(uint8_t i=0 ;i<4;i++)
        inv[3-i]=tmp[i];
    memcpy(longitude,inv,4 );
 }   


// Logging
void SaveLogIndex(uint8_t index)
{
    //i2c_write_8(EEPROM_I2C_ADDRESS, LOG_INDEX_ADDR,index);
    EEPROM_WriteBytes(LOG_INDEX_ADDR, &index, 1);
}



uint8_t LoadLogIndex(void)
{
    uint8_t data;
    
    EEPROM_ReadBytes(LOG_INDEX_ADDR, &data, 1 );
    if(data==0xFF)data =0;
    return data;
}

void SaveLogEntry(uint8_t* idx, char* txt){
    
    SaveLogIndex(*idx);
    EEPROM_WriteBytes(LOG_DATA_START_ADDR+LOG_PAGE_LENGTH*(*idx), (uint8_t*)txt, LOG_TXT_LENGTH);
    (*idx)++;
    if((*idx)>=LOG_MAX_ENTRIES)(*idx)=0;
    
}

void GetLog(char* log_record){
    uint8_t log_last_entry_idx= LoadLogIndex();
    for(uint8_t i=0;i<LOG_MAX_ENTRIES;i++ )
            GetLogEntry((log_last_entry_idx+i)%LOG_MAX_ENTRIES, &log_record[i*LOG_TXT_LENGTH]);
}


void GetLogEntry(uint8_t idx, char* txt){
    //char entry[LOG_TXT_LENGTH];
    EEPROM_ReadBytes(LOG_DATA_START_ADDR+LOG_PAGE_LENGTH*idx, (uint8_t*)txt, LOG_TXT_LENGTH );
    //if(entry[0]==0xFF)
    //    memcpy(txt,0, LOG_TXT_LENGTH);
    if(*txt==0xFF) 
    for(uint8_t i=0 ;i<LOG_TXT_LENGTH;i++)
        *(txt++)=0;
}


void EraseLog(void){
    SaveLogIndex(0);
    uint8_t data=0xFF;
    EEPROM_WriteBytes(LOG_INDEX_ADDR, &data, 1);
    for(uint16_t i =0; i < LOG_TXT_LENGTH ;i++){
        EEPROM_WriteBytes(LOG_DATA_START_ADDR+i*LOG_PAGE_LENGTH, &data, 1);
    }
    
}

void SerializeLogEntry(const LogEntry* entry, uint8_t* buffer)
{
    buffer[0] = entry->channel;
    buffer[1] = (entry->timestamp >> 8) & 0xFF;
    buffer[2] = entry->timestamp & 0xFF;
    buffer[3] = (entry->reading >> 24) & 0xFF;
    buffer[4] = (entry->reading >> 16) & 0xFF;
    buffer[5] = (entry->reading >> 8) & 0xFF;
    buffer[6] = entry->reading & 0xFF;
}

void DeserializeLogEntry(const uint8_t* buffer, LogEntry* entry)
{
    entry->channel = buffer[0];
    entry->timestamp = ((uint16_t)buffer[1] << 8) | buffer[2];
    entry->reading = ((uint32_t)buffer[3] << 24) |
                     ((uint32_t)buffer[4] << 16) |
                     ((uint32_t)buffer[5] << 8) |
                     buffer[6];
}


void GetDeviceTag(char* tag){
    EEPROM_ReadBytes(EEPROM_DEVICE_TAG_REG, (uint8_t*)tag, DEVICE_TAG_LENGTH );
}

void SetDeviceTag(char* tag){
    EEPROM_WriteBytes(EEPROM_DEVICE_TAG_REG, (uint8_t*)tag, DEVICE_TAG_LENGTH);
}


/* [] END OF FILE */
