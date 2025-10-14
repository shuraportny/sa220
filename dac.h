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

#define MCP47VB02T_VREF    (3.3f)    // DAC reference voltage

#define AD5627_VREF    (2.5f)    // DAC reference voltage

void AD5627_InitDAC(void);
void AD5627_SetDAC(uint8_t ch, uint16_t value12bit);
void MCP47VB02T_SetDAC(uint16_t value12bit);

uint16_t Convert_mA_to_DAC(float mA);
float SensorTo_mA(float sensor_value, float sensor_min, float sensor_max);
void UpdateCurrentLoopOutput(uint8_t ch , float sensor_value, float sensor_min, float sensor_max);
void Set_Output_mA(uint8_t ch, float desired_mA);
/* [] END OF FILE */
