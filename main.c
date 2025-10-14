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
#include <project.h>
#include <interrupts.h>
#include <stdio.h>
#include "modbus.h"
#include "runtime.h"
#include "sensor.h"
#include "display.h"
#include "relay.h"

#include "led.h"
#include "eeprom.h"
#include "rtc.h"
#include "i2c_master.h"
#include "dac.h"

volatile uint8_t runtimeMode;

extern uint8_t mySlaveAddr;
extern uint8_t deepSleep;
extern uint8_t bleConnected;
extern uint8_t bleTimerExpired;

uint8_t swPressed;
uint8_t swReleased;

extern uint32_t sTick;
extern uint8_t sendCommand;
extern uint8_t bytesIn;

uint8_t swLED =0;


extern uint8_t slaveBuf[I2CS_BUFF_SIZE];
extern uint8_t modbus_rxBuffer[MODBUS_RX_BUFFER_SIZE];


uint8_t rcvByte;

extern uint16 sleepPeriod;
extern uint8_t sleepEnable;

extern uint8_t blinker;


uint8 newM;

extern S_DateTime date_time;




void SendLog(char* str, uint32_t val){
    char buf[50];
    memset(buf, 0 , sizeof(buf));
    sprintf(buf, "%s=%d\r\n",str, val) ;
    UART2_PutString(buf);
}

int main(void)
{   
    
    /* Start system timer */
    CySysTickSetClockSource(CY_SYS_SYST_CSR_CLK_SRC_LFCLK);
    
    //new
    CySysTickSetReload(32768 - 1);  // 32768 ticks = 1 second
    
    CySysTickStart();
    CySysTickSetCallback(0,SysTick_isr );
    
    /* Start SW interrupt handler*/
    isr_SW_Start();
    isr_SW_StartEx(SW_isr);
    isr_SW2_Start();
    isr_SW2_StartEx(SW2_isr);
    
    
    /* Start I2C */
    I2C_Start();
    
    
    UART2_Start();
    MessageTimer_Start();
    isr_NewMessage_StartEx(NewMessage_isr);
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    uint8_t ctrlreg = read_RTCReg(0x0E);
    uint8_t agingreg = read_RTCReg(0x10);
    
    // BLE
    BLE_Start();
    TXEN_Write(0);

    
    isr_BLE_Start();
    isr_BLE_StartEx(BLE_isr);
    
    TLC_Init();
    AD5627_InitDAC();
    DS3231_EnableDateTime();
    
    
    Nextion_WakeUp();
    for(uint8_t i=0;i<100 ;i++){
        
        if(!blinker)
            system_SetLed(0, SYSTEM_LED_GREEN, 0, 0);
        else
            system_SetLed(0, SYSTEM_LED_OFF, 0, 0); 
        LED_Update();
        CyDelay(50);
    }
    InitSystem();
    
    /* Start UART */
    ModbusUART_Start();
    isr_ModbusByteIn_StartEx(ModbusByteIn_isr);
    
    
    
      
    
        // TEST  AREA===========================
    extern uint8_t newMode;
    
    // TEST  AREA===========================
//    newMode=1;
//    LoadSensorInfo(0);
//    LoadSensorInfo(1);

//    UpdateCurrentLoopOutput(0, -2, 0, 10);
//    for(;;){  
//        system_SetLed(1 , SYSTEM_LED_MAGENTA, 2, 0);
//        LED_Update();
//        runtimeAlarmConfig();
//        CyDelay(50);
//    }
   
    // END TEST AREA========================

    
    
    //SysWdtInit();
    
    setMode(RUNTIME_POWERUP_MODE);
    
    
    for(;;)
    {   
        switch(runtimeMode){
            case RUNTIME_POWERUP_MODE:
                runtimePowerUp();
            break;
            case RUNTIME_REALTIME_MODE1:
                runtimeRealTime();
            break;
            case RUNTIME_REALTIME_MODE2:
                runtimeRealTime();
            break;
                
            case RUNTIME_ZEROCAL_MODE1:
                runtimeZeroCal(0);
            break;    
            case RUNTIME_ZEROCAL_MODE2:
                runtimeZeroCal(1);
            break;
            case RUNTIME_SPANCAL_MODE1:
                runtimeSpanCal(0);
            break;    
            case RUNTIME_SPANCAL_MODE2:
                runtimeSpanCal(1);
            break; 
            case RUNTIME_SETTINGS_MODE:
                runtimeSesttings();
            break;
            case RUNTIME_RELAYCONF_MODE:
                runtimeRelayConfig();
            break; 
            case RUNTIME_ALARMCONF_MODE:
                runtimeAlarmConfig();
            break; 
            case RUNTIME_DATETIME_MODE:
                runtimeDateTime();
            break;    
            case RUNTIME_POLLRATE_MODE:
                runtimePollRate();
            break;
            case RUNTIME_SLEEPCONF_MODE:
                runtimeSleepTime();
            break;    
            case RUNTIME_MODBUS_MODE:
                runtimeModbusAddress();
            break;
            case RUNTIME_GPS_MODE:
                runtimeGps();
            break;    
            case RUNTIME_LOG_MODE:
                runtimeLog();
            break;
            case RUNTIME_INFO_MODE:
                runtimeInfo();
            break;
                
              
        }
    }// for
}//main



/* [] END OF FILE */
