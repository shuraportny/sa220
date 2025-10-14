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
#include "dac.h"


void AD5627_InitDAC(){
    uint8_t buffer[3]={0};

    // Set internal reference
    buffer[0] = 0x38;                // 00111_000    
    buffer[1] = 0; // Upper 8 bits
    buffer[2] = 1; // Lower 4 bits in upper nibble
    I2C_I2CMasterClearStatus();
    I2C_I2CMasterWriteBuf(I2C_DAC_SLAVE_ADDR, buffer, 3, I2C_I2C_MODE_COMPLETE_XFER);
    while (I2C_I2CMasterStatus() & I2C_I2C_MSTAT_XFER_INP); // Wait until transfer completes
    
    // Software Reset
    buffer[0] = 0x28;                // 00101_000
    buffer[1] = 0; // Upper 8 bits
    buffer[2] = 0; // Lower 4 bits in upper nibble
    I2C_I2CMasterClearStatus();
    I2C_I2CMasterWriteBuf(I2C_DAC_SLAVE_ADDR, buffer, 3, I2C_I2C_MODE_COMPLETE_XFER);
    while (I2C_I2CMasterStatus() & I2C_I2C_MSTAT_XFER_INP); // Wait until transfer completes
    
    // LDAC Register Setup
    buffer[0] = 0x30;                // 00110_000
    buffer[1] = 0; // Upper 8 bits
    buffer[2] = 0; // Lower 4 bits in upper nibble
    I2C_I2CMasterClearStatus();
    I2C_I2CMasterWriteBuf(I2C_DAC_SLAVE_ADDR, buffer, 3, I2C_I2C_MODE_COMPLETE_XFER);
    while (I2C_I2CMasterStatus() & I2C_I2C_MSTAT_XFER_INP); // Wait until transfer completes
}


// Set DAC output (0–4095 is 0–100%)
void AD5627_SetDAC(uint8_t ch,  uint16_t value12bit)
{
    uint8_t buffer[3]={0};
    
    buffer[0] = 0x18 | ch ;                // 0b00011(write and update)_xxx(channlel) 
    buffer[1] = (value12bit >> 4) & 0xFF; // Upper 8 bits
    buffer[2] = (value12bit & 0x0F) << 4; // Lower 4 bits in upper nibble
    I2C_I2CMasterClearStatus();
    I2C_I2CMasterWriteBuf(I2C_DAC_SLAVE_ADDR, buffer, 3, I2C_I2C_MODE_COMPLETE_XFER);
    while (I2C_I2CMasterStatus() & I2C_I2C_MSTAT_XFER_INP); // Wait until transfer completes
}



// Set DAC output (0–4095 is 0–100%)
void MCP47VB02T_SetDAC(uint16_t value12bit)
{
    uint8_t buffer[3];
    
    buffer[0] = 0x08;                    // Volatile DAC input register (Channel A)
    buffer[1] = (value12bit >> 4) & 0xFF; // Upper 8 bits
    buffer[2] = (value12bit & 0x0F) << 4; // Lower 4 bits in upper nibble
    I2C_I2CMasterClearStatus();
    I2C_I2CMasterWriteBuf(I2C_DAC_SLAVE_ADDR, buffer, 3, I2C_I2C_MODE_COMPLETE_XFER);
    while (I2C_I2CMasterStatus() & I2C_I2C_MSTAT_XFER_INP); // Wait until transfer completes
}

////--------------------------------------------------
//// Convert 4–20 mA to 12-bit DAC value
////--------------------------------------------------
//uint16_t Convert_mA_to_DAC(float mA)
//{
//    // This assumes external voltage-to-current converter maps DAC voltage to 4–20 mA
//    //float vOut = (mA / 20.0f) * MCP47VB02T_VREF;
//    //uint16_t dacValue = (uint16_t)((vOut / MCP47VB02T_VREF) * 4095.0f);
//
//    float vOut = (mA / 20.0f) * AD5627_VREF;
//    uint16_t dacValue = (uint16_t)((vOut / AD5627_VREF) * 4095.0f);
//    
//    if (dacValue > 4095) dacValue = 4095;
//    return dacValue;
//}
//
//
////--------------------------------------------------
//// Convert sensor value (float) to output current (mA)
////--------------------------------------------------
//float SensorTo_mA(float sensor_value, float sensor_min, float sensor_max)
//{
//    if (sensor_value < sensor_min) sensor_value = sensor_min;
//    if (sensor_value > sensor_max) sensor_value = sensor_max;
//
//    float percent = (sensor_value - sensor_min) / (sensor_max - sensor_min);
//    return 4.0f + (percent * 16.0f); // Maps to 4–20 mA
//}
//
//
//
//
//
//
////--------------------------------------------------
//// Main update function: call this regularly
////--------------------------------------------------
//void UpdateCurrentLoopOutput(uint8_t ch , float sensor_value, float sensor_min, float sensor_max)
//{
//    float current_mA = SensorTo_mA(sensor_value, sensor_min, sensor_max);
//    uint16_t dac_value = Convert_mA_to_DAC(current_mA);
//    AD5627_SetDAC(ch, dac_value);
//}



//--------------------------------------------------
// Convert desired current (4–20 mA) to DAC value
// Hardware: 0 mA at DAC=0, 22 mA at DAC=4095
//--------------------------------------------------
uint16_t Convert_mA_to_DAC(float mA)
{
    if (mA < 0.0)   mA = 0.0;
    if (mA > 22.0)  mA = 22.0;

    // Scale for 0–22 mA full DAC range
    float percent = mA / 22.0;
    return (uint16_t)(percent * 4095 );
}

//--------------------------------------------------
// Convert sensor value to desired current (4–20 mA)
//--------------------------------------------------
float SensorTo_mA(float sensor_value, float sensor_min, float sensor_max)
{
    float percent;
    float increment; 
    
    
        //if (sensor_value < sensor_min) sensor_value = sensor_min;
        if (sensor_value > sensor_max) sensor_value = sensor_max;

        percent = (sensor_value - sensor_min) / (sensor_max - sensor_min);
        increment = percent * 16.0;
        return 4.0 + increment;  // Map to 4–20 mA
    
}

//--------------------------------------------------
// Main update function
//--------------------------------------------------
void UpdateCurrentLoopOutput(uint8_t ch, float sensor_value, float sensor_min, float sensor_max)
{
    float desired_mA = SensorTo_mA(sensor_value, sensor_min, sensor_max);
    uint16_t dac_value = Convert_mA_to_DAC(desired_mA);
    AD5627_SetDAC(ch, dac_value);
}


void Set_Output_mA(uint8_t ch, float desired_mA)
{
    uint16_t dac_value = Convert_mA_to_DAC(desired_mA);
    AD5627_SetDAC(ch, dac_value);
}



//void UpdateCurrentLoopOutput(float sensor_value, float sensor_min, float sensor_max)
//{
//    float current_mA = SensorTo_mA(sensor_value, sensor_min, sensor_max);
//    uint16_t dac_value = Convert_mA_to_DAC(current_mA);
//    MCP47VB02T_SetDAC(dac_value);
//}

/*
                Example Usage

                float sensor_reading = ReadSensor(); // Your float value
                UpdateCurrentLoopOutput(sensor_reading, 0.0f, 500.0f); // Example: 0–500 ppm gas range

*/



/* [] END OF FILE */
