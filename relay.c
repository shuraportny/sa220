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
#include "relay.h"

S_Rly relay[4];


void TCA9534_SetRelay(uint8_t relayNum, uint8_t state) {
    if (relayNum > 3) return;

    uint8_t current = i2c_Read_8(I2C_RELAY_SLAVE_ADDR, TCA9534_OUTPUT_REG);
    if (state)
        current |= (1 << relayNum);   // Set bit to turn ON
    else
        current &= ~(1 << relayNum);  // Clear bit to turn OFF

    i2c_write_8(I2C_RELAY_SLAVE_ADDR, TCA9534_OUTPUT_REG, current);
}


void  TCA9534_Init(uint8_t configMask) {
    uint8 buf[2] = {TCA9534_CONFIG_REG, configMask};
    I2C_I2CMasterWriteBuf(I2C_RELAY_SLAVE_ADDR, buf, 2, I2C_I2C_MODE_COMPLETE_XFER);
    while(I2C_I2CMasterStatus() & I2C_I2C_MSTAT_XFER_INP);
}

void TCA9534_SetAllRelays(uint8_t relayMask) {
    uint8_t current = i2c_Read_8(I2C_RELAY_SLAVE_ADDR, TCA9534_OUTPUT_REG);
    current &= 0xF0;
    current |= (relayMask & 0x0F);
    i2c_write_8(I2C_RELAY_SLAVE_ADDR, TCA9534_OUTPUT_REG, current);
}


void TCA9534_WriteReg(uint8 value) {
    uint8 buf[2] = {TCA9534_OUTPUT_REG, value};
    I2C_I2CMasterWriteBuf(I2C_RELAY_SLAVE_ADDR, buf, 2, I2C_I2C_MODE_COMPLETE_XFER);
    while(I2C_I2CMasterStatus() & I2C_I2C_MSTAT_XFER_INP);
}


uint8_t TCA9534_GetStatus(){
    
    if(I2C_I2C_MSTR_NO_ERROR!= i2c_status(I2C_RELAY_SLAVE_ADDR))
        return 0;
    else 
        return 1;   
    
}


uint8_t TCA9534_GetReg() {
    
    uint8_t reg =TCA9534_OUTPUT_REG;
    uint8_t data;
    uint32_t status;
    
    
    
    // Send memory address
    status = I2C_I2CMasterWriteBuf(I2C_RELAY_SLAVE_ADDR, &reg, 1, I2C_I2C_MODE_COMPLETE_XFER);
    while (I2C_I2CMasterStatus() & I2C_I2C_MSTAT_XFER_INP);

    // Read data
    I2C_I2CMasterReadBuf(I2C_RELAY_SLAVE_ADDR, &data, 1, I2C_I2C_MODE_COMPLETE_XFER);
    while (I2C_I2CMasterStatus() & I2C_I2C_MSTAT_XFER_INP);
    return data;
}


uint8_t GetRelayState(){
    uint8_t  reg = TCA9534_GetReg();
    
    uint8_t faultbit = (reg>>3)&0x1; 
    uint8_t alarm3bit =(reg>>2)&0x1;
    uint8_t alarm2bit =(reg>>1)&0x1;
    uint8_t alarm1bit =reg&0x1;
    if( (alarm3bit && !relay[2].energ)|| (!alarm3bit && relay[2].energ))
        return 3;
    if( (alarm2bit && !relay[1].energ)|| (!alarm2bit && relay[1].energ))
        return 2;
    if( (alarm1bit && !relay[0].energ)|| (!alarm1bit && relay[0].energ))
        return 1;
    if( (faultbit && !relay[3].energ)|| (!faultbit && relay[3].energ))
        return 4;
    else
        return 0;
}

uint8_t GetRelayMask(){
    uint8_t  reg= TCA9534_GetReg();
    return reg;
}

/* [] END OF FILE */
