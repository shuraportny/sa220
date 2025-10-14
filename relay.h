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

#define TCA9534_INPUT_REG   0x00
#define TCA9534_OUTPUT_REG  0x01
#define TCA9534_POL_REG     0x02
#define TCA9534_CONFIG_REG  0x03




#define EEPROM_RELAY_ENERG_DEFAULT      0
#define EEPROM_RELAY_LATCH_DEFAULT      0
#define EEPROM_RELAY1_ACK_DEFAULT       1


#define RELAYS_NUM 4

#define RELAY1_ENRGD_DEFAULT 0
#define RELAY2_ENRGD_DEFAULT 0
#define RELAY3_ENRGD_DEFAULT 0
#define RELAY4_ENRGD_DEFAULT 1

#define RELAY1_LATCH_DEFAULT 0
#define RELAY2_LATCH_DEFAULT 0
#define RELAY3_LATCH_DEFAULT 0
#define RELAY4_LATCH_DEFAULT 0

#define RELAY1_ACK_DEFAULT 1
#define RELAY2_ACK_DEFAULT 1
#define RELAY3_ACK_DEFAULT 1
#define RELAY4_ACK_DEFAULT 1


typedef struct {
    uint8_t energ;  // normally energised
    uint8_t latched;// latched/non-lached 
    uint8_t ack;    // acknawlidegd
}S_Rly;


void TCA9534_SetRelay(uint8_t relayNum, uint8_t state);
void  TCA9534_Init(uint8_t configMask);
void TCA9534_SetAllRelays(uint8_t relayMask);
void TCA9534_WriteReg(uint8 value);

uint8_t TCA9534_GetStatus();
uint8_t TCA9534_GetReg();
uint8_t GetRelayState();
uint8_t GetRelayMask();


/* [] END OF FILE */
