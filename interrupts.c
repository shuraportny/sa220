 /* ========================================
 *
 * Copyright Sam Walsh, 2014
 * All Rights Reserved.
 *
 * Interrupts.c Contains all the interrupts, put your own in here..
 *
 * ========================================
*/
#include <project.h>
#include <cydevice_trm.h>
#include <CyLib.h>
#include <interrupts.h>
#include <ModbusUART.h>
#include "modbus.h"
#include "runtime.h"


#define SW_PRESSED    0x00
#define SW_RELEASED   0x01


//#define BTN_NONE    0x03
//#define BTN_RIGHT   0x02
//#define BTN_LEFT    0x01

uint32_t msTick = 0;
uint32_t sTick = 0; 

uint32_t blinkerTick = 0;
uint32_t strobeTick = 0;
extern uint8_t blinker;
extern uint8_t strobe;

extern uint8_t rcvByte;
extern uint8_t bytesIn;
extern uint8_t modbusRequestReceived;
extern uint8_t modbus_rxBuffer[MODBUS_RX_BUFFER_SIZE];

extern uint8_t SW1_Pressed;
extern uint8_t SW2_Pressed;
extern uint8_t SW1_2_Released;
extern uint8_t SW3_Pressed;
extern uint8_t SW4_Pressed;
extern uint8_t SW3_4_Released;



extern uint16_t i2cPeriod;
extern uint8_t startI2CRead;
extern uint8_t startI2CWrite;



volatile uint8_t deepSleep;
extern uint8_t mySlaveAddr,msa;
volatile uint8_t bleConnected;

extern uint8_t bleTimerExpired;

extern uint8_t updateEn;

extern uint8_t rs232RxBuffer[RS232_RX_BUFFER_SIZE];
extern uint8_t rs232RequestReceived;




//CY_ISR(RS232_isr){
//    static uint8_t byteCnt; 
//    
//    isr_RS232Rx_ClearPending();
//    rs232RxBuffer[byteCnt++] = RS232UART_GetChar(); 
//    if(byteCnt >=8){
//        byteCnt=0;
//        rs232RequestReceived=1;
//    }
//} 


CY_ISR(SysWdtIsrHandler)
{
	CySysWdtClearInterrupt(CY_SYS_WDT_COUNTER0_INT);                            /* Clear the counter interrupt */
    CySysWdtClearInterrupt(CY_SYS_WDT_COUNTER1_INT);                            /* Clear the counter interrupt */
    WdtIsr_ClearPending();                                                      /* Clear WDT interrupt */
}


extern uint8 newM;

CY_ISR(NewMessage_isr){

    newM=1;
    uint32 source = MessageTimer_GetInterruptSource();
    MessageTimer_ClearInterrupt(source);
    
}


CY_ISR(ModbusByteIn_isr) 
{  
    uint32 size;
    uint32 InterruptSource;
    
#ifdef DEEPSLEEP
    BLETimer_WriteCounter(0);
    BLETimer_Start();
#endif 
    

     if(newM){
        newM=0;
        bytesIn =0;
    }
    //size = ModbusUART_SpiUartGetRxBufferSize(); // this helps not to miss incoming bytes!
    //for(uint8 i = 0 ;i<size; i++){
        modbus_rxBuffer[bytesIn++] = ModbusUART_UartGetByte();
    //}
    
        
    //modbus_rxBuffer[bytesIn++] = ModbusUART_UartGetByte();
    MessageTimer_WriteCounter(0);
    MessageTimer_Start(); 
        
    if(deepSleep){
        //SysWdtInit(); // This Init might be temporary
        deepSleep=0;
        bytesIn =0;
        modbusRequestReceived =1;
        CyDelay(500);// This delay affects on aligning recieved bytes. If to remove it , it will never recovering from deep sleep mode
    }else{
        
        
        
        // NEW
        //if(modbus_rxBuffer[0]!=mySlaveAddr){
        //    bytesIn =0;        
        //}
        if(bytesIn>2){
            switch(modbus_rxBuffer[1]){
                case 0x3:               
                case 0x6:
                    if(bytesIn>=8)
                        modbusRequestReceived =1;
                break;
                case 0x10:
                    if(bytesIn>= modbus_rxBuffer[6]+9)
                        modbusRequestReceived =1;
                break;
                default:
                    bytesIn=0;
                break;
            }
        }
        
//        if(bytesIn>=MODBUS_COMMAND_SIZE){
//            //bytesIn =0;
//            modbusRequestReceived =1;
//        }
        

        }
    
    InterruptSource = ModbusUART_GetRxInterruptSourceMasked();    
    ModbusUART_ClearRxInterruptSource(InterruptSource);
}





CY_ISR(SW_isr)
{
    uint8 InterruptState = SW_ClearInterrupt();	
    uint8 value = SW_Read();
    
    if(InterruptState == 0x1)
	{
       SW1_Pressed = 1;
       SW1_2_Released = 0;
       SWTimer_Start(); 
	} 
    
    if (InterruptState == 0x2)  {
       SW2_Pressed = 1;
       SW1_2_Released = 0;
        SWTimer_Start();
         
    }
        
    if (value == 0x3) {
        SW1_2_Released = 1;
        
    }
    
 
    deepSleep = 0;
    
#ifdef DEEPSLEEP    
    BLETimer_Start();
#endif    

    
}   


CY_ISR(SW2_isr)
{
    uint8 InterruptState = SW2_ClearInterrupt();	
    uint8 value = SW2_Read();
    
    if(InterruptState == 0x1)
	{
       SW3_Pressed = 1;
       SW3_4_Released = 0;
       SWTimer_Start(); 
	} 
    
    if (InterruptState == 0x2)  {
       SW4_Pressed = 1;
       SW3_4_Released = 0;
        SWTimer_Start();
         
    }
        
    if (value == 0x3) {
        SW3_4_Released = 1;
        
    }
    
 
    deepSleep = 0;
    
#ifdef DEEPSLEEP    
    BLETimer_Start();
#endif    

    
}   


CY_ISR(BLE_isr)
{
    uint8 InterruptState = SW_ClearInterrupt();	
    uint8 value = SW_Read();
    static uint32_t cnt;
    
#ifdef DEEPSLEEP
    ResetBLETimer();
    
    
#endif
    
    bleTimerExpired =1;
    

    
}   





CY_ISR(SysTick_isr){
    static uint8_t pm;  
    
    msTick++;
//    if(++msTick >= 1000){    
//        msTick = 0;
//    }
    
    if(++blinkerTick>=500){
        blinkerTick=0;
        blinker ^=1;
    }
    
    if(++strobeTick>=500){
        strobeTick=0;
        strobe^=1;
        
        
    }
    
    
}


/* [] END OF FILE */
