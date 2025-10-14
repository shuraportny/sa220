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

#define  SYSTEM_LED_OFF     0x00
#define  SYSTEM_LED_RED     0x01
#define  SYSTEM_LED_GREEN   0x02
#define  SYSTEM_LED_YELLOW  0x03
#define  SYSTEM_LED_BLUE    0x04
#define  SYSTEM_LED_MAGENTA 0x05
#define  SYSTEM_LED_WHITE   0x07
#define  SYSTEM_LED_ORANGE  0x08
#define  SYSTEM_LED_CYAN    0x09







void system_SetLED( uint8 color);

uint8 TLC_ReadReg(uint8 reg);
void TLC_WriteReg(uint8 reg, uint8 value);

void TLC_Init(void);
void TLC_SetRGB(uint8 led_index, uint8 r, uint8 g, uint8 b);
void TLC_SetChannel(uint8 channel, uint8 brightness);
void TLC_UpdateAllPWM(uint8* pwm_values); // 16 bytes
//void TLC_UpdateRGBLED(RGBLED* led);

void LED_Update();
void SetLEDmode(uint8_t num , uint8_t mode, uint8_t ch2Active);
void SetLEDdouble(uint8_t num , uint8_t color, uint8_t breath);
void SetLEDsingle(uint8_t color, uint8_t breath);
void system_SetLed(uint8_t num , uint8_t color, uint8_t breath, uint8_t doubleCh);
uint8_t system_IsLEDInBreathMode(uint8_t ch);
/* [] END OF FILE */
