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

#include "runtime.h"
#include "modbus.h"
#include "sensor.h"
#include "display.h"
#include "relay.h"
#include "eeprom.h"
#include "i2c_master.h"
#include "rtc.h"
#include "dac.h"
#include <math.h>





typedef struct {
    uint8 reg;
    uint8 data[BUFFER_SIZE];
}S_I2C;

S_DateTime date_time;

extern Point circlePoints[NUM_POINTS];
extern Point circlePoints2[NUM_POINTS];

//extern S_Sensor s;
char firmwareVersion[10];
char txt[50];
char device_tag[DEVICE_TAG_LENGTH];

uint8_t ch2Active=0;;


uint8_t runtimeMode;
uint8_t newMode;

uint8_t sensor[2][DATA_LENGTH];
float  sysReading[2];
S_Static sensor_status[2];

extern S_Rly relay[4];
uint8_t relays_state;

//uint8_t sensor2[DATA_LENGTH];
extern uint8_t modbus_rxBuffer[MODBUS_RX_BUFFER_SIZE];
extern uint8_t modbus_txBuffer[MODBUS_TX_BUFFER_SIZE];
extern char settings_obj[DISP_OBJ_SETTINGS_NUM][10];
extern char relay_obj[DISP_OBJ_RLY_NUM][10];
extern char alarm_obj[DISP_OBJ_ALARM_NUM][10];
extern char date_time_obj[DISP_OBJ_DATETIME_NUM][10];
extern char modbus_obj[DISP_OBJ_MODBUS_NUM][10];
extern char gps_obj[DISP_OBJ_GPS_NUM][10];
extern char pollrate_obj[1][10];
extern char modbusaddr_obj[3][10];
extern char warmup_obj[2][10];
extern char retry_obj[2][10];
extern char span_reset_obj[10];
extern char log_obj[DISP__OBJ_EVENT_CNT][10];
extern char display_obj_text[2][10];
extern char display_obj_status[2][10];

uint8_t rs232RxBuffer[RS232_RX_BUFFER_SIZE];
//uint8_t rs232TxBuffer[MODBUS_TX_BUFFER_SIZE];
uint8_t rs232RequestReceived=0;

int32_t latitude = -1;
int32_t longitude= -1;
float flatitude;
float flongitude;

uint8_t ack_received=0;
uint8_t reset_alarms;

uint8_t mySlaveAddr,msa;
uint16_t i2cPeriod;
uint32_t sleepPeriod;

uint8_t bytesIn;;

uint8_t startI2CRead;
uint8_t startI2CWrite;

uint8_t sendCommand=0;
uint8_t deepSleep;
uint8_t sleepEnable;
uint8_t wasSleeping=0;

uint8_t BleControl;
uint8_t bleTimerExpired;

uint16_t slavePersent; 
uint16_t sleepCnt; 

uint8_t SW1_Pressed;
uint8_t SW2_Pressed;
uint8_t SW1_2_Released;
uint8_t SW3_Pressed;
uint8_t SW4_Pressed;
uint8_t SW3_4_Released;


uint8_t prev_status[2];
uint8_t prev_alarm_status[2];
uint8_t alarm_status[2];
uint8_t relay_status;

uint32_t last_rtc_time = 0;
uint32_t last_relay_time = 0;
uint32_t last_led_time = 0;
uint32_t last_display_time = 0;
uint32_t last_sensor_time = 0;
extern uint32_t msTick; 

uint8_t blinker=0;
uint8_t strobe=0;

uint8  arr[DATA_LENGTH]; // input I2C buffer
S_I2C  txarr; // output I2C data structure      

char log_record[LOG_MAX_ENTRIES][LOG_TXT_LENGTH];
uint8_t log_last_entry_idx; // last record
char log_txt[LOG_TXT_LENGTH];

uint8_t ef1=1;
uint8_t ef2=1;

uint8_t newEvent;

int16 iReading;
float rfReading;
uint32 as_found;

uint8_t alarm_mask[2]={0};

uint8_t relay_present;

uint8_t BLE_ch;


void Test_Increment(uint8 reg , uint8 bytes_to_send){
    txarr.reg=reg;
    for(uint8 i =0 ;i<bytes_to_send;i++ ){
         txarr.data[i]+=1;
    }
    CyDelay(1000);
    slave_WriteDataRaw(0, bytes_to_send, &txarr.reg);
}



void InitSystem(){
    char text[10];
    static char gas_type[12];
    static char str[12];
    
    RTC_RST_Write(1);
    CyDelay(100);
    
    mySlaveAddr = EEPROM_GetMyAddress();
    msa = mySlaveAddr;
    i2cPeriod = EEPROM_GetI2CPeriod();
    
    GetDeviceTag(device_tag);
    if(device_tag[0]==0xFF)
        strcpy(device_tag, "SAFEGUARD ANALYTICS");
    
#ifndef SINGLE_SENSOR_SYSTEM    
    ch2Active = EEPROM_GetCh2Activity();
#else
    ch2Active=0;
#endif    
    
    relay_present = TCA9534_GetStatus();
    
    #ifdef DEEPSLEEP
    sleepPeriod = EEPROM_GetSleepPeriod();    
    //sleepEnable = (sleepPeriod > 0)? 1:0;
    BLETimer_Start();
    BLETimer_Stop();
    SetSleepPeriod(sleepPeriod);
    BLETimer_Start();
    #endif
    
    sensor[0][DEVTYPEMSB] = 0;
    sensor[0][DEVTYPELSB] = DEV_TYPE;
    
    sensor[1][DEVTYPEMSB] = 0;
    sensor[1][DEVTYPELSB] = DEV_TYPE;
    
    
    strncpy(firmwareVersion, FIRMWARE_VISION, 10);
    
    char SysSerialNumber[] = "11111111"; 
    uint32 SensorSerialNumber = 0x123456; 
    
    
    memset(gas_type , 0 , sizeof(gas_type));
    
    //Load Sensors Info HERE
    if(LoadSensorInfo(0)!=POLL_SENSOR_STATUS_ABSENT){
        RecalculateAlarms(0);
        decodeLastCalDate(0);
        sensor_status[0].fullScaleRange =   (uint32_t)(sensor[0][FULLSCALERANGE1]<<24)|
                                            (uint32_t)(sensor[0][FULLSCALERANGE2]<<16)|
                                            (uint32_t)(sensor[0][FULLSCALERANGE3]<<8)|
                                            (uint32_t)sensor[0][FULLSCALERANGE4];
        memset(str , 0 , sizeof(str));
        remove_trailing_spaces(str, (char*) &sensor[0][COMPAUND1]);
        strncat(gas_type, str, strlen(str));
    }else{
        strcat(gas_type, "?");
    }
    
    
    if(ch2Active){
        if(LoadSensorInfo(1)!=POLL_SENSOR_STATUS_ABSENT){
            RecalculateAlarms(1);
            decodeLastCalDate(1);
            sensor_status[1].fullScaleRange =   (uint32_t)(sensor[1][FULLSCALERANGE1]<<24)|
                                                (uint32_t)(sensor[1][FULLSCALERANGE2]<<16)|
                                                (uint32_t)(sensor[1][FULLSCALERANGE3]<<8)|
                                                (uint32_t)sensor[1][FULLSCALERANGE4];
            memset(str , 0 , sizeof(str)); 
            strcat(gas_type, "-");
            memset(str , 0 , sizeof(str));
            remove_trailing_spaces(str, (char*) &sensor[1][COMPAUND1]);
            strncat(gas_type, str, strlen(str));
        }else{
            strcat(gas_type, "-");
            strcat(gas_type, "?");
        }
    }
    
    
    
    // Get Relays Settings
    EEPROM_GetRelaySetting((uint8_t*)relay);
    if(relay[0].energ>1)relay[0].energ = RELAY1_ENRGD_DEFAULT;
    if(relay[1].energ>1)relay[1].energ = RELAY2_ENRGD_DEFAULT;
    if(relay[2].energ>1)relay[2].energ = RELAY3_ENRGD_DEFAULT;
    if(relay[3].energ>1)relay[3].energ = RELAY4_ENRGD_DEFAULT;
    
    if(relay[0].latched>1)relay[0].latched = RELAY1_LATCH_DEFAULT;
    if(relay[1].latched>1)relay[1].latched = RELAY2_LATCH_DEFAULT;
    if(relay[2].latched>1)relay[2].latched = RELAY3_LATCH_DEFAULT;
    if(relay[3].latched>1)relay[3].latched = RELAY4_LATCH_DEFAULT;
    
    if(relay[0].ack>1)relay[0].ack = RELAY1_ACK_DEFAULT;
    if(relay[1].ack>1)relay[1].ack = RELAY2_ACK_DEFAULT;
    if(relay[2].ack>1)relay[2].ack = RELAY3_ACK_DEFAULT;
    if(relay[3].ack>1)relay[3].ack = RELAY4_ACK_DEFAULT;
    
    TCA9534_Init(0xF0); // Init realys control chip outputs
    Reset_relays();
    
    
    
    EEPROM_GetGPS(&flatitude,  &flongitude);
    
    
    // Get Time
    DS3231_GetDateTime(&date_time);
    
    
    
    

    // Get Log Entries
    //GetLog((char*) &log_record);
    
    log_last_entry_idx= LoadLogIndex()+1;
    for(uint8_t i=0;i<LOG_MAX_ENTRIES;i++ )
        GetLogEntry((log_last_entry_idx+i)%LOG_MAX_ENTRIES, log_record[i]);
        
    
    
    
//    // UPDATE TEXT in DISPLY INFO PAGE
//    Nextion_SetTextIncognito(PAGE_INFO, DISP_OBJ_CH1 , "SFDGFHGHHJHJGHKGJLKJGL");
//    CyDelay(50);
//    Nextion_SetTextIncognito(PAGE_INFO, DISP_OBJ_CH2 , "fgfdgsfgfgsfgfgsfgsfg");
//    CyDelay(50);
//    
//    
    
    
    
    
    char BleName[50];
    memset(BleName, 0 , sizeof(BleName));
    strcpy( BleName, "SA-220-" );
    strncat(BleName, gas_type,strlen(gas_type) );
    strcat(BleName, " ");
    
    strncat(BleName, device_tag,strlen(device_tag) );
    //strcat(BleName, " ");
    //strncat(BleName, (char*)&sensor[0][SERIALNUMBER1],6 );
    
    
    CyBle_GapSetLocalName( BleName );
    CyBle_DissSetCharacteristicValue(CYBLE_DIS_SYSTEM_ID, 8, (uint8 *)SysSerialNumber);
    memset(str , 0 , sizeof(str));
    strncpy(&str[2], (char*)&sensor[0][SERIALNUMBER1],6);
    CyBle_DissSetCharacteristicValue(CYBLE_DIS_SYSTEM_ID, 8, (uint8 *)&str);
    CyBle_DissSetCharacteristicValue(CYBLE_DIS_FIRMWARE_REV, 10, (uint8 *)FIRMWARE_VISION);
    CyBle_DissSetCharacteristicValue(CYBLE_DIS_SOFTWARE_REV, 10, (uint8 *)FIRMWARE_VISION);
    CyBle_DissSetCharacteristicValue(CYBLE_DIS_SERIAL_NUMBER, 12, (uint8 *)&sensor[0][SERIALNUMBER1]);
    CyBle_DissSetCharacteristicValue(CYBLE_DIS_HARDWARE_REV, 4, (uint8 *)HARDWARE_VISION);
    
    
    
}


void RecalculateAlarms(uint8_t ch){
    sensor_status[ch].fullScaleRange = ARRtoINT32(sensor[ch],FULLSCALERANGE1);
    sensor_status[ch].alarm1val = ARRtoINT16(sensor[ch],ALARM1MSB);
    sensor_status[ch].alarm2val = ARRtoINT16(sensor[ch],ALARM2MSB);
    sensor_status[ch].alarm3val = ARRtoINT16(sensor[ch],ALARM3MSB);
    sensor_status[ch].alarm1asc = ARRtoINT16(sensor[ch],ALARM1ASCMSB);
    sensor_status[ch].alarm2asc = ARRtoINT16(sensor[ch],ALARM2ASCMSB);
    sensor_status[ch].alarm3asc = ARRtoINT16(sensor[ch],ALARM3ASCMSB);
    sensor_status[ch].alarm1fval =  sensor_status[ch].fullScaleRange * (sensor_status[ch].alarm1val/10000.0); // alarm1val/100/100%
    sensor_status[ch].alarm2fval =  sensor_status[ch].fullScaleRange * (sensor_status[ch].alarm2val/10000.0);
    sensor_status[ch].alarm3fval =  sensor_status[ch].fullScaleRange * (sensor_status[ch].alarm3val/10000.0);
}

//uint8_t EEPROM_GetMyAddress(void){
//    uint8_t myaddr;
//    myaddr =  i2c_Read_8(EEPROM_I2C_ADDRESS,EEPROM_MY_ADDRESS_REG );
//    CyDelay(50);
//    if(myaddr==0xFF)
//        myaddr = MODBUS_MY_DEFAULT_ADDRESS;
//    return myaddr;
//}
//
//void EEPROM_SetMyAddress(uint8_t addr){
//    i2c_write_8(EEPROM_I2C_ADDRESS,EEPROM_MY_ADDRESS_REG,addr);
//    CyDelay(100);
//}
//
//uint16_t EEPROM_GetI2CPeriod(void){
//    uint16_t i2c_period;
//    i2c_period =  (i2c_Read_8(EEPROM_I2C_ADDRESS,EEPROM_I2C_PERIOD_MSB_REG ))<<8;
//    CyDelay(50);
//    i2c_period +=  i2c_Read_8(EEPROM_I2C_ADDRESS,EEPROM_I2C_PERIOD_LSB_REG );
//    CyDelay(50);
//    if((i2c_period==0xFFFF)||((i2c_period<MINIMAL_DEFAULT_I2C_PERIOD)))
//        i2c_period = MINIMAL_DEFAULT_I2C_PERIOD;
//    return i2c_period;
//}
//
//void EEPROM_SetI2CPeriod(uint16_t period){
//    i2c_write_8(EEPROM_I2C_ADDRESS,EEPROM_I2C_PERIOD_MSB_REG,(period>>8)&0xFF);
//    CyDelay(100);
//    i2c_write_8(EEPROM_I2C_ADDRESS,EEPROM_I2C_PERIOD_LSB_REG, period & 0xFF );
//    CyDelay(100);
//}
//
//
//uint16_t EEPROM_GetSleepPeriod(void){
//    uint16_t sleep_period;
//    sleep_period =  (i2c_Read_8(EEPROM_I2C_ADDRESS,EEPROM_SLEEP_PERIOD_MSB_REG ))<<8;
//    CyDelay(50);
//    sleep_period +=  i2c_Read_8(EEPROM_I2C_ADDRESS,EEPROM_SLEEP_PERIOD_LSB_REG );
//    CyDelay(50);
//    if(sleep_period==0xFFFF)
//        sleep_period = DEFAULT_SLEEP_PERIOD;
//    return sleep_period;
//}
//
//void EEPROM_SetSleepPeriod(uint16_t period){
//    i2c_write_8(EEPROM_I2C_ADDRESS,EEPROM_SLEEP_PERIOD_MSB_REG,(period>>8)&0xFF);
//    CyDelay(100);
//    i2c_write_8(EEPROM_I2C_ADDRESS,EEPROM_SLEEP_PERIOD_LSB_REG, period & 0xFF );
//    CyDelay(100);
//    
//}
//
//uint8_t EEPROM_GetCh2Activity(void){
//    uint16_t ch2Active;
//    ch2Active =  i2c_Read_8(EEPROM_I2C_ADDRESS,EEPROM_CH2_ACTIVE_REG );
//    CyDelay(50);
//    if(ch2Active==0xFF)
//        ch2Active = CHANNEL2_ACTIVE_DEFAULT;
//    return ch2Active;
//
//}
//
//void EEPROM_SetCh2Activity(uint8_t active){
//    i2c_write_8(EEPROM_I2C_ADDRESS,EEPROM_CH2_ACTIVE_REG,active);
//    CyDelay(50);
//    
//}
//
//uint8_t EEPROM_GetRelaySetting(uint8_t reg){
//    uint16_t ch2Active;
//    ch2Active =  i2c_Read_8(EEPROM_I2C_ADDRESS,reg );
//    CyDelay(50);
//    if(ch2Active==0xFF)
//        ch2Active = 0;
//    return ch2Active;
//}
//
//void EEPROM_SetRelaySetting(uint8_t reg ,uint8_t val ){
//    i2c_write_8(EEPROM_I2C_ADDRESS,reg,val);
//    CyDelay(50);
//}




int32_t LoadSensorInfo(uint8_t ch){
    uint32_t status;
   uint8_t tmp[4], tmp2[4];
    float f;
    static uint8_t prev_state[2];
    static uint8_t wait[2];
    

    sensor[ch][STATUSLSB]=POLL_SENSOR_STATUS_OK;
    SelectChannel(ch);
    status = i2c_status(I2C_SENSOR_SLAVE_ADDR);
    //memset(sensor, 0 , sizeof(sensor));    
    if(status !=I2C_I2C_MSTR_NO_ERROR){
        sensor[ch][STATUSLSB]=POLL_SENSOR_STATUS_ABSENT;
        prev_state[ch]=POLL_SENSOR_STATUS_ABSENT;
        wait[ch]=0;
        status = i2c_status(I2C_RTC_SLAVE_ADDR);
            if(status !=I2C_I2C_MSTR_NO_ERROR){
                // If other memer doesnt respond
                // Reset I2C peripheral . It helps to recover I2C bus
                I2C_Recover();
            }
        Set_Output_mA(ch,0);  
        status= POLL_SENSOR_STATUS_ABSENT;
    }
    else{
        sensor[ch][STATUSLSB]=POLL_SENSOR_STATUS_OK;
        
        
        tmp[3]=sensor[ch][SYSREADING1];
        tmp[2]=sensor[ch][SYSREADING2];
        tmp[1]=sensor[ch][SYSREADING3];
        tmp[0]=sensor[ch][SYSREADING4];
            
        memcpy( &f, tmp,4);
        
                
        // Here we get all sensor data
        slave_ReadDataRaw(0, (DATA_LENGTH-4) ,&sensor[ch][SYSREADING1] );
        sensor_status[ch].fullScaleRange =   (uint32_t)(sensor[ch][FULLSCALERANGE1]<<24)|
                                            (uint32_t)(sensor[ch][FULLSCALERANGE2]<<16)|
                                            (uint32_t)(sensor[ch][FULLSCALERANGE3]<<8)|
                                            (uint32_t)sensor[ch][FULLSCALERANGE4];
        if(sensor[ch][STATUSLSB]==RUNTIME_SENSOR_IN_WARMUP)
            Set_Output_mA(ch,4);
        else if(sensor[ch][STATUSLSB]==RUNTIME_SENSOR_IN_CALIBRATION){
            Set_Output_mA(ch,2);
        }else if (sensor[ch][STATUSLSB]==RUNTIME_SENSOR_CAL_FAULT){
            Set_Output_mA(ch, 0);        
        }
        else{
            UpdateCurrentLoopOutput(ch , f , 0 , sensor_status[ch].fullScaleRange);
        }
        
        
        // for old 
        if(ch==0){
            
            memcpy(&rfReading ,&f, 4 );
        }
                
        iReading = (int16)f;
        // AS FOUND 
        memcpy(&as_found, &sensor[0][SPANASFOUND1], 4);
        
        if( prev_state[ch]==POLL_SENSOR_STATUS_ABSENT){
            wait[ch]++;
            if(wait[ch]>20){
                wait[ch]=0;
                prev_state[ch]=POLL_SENSOR_STATUS_OK;
                
                    ReloadSensorGasType(ch);
            }
        }
        
            
    }
     
    
    
    // SIMULATION
//    static float sim,sim2;
//    static int8_t dir, dir2;
//    
//    
//    if(sim<15) dir=1;
//    else if (sim>25) dir =0;
//    
//    if(sim2<0) dir2=1;
//    else if (sim2>5) dir2 =0;
//    
//    if(dir)
//        sim+=0.1;
//    else
//        sim-=0.1;
//    
//    if(dir2)
//        sim2+=0.1;
//    else
//        sim2-=0.1;
//    
//    
//    memcpy(tmp, &sim, sizeof(float));
//    memcpy(tmp2, &sim2, sizeof(float));
//    
//    sensor[0][SYSREADING1]=tmp[3];
//    sensor[0][SYSREADING2]=tmp[2];
//    sensor[0][SYSREADING3]=tmp[1];
//    sensor[0][SYSREADING4]=tmp[0];
//        
//    sensor[1][SYSREADING1]=tmp2[3];
//    sensor[1][SYSREADING2]=tmp2[2];
//    sensor[1][SYSREADING3]=tmp2[1];
//    sensor[1][SYSREADING4]=tmp2[0];
//    
//    
//    if((sensor[ch][STATUSLSB]!=RUNTIME_SENSOR_IN_CALIBRATION)&&
//       (sensor[ch][STATUSLSB]!=RUNTIME_SENSOR_IN_WARMUP)&&
//       (sensor[ch][STATUSLSB]!=RUNTIME_SENSOR_CAL_FAULT)&&
//        (sensor[ch][STATUSLSB]!=POLL_SENSOR_STATUS_ABSENT)    
//    ){
//        UpdateCurrentLoopOutput(0 , sim, 0 , sensor_status[0].fullScaleRange);
//        UpdateCurrentLoopOutput(1 , sim2, 0 , sensor_status[2].fullScaleRange);
//    }
//    
//    if(sensor[ch][STATUSLSB]==POLL_SENSOR_STATUS_ABSENT)
//        Set_Output_mA(ch,0);
//    if(sensor[ch][STATUSLSB]==RUNTIME_SENSOR_IN_WARMUP)
//        Set_Output_mA(ch,4);
 // SIMULATION END       
        
    return status;
}


void runtime_PollSensor(void){
    
    //uint8 data = i2c_Read_8( I2C_SLAVE_ADDR,  I2C_SYS_READING_OFFSET );
    slave_ReadDataRaw(0, 20 ,&sensor[0][SYSREADING1] );
    return;    
}

void runtime_CtrlSensor(uint8_t ch , uint8_t reg, uint16 data){
    SelectChannel(ch);
    //CyDelay(50);
    i2c_write_16(I2C_SENSOR_SLAVE_ADDR ,reg ,  data);
    //CyDelay(50);
    return;
}

void runtime_UpdateSensor(uint8_t ch , uint8_t reg, uint16 data){
    SelectChannel(ch);
    //CyDelay(50);
    i2c_write_16(I2C_SENSOR_SLAVE_ADDR ,reg ,  data);
    //CyDelay(50);
    return;
}





void runtime_ProcessRequest(void){
    uint16_t regsToSend;
    uint16_t  startRegister;
    uint8_t  modbusCommand;
    uint16_t data;
     uint8_t dev; 
    // Is this my packet?
    if( (mySlaveAddr != modbus_rxBuffer[0]) && (modbus_rxBuffer[0]!= MODBUS_BROADCAST_ADDRESS) ) {   // mySlaveAddr is initialized in initSensor() now
        bytesIn =0;
        return;
    }    
    
    uint16 crc;
    uint16 recvdcrc = (modbus_rxBuffer[bytesIn-2]<<8)+modbus_rxBuffer[bytesIn-1];
    crc = (uint16) modbus_GenerateCRC(modbus_rxBuffer,bytesIn); // bytesIn
    if(crc!=recvdcrc){
        bytesIn =0;
        return;
    } 
    
    
    modbusCommand = modbus_rxBuffer[1];
    
    // Broadcast request for My Slave Address
    if(modbus_rxBuffer[0]==MODBUS_BROADCAST_ADDRESS){
        if ( MODBUS_READ_CMD == modbusCommand){
            startRegister = modbus_rxBuffer[3];
            regsToSend = modbus_rxBuffer[5];
            modbus_txBuffer[3] = 0;
            modbus_txBuffer[4] = mySlaveAddr;
            modbusSendBulk(MODBUS_BROADCAST_ADDRESS , modbusCommand , regsToSend);     
        }
        bytesIn =0;
        return;
    }
    
    
    
    dev = modbus_rxBuffer[3]/100;
    startRegister = modbus_rxBuffer[3]%100;
        
    
    if ( MODBUS_READ_CMD == modbusCommand){
        // Send Read Response to Modbus Master
        bytesIn =0;
        
        //dev = modbus_rxBuffer[3]/100;
        //startRegister = modbus_rxBuffer[3]%100;
        regsToSend = modbus_rxBuffer[5];
        uint8_t idx=3;
        uint8_t byteCount = regsToSend;
        uint8_t lat[4];
        uint8_t lon[4];
        memcpy(lat, &flatitude, 4);
        memcpy(lon, &flongitude, 4);
        uint8_t sensors;
        uint8_t alarm;
        
        
        
        if(!ch2Active){
            if(sensor[0][STATUSLSB]==POLL_SENSOR_STATUS_ABSENT)
                sensors =0;
            else
                sensors =1;
        }
        else{
            if(sensor[1][STATUSLSB]==POLL_SENSOR_STATUS_ABSENT)
                sensors =1;
            else
                sensors =2;
        }
        
        if((alarm_status[0]==0)&&(alarm_status[1]==0))alarm=0;
        if((alarm_status[0]==4)||(alarm_status[1]==4))alarm=4;
        if((alarm_status[0]==1)||(alarm_status[1]==1))alarm=1;
        if((alarm_status[0]==2)||(alarm_status[1]==2))alarm=2;
        if((alarm_status[0]==3)||(alarm_status[1]==3))alarm=3;
        
        
        if(dev==0){
            
            switch(startRegister){
                case 0:
                    modbus_txBuffer[idx++] = 0 ;
                    modbus_txBuffer[idx++] = sensor[0][DEVTYPELSB] ;
                    if((--byteCount)==0) break;
                    
                case 1:
                    modbus_txBuffer[idx++] = 0;
                    modbus_txBuffer[idx++] = sensors;//number of sensors
                    if((--byteCount)==0) break;
                case 2:
                    modbus_txBuffer[idx++] = 0;
                    modbus_txBuffer[idx++] = mySlaveAddr;
                    if((--byteCount)==0) break;
                case 3:    
                    modbus_txBuffer[idx++] = (sleepPeriod>>8)&0xFF;
                    modbus_txBuffer[idx++] = sleepPeriod&0xFF;
                    if((--byteCount)==0)break;
                    
                case 4:    
                    modbus_txBuffer[idx++] = 0;
                    modbus_txBuffer[idx++] = 0;
                    if((--byteCount)==0)break;
                case 5:    
                    modbus_txBuffer[idx++] = lat[3];
                    modbus_txBuffer[idx++] = lat[2];
                    if((--byteCount)==0)break; 
                case 6:    
                    modbus_txBuffer[idx++] = lat[1];
                    modbus_txBuffer[idx++] = lat[0];
                    if((--byteCount)==0)break;
                case 7:    
                    modbus_txBuffer[idx++] = lon[3];
                    modbus_txBuffer[idx++] = lon[2];
                    if((--byteCount)==0)break;  
                case 8:    
                    modbus_txBuffer[idx++] = lon[1];
                    modbus_txBuffer[idx++] = lon[0];
                    if((--byteCount)==0)break;      
                case 9:    
                    modbus_txBuffer[idx++] = 0;
                    modbus_txBuffer[idx++] = relays_state;//alarm;
                    if((--byteCount)==0)break;    
                case 10:    
                    modbus_txBuffer[idx++] = 0;
                    modbus_txBuffer[idx++] = date_time.month;
                    if((--byteCount)==0)break;
                case 11:    
                    modbus_txBuffer[idx++] = 0;
                    modbus_txBuffer[idx++] = date_time.day;
                    if((--byteCount)==0)break;   
                case 12:    
                    modbus_txBuffer[idx++] = 0;
                    modbus_txBuffer[idx++] = date_time.year;
                    if((--byteCount)==0)break;
                case 13:    
                    modbus_txBuffer[idx++] = 0;
                    modbus_txBuffer[idx++] = date_time.hours;
                    if((--byteCount)==0)break;
                 case 14:    
                    modbus_txBuffer[idx++] = 0;
                    modbus_txBuffer[idx++] = date_time.minutes;
                    break;   
                    
            }
            modbusSendBulk(mySlaveAddr , modbusCommand , regsToSend); 
        }
        else{
            for(uint16 i=0;i<regsToSend*2;i++){
                modbus_txBuffer[3+i] = sensor[dev-1][startRegister*2+2+i];
            }
            modbusSendBulk(mySlaveAddr , modbusCommand , regsToSend); 
            
        } 
    }
    
    
    
    
    
    if ( MODBUS_WRITE_CMD == modbusCommand){
        bytesIn =0;
        
        // send Write response
        //dev = modbus_rxBuffer[3]/100;
        //startRegister = modbus_rxBuffer[3]%100;
        data = modbus_rxBuffer[4];   // high byte to write
        data = (data << 8)+ modbus_rxBuffer[5]; // low byte to write
        
        if(startRegister ==MODBUS_SYS_REMOTE_REG){
            if(dev>0)
                runtime_CtrlSensor(dev-1 , SENSOR_SYS_REMOTE_REG, data);   
        }
        if(startRegister ==MODBUS_ALARM1_REG){
            if(dev>0){
                runtime_CtrlSensor(dev-1 , SENSOR_ALARM1_REG, data);   
                runtime_CtrlSensor(dev-1 , SENSOR_SYS_REMOTE_REG, RUNTIME_SMART_SENSOR_SYS_REMOTE_SAVE_TO_EEPROM);
                sensor_status[dev-1].alarm1val= data;
                sensor_status[dev-1].alarm1fval =  sensor_status[dev-1].fullScaleRange * (sensor_status[dev-1].alarm1val/10000.0);
                
            }
        }
        if(startRegister ==MODBUS_ALARM1_ASC_REG){
            if(dev>0){
                runtime_CtrlSensor(dev-1 , SENSOR_ALARM1_ASC_REG, data);   
                runtime_CtrlSensor(dev-1 , SENSOR_SYS_REMOTE_REG, RUNTIME_SMART_SENSOR_SYS_REMOTE_SAVE_TO_EEPROM);
                sensor_status[dev-1].alarm1asc = ARRtoINT16(sensor[dev-1],ALARM1ASCMSB);
                //UpdateSensorStatus(dev-1);
                
            }
        }
        if(startRegister ==MODBUS_ALARM2_REG){
            if(dev>0){
                runtime_CtrlSensor(dev-1 , SENSOR_ALARM2_REG, data);   
                runtime_CtrlSensor(dev-1 , SENSOR_SYS_REMOTE_REG, RUNTIME_SMART_SENSOR_SYS_REMOTE_SAVE_TO_EEPROM);
                sensor_status[dev-1].alarm2val= data;
                sensor_status[dev-1].alarm2fval =  sensor_status[dev-1].fullScaleRange * (sensor_status[dev-1].alarm2val/10000.0);
                
            }
        }
        if(startRegister ==MODBUS_ALARM2_ASC_REG){
            if(dev>0){
                runtime_CtrlSensor(dev-1 , SENSOR_ALARM2_ASC_REG, data);  
                runtime_CtrlSensor(dev-1 , SENSOR_SYS_REMOTE_REG, RUNTIME_SMART_SENSOR_SYS_REMOTE_SAVE_TO_EEPROM);
                sensor_status[dev-1].alarm1asc = ARRtoINT16(sensor[dev-1],ALARM1ASCMSB);
                //UpdateSensorStatus(dev-1);
            }
        }
        if(startRegister ==MODBUS_ALARM3_REG){
            if(dev>0){
                runtime_CtrlSensor(dev-1 , SENSOR_ALARM3_REG, data);  
                runtime_CtrlSensor(dev-1 , SENSOR_SYS_REMOTE_REG, RUNTIME_SMART_SENSOR_SYS_REMOTE_SAVE_TO_EEPROM);
                sensor_status[dev-1].alarm3val= data;
                sensor_status[dev-1].alarm3fval =  sensor_status[dev-1].fullScaleRange * (sensor_status[dev-1].alarm3val/10000.0);
            }
        }
        if(startRegister ==MODBUS_ALARM3_ASC_REG){
            if(dev>0){
                runtime_CtrlSensor(dev-1 , SENSOR_ALARM3_ASC_REG, data);   
                runtime_CtrlSensor(dev-1 , SENSOR_SYS_REMOTE_REG, RUNTIME_SMART_SENSOR_SYS_REMOTE_SAVE_TO_EEPROM);
                sensor_status[dev-1].alarm1asc = ARRtoINT16(sensor[dev-1],ALARM1ASCMSB);
                //UpdateSensorStatus(dev-1);
            }
        }
        if(startRegister ==MODBUS_CAL_LEVEL_REG){
            if(dev>0){
                runtime_CtrlSensor(dev-1 , SENSOR_CAL_LEVEL_REG, data);  
                runtime_CtrlSensor(dev-1 , SENSOR_SYS_REMOTE_REG, RUNTIME_SMART_SENSOR_SYS_REMOTE_SAVE_TO_EEPROM);
                //UpdateSensorStatus(dev-1);
            }
        }
        if(startRegister ==MODBUS_RESP_FACTOR_REG){
            if(dev>0){
                runtime_CtrlSensor(dev-1 , SENSOR_RESP_FACTOR_REG, data);  
                runtime_CtrlSensor(dev-1 , SENSOR_SYS_REMOTE_REG, RUNTIME_SMART_SENSOR_SYS_REMOTE_SAVE_TO_EEPROM);
                //UpdateSensorStatus(dev-1);
            }
        }
        if(startRegister ==MODBUS_ZERO_CAL_DATE_REG){
            if(dev>0){
                runtime_CtrlSensor(dev-1 , SENSOR_ZER_CAL_DATE_REG, data);  
                runtime_CtrlSensor(dev-1 , SENSOR_SYS_REMOTE_REG, RUNTIME_SMART_SENSOR_SYS_REMOTE_SAVE_TO_EEPROM);
            }
#ifndef SINGLE_SENSOR_SYSTEM                   
                    sprintf(log_txt,"%02d/%02d/%02d at %02d:%02d CH%d ZERO CAL",date_time.month, date_time.day, date_time.year, date_time.hours, date_time.minutes, dev);
#else
                    sprintf(log_txt,"%02d/%02d/%02d at %02d:%02d ZERO CAL",date_time.month, date_time.day, date_time.year, date_time.hours, date_time.minutes);
#endif
                SaveLogEntry(&log_last_entry_idx, log_txt);
        }
        if(startRegister ==MODBUS_SPAN_CAL_DATE_REG){
            if(dev>0){
                runtime_CtrlSensor(dev-1 , SENSOR_SPAN_CAL_DATE_REG, data);  
                runtime_CtrlSensor(dev-1 , SENSOR_SYS_REMOTE_REG, RUNTIME_SMART_SENSOR_SYS_REMOTE_SAVE_TO_EEPROM);
            }
#ifndef SINGLE_SENSOR_SYSTEM                   
                    sprintf(log_txt,"%02d/%02d/%02d at %02d:%02d CH%d SPAN CAL",date_time.month, date_time.day, date_time.year, date_time.hours, date_time.minutes, dev);
#else
                    sprintf(log_txt,"%02d/%02d/%02d at %02d:%02d ZERO CAL",date_time.month, date_time.day, date_time.year, date_time.hours, date_time.minutes);
#endif
                SaveLogEntry(&log_last_entry_idx, log_txt);
        }
        
        if(dev==0){
            if(startRegister ==MODBUS_ADDRESS_REG){
                // Setting My Modbus address
                mySlaveAddr = data;
                EEPROM_SetMyAddress(&mySlaveAddr);
            }

            else if (startRegister ==MODBUS_SLEEP_PERIOD_REG){
                // Setting I2C Period
                if(data<MINIMAL_SLEEP_PERIOD) 
                    data=0;
                sleepPeriod = data;
                sleepEnable = (sleepPeriod>MINIMAL_SLEEP_PERIOD)? 1:0;
                EEPROM_SetSleepPeriod(data);
                SetSleepPeriod(data);
            }
            else if (startRegister ==MODBUS_MONTH_REG){
                if((data>12)||(data==0))return;
                date_time.month=data;
                DS3231_SetDateTime(&date_time);
            }
            else if (startRegister ==MODBUS_DAY_REG){
                if(data>31)return;
                date_time.day=data;
                DS3231_SetDateTime(&date_time);
            }
            
            else if (startRegister ==MODBUS_YEAR_REG){
                if(data>99)return;
                date_time.year=data;
                DS3231_SetDateTime(&date_time);
            }
            else if (startRegister ==MODBUS_HOURS_REG){
                if(data>23)return;
                date_time.hours=data;
                DS3231_SetDateTime(&date_time);
            }
            else if (startRegister ==MODBUS_MINUTES_REG){
                if(data>59)return;
                date_time.minutes=data;
                DS3231_SetDateTime(&date_time);
            }
        }
        // Send Response to Modbus Master
        modbusWriteResponse(mySlaveAddr, modbusCommand , startRegister, data);
    }
    
    
    
    if(MODBUS_WRITE_xREGS_CMD == modbusCommand){
        bytesIn =0;
        uint8_t tmp[4];  
        
        if(dev==0){
            if(startRegister ==MODBUS_LATITUDE_REG){
                for(uint8_t i=0;i< sizeof(float);i++)
                    tmp[3-i]=modbus_rxBuffer[7+i];
                memcpy(&flatitude, tmp, 4); 
                EEPROM_SaveGPSLat(flatitude);
            }
            if(startRegister ==MODBUS_LONGITUDE_REG){
                for(uint8_t i=0;i< sizeof(float);i++)
                    tmp[3-i]=modbus_rxBuffer[7+i];
                memcpy(&flongitude, tmp, 4); 
                EEPROM_SaveGPSLong(flongitude);
            }
            
            
        }
    }
    
    return;    
}








void GoToSleep(){
    bytesIn=0;
    deepSleep=1;
    
    
    ModbusUART_Sleep();
    system_SetLed(0, SYSTEM_LED_OFF, 0, 0);
    LED_Update();
    
    //Nextion_ClearScreen();
    //CyDelay(400);
    Nextion_Sleep();
    
    //Nextion_DeepSleep(); // Wake up from Deep Sleep can be made by touching display , it doesnt wake up with Switch or Uart
    //CyDelay(400);
    
    wasSleeping=1;
    CyDelay(5);
    CySysPmDeepSleep();
    
    ModbusUART_Wakeup();
    
    Nextion_WakeUp();
    //Nextion_DeepSleepWakeUp();
    
    GoRealtime(ch2Active);
    
    
    //RS232UART_RestoreConfig() ;
    //RS232UART_Wakeup();
    
}

void LEDShow(void){
    //LED_Write(deepSleep);
}


//uint8 BleNotifyReading;

void BLE_Start(){
    CyBle_Start(Stack_Handler);
}


void updateData(void){
    uint8_t sys[4];
    
    
    CYBLE_GATTS_HANDLE_VALUE_NTF_T tempHandle;
    
    if( CyBle_GetState() != CYBLE_STATE_CONNECTED ) return;
    
    /* Update sys gas reading value */
    sys[0] =  sensor[0][SYSREADING1];
    sys[1] =  sensor[0][SYSREADING2];
    sys[2] =  sensor[0][SYSREADING3];
    sys[3] =  sensor[0][SYSREADING4];
    
    
    
    
    /* Update sys remote control value */
    tempHandle.attrHandle = CYBLE_SGA1_SYS_REMOTE_CHAR_HANDLE;
    tempHandle.value.val = (uint8*)&System.BleControl;
    tempHandle.value.len = 1;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
    /* If notifications have been requested for remote control value */
    if( System.BleNotifyControl && ( System.BleControl != System.BleControlPrevious ) )
    {
        CyBle_GattsNotification( cyBle_connHandle, &tempHandle );               /* Notify the client */
        System.BleControlPrevious = System.BleControl;
    }
    
    
    
    tempHandle.attrHandle = CYBLE_SGA1_SYS_READING_CHAR_HANDLE;
    tempHandle.value.val= (uint8* )&sys;
    tempHandle.value.len = 4;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
    if(System.BleNotifyReading){
        CyBle_GattsNotification( cyBle_connHandle, &tempHandle );               /* Notify the client */
        Sensor.ReadingPrevious = Sensor.Reading;
    }
    
    /* Update SENSOR_STATUS value */
    tempHandle.attrHandle = CYBLE_SGA1_SYS_STATUS_CHAR_HANDLE;
    tempHandle.value.val=(uint8*)&sensor[CALSTATUSMSB];
    tempHandle.value.len = 2;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
    if(System.BleNotifyReading){
        CyBle_GattsNotification( cyBle_connHandle, &tempHandle );               /* Notify the client */
        Sensor.StatusPrevious = Sensor.Status;
    }
    
    
    /* Update SENSOR_NOTE value */
    tempHandle.attrHandle = CYBLE_SGA2_SENSOR_GAS_NOTE_CHAR_HANDLE;
    tempHandle.value.val = (uint8*)&sensor[COMPAUND1];
    tempHandle.value.len = 10;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
    
    /* Update SENSOR_PIN value */
    tempHandle.attrHandle = CYBLE_SGA2_PIN_CHAR_HANDLE;
    tempHandle.value.val = (uint8*)&sensor[UNIT1];
    tempHandle.value.len = 4;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
    // Indication of SYS REMOTE control
//    switch(BleControl){
//    case 10: LED_Write(0); LED3_Write(0); break;
//    case 20: LED_Write(0); LED3_Write(1); break;
//    default: LED_Write(1); LED3_Write(1); break;    
//    }
    
}



void updateOurBleData()
{
    uint8 testVal8 = 0;
    uint16 testVal16 = 0;
    uint32 testVal32 = 0;
    
    CYBLE_GATTS_HANDLE_VALUE_NTF_T tempHandle;
    
    uint8_t rly[4];
    uint8_t dt[5];
    char log[LOG_INFO_LENGTH];
    float fval;
    
    if( CyBle_GetState() != CYBLE_STATE_CONNECTED ) return;

    /* Update sys remote control value */
    tempHandle.attrHandle = CYBLE_SGA1_SYS_REMOTE_CHAR_HANDLE;
    tempHandle.value.val = (uint8*)&System.BleControl;//&sensor[0][SYSREMOTELSB];//&System.BleControl;
    tempHandle.value.len = 1;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
    /* If notifications have been requested for remote control value */
    if( System.BleNotifyControl && ( System.BleControl != System.BleControlPrevious ) )
    {
        CyBle_GattsNotification( cyBle_connHandle, &tempHandle );               /* Notify the client */
        System.BleControlPrevious = System.BleControl;
    }
    
    ///////////////////SELECTED CHANNEL////////////////////////
    tempHandle.attrHandle = CYBLE_SGA1_SELECTED_CHANNEL_CHAR_HANDLE;
    tempHandle.value.val = (uint8_t*)&BLE_ch;
    tempHandle.value.len = 1;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
    
////////////////////////////////////////////////////////////////////////    
    memset(rly, '\0', sizeof(rly));
    for(uint8_t i=0;i<4;i++){
        rly[i]|= (relay[i].energ<<2);
        rly[i]|= (relay[i].latched<<1);
        rly[i]|= relay[i].ack;
    }
    tempHandle.attrHandle = CYBLE_SGA1_RELAYS_CHAR_HANDLE;
    tempHandle.value.val = (uint8_t*)rly;
    tempHandle.value.len = 4;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
    
    memset(log, 0, sizeof(log));
    strcpy(log, log_record[LOG_MAX_ENTRIES-1]);
    tempHandle.attrHandle = CYBLE_SGA1_LOG1_CHAR_HANDLE;
    tempHandle.value.val = (uint8_t*)log;
    tempHandle.value.len = LOG_INFO_LENGTH;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
    
    memset(log, 0, sizeof(log));
    strcpy(log, log_record[LOG_MAX_ENTRIES-2]);
    tempHandle.attrHandle = CYBLE_SGA1_LOG2_CHAR_HANDLE;
    tempHandle.value.val = (uint8_t*)log;
    tempHandle.value.len = LOG_INFO_LENGTH;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
    
    memset(log, 0, sizeof(log));
    strcpy(log, log_record[LOG_MAX_ENTRIES-3]);
    tempHandle.attrHandle = CYBLE_SGA1_LOG3_CHAR_HANDLE;
    tempHandle.value.val = (uint8_t*)log;
    tempHandle.value.len = LOG_INFO_LENGTH;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
    
    memset(log, 0, sizeof(log));
    strcpy(log, log_record[LOG_MAX_ENTRIES-4]);
    tempHandle.attrHandle = CYBLE_SGA1_LOG4_CHAR_HANDLE;
    tempHandle.value.val = (uint8_t*)log;
    tempHandle.value.len = LOG_INFO_LENGTH;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
    
    memset(log, 0, sizeof(log));
    strcpy(log, log_record[LOG_MAX_ENTRIES-5]);
    tempHandle.attrHandle = CYBLE_SGA1_LOG5_CHAR_HANDLE;
    tempHandle.value.val = (uint8_t*)log;
    tempHandle.value.len = LOG_INFO_LENGTH;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
    
    memset(log, 0, sizeof(log));
    strcpy(log, log_record[LOG_MAX_ENTRIES-6]);
    tempHandle.attrHandle = CYBLE_SGA1_LOG6_CHAR_HANDLE;
    tempHandle.value.val = (uint8_t*)log;
    tempHandle.value.len = LOG_INFO_LENGTH;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
    
    memset(log, 0, sizeof(log));
    strcpy(log, log_record[LOG_MAX_ENTRIES-7]);
    tempHandle.attrHandle = CYBLE_SGA1_LOG7_CHAR_HANDLE;
    tempHandle.value.val = (uint8_t*)log;
    tempHandle.value.len = LOG_INFO_LENGTH;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
    
    memset(log, 0, sizeof(log));
    strcpy(log, log_record[LOG_MAX_ENTRIES-8]);
    tempHandle.attrHandle = CYBLE_SGA1_LOG8_CHAR_HANDLE;
    tempHandle.value.val = (uint8_t*)log;
    tempHandle.value.len = LOG_INFO_LENGTH;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
    
    memset(log, 0, sizeof(log));
    strcpy(log, log_record[LOG_MAX_ENTRIES-9]);
    tempHandle.attrHandle = CYBLE_SGA1_LOG9_CHAR_HANDLE;
    tempHandle.value.val = (uint8_t*)log;
    tempHandle.value.len = LOG_INFO_LENGTH;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
    
    memset(log, 0, sizeof(log));
    strcpy(log, log_record[LOG_MAX_ENTRIES-10]);
    tempHandle.attrHandle = CYBLE_SGA1_LOG10_CHAR_HANDLE;
    tempHandle.value.val = (uint8_t*)log;
    tempHandle.value.len = LOG_INFO_LENGTH;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
    
    
    tempHandle.attrHandle = CYBLE_SGA1_SLEEP_TIME_CHAR_HANDLE;
    tempHandle.value.val = (uint8*)&sleepPeriod;
    tempHandle.value.len = 2;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
    
    
    tempHandle.attrHandle = CYBLE_SGA1_GPS_LAT_CHAR_HANDLE;
    tempHandle.value.val = (uint8*)&flatitude;
    tempHandle.value.len = 4;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
    
    tempHandle.attrHandle = CYBLE_SGA1_GPS_LONG_CHAR_HANDLE;
    tempHandle.value.val = (uint8*)&flongitude;
    tempHandle.value.len = 4;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
    
    
    dt[0]=date_time.month;
    dt[1]=date_time.day;
    dt[2]=date_time.year;
    dt[3]=date_time.hours;
    dt[4]=date_time.minutes;
    tempHandle.attrHandle = CYBLE_SGA1_DATE_TIME_CHAR_HANDLE;
    tempHandle.value.val = (uint8*)&dt;
    tempHandle.value.len = 5;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
    /////////////////////////////////////////////////////////////////////
    
    static uint16_t prev_resolution;
    tempHandle.attrHandle = CYBLE_SGA1_RESOLUTION_CHAR_HANDLE;
    tempHandle.value.val = (uint8_t*)&sensor[0][RESOLUTIONLSB];
    tempHandle.value.len = 1;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
    
    tempHandle.attrHandle = CYBLE_SGA1_RESOLUTION2_CHAR_HANDLE;
    tempHandle.value.val = (uint8_t*)&sensor[1][RESOLUTIONLSB];
    tempHandle.value.len = 1;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
    
    
    
    tempHandle.attrHandle = CYBLE_SGA1_SYS_READING_CHAR_HANDLE;
    Sensor.Reading = iReading;
    tempHandle.value.val = (uint8*)&iReading;
    tempHandle.value.len = 2;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
    /* If notifications have been requested for Sensor Reading */
    if( System.BleNotifyReading && ( Sensor.Reading != Sensor.ReadingPrevious ) )
    {
        CyBle_GattsNotification( cyBle_connHandle, &tempHandle );               /* Notify the client */
        Sensor.ReadingPrevious = Sensor.Reading;
    }

    /* Update sys remote status value */
    tempHandle.attrHandle = CYBLE_SGA1_SYS_STATUS_CHAR_HANDLE;
    tempHandle.value.val = (uint8*)&System.Status;
    tempHandle.value.len = 2;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
    /* If notifications have been requested for Sensor Status */
    if( System.BleNotifyStatus)// && ( System.Status != System.StatusPrevious ) )
    {
        CyBle_GattsNotification( cyBle_connHandle, &tempHandle );               /* Notify the client */
        System.StatusPrevious = System.Status;
    }

    /* Update CAN Channel value */
    tempHandle.attrHandle = CYBLE_SGA1_SYS_CAN_CHAN_CHAR_HANDLE;
    tempHandle.value.val = (uint8*)&System.CanChannel;//&Sensor.CountsHalfScale;
    tempHandle.value.len = 1;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);

    /* Update CAN Node ID value */
    tempHandle.attrHandle = CYBLE_SGA1_SYS_CAN_ID_CHAR_HANDLE;
    tempHandle.value.val = (uint8*)&mySlaveAddr;//&System.CanNodeId;
    tempHandle.value.len = 1;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);

    /* Update SYS_CAL_LOOP value */
    tempHandle.attrHandle = CYBLE_SGA1_SYS_CAL_LOOP_CHAR_HANDLE;
    tempHandle.value.val = (uint8*)&System.BleCalLoop;
    tempHandle.value.len = 2;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);

    /* Update SYS_ADC value */
    tempHandle.attrHandle = CYBLE_SGA1_SYS_ADC_CHAR_HANDLE;
    tempHandle.value.val = (uint8*)&System.AdcCounts;
    tempHandle.value.len = 2;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
    /* If notifications have been requested for AdcCounts */
    if( System.BleNotifyAdcCounts && ( System.AdcCounts != System.AdcCountsPrevious ) )
    {
        CyBle_GattsNotification( cyBle_connHandle, &tempHandle );               /* Notify the client */
        System.AdcCountsPrevious = System.AdcCounts;
    }

    /* Update SYS_ALERT_1 value */
    tempHandle.attrHandle = CYBLE_SGA1_SYS_ALERT_1_CHAR_HANDLE;
    tempHandle.value.val = (uint8*)&testVal8;
    tempHandle.value.len = 2;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);

    /* Update SYS_ALERT_2 value */
    tempHandle.attrHandle = CYBLE_SGA1_SYS_ALERT_2_CHAR_HANDLE;
    tempHandle.value.val = (uint8*)&testVal8;
    tempHandle.value.len = 2;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);

    /* Update PIN value */
    tempHandle.attrHandle = CYBLE_SGA2_PIN_CHAR_HANDLE;
    tempHandle.value.val = (uint8*)&System.Pin;
    tempHandle.value.len = 4;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);

///////         DEVICE TYPE      //////////////////////////////////////////////////    
    /* Update SENSOR_TYPE value ( This is actually mfg ) */
    tempHandle.attrHandle = CYBLE_SGA2_SENSOR_TYPE_CHAR_HANDLE;
    tempHandle.value.val = (uint8*)&sensor[0][DEVTYPELSB];
    tempHandle.value.len = 1;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
    
    tempHandle.attrHandle = CYBLE_SGA2_SENSOR_TYPE2_CHAR_HANDLE;
    tempHandle.value.val = (uint8*)&sensor[1][DEVTYPELSB];
    tempHandle.value.len = 1;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
/////////////////////////////////////////////////////////////////////////////
    
///////        FULL SCALE RANGE      ////////////////////////////////////////////////    
    /* Update gas range value */
    Sensor.Range= (sensor[0][FULLSCALERANGE3]<<8)+sensor[0][FULLSCALERANGE4];
    tempHandle.attrHandle = CYBLE_SGA2_SENSOR_RANGE_CHAR_HANDLE;
    tempHandle.value.val = (uint8*)&Sensor.Range; //&sensor[FULLSCALERANGE3];//&testVal8;  
    tempHandle.value.len = 2;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
    
    Sensor.Range= (sensor[1][FULLSCALERANGE3]<<8)+sensor[1][FULLSCALERANGE4];
    tempHandle.attrHandle = CYBLE_SGA2_SENSOR_RANGE2_CHAR_HANDLE;
    tempHandle.value.val = (uint8*)&Sensor.Range; //&sensor[FULLSCALERANGE3];//&testVal8;  
    tempHandle.value.len = 2;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
////////////////////////////////////////////////////////////////////////////    
    
//////           GAS NOTE          ///////////////////////////////////////////////////////////
    /* Update SENSOR_NOTE value */
    tempHandle.attrHandle = CYBLE_SGA2_SENSOR_GAS_NOTE_CHAR_HANDLE;
    char str[12];
    remove_trailing_spaces(str, (char*)&sensor[0][COMPAUND1]);
    strcat(str, " ");
    strncat(str ,(char*)&sensor[0][UNIT1], 4 );
    tempHandle.value.val = (uint8*)str;
    tempHandle.value.len = 10;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
    
    
    tempHandle.attrHandle = CYBLE_SGA2_SENSOR_GAS_NOTE2_CHAR_HANDLE;
    memset(str , 0 , sizeof(str));
    remove_trailing_spaces(str, (char*)&sensor[1][COMPAUND1]);
    strcat(str, " ");
    strncat(str ,(char*)&sensor[1][UNIT1], 4 );
    tempHandle.value.val = (uint8*)str;
    tempHandle.value.len = 10;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
/////////////////////////////////////////////////////////////////////////////
    
    
    /* Update SENSOR_TAG value */
    tempHandle.attrHandle = CYBLE_SGA2_SENSOR_TAG_CHAR_HANDLE;
    tempHandle.value.val = (uint8*)device_tag;//&System.SensorTag;
    tempHandle.value.len = SENSOR_TAG_LENGTH;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);

    /* Update temperature */
    tempHandle.attrHandle = CYBLE_SGA2_SENSOR_TEMPERATURE_CHAR_HANDLE;
    tempHandle.value.val = (uint8*)&sensor[0][TEMPERATURELSB];//&testVal8;
    tempHandle.value.len = 1;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
    /* If notifications have been requested for Sensor CalLevel */
//    if( System.BleNotifyTemperature && ( Sensor.Temperature != Sensor.TemperaturePrevious ) )
//    {
//        CyBle_GattsNotification( cyBle_connHandle, &tempHandle );               /* Notify the client */
//        Sensor.TemperaturePrevious = Sensor.Temperature;
//    }

    /* Update SENSOR_CAL_LEVEL value */
    tempHandle.attrHandle = CYBLE_SGA2_SENSOR_CAL_LEVEL_CHAR_HANDLE;
    memcpy(&Sensor.CalLevel, &sensor[0][CALLEVEL3],2);
    tempHandle.value.val = (uint8*)&Sensor.CalLevel;
    tempHandle.value.len = 2;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
    /* If notifications have been requested for Sensor CalLevel */
//    if( System.BleNotifyCalLevel && ( Sensor.CalLevel != Sensor.CalLevelPrevious ) )
//    {
//        CyBle_GattsNotification( cyBle_connHandle, &tempHandle );               /* Notify the client */
//        Sensor.CalLevelPrevious = Sensor.CalLevel;
//    }

    /* Update CAL_Z_COUNTS value */
    tempHandle.attrHandle = CYBLE_SGA2_CAL_Z_COUNTS_CHAR_HANDLE;
    tempHandle.value.val = (uint8*)&System.CalZCounts;
    tempHandle.value.len = 2;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);

    /* Update CAL_ZERO_ERROR value */
    tempHandle.attrHandle = CYBLE_SGA2_CAL_ZERO_ERROR_CHAR_HANDLE;
    tempHandle.value.val = (uint8*)&System.CalZeroError;
    tempHandle.value.len = 1;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
    /* If notifications have been requested for System.CalZeroError */
    if( System.BleNotifyCalZeroError && ( System.CalZeroError != System.CalZeroErrorPrevious ) )
    {
        CyBle_GattsNotification( cyBle_connHandle, &tempHandle );               /* Notify the client */
        System.CalZeroErrorPrevious = System.CalZeroError;
    }

    /* Update CAL_STEP value */
    tempHandle.attrHandle = CYBLE_SGA2_CAL_STEP_CHAR_HANDLE;
    tempHandle.value.val = (uint8*)&System.CalStep;
    tempHandle.value.len = 1;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
    /* If notifications have been requested for Sensor Life */
    if( System.BleNotifyCalStep && ( System.CalStep != System.CalStepPrevious ) )
    {
        CyBle_GattsNotification( cyBle_connHandle, &tempHandle );               /* Notify the client */
        System.CalStepPrevious = System.CalStep;
    }

    /* Update CAL_SPEED value */
    tempHandle.attrHandle = CYBLE_SGA2_CAL_SPEED_CHAR_HANDLE;
    tempHandle.value.val = (uint8*)&System.CalSpeed;
    tempHandle.value.len = 2;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
    /* If notifications have been requested for System.CalSpeed */
    if( System.BleNotifyCalSpeed && ( System.CalSpeed != System.CalSpeedPrevious ) )
    {
        CyBle_GattsNotification( cyBle_connHandle, &tempHandle );               /* Notify the client */
        System.CalSpeedPrevious = System.CalSpeed;
    }

    /* Update CAL_AS_FOUND value */
    tempHandle.attrHandle = CYBLE_SGA2_CAL_AS_FOUND_CHAR_HANDLE;
    memcpy(&System.CalAsFound,&as_found,sizeof(float));
    tempHandle.value.val = (uint8*)&System.CalAsFound;
    tempHandle.value.len = 4;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
    /* If notifications have been requested for System.CalAsFound */
    if( System.BleNotifyCalAsFound && ( System.CalAsFound != System.CalAsFoundPrevious ) )
    {
        CyBle_GattsNotification( cyBle_connHandle, &tempHandle );               /* Notify the client */
        System.CalAsFoundPrevious = System.CalAsFound;
    }

    /* Update CAL_ADJUSTED value */
    tempHandle.attrHandle = CYBLE_SGA2_CAL_ADJUSTED_CHAR_HANDLE;
    tempHandle.value.val = (uint8*)&System.CalAdjusted;
    tempHandle.value.len = 1;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
    /* If notifications have been requested for System.CalAsFound */
    if( System.BleNotifyCalAdjusted && ( System.CalAdjusted != System.CalAdjustedPrevious ) )
    {
        CyBle_GattsNotification( cyBle_connHandle, &tempHandle );               /* Notify the client */
        System.CalAdjustedPrevious = System.CalAdjusted;
    }

    /* Update CAL_COUNTS value */
    tempHandle.attrHandle = CYBLE_SGA2_CAL_COUNTS_CHAR_HANDLE;
    tempHandle.value.val = (uint8*)&System.CalCounts;
    tempHandle.value.len = 2;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);

    /* Update SENSOR_LIFE value */
    tempHandle.attrHandle = CYBLE_SGA2_SENSOR_LIFE_CHAR_HANDLE;
    tempHandle.value.val = (uint8*)&sensor[0][SENSORLIFELSB];
    tempHandle.value.len = 1;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
    /* If notifications have been requested for Sensor Life */
//    if( System.BleNotifyLife && ( Sensor.Life != Sensor.LifePrevious ) )
//    {
//        CyBle_GattsNotification( cyBle_connHandle, &tempHandle );               /* Notify the client */
//        Sensor.LifePrevious = Sensor.Life;
//    }

    /* Update CAL_CLEARING value */
    tempHandle.attrHandle = CYBLE_SGA2_CAL_CLEARING_CHAR_HANDLE;
    tempHandle.value.val = (uint8*)&System.CalClearing;
    tempHandle.value.len = 2;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
    /* If notifications have been requested for Sensor CalClearing */
    if( System.BleNotifyCalClearing && ( System.CalClearing != System.CalClearingPrevious ) )
    {
        CyBle_GattsNotification( cyBle_connHandle, &tempHandle );               /* Notify the client */
        System.CalClearingPrevious = System.CalClearing;
    }

    /* Update CAL_SPAN_ERROR value */
    tempHandle.attrHandle = CYBLE_SGA2_CAL_SPAN_ERROR_CHAR_HANDLE;
    tempHandle.value.val = (uint8*)&System.CalSpanError;
    tempHandle.value.len = 1;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
    /* If notifications have been requested for System.CalSpanError */
    if( System.BleNotifyCalSpanError && ( System.CalSpanError != System.CalSpanErrorPrevious ) )
    {
        CyBle_GattsNotification( cyBle_connHandle, &tempHandle );               /* Notify the client */
        System.CalSpanErrorPrevious = System.CalSpanError;
    }

    /* Update CALIBRATION_STATUS value */
    tempHandle.attrHandle = CYBLE_SGA2_CALIBRATION_STATUS_CHAR_HANDLE;
    tempHandle.value.val = (uint8*)&System.CalibrationStatus;
    tempHandle.value.len = 1;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
    
    /* If notifications have been requested for System.CalibrationStatus */
    if(( System.CalibrationStatus != System.CalibrationStatusPrevious ) )
    //if( System.BleNotifyCalibrationStatus && ( System.CalibrationStatus != System.CalibrationStatusPrevious ) )
    {
        CyBle_GattsNotification( cyBle_connHandle, &tempHandle );               /* Notify the client */
        System.CalibrationStatusPrevious = System.CalibrationStatus;
    }
    
    
    /* Update READING_SCALED value */
    tempHandle.attrHandle = CYBLE_SGA2_READING_SCALED_CHAR_HANDLE;
    memcpy(&Sensor.ReadingScaled, &rfReading, sizeof(float));
    tempHandle.value.val = (uint8*)&Sensor.ReadingScaled;//(uint8*)&testVal8;
    tempHandle.value.len = 4;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
    /* If notifications have been requested for Sensor.ReadingScaled */
    if( System.BleNotifyReadingScaled && ( Sensor.ReadingScaled != Sensor.ReadingScaledPrevious ) )
    {
        CyBle_GattsNotification( cyBle_connHandle, &tempHandle );               /* Notify the client */
        Sensor.ReadingScaledPrevious = Sensor.ReadingScaled;
    }

    /* Update ALERT_STATUS value */
    memcpy(&System.AlertStatus, &alarm_status[0], 1);
    tempHandle.attrHandle = CYBLE_SGA2_ALERT_STATUS_CHAR_HANDLE;
    tempHandle.value.val = (uint8*)&System.AlertStatus;
    tempHandle.value.len = 1;
    CyBle_GattsWriteAttributeValue(&tempHandle,0,&cyBle_connHandle,0);
    /* If notifications have been requested for System.AlertStatus */
    if( System.BleNotifyAlertStatus && ( System.AlertStatus != System.AlertStatusPrevious ) )
    {
        CyBle_GattsNotification( cyBle_connHandle, &tempHandle );               /* Notify the client */
        System.AlertStatusPrevious = System.AlertStatus;
    }

}





int capsenseNotify;
uint8_t bleConnected;

CYBLE_CONN_HANDLE_T connectionHandle;

/* define the test register to switch the PA/LNA hardware control pins */
#define CYREG_SRSS_TST_DDFT_CTRL 0x40030008

//void Stack_Handler(uint32 eventCode, void *eventParam){
//    
//    CYBLE_GATTS_WRITE_REQ_PARAM_T *wrReqParam;
//    
//    switch (eventCode){
//        case CYBLE_EVT_STACK_ON:
//            /* Enable the Skyworks SE2438T PA/LNA */
//            CSD_Write(1);
//            CPS_Write(1);
//            /* Configure the Link Layer to automatically switch PA control pin P3[2] and LNA control pin P3[3] */
//            CY_SET_XTND_REG32((void CYFAR *)(CYREG_BLE_BLESS_RF_CONFIG), 0x0331);
//            CY_SET_XTND_REG32((void CYFAR *)(CYREG_SRSS_TST_DDFT_CTRL), 0x80000302);
//        case CYBLE_EVT_GAP_DEVICE_DISCONNECTED:
//            CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
//            bleConnected =0;
//            BLETimer_Start();
//            
//            break;
//            
//        case CYBLE_EVT_GATT_CONNECT_IND:
//            connectionHandle = *(CYBLE_CONN_HANDLE_T *)eventParam;
//            bleConnected =1;
//            
//#ifdef DEEPSLEEP            
//            ResetBLETimer();
//#endif            
//            //updateData();
//            updateOurBleData();
//            
//            break;
//        
//        case CYBLE_EVT_GATTS_WRITE_REQ:
//            wrReqParam = (CYBLE_GATTS_WRITE_REQ_PARAM_T *)eventParam;
//            
//            /* Requests to write variables */
//            /* It's our sgaBleRemote variable */            
//            if(wrReqParam->handleValPair.attrHandle == CYBLE_SGA1_SYS_REMOTE_CHAR_HANDLE)
//            {
//                /* Only update and respond if the write to the GATT Database is allowed */
//                if(CYBLE_GATT_ERR_NONE == CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair,0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED))
//                {
//                    BleControl = wrReqParam->handleValPair.value.val[0];      /* Update our local copy */
//                    CyBle_GattsWriteRsp(cyBle_connHandle);                      /* respond to the client */
//                }
//                
//                // send i2c command here?
//                runtime_CtrlSensor( CYBLE_SGA1_SYS_REMOTE_CHAR_HANDLE+2 , BleControl);
//            }
//            
//            /* It's a request for notifications for our Sensor.Reading (SYS_READING) */
//            if( wrReqParam->handleValPair.attrHandle == CYBLE_SGA1_SYS_READING_SYS_READING_CCCD_DESC_HANDLE )
//            {
//                CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair,0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED);
//                System.BleNotifyReading = wrReqParam->handleValPair.value.val[0] & 0x01; /* Update local notification state */
//                CyBle_GattsWriteRsp(cyBle_connHandle);                          /* respond to the client */
//            }
//            
//            /* It's a request for notifications for our Sensor.Status (SYS_STATUS) */
//            if( wrReqParam->handleValPair.attrHandle == CYBLE_SGA1_SYS_STATUS_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_HANDLE )
//            {
//                CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair,0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED);
//                System.BleNotifyReading = wrReqParam->handleValPair.value.val[0] & 0x01; /* Update local notification state */
//                CyBle_GattsWriteRsp(cyBle_connHandle);                          /* respond to the client */
//            }
//            break;
//            
//            break;
//        default:
//            break;    
//    }
//    
//    
//    
//    
//    
//    
//}


/*******************************************************************************
* Function Name: Stack_Handler
********************************************************************************
* Summary:
*  Stack handler for BLE communications.
* 
* Parameters:
*  eventCode
*  eventParam
*
* Return:
*  None
*
*******************************************************************************/
void Stack_Handler( uint32 eventCode, void *eventParam )
{
    CYBLE_GATTS_WRITE_REQ_PARAM_T *wrReqParam;    

    switch( eventCode )                                                         /* Handle any BLE events */
    {
        case CYBLE_EVT_STACK_ON:                                                /* Handle BLE stack started */
            /* Mandatory events to be handled by BLE application code */
            /* Enable the Skyworks SE2438T PA/LNA */
            CSD_Write(1);
            CPS_Write(1);
            /* Configure the Link Layer to automatically switch PA control pin P3[2] and LNA control pin P3[3] */
            CY_SET_XTND_REG32((void CYFAR *)(CYREG_BLE_BLESS_RF_CONFIG), 0x0331);
            CY_SET_XTND_REG32((void CYFAR *)(CYREG_SRSS_TST_DDFT_CTRL), 0x80000302);
        case CYBLE_EVT_GAP_DEVICE_DISCONNECTED:                                 /* Handle BLE disconnection */
            CyBle_GappStartAdvertisement( CYBLE_ADVERTISING_FAST );             /* Start advertising */
            bleConnected =0;
            BLETimer_Start();
            if( System.BleInAuxCalState == TRUE ) System.BleControl = 0;        /* In loop cal or other non normal state */
            break;
        case CYBLE_EVT_GATT_CONNECT_IND:                                        /* Handle BLE connection */
            //updateData();
            updateOurBleData();                                                 /* Update the GATT DB with our data */
            connectionHandle = *(CYBLE_CONN_HANDLE_T *)eventParam;
            bleConnected =1;
            
#ifdef DEEPSLEEP            
            ResetBLETimer();
#endif            
            break;
        case CYBLE_EVT_GATTS_WRITE_REQ:                                           /* Handle a write request from client */
            wrReqParam = (CYBLE_GATTS_WRITE_REQ_PARAM_T *) eventParam;          /* Get the attribute trying to be written */

            /* Requests to write variables */
            /* It's our sgaBleRemote variable */            
            if(wrReqParam->handleValPair.attrHandle == CYBLE_SGA1_SYS_REMOTE_CHAR_HANDLE)
            {
                /* Only update and respond if the write to the GATT Database is allowed */
                if(CYBLE_GATT_ERR_NONE == CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair,0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED))
                {
                    System.BleControl = wrReqParam->handleValPair.value.val[0];      /* Update our local copy */
                    sensor[0][SYSREMOTELSB] = wrReqParam->handleValPair.value.val[0]; 
                    sensor[1][SYSREMOTELSB] = wrReqParam->handleValPair.value.val[1]; 
                    
                    CyBle_GattsWriteRsp(cyBle_connHandle);                      /* respond to the client */
                }
                runtime_CtrlSensor( 0, CYBLE_SGA1_SYS_REMOTE_CHAR_HANDLE+2 , System.BleControl); //??? Needs testing!!!!!!
            }
            /* It's our CAN Node ID variable */            
            if(wrReqParam->handleValPair.attrHandle == CYBLE_SGA1_SYS_CAN_ID_CHAR_HANDLE)
            {
                /* Only update and respond if the write to the GATT Database is allowed */
                if(CYBLE_GATT_ERR_NONE == CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair,0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED))
                {
                    CyBle_GattsWriteRsp(cyBle_connHandle);                      /* respond to the client */
                    if(wrReqParam->handleValPair.value.val[0]>0 && wrReqParam->handleValPair.value.val[0]<=MODBUS_MAXIMAL_ADDRESS){
                        System.CanNodeId = wrReqParam->handleValPair.value.val[0];  /* Update our local copy */
                        EEPROM_SetMyAddress(System.CanNodeId);
                        mySlaveAddr =System.CanNodeId; 
                    }
                    
//                    FM24VN10_Write_8( SYS_I2C_FRAM_ADDR_B0, SYS_EE8_CAN_NODE_ID, System.CanNodeId );
//                    if( SA100_I2C_INSTALLED ) SA100I2C_Write_8( SA100_CEC_I2C_ADDR, SA100_I2C_CanNodeId, System.CanNodeId );
//                    SA100CEC_Write_8( CEC_OD_SYS_U8_CANNodeID, 0, System.CanNodeId );
//                    SA100CEC_Reset( CEC_RESET_COMMS );
                }
            }
            
            // SELECTED CHANNEL
            
            if(wrReqParam->handleValPair.attrHandle == CYBLE_SGA1_SELECTED_CHANNEL_CHAR_HANDLE){
                if(CYBLE_GATT_ERR_NONE == CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair,0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED)){
                    CyBle_GattsWriteRsp(cyBle_connHandle);                      /* respond to the client */
                    BLE_ch = wrReqParam->handleValPair.value.val[0];
                }
            }
            
            // Relays settings
            if(wrReqParam->handleValPair.attrHandle == CYBLE_SGA1_RELAYS_CHAR_HANDLE){
                if(CYBLE_GATT_ERR_NONE == CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair,0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED)){
                    CyBle_GattsWriteRsp(cyBle_connHandle);                      /* respond to the client */
                    for(uint8_t i=0;i<4;i++){
                        relay[i].energ = (wrReqParam->handleValPair.value.val[i]>>2)& 0x1;
                        relay[i].latched = (wrReqParam->handleValPair.value.val[i]>>1)& 0x1;
                        relay[i].ack = wrReqParam->handleValPair.value.val[i]& 0x1;
                    }
                    EEPROM_WriteBytes(EEPROM_RELAY1_ENERG_REG, (uint8_t*)relay, sizeof(relay));
                }
            }
            
            // Sleep Time Setting
            if(wrReqParam->handleValPair.attrHandle == CYBLE_SGA1_SLEEP_TIME_CHAR_HANDLE){
                if(CYBLE_GATT_ERR_NONE == CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair,0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED)){
                    CyBle_GattsWriteRsp(cyBle_connHandle);                      /* respond to the client */
                    uint16_t par = (wrReqParam->handleValPair.value.val[1]<<8) + wrReqParam->handleValPair.value.val[0];
                    sleepPeriod = par;
                    if(sleepPeriod<MINIMAL_SLEEP_PERIOD ) {sleepPeriod =0;sleepEnable=0;}
                    else if( sleepPeriod > MAXIMAL_SLEEP_PERIOD) {sleepPeriod = MAXIMAL_SLEEP_PERIOD; sleepEnable=1;}
                    else {sleepEnable=1;}
                    EEPROM_SetSleepPeriod(sleepPeriod);
                }
            }
            
            // GPS Lattitude setting
            if(wrReqParam->handleValPair.attrHandle == CYBLE_SGA1_GPS_LAT_CHAR_HANDLE){
                if(CYBLE_GATT_ERR_NONE == CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair,0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED)){
                    CyBle_GattsWriteRsp(cyBle_connHandle);                      /* respond to the client */
                    uint8_t par[4];
                    for(uint8_t i=0;i<4 ;i++)
                        par[i]=wrReqParam->handleValPair.value.val[i];
                    memcpy(&flatitude, par, sizeof(float));    
                    EEPROM_SaveGPSLat(flatitude);
                }
            }
            // GPS Longitude setting
            if(wrReqParam->handleValPair.attrHandle == CYBLE_SGA1_GPS_LONG_CHAR_HANDLE){
                if(CYBLE_GATT_ERR_NONE == CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair,0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED)){
                    CyBle_GattsWriteRsp(cyBle_connHandle);                      /* respond to the client */
                    uint8_t par[4];
                    for(uint8_t i=0;i<4 ;i++)
                        par[i]=wrReqParam->handleValPair.value.val[i];
                    memcpy(&flongitude, par, sizeof(float));    
                    EEPROM_SaveGPSLong(flongitude);
                }
            }
            
            // Date and Time setting
            if(wrReqParam->handleValPair.attrHandle == CYBLE_SGA1_DATE_TIME_CHAR_HANDLE){
                if(CYBLE_GATT_ERR_NONE == CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair,0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED)){
                    CyBle_GattsWriteRsp(cyBle_connHandle);                      /* respond to the client */
                    date_time.month = wrReqParam->handleValPair.value.val[0];
                    date_time.day = wrReqParam->handleValPair.value.val[1];
                    date_time.year = wrReqParam->handleValPair.value.val[2];
                    date_time.hours = wrReqParam->handleValPair.value.val[3];
                    date_time.minutes = wrReqParam->handleValPair.value.val[4];
                    DS3231_SetDateTime(&date_time);
                }
            }
            
            /* It's our CAN Channel variable */            
            if(wrReqParam->handleValPair.attrHandle == CYBLE_SGA1_SYS_CAN_CHAN_CHAR_HANDLE)
            {
                /* Only update and respond if the write to the GATT Database is allowed */
                if(CYBLE_GATT_ERR_NONE == CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair,0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED))
                {
                    System.CanChannel = wrReqParam->handleValPair.value.val[0];  /* Update our local copy */
                    CyBle_GattsWriteRsp(cyBle_connHandle);                      /* respond to the client */
                    EEPROM_SetMyAddress(System.CanNodeId);
                    mySlaveAddr =System.CanNodeId;
                    
//                    FM24VN10_Write_8( SYS_I2C_FRAM_ADDR_B0, SYS_EE8_CAN_CHANNEL, System.CanChannel);
//                    if( SA100_I2C_INSTALLED ) SA100I2C_Write_8( SA100_CEC_I2C_ADDR, SA100_I2C_CanChannel, System.CanChannel );
//                    SA100CEC_Write_8( CEC_OD_SYS_U8_channel, 0, System.CanChannel );
//                    SA100CEC_Reset( CEC_RESET_COMMS );
                }
            }
            /* It's our ALERT_1 variable */            
            if(wrReqParam->handleValPair.attrHandle == CYBLE_SGA1_SYS_ALERT_1_CHAR_HANDLE)
            {
                /* Only update and respond if the write to the GATT Database is allowed */
                if(CYBLE_GATT_ERR_NONE == CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair,0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED))
                {
                    CyBle_GattsWriteRsp(cyBle_connHandle);                      /* respond to the client */
                }
            }
            /* It's our ALERT_2 variable */            
            if(wrReqParam->handleValPair.attrHandle == CYBLE_SGA1_SYS_ALERT_2_CHAR_HANDLE)
            {
                /* Only update and respond if the write to the GATT Database is allowed */
                if(CYBLE_GATT_ERR_NONE == CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair,0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED))
                {
                    CyBle_GattsWriteRsp(cyBle_connHandle);                      /* respond to the client */
                }
            }
            /* It's our PIN variable */            
            if(wrReqParam->handleValPair.attrHandle == CYBLE_SGA2_PIN_CHAR_HANDLE)
            {
                /* Only update and respond if the write to the GATT Database is allowed */
                if(CYBLE_GATT_ERR_NONE == CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair,0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED))
                {
                    CyBle_GattsWriteRsp(cyBle_connHandle);                      /* respond to the client */

                }
            }
            /* It's our SENSOR_TAG variable */            
            if(wrReqParam->handleValPair.attrHandle == CYBLE_SGA2_SENSOR_TAG_CHAR_HANDLE)
            {
                /* Only update and respond if the write to the GATT Database is allowed */
                if(CYBLE_GATT_ERR_NONE == CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair,0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED))
                {
                    CyBle_GattsWriteRsp(cyBle_connHandle);                      /* respond to the client */
                    memset(device_tag, 0, sizeof(device_tag));
                    for( uint8 i = 0; i < SENSOR_TAG_LENGTH; i++ )
                    {
                        device_tag[i] = wrReqParam->handleValPair.value.val[i];  /* Update our local copy */
                    }
                    
                    SetDeviceTag(device_tag);
//                    SysSaveSensorTagString();
                }
            }
            /* It's our SENSOR_CAL_LEVEL variable */            
            if(wrReqParam->handleValPair.attrHandle == CYBLE_SGA2_SENSOR_CAL_LEVEL_CHAR_HANDLE)
            {
                /* Only update and respond if the write to the GATT Database is allowed */
                if(CYBLE_GATT_ERR_NONE == CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair,0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED))
                {
                    CyBle_GattsWriteRsp(cyBle_connHandle);                      /* respond to the client */
                }                    
            }
            /* It's our CAL LOOP Channel variable */            
            if(wrReqParam->handleValPair.attrHandle == CYBLE_SGA1_SYS_CAL_LOOP_CHAR_HANDLE)
            {
                /* Only update and respond if the write to the GATT Database is allowed */
                if(CYBLE_GATT_ERR_NONE == CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair,0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED))
                {
                    System.BleCalLoop = wrReqParam->handleValPair.value.val[1];  /* Update our local copy */
                    System.BleCalLoop <<=8;
                    System.BleCalLoop |= wrReqParam->handleValPair.value.val[0];  /* Update our local copy */
                    CyBle_GattsWriteRsp(cyBle_connHandle);                      /* respond to the client */
                }
            }
            /* It's our CalibrationStatus variable */            
            if(wrReqParam->handleValPair.attrHandle == CYBLE_SGA2_CALIBRATION_STATUS_CHAR_HANDLE)
            {
                /* Only update and respond if the write to the GATT Database is allowed */
                if(CYBLE_GATT_ERR_NONE == CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair,0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED))
                {
                    System.CalibrationStatus = wrReqParam->handleValPair.value.val[0];  /* Update our local copy */
                    CyBle_GattsWriteRsp(cyBle_connHandle);                      /* respond to the client */
                }
            }

            /* Request for Notifications */
            /* It's a request for notifications for our System.BleControl (SYS_REMOTE) */
            if( wrReqParam->handleValPair.attrHandle == CYBLE_SGA1_SYS_REMOTE_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_HANDLE )
            {
                CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair,0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED);
                System.BleNotifyControl = wrReqParam->handleValPair.value.val[0] & 0x01; /* Update local notification state */
                CyBle_GattsWriteRsp(cyBle_connHandle);                          /* respond to the client */
            }
            /* It's a request for notifications for our Sensor.Reading (SYS_READING) */
            if( wrReqParam->handleValPair.attrHandle == CYBLE_SGA1_SYS_READING_SYS_READING_CCCD_DESC_HANDLE )
            {
                CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair,0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED);
                System.BleNotifyReading = wrReqParam->handleValPair.value.val[0] & 0x01; /* Update local notification state */
                CyBle_GattsWriteRsp(cyBle_connHandle);                          /* respond to the client */
            }
            /* It's a request for notifications for our Sensor.Status (SYS_STATUS) */
            if( wrReqParam->handleValPair.attrHandle == CYBLE_SGA1_SYS_STATUS_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_HANDLE )
            {
                CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair,0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED);
                System.BleNotifyStatus = wrReqParam->handleValPair.value.val[0] & 0x01; /* Update local notification state */
                CyBle_GattsWriteRsp(cyBle_connHandle);                          /* respond to the client */
            }
            /* It's a request for notifications for our System.AdcCounts (SYS_ADC) */
            if( wrReqParam->handleValPair.attrHandle == CYBLE_SGA1_SYS_ADC_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_HANDLE )
            {
                CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair,0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED);
                System.BleNotifyAdcCounts = wrReqParam->handleValPair.value.val[0] & 0x01; /* Update local notification state */
                CyBle_GattsWriteRsp(cyBle_connHandle);                          /* respond to the client */
            }
            /* It's a request for notifications for our Sensor.Temperature */
            if( wrReqParam->handleValPair.attrHandle == CYBLE_SGA2_SENSOR_TEMPERATURE_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_HANDLE )
            {
                CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair,0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED);
                System.BleNotifyTemperature = wrReqParam->handleValPair.value.val[0] & 0x01; /* Update local notification state */
                CyBle_GattsWriteRsp(cyBle_connHandle);                          /* respond to the client */
            }
            /* It's a request for notifications for our Sensor.CalLevel */
            if( wrReqParam->handleValPair.attrHandle == CYBLE_SGA2_SENSOR_CAL_LEVEL_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_HANDLE )
            {
                CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair,0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED);
                System.BleNotifyCalLevel = wrReqParam->handleValPair.value.val[0] & 0x01; /* Update local notification state */
                CyBle_GattsWriteRsp(cyBle_connHandle);                          /* respond to the client */
            }
            /* It's a request for notifications for our System.CalZeroError */
            if( wrReqParam->handleValPair.attrHandle == CYBLE_SGA2_CAL_ZERO_ERROR_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_HANDLE )
            {
                CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair,0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED);
                System.BleNotifyCalZeroError = wrReqParam->handleValPair.value.val[0] & 0x01; /* Update local notification state */
                CyBle_GattsWriteRsp(cyBle_connHandle);                          /* respond to the client */
            }
            /* It's a request for notifications for our System.CalStep */
            if( wrReqParam->handleValPair.attrHandle == CYBLE_SGA2_CAL_STEP_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_HANDLE )
            {
                CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair,0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED);
                System.BleNotifyCalStep = wrReqParam->handleValPair.value.val[0] & 0x01; /* Update local notification state */
                CyBle_GattsWriteRsp(cyBle_connHandle);                          /* respond to the client */
            }
            /* It's a request for notifications for our System.CalSpeed */
            if( wrReqParam->handleValPair.attrHandle == CYBLE_SGA2_CAL_SPEED_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_HANDLE )
            {
                CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair,0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED);
                System.BleNotifyCalSpeed = wrReqParam->handleValPair.value.val[0] & 0x01; /* Update local notification state */
                CyBle_GattsWriteRsp(cyBle_connHandle);                          /* respond to the client */
            }
            /* It's a request for notifications for our System.CalAsFound */
            if( wrReqParam->handleValPair.attrHandle == CYBLE_SGA2_CAL_AS_FOUND_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_HANDLE )
            {
                CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair,0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED);
                System.BleNotifyCalAsFound = wrReqParam->handleValPair.value.val[0] & 0x01; /* Update local notification state */
                CyBle_GattsWriteRsp(cyBle_connHandle);                          /* respond to the client */
            }
            /* It's a request for notifications for our System.CalAdjusted */
            if( wrReqParam->handleValPair.attrHandle == CYBLE_SGA2_CAL_ADJUSTED_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_HANDLE )
            {
                CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair,0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED);
                System.BleNotifyCalAdjusted = wrReqParam->handleValPair.value.val[0] & 0x01; /* Update local notification state */
                CyBle_GattsWriteRsp(cyBle_connHandle);                          /* respond to the client */
            }
            /* It's a request for notifications for our Sensor.Life */
            if( wrReqParam->handleValPair.attrHandle == CYBLE_SGA2_SENSOR_LIFE_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_HANDLE )
            {
                CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair,0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED);
                System.BleNotifyLife = wrReqParam->handleValPair.value.val[0] & 0x01; /* Update local notification state */
                CyBle_GattsWriteRsp(cyBle_connHandle);                          /* respond to the client */
            }
            /* It's a request for notifications for our Sensor.CalClearing */
            if( wrReqParam->handleValPair.attrHandle == CYBLE_SGA2_CAL_CLEARING_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_HANDLE )
            {
                CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair,0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED);
                System.BleNotifyCalClearing = wrReqParam->handleValPair.value.val[0] & 0x01; /* Update local notification state */
                CyBle_GattsWriteRsp(cyBle_connHandle);                          /* respond to the client */
            }
            /* It's a request for notifications for our System.CalSpanError */
            if( wrReqParam->handleValPair.attrHandle == CYBLE_SGA2_CAL_SPAN_ERROR_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_HANDLE )
            {
                CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair,0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED);
                System.BleNotifyCalSpanError = wrReqParam->handleValPair.value.val[0] & 0x01; /* Update local notification state */
                CyBle_GattsWriteRsp(cyBle_connHandle);                          /* respond to the client */
            }
            /* It's a request for notifications for our System.CalibrationStatus */
            if( wrReqParam->handleValPair.attrHandle == CYBLE_SGA2_CALIBRATION_STATUS_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_HANDLE )
            {
                CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair,0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED);
                System.BleNotifyCalibrationStatus = wrReqParam->handleValPair.value.val[0] & 0x01; /* Update local notification state */
                CyBle_GattsWriteRsp(cyBle_connHandle);                          /* respond to the client */
            }
            /* It's a request for notifications for our Sensor.ReadingScaled */
            if( wrReqParam->handleValPair.attrHandle == CYBLE_SGA2_READING_SCALED_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_HANDLE )
            {
                CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair,0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED);
                System.BleNotifyReadingScaled = wrReqParam->handleValPair.value.val[0] & 0x01; /* Update local notification state */
                CyBle_GattsWriteRsp(cyBle_connHandle);                          /* respond to the client */
            }
            /* It's a request for notifications for our System.AlertStatus */
            if( wrReqParam->handleValPair.attrHandle == CYBLE_SGA2_ALERT_STATUS_CLIENT_CHARACTERISTIC_CONFIGURATION_DESC_HANDLE )
            {
                CyBle_GattsWriteAttributeValue(&wrReqParam->handleValPair,0, &cyBle_connHandle, CYBLE_GATT_DB_PEER_INITIATED);
                System.BleNotifyAlertStatus = wrReqParam->handleValPair.value.val[0] & 0x01; /* Update local notification state */
                CyBle_GattsWriteRsp(cyBle_connHandle);                          /* respond to the client */
            }
            break;
        default:
            break;
            
    }
}



void ResetBLETimer(){
    BLETimer_Stop();
    BLETimer_WriteCounter(0);
}

void SetSleepPeriod(uint32_t period){
    BLETimer_Stop();
    BLETimer_WritePeriod(period);
    BLETimer_WriteCounter(0);
}

// if BLE gets disconnected we put MCU to Deeep Sleep Mode


void SysWdtInit( void )
{
//    /* Determin reset cause and proceed accordingly */
//    if(CySysGetResetReason(CY_SYS_RESET_WDT) == CY_SYS_RESET_WDT){
//        for(uint8_t i =0; i<20;i++){
//             LED_Write(~LED_Read());
//            CyDelay(50);
//        }
//    }else{
//        for(uint8_t i =0; i<6;i++){
//             LED_Write(~LED_Read());
//            CyDelay(200);
//        }
//    }
    
//    if ( 0u == CySysGetResetReason( CY_SYS_RESET_WDT ) )                        /* Startup after PowerUp/XRES event. */
//    {
//        CyDelay( 500 /* msecs */);                                             /* Delay so user can detect normal reset. */
//    }else{                                                                      /* Startup after WDT reset event. */
//        
//        /* Create fault condition(s) here with a delay. */
//        /* Danger output needs to be set appropiately here */
//        //System.AlertBits = SYS_ALERT_BIT_FAULT;
//        //System.AlertStatus = SYS_ALERT_FAULT;
//        //SA100CEC_UpdateRuntimeValues( SENSORS_CALC_READING_WARMUP );
//        //SAFETY_OUTPUT_Write( SYS_SAFE_NO );
//        //MC_RGBResetCauseWDT
//        //CyDelay( 10000 /* msecs */);                                             /* Delay so user can detect WDT reset. */
//        
//        
//    }

    /* Setup the Watchdog timer environment */
    //WdtIsr_StartEx( SysWdtIsrHandler );                                            /* Setup ISR for interrupts at WDT counter 0 events. */
    CyGlobalIntEnable;                                                          /* Enable global interrupts. */
	
	/* Set WDT counter 0 to generate interrupt on match */
	CySysWdtWriteMode( CY_SYS_WDT_COUNTER0, CY_SYS_WDT_MODE_INT );
	CySysWdtWriteMatch( CY_SYS_WDT_COUNTER0, WDT_COUNT0_MATCH );
	CySysWdtWriteClearOnMatch( CY_SYS_WDT_COUNTER0, 1u );

    CySysWdtWriteCascade( CY_SYS_WDT_CASCADE_01 );                              /* Enable WDT counters 0 and 1 cascade */
    
	/* Set WDT counter 1 to generate reset on match */
	CySysWdtWriteMatch( CY_SYS_WDT_COUNTER1, WDT_COUNT1_MATCH );
	CySysWdtWriteMode( CY_SYS_WDT_COUNTER1, CY_SYS_WDT_MODE_RESET );
    CySysWdtWriteClearOnMatch( CY_SYS_WDT_COUNTER1, 1u );
	
	CySysWdtEnable( CY_SYS_WDT_COUNTER0_MASK | CY_SYS_WDT_COUNTER1_MASK );      /* Enable WDT counters 0 and 1 */
	
	/* Lock WDT registers and try to disable WDT counters 0 and 1 */
	CySysWdtLock();
	CySysWdtDisable( CY_SYS_WDT_COUNTER1_MASK );
	CySysWdtUnlock();
}




void SysWdtReset( void )
{
	CySysWdtResetCounters( CY_SYS_WDT_COUNTER0_RESET );                         /* Reset WDT counter 0 */
    CySysWdtResetCounters( CY_SYS_WDT_COUNTER1_RESET );                         /* Reset WDT counter 1 */
    CyDelayUs( 200 );                                                           /* Pause 200ms */
}

void remove_leading_spaces(char *str) {
    int i, j, len = strlen(str);
    for (i = 0; str[i] == ' '; i++) {
        // Skip leading spaces
    }
    if (i > 0) {
        for (j = 0; i + j <= len; j++) {
            str[j] = str[i + j];
        }
    }
}


void remove_trailing_spaces(char* dst, char *str){
    int i=0;
    int len = strlen(str);
    while(i<len){
        if(str[i] == ' '){
            dst[i] = '\0';
        }else{
            dst[i] =str[i];
        }
        i++;
    }
}

void setMode(uint8_t mode){
    runtimeMode=mode;
    newMode =1;
}


void runtimePowerUp(void){
    if(newMode){
        newMode=0;
        Nextion_SetPage(PAGE_POWER_UP);
        char version[20];
        strcpy(version, "Ver.");
        strcat(version, firmwareVersion);
        Nextion_SetText(DISP_OBJ_FIRMWARE_VERSION, version);
    }
    
    
    CyDelay(2000);
    GoRealtime(ch2Active);

}


 
void runtimeRealTime(void){
    char str[10];
    char units[4];
    //ch2Active=1;
    
    
    
    if(newMode){
        newMode=0;
        
        //SysWdtInit();
        
        sleepEnable = (sleepPeriod > 0)? 1:0;
        
        Reset_relays();
        Reset_Inactivity_timer();
        
        SetRealTimePage(ch2Active);
#ifdef SINGLE_SENSOR_SYSTEM   
        Nextion_ObjVisible(DISP_OBJ_CH1_ACTIVE,0);
        Nextion_ObjVisible(DISP_OBJ_CH2_ACTIVE,0);
#endif    
        Nextion_ObjVisible(DISP_OBJ_BLE_CONNECTED,bleConnected);
        CyDelay(150);
        Nextion_ObjVisible(DISP_OBJ_TEXT1, 0);
        Nextion_ObjVisible(DISP_OBJ_EF1, 0);
        
        
        if(ch2Active==1){
            
            Nextion_ObjVisible(DISP_OBJ_EF2, 0);     
            
            sensor_status[1].alarm1=0;
            sensor_status[1].alarm2=0;
            sensor_status[1].alarm3=0;
            alarm_status[1]=0;
            prev_alarm_status[1]=0;
            ef2=1;
            LoadSensorInfo(1);
            ReloadSensorGasType(1);
            
            
            HandleSensor2(1);// forced update
            
            
        }
        
        sensor_status[0].alarm1=0;
        sensor_status[0].alarm2=0;
        sensor_status[0].alarm3=0;
        alarm_status[0]=0;
        prev_alarm_status[0]=0;
        ef1=1;
        LoadSensorInfo(0);
        ReloadSensorGasType(0);
        
        HandleSensor1(ch2Active, 1);
        
        if (ch2Active){
            Nextion_SetObjAttr(DISP_OBJ_CH2_ACTIVE,"pco",DISP_YELLOW);
        }
        else{
            Nextion_SetObjAttr(DISP_OBJ_CH2_ACTIVE,"pco",DISP_GREY);
        }
        
        
        
    }
    //CySysWatchdogFeed(CY_SYS_WDT_COUNTER0_MASK);
    uint8_t cal_status1 = sensor[0][CALSTATUSLSB];
    uint8_t cal_status2 = sensor[1][CALSTATUSLSB];
    SysWdtReset();
    
    //CheckEF();
    
    uint32_t now = msTick;
    
    if(now- last_rtc_time>= TASK_PERIOD_RTC_MS){
        // READ RTC HERE
        last_rtc_time=now;
        DS3231_GetDateTime(&date_time);
        
        memset(txt, 0 , sizeof(txt));
        sprintf((char*)txt , "%02d/%02d/%02d",date_time.month , date_time.day , date_time.year);
        Nextion_SetText(DISP_OBJ_DATE , txt);
        memset(txt, 0 , sizeof(txt));
        sprintf((char*)txt , "%02d:%02d",date_time.hours , date_time.minutes);
        Nextion_SetText(DISP_OBJ_TIME , txt);
    }
    
    
        
        
        
    if(now- last_led_time>= TASK_PERIOD_LED_MS){
        last_led_time=now;
        
        SetLED1();
        if(ch2Active)SetLED2();    
        LED_Update();
    }
    
    //if(now- last_sensor_time>= i2cPeriod){
    if(now- last_sensor_time>= TASK_PERIOD_SENSOR_MS){  
        
        last_sensor_time = now;
        startI2CRead =0;
        LoadSensorInfo(0); // Get data from sensor
        
        
        if(ch2Active){
            LoadSensorInfo(1);
            
        }
        if(bleConnected)
            handleBle();// This is the old cal handling for BLE
        
        
        HandleSensors(ch2Active);  
        HandleRelays();
        
        
        if(bleConnected){//bleConnected variable is controlled by Stack_Handler() in runtime.c
            updateOurBleData();
        }
        Nextion_ObjVisible(DISP_OBJ_BLE_CONNECTED,bleConnected);
        
        // Gettting Relays State;
        if(relay_present)
            relays_state = GetRelayState();
        else{
            if( (alarm_status[0]==3)||(alarm_status[1]==3) ){
               relays_state=3; 
            }else if ((alarm_status[0]==2)||(alarm_status[1]==2)){
                   relays_state=2;
            }else if( (alarm_status[0]==1)||(alarm_status[1]==1) ){
                   relays_state=1;
            }else if((alarm_status[0]==4)||(alarm_status[1]==4)||
                    !ef1 || (ch2Active&&!ef2) ||
                    (sensor[0][STATUSLSB]==RUNTIME_SENSOR_CAL_FAULT) ||
                    (ch2Active && (sensor[1][STATUSLSB]==RUNTIME_SENSOR_CAL_FAULT))){
                   relays_state=4;
            }else {
                  relays_state=0;
            }
        
        }    
            
        //LED_Update();
    }
        
    if(now- last_display_time>= TASK_PERIOD_DISPLAY_MS){
        last_display_time=now;
        DisplayRealTime(ch2Active);
    }
    
    
    /*Check Modbus request here*/
    if(modbusRequestReceived){
        modbusRequestReceived=0;
        runtime_ProcessRequest();// Response to Modbus
    }
        
#ifdef DEEPSLEEP
    if(bleTimerExpired && !bleConnected && sleepEnable){
        bleTimerExpired=0;
        SysWdtReset();
        CySysWdtDisable( CY_SYS_WDT_COUNTER0_MASK | CY_SYS_WDT_COUNTER1_MASK ); 
        GoToSleep();
    }
#endif

    // Process Ble events
    CyBle_ProcessEvents();
    runtimeRealTimeCheckSwitches();
    
    
}


void HandleSensors(uint8_t ch2Active){
    HandleSensor1(ch2Active, 0); // if only one sensor show 1 sensor screen 
    if(ch2Active)
        HandleSensor2(0); // if 2 sensors show 2 sensor screen
}



///////////////////////////////////////////////////////////////////////////////////////////
void HandleSensor1(uint8_t ch2Active, uint8_t forced){
    char str[20];
    static uint8_t prev_status;
    static uint8_t prev_ef1;
    uint8_t status_changed=0;
    uint8_t ef_changed=0;
    uint16_t warmupcnt;
    uint8_t in_alarm=0;
    uint8_t tmp[4];
    static uint8_t prev_alarm;
    static uint8_t prev_cal_status;
    static uint8_t wait;
    static uint8_t cal_wait;
    static uint8_t  zero_cal_complete;
    static uint8_t  span_cal_complete;
    
    
    
    
    if(prev_status != sensor[0][STATUSLSB])status_changed=1;
    ef1= EF1_Read();
    if(prev_ef1 !=ef1)ef_changed=1;
    
    
    
    if(status_changed || forced){
        
        
        
        if(forced)prev_alarm=0;
        if( (sensor[0][STATUSLSB]==POLL_SENSOR_STATUS_ABSENT)||
            (sensor[0][STATUSLSB]==RUNTIME_SENSOR_CAL_FAULT)) 
             {
             
            if(sensor[0][STATUSLSB]==RUNTIME_SENSOR_CAL_FAULT)
                alarm_status[0]=CAL_FAULT;
            else
                alarm_status[0]=SYSREADING_FAULT;
            alarm_mask[0]=(1<<3);
                 
            system_SetLed(0, SYSTEM_LED_BLUE, 0,ch2Active);
            Nextion_ObjVisible(DISP_OBJ_SYS_READING1, 0);
            Nextion_ObjVisible(DISP_OBJ_TEXT1, 1);
            Nextion_SetObjAttr(DISP_OBJ_STATUS1,"pic",FAULT_ICON);
            if(!ch2Active)
                Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_SINGLE_FAULT_ICON);
            else
                Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_DOUBLE_FAULT_ICON);
            CyDelay(200);
        
            Set_relay_Fault();
            if(sensor[0][STATUSLSB]==RUNTIME_SENSOR_CAL_FAULT){
                sprintf(str, "CAL FAULT");
#ifndef  SINGLE_SENSOR_SYSTEM                
                sprintf(log_txt,"%02d/%02d/%02d at %02d:%02d CH1 CAL FAULT",date_time.month, date_time.day, date_time.year, date_time.hours, date_time.minutes);
#else
                sprintf(log_txt,"%02d/%02d/%02d at %02d:%02d CAL FAULT",date_time.month, date_time.day, date_time.year, date_time.hours, date_time.minutes);
#endif    
                SaveLogEntry(&log_last_entry_idx, log_txt);
            }
            else if (sensor[0][STATUSLSB]==POLL_SENSOR_STATUS_ABSENT){
                sprintf(str, "NO SENSOR");
#ifndef  SINGLE_SENSOR_SYSTEM 
                sprintf(log_txt,"%02d/%02d/%02d at %02d:%02d CH1 FAULT",date_time.month, date_time.day, date_time.year, date_time.hours, date_time.minutes);
#else
                sprintf(log_txt,"%02d/%02d/%02d at %02d:%02d FAULT",date_time.month, date_time.day, date_time.year, date_time.hours, date_time.minutes);
#endif    
                SaveLogEntry(&log_last_entry_idx, log_txt);
            }else{
                sprintf(str, "FAULT");
#ifndef  SINGLE_SENSOR_SYSTEM 
                sprintf(log_txt,"%02d/%02d/%02d at %02d:%02d CH1 FAULT",date_time.month, date_time.day, date_time.year, date_time.hours, date_time.minutes);
#else
                sprintf(log_txt,"%02d/%02d/%02d at %02d:%02d FAULT",date_time.month, date_time.day, date_time.year, date_time.hours, date_time.minutes);
#endif    
                SaveLogEntry(&log_last_entry_idx, log_txt);            
            
            }
            Nextion_SetText(DISP_OBJ_TEXT1, str);
            prev_status = sensor[0][STATUSLSB];
            sensor_status[0].alarm1=0;
            sensor_status[0].alarm2=0;
            sensor_status[0].alarm3=0;
            prev_alarm=0;
            return;
        }else if(sensor[0][STATUSLSB]==POLL_SENSOR_STATUS_WARMUP){
            
            Nextion_ObjVisible(DISP_OBJ_SYS_READING1, 0);
            Nextion_ObjVisible(DISP_OBJ_TEXT1, 1);
            Nextion_SetObjAttr(DISP_OBJ_STATUS1,"pic",WARMUP_ICON);
            if(!ch2Active)
                Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_SINGLE_OK_ICON);
            else
                Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_DOUBLE_OK_ICON);
            CyDelay(200);
            sensor_status[0].alarm1=0;
            sensor_status[0].alarm2=0;
            sensor_status[0].alarm3=0;
            prev_alarm=0;
            
        }else if(sensor[0][STATUSLSB]== RUNTIME_SENSOR_OVERRANGE){
            alarm_mask[0]=(1<<3);
            Set_relay_Fault();
            RecalculateAlarms(0);
        }
        else if(sensor[0][STATUSLSB]== RUNTIME_SENSOR_UNDERRANGE){
            alarm_mask[0]=(1<<3);
            alarm_status[0]=SYSREADING_FAULT;
            system_SetLed(0, SYSTEM_LED_BLUE, 1,ch2Active);
            Nextion_ObjVisible(DISP_OBJ_SYS_READING1, 1);
            Nextion_ObjVisible(DISP_OBJ_TEXT1, 0);
            Nextion_SetObjAttr(DISP_OBJ_STATUS1,"pic",FAULT_ICON);
            if(!ch2Active)
                Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_SINGLE_FAULT_ICON);
            else
                Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_DOUBLE_FAULT_ICON);
            CyDelay(200);
        
            Set_relay_Fault();
        }
        else if (sensor[0][STATUSLSB]==POLL_SENSOR_STATUS_OK){
            Nextion_ObjVisible(DISP_OBJ_SYS_READING1, 1);
            Nextion_ObjVisible(DISP_OBJ_TEXT1, 0);
            if(!relay_present || ( relay_present && !relay[3].latched))
                Nextion_SetObjAttr(DISP_OBJ_STATUS1,"pic",OK_ICON);
            
           RecalculateAlarms(0);     
                  
        }
    }
    
    if(sensor[0][STATUSLSB]==POLL_SENSOR_STATUS_WARMUP){
        system_SetLed(0, SYSTEM_LED_GREEN, 1,ch2Active);
        warmupcnt = sensor[0][WARMUPTIMERMSB];
        warmupcnt = (warmupcnt<<8)+sensor[0][WARMUPTIMERLSB];
        uint16_t h = warmupcnt/3600;
        sprintf(str, "%02d:%02d:%02d",h,(warmupcnt - (h*3600))/60, warmupcnt%60  );
        Nextion_SetText(DISP_OBJ_TEXT1, str);
        prev_status = sensor[0][STATUSLSB];
        alarm_status[0]=0;
        return;
    }
    
//    if(status_changed && sensor[0][STATUSLSB]==RUNTIME_SENSOR_IN_CALIBRATION){
//        if(sensor[0][CALSTATUSLSB]==RUNTIME_SMART_SENSOR_ZERO_CAL_INPROGRESS){
//            if(!ch2Active)
//                Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_SINGLE_ZEROCAL_ICON);   
//            else
//                Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_DOUBLE_ZEROCAL_ICON); 
//                        
//            prev_status = sensor[0][STATUSLSB];
//            return;
//        }
//    }
    
    

    //We wait for valid data here  - sensor must have serial numbe - for now this is the critea that sensor data received 
    // This should filter false alerts
    if(!sensor[0][SERIALNUMBER1]&&!sensor[0][SERIALNUMBER2]&&!sensor[0][SERIALNUMBER3]&&
       !sensor[0][SERIALNUMBER4]&&!sensor[0][SERIALNUMBER5]&&!sensor[0][SERIALNUMBER6])
        wait =1;
    else{
        if(wait)
            RecalculateAlarms(0);
        wait =0;
    }
    
    if ( (sensor[0][STATUSLSB]==POLL_SENSOR_STATUS_OK) && !wait){
        alarm_status[0] =CheckAlarms(0); 
        
        if(prev_alarm!= alarm_status[0]){
                switch(alarm_status[0]){
                    case SYSREADING_ALARM1: 
                        if(!ch2Active)
                            Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_SINGLE_ALARM1_ICON);
                        else
                            Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_DOUBLE_ALARM1_ICON);
                        Set_relay_Alarm1();
#ifndef SINGLE_SENSOR_SYSTEM                        
                        sprintf(log_txt,"%02d/%02d/%02d at %02d:%02d CH1 ALARM1",date_time.month, date_time.day, date_time.year, date_time.hours, date_time.minutes);
#else
                        sprintf(log_txt,"%02d/%02d/%02d at %02d:%02d ALARM1",date_time.month, date_time.day, date_time.year, date_time.hours, date_time.minutes);
#endif    
                        SaveLogEntry(&log_last_entry_idx, log_txt);
                    break;
                    case SYSREADING_ALARM2: 
                        if(!ch2Active)    
                            Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_SINGLE_ALARM2_ICON);  
                        else
                            Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_DOUBLE_ALARM2_ICON);  
                        Set_relay_Alarm2(); 
#ifndef SINGLE_SENSOR_SYSTEM                        
                        sprintf(log_txt,"%02d/%02d/%02d at %02d:%02d CH1 ALARM2",date_time.month, date_time.day, date_time.year, date_time.hours, date_time.minutes);
#else 
                        sprintf(log_txt,"%02d/%02d/%02d at %02d:%02d ALARM2",date_time.month, date_time.day, date_time.year, date_time.hours, date_time.minutes);
#endif    
                        SaveLogEntry(&log_last_entry_idx, log_txt);
                        break;
                    case SYSREADING_ALARM3: 
                        if(!ch2Active)
                            Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_SINGLE_ALARM3_ICON);   
                        else
                            Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_DOUBLE_ALARM3_ICON); 
                            Set_relay_Alarm3();
#ifndef SINGLE_SENSOR_SYSTEM                    
                            sprintf(log_txt,"%02d/%02d/%02d at %02d:%02d CH1 ALARM3",date_time.month, date_time.day, date_time.year, date_time.hours, date_time.minutes);
#else
                            sprintf(log_txt,"%02d/%02d/%02d at %02d:%02d ALARM3",date_time.month, date_time.day, date_time.year, date_time.hours, date_time.minutes);
#endif
                            SaveLogEntry(&log_last_entry_idx, log_txt);
                    break;
                    case SYSREADING_OK:
                        if(!ef1){
                            if(!ch2Active)
                                Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_SINGLE_FAULT_ICON);
                            else
                                Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_DOUBLE_FAULT_ICON);
                        }
                        else{
                            if(!ch2Active)
                                Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_SINGLE_OK_ICON);
                            else
                                Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_DOUBLE_OK_ICON);
                        }
                    break;
                    
                    
                    
                }
            }
        prev_alarm= alarm_status[0];
    }
    
    // no alrams - check EF
    if(ef1){
        Nextion_ObjVisible(DISP_OBJ_EF1,0);
        
    }else{
        Nextion_ObjVisible(DISP_OBJ_EF1,1);
        Set_relay_Fault();
    }
    
    if(!alarm_status[0] && ef_changed){
        if(!ef1){
            Nextion_SetObjAttr(display_obj_status[0],"pic",FAULT_ICON);
            if(!ch2Active)
                Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_SINGLE_FAULT_ICON);
            else
                Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_DOUBLE_FAULT_ICON);
            CyDelay(200);
            Set_relay_Fault();
        }else{
            if(!relay_present || (relay_present && !relay[3].latched) )
                Nextion_SetObjAttr(display_obj_status[0],"pic",OK_ICON);
            if(!ch2Active)
                Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_SINGLE_OK_ICON);
            else
                Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_DOUBLE_OK_ICON);
            CyDelay(200);
        }
    }
    
    
    prev_ef1 = ef1;
    
    // now check lowest priorities
    if(status_changed){
        if(sensor[0][STATUSLSB]==RUNTIME_SENSOR_CAL_FAULT){
            Nextion_ObjVisible(DISP_OBJ_SYS_READING1, 0);
            Nextion_ObjVisible(DISP_OBJ_TEXT1, 1);
            if(!ch2Active)
                Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_SINGLE_FAULT_ICON);
            else
                Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_DOUBLE_FAULT_ICON);
            CyDelay(200);
            Set_relay_Fault();
            
            system_SetLed(0, SYSTEM_LED_ORANGE, 0, ch2Active);
            LED_Update();
            sprintf(str, "CAL FAULT");
            Nextion_SetText(DISP_OBJ_TEXT1, str);
        }else if(sensor[0][STATUSLSB]==RUNTIME_SENSOR_IN_CALIBRATION){
            cal_wait=0;
            Nextion_ObjVisible(DISP_OBJ_SYS_READING1, 0);
            Nextion_ObjVisible(DISP_OBJ_TEXT1, 1);
            if(sensor[0][CALSTATUSLSB]==RUNTIME_SMART_SENSOR_ZERO_CAL_INPROGRESS){
                zero_cal_complete=0;
                if(!ch2Active)
                    Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_SINGLE_ZEROCAL_ICON);   
                else
                    Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_DOUBLE_ZEROCAL_ICON); 
            }
            if(sensor[0][CALSTATUSLSB]==RUNTIME_SMART_SENSOR_SPAN_CAL_INPROGRESS){
                span_cal_complete=0;
                if(!ch2Active)
                    Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_SINGLE_SPANCAL_ICON);   
                else
                    Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_DOUBLE_SPANCAL_ICON); 
            }
            CyDelay(200);
            sprintf(str, "CALIBRATION");
            Nextion_SetText(DISP_OBJ_TEXT1, str);
            
            prev_cal_status= sensor[0][CALSTATUSLSB];
        }
        else if ((sensor[0][STATUSLSB]==SYSREADING_OK)&& !alarm_status[0]){
            if(relay_present && relay[3].latched){
                uint8_t realy_state = GetRelayMask();
                if ( (!relay[3].energ && (realy_state>>3)==1) || (relay[3].energ && (realy_state>>3)==0))
                    Nextion_SetObjAttr(DISP_OBJ_STATUS1,"pic",FAULT_ICON);
            }
            if(!ch2Active)
                Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_SINGLE_OK_ICON);
            else
                Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_DOUBLE_OK_ICON);
            
            
            CyDelay(200);
        }
        
        if(prev_cal_status!=sensor[0][CALSTATUSLSB]){
            if(sensor[0][CALSTATUSLSB] == RUNTIME_SMART_SENSOR_ZERO_CAL_COMPLETE){
                zero_cal_complete=1;
            }
            if(sensor[0][CALSTATUSLSB] == RUNTIME_SMART_SENSOR_SPAN_CAL_COMPLETE){
                span_cal_complete=1;
            }
        }
    }    
    
    if(zero_cal_complete){
        if(cal_wait>8){
            zero_cal_complete=0;
            SaveZeroCalDate(0);
        }
        cal_wait++;
    }
    
    if(span_cal_complete){
        if(cal_wait>8){
            span_cal_complete=0;
            SaveSpanCalDate(0);
        }
        cal_wait++;
    }
    
     if ( (sensor[0][STATUSLSB]==POLL_SENSOR_STATUS_OK) ||(sensor[0][STATUSLSB]== RUNTIME_SENSOR_UNDERRANGE)||(sensor[0][STATUSLSB]== RUNTIME_SENSOR_OVERRANGE) ){
        //display sys reading here and 
        tmp[3]=sensor[0][SYSREADING1];
        tmp[2]=sensor[0][SYSREADING2];
        tmp[1]=sensor[0][SYSREADING3];
        tmp[0]=sensor[0][SYSREADING4];
        float fval;
        memcpy(&fval,tmp,sizeof(float)) ;
        uint8_t res1 = sensor[0][RESOLUTIONLSB];
        Nextion_ObjVisible(DISP_OBJ_SYS_READING1, 1);
        Nextion_SendFloat(DISP_OBJ_SYS_READING1, (int32_t)(fval*pow(10, res1)),  res1);   
        prev_status = sensor[0][STATUSLSB];  
        return;
    }   
    
    prev_status = sensor[0][STATUSLSB];
    
}

void SetLED1(void){
    if(!SW1_Pressed && !SW2_Pressed && !SW4_Pressed){
        if(sensor[0][STATUSLSB]==POLL_SENSOR_STATUS_WARMUP){
            system_SetLed(0, SYSTEM_LED_GREEN, 1,ch2Active);
            return;
        }
        
        if(sensor[0][STATUSLSB]==RUNTIME_SENSOR_UNDERRANGE){
            system_SetLed(0, SYSTEM_LED_BLUE, 1,ch2Active);
            return;
        }
        if(sensor[0][STATUSLSB]==RUNTIME_SENSOR_OVERRANGE){
            system_SetLed(0, SYSTEM_LED_RED, 1,ch2Active);
            return;
        }
        
        if(sensor[0][STATUSLSB]==RUNTIME_SENSOR_IN_CALIBRATION){
            //if(sensor[0][CALSTATUSLSB]==RUNTIME_SMART_SENSOR_ZERO_CAL_INPROGRESS)
            //    system_SetLed(0, SYSTEM_LED_WHITE, 1,ch2Active);
            //if(sensor[0][CALSTATUSLSB]==RUNTIME_SMART_SENSOR_SPAN_CAL_INPROGRESS){
                switch(sensor[0][CALSTATUSLSB]){
                    case RUNTIME_SMART_SENSOR_ZERO_CAL_INPROGRESS:
                        system_SetLed(0, SYSTEM_LED_WHITE, 1,ch2Active);
                    break;    
                    case RUNTIME_SMART_SENSOR_SPAN_CAL_INPROGRESS: 
                    if(blinker)
                        system_SetLed(0, SYSTEM_LED_MAGENTA, 0, ch2Active);
                    else
                        system_SetLed(0, SYSTEM_LED_OFF, 0, ch2Active);
                    break;
                    case RUNTIME_SMART_SENSOR_SPAN_CAL_NOGASAPPLIED:
                        if(blinker)
                            system_SetLed(0, SYSTEM_LED_ORANGE, 0, ch2Active);
                        else
                            system_SetLed(0, SYSTEM_LED_OFF, 0, ch2Active);        
                    break; 
                    case RUNTIME_SMART_SENSOR_SPAN_CAL_GASDETECTED:
                        system_SetLed(0, SYSTEM_LED_MAGENTA, 0, ch2Active);
                    break;
                    case RUNTIME_SMART_SENSOR_SPAN_CAL_AS_FOUND_SAVED:
                        system_SetLed(0, SYSTEM_LED_MAGENTA, 0, ch2Active);    
                    break; 
                    case RUNTIME_SMART_SENSOR_SPAN_CAL_ADJUSTED:
                        system_SetLed(0, SYSTEM_LED_MAGENTA, 0, ch2Active);
                    break; 
                    case RUNTIME_SMART_SENSOR_SPAN_CAL_STABILITYCHECKSTARTED:
                        system_SetLed(0, SYSTEM_LED_MAGENTA, 1, ch2Active);    
                    break; 
                    case RUNTIME_SMART_SENSOR_SPAN_CAL_STABILITYCHECKFAILED: 
                        if(blinker)
                            system_SetLed(0, SYSTEM_LED_ORANGE, 0, ch2Active);
                        else
                            system_SetLed(0, SYSTEM_LED_OFF, 0, ch2Active);   
                    break;
                    case RUNTIME_SMART_SENSOR_SPAN_CAL_REMOVEGAS:
                        if(blinker)
                            system_SetLed(0, SYSTEM_LED_MAGENTA, 0, ch2Active);
                         else
                            system_SetLed(0, SYSTEM_LED_OFF, 0, ch2Active);        
                    break;
                    case RUNTIME_SMART_SENSOR_SPAN_CAL_COMPLETE:break;
                    case RUNTIME_SMART_SENSOR_SPAN_CAL_CLEARINGFAILED:
                        if(blinker)
                            system_SetLed(0, SYSTEM_LED_ORANGE, 0, ch2Active);
                        else
                            system_SetLed(0, SYSTEM_LED_OFF, 0, ch2Active);
                    break;
            
                }
            //}    
            return;
        }
        
       
        
        switch(alarm_status[0]){
            case SYSREADING_OK:
                if(ef1){
                    if(sysReading[0]<0)
                        system_SetLed(0, SYSTEM_LED_BLUE, 1,ch2Active);
                    else
                        system_SetLed(0, SYSTEM_LED_GREEN, 0,ch2Active);
                }
                else
                {
                    if(!strobe)
                        system_SetLed(0, SYSTEM_LED_BLUE, 0,ch2Active);
                    else
                        system_SetLed(0, SYSTEM_LED_OFF, 0,ch2Active);
                }    
            break;
            case SYSREADING_ALARM1:
                if(!strobe)
                        system_SetLed(0, SYSTEM_LED_YELLOW, 0,ch2Active);
                else
                        system_SetLed(0, SYSTEM_LED_OFF, 0,ch2Active);    
            break;
            case SYSREADING_ALARM2:
                if(!strobe)
                    system_SetLed(0, SYSTEM_LED_RED, 0,ch2Active);
                else
                    system_SetLed(0, SYSTEM_LED_OFF, 0,ch2Active);            
            break;
            case SYSREADING_ALARM3:system_SetLed(0, SYSTEM_LED_RED, 0, ch2Active); break;
            case SYSREADING_FAULT: system_SetLed(0, SYSTEM_LED_BLUE, 0, ch2Active); break;
            case CAL_FAULT:
                if(!strobe)
                    system_SetLed(0, SYSTEM_LED_ORANGE, 0, ch2Active);
                else
                    system_SetLed(0, SYSTEM_LED_OFF, 0, ch2Active);
            break;
            
        }
        
        
        
    }//SW

}



void SetLED2(void){
    if(!SW1_Pressed && !SW2_Pressed && !SW4_Pressed){
        if(sensor[1][STATUSLSB]==POLL_SENSOR_STATUS_WARMUP){
            system_SetLed(1, SYSTEM_LED_GREEN, 1,ch2Active);
            return;
        }
        
        if(sensor[1][STATUSLSB]==RUNTIME_SENSOR_UNDERRANGE){
            system_SetLed(1, SYSTEM_LED_BLUE, 1,ch2Active);
            return;
        }
        if(sensor[1][STATUSLSB]==RUNTIME_SENSOR_OVERRANGE){
            system_SetLed(1, SYSTEM_LED_RED, 1,ch2Active);
            return;
        }
        
        if(sensor[1][STATUSLSB]==RUNTIME_SENSOR_IN_CALIBRATION){
            //if(sensor[1][CALSTATUSLSB]==RUNTIME_SMART_SENSOR_ZERO_CAL_INPROGRESS)
            //    system_SetLed(1, SYSTEM_LED_WHITE, 1,ch2Active);
                
            //if(sensor[1][CALSTATUSLSB]==RUNTIME_SMART_SENSOR_SPAN_CAL_INPROGRESS){
                switch(sensor[1][CALSTATUSLSB]){
                    case RUNTIME_SMART_SENSOR_ZERO_CAL_INPROGRESS:
                        system_SetLed(1, SYSTEM_LED_WHITE, 1,ch2Active);
                    break;
                    case RUNTIME_SMART_SENSOR_SPAN_CAL_INPROGRESS: 
                    if(blinker)
                        system_SetLed(1, SYSTEM_LED_MAGENTA, 0, ch2Active);
                    else
                        system_SetLed(1, SYSTEM_LED_OFF, 0, ch2Active);
                    break;
                    case RUNTIME_SMART_SENSOR_SPAN_CAL_NOGASAPPLIED:
                        if(blinker)
                            system_SetLed(1, SYSTEM_LED_ORANGE, 0, ch2Active);
                        else
                            system_SetLed(1, SYSTEM_LED_OFF, 0, ch2Active);        
                    break; 
                    case RUNTIME_SMART_SENSOR_SPAN_CAL_GASDETECTED:
                        system_SetLed(1, SYSTEM_LED_MAGENTA, 0, ch2Active);
                    break;
                    case RUNTIME_SMART_SENSOR_SPAN_CAL_AS_FOUND_SAVED:
                        system_SetLed(1, SYSTEM_LED_MAGENTA, 0, ch2Active);    
                    break; 
                    case RUNTIME_SMART_SENSOR_SPAN_CAL_ADJUSTED:
                        system_SetLed(1, SYSTEM_LED_MAGENTA, 0, ch2Active);
                    break; 
                    case RUNTIME_SMART_SENSOR_SPAN_CAL_STABILITYCHECKSTARTED:
                        system_SetLed(1, SYSTEM_LED_MAGENTA, 1, ch2Active);    
                    break; 
                    case RUNTIME_SMART_SENSOR_SPAN_CAL_STABILITYCHECKFAILED: 
                        if(blinker)
                            system_SetLed(1, SYSTEM_LED_ORANGE, 0, ch2Active);
                        else
                            system_SetLed(1, SYSTEM_LED_OFF, 0, ch2Active);   
                    break;
                    case RUNTIME_SMART_SENSOR_SPAN_CAL_REMOVEGAS:
                        if(blinker)
                            system_SetLed(1, SYSTEM_LED_MAGENTA, 0, ch2Active);
                         else
                            system_SetLed(1, SYSTEM_LED_OFF, 0, ch2Active);        
                    break;
                    case RUNTIME_SMART_SENSOR_SPAN_CAL_COMPLETE:break;
                    case RUNTIME_SMART_SENSOR_SPAN_CAL_CLEARINGFAILED:
                        if(blinker)
                            system_SetLed(1, SYSTEM_LED_ORANGE, 0, ch2Active);
                        else
                            system_SetLed(1, SYSTEM_LED_OFF, 0, ch2Active);
                    break;
            
                }
            //}        
            return;
        }
        
        
        
        switch(alarm_status[1]){
            case SYSREADING_OK:
                if(ef2){
                    if(sysReading[1]<0)
                        system_SetLed(1, SYSTEM_LED_BLUE, 1,ch2Active);
                    else
                        system_SetLed(1, SYSTEM_LED_GREEN, 0,ch2Active);
                }
                else
                {
                    if(!strobe)
                        system_SetLed(1, SYSTEM_LED_BLUE, 0,ch2Active);
                    else
                        system_SetLed(1, SYSTEM_LED_OFF, 0,ch2Active);
                }
                
            break;
            case SYSREADING_ALARM1:
                if(!strobe)
                        system_SetLed(1, SYSTEM_LED_YELLOW, 0,ch2Active);
                else
                        system_SetLed(1, SYSTEM_LED_OFF, 0,ch2Active);    
            break;
            case SYSREADING_ALARM2:
                if(!strobe)
                    system_SetLed(1, SYSTEM_LED_RED, 0,ch2Active);
                else
                    system_SetLed(1, SYSTEM_LED_OFF, 0,ch2Active);            
            break;
            case SYSREADING_ALARM3:system_SetLed(1, SYSTEM_LED_RED, 0, ch2Active); break;
            case SYSREADING_FAULT: system_SetLed(1, SYSTEM_LED_BLUE, 0, ch2Active); break;
            case CAL_FAULT:
                if(!strobe)
                    system_SetLed(1, SYSTEM_LED_ORANGE, 0, ch2Active);
                else
                    system_SetLed(1, SYSTEM_LED_OFF, 0, ch2Active);
            break;
        }
    }//SW

}

/////////////////////////////////////////////////////////////////////////////////////////////////
void HandleSensor2(uint8_t forced){

    char str[20];
    static uint8_t prev_status;
    static uint8_t prev_ef2;
    uint8_t status_changed=0;
    uint8_t ef_changed=0;
    uint32_t warmupcnt;
    uint8_t in_alarm=0;
    uint8_t tmp[4];
    static uint8_t prev_alarm;
    static uint8_t prev_cal_status;
    static uint8_t wait;
    static uint8_t cal_wait;
    static uint8_t  zero_cal_complete;
    static uint8_t  span_cal_complete;
    
    
    
    if(prev_status != sensor[1][STATUSLSB])status_changed=1;
    ef2= EF2_Read();
    if(prev_ef2 !=ef2)ef_changed=1;
    
    
    
    
    
    
    if(status_changed || forced){
        if(forced)prev_alarm=0;
        if( (sensor[1][STATUSLSB]==POLL_SENSOR_STATUS_ABSENT)||
            (sensor[1][STATUSLSB]==RUNTIME_SENSOR_CAL_FAULT)){
            if(sensor[1][STATUSLSB]==RUNTIME_SENSOR_CAL_FAULT)
                alarm_status[1]=CAL_FAULT;
            else
                alarm_status[1]=SYSREADING_FAULT;
                
            alarm_mask[1]=(1<<3);
            
            system_SetLed(1, SYSTEM_LED_BLUE, 0,ch2Active);
            Nextion_ObjVisible(DISP_OBJ_SYS_READING2, 0);
            Nextion_ObjVisible(DISP_OBJ_TEXT2, 1);
            Nextion_SetObjAttr(DISP_OBJ_STATUS2,"pic",FAULT_ICON);
            Nextion_SetObjAttr(DISP_OBJ_RING2,"pic",RING_DOUBLE_FAULT_ICON);
            CyDelay(200);
            Set_relay_Fault();
            if(sensor[1][STATUSLSB]==RUNTIME_SENSOR_CAL_FAULT){
                sprintf(str, "CAL FAULT");
                sprintf(log_txt,"%02d/%02d/%02d at %02d:%02d CH2 CAL FAULT",date_time.month, date_time.day, date_time.year, date_time.hours, date_time.minutes );
                SaveLogEntry(&log_last_entry_idx, log_txt);
            }
            else if(sensor[1][STATUSLSB]==POLL_SENSOR_STATUS_ABSENT){
                sprintf(str, "NO SENSOR");
                sprintf(log_txt,"%02d/%02d/%02d at %02d:%02d CH2 FAULT",date_time.month, date_time.day, date_time.year, date_time.hours, date_time.minutes);
                SaveLogEntry(&log_last_entry_idx, log_txt);
            }else{
                sprintf(str, "FAULT");
                sprintf(log_txt,"%02d/%02d/%02d at %02d:%02d CH2 FAULT",date_time.month, date_time.day, date_time.year, date_time.hours, date_time.minutes);
                SaveLogEntry(&log_last_entry_idx, log_txt);
            
            }   
                
            Nextion_SetText(DISP_OBJ_TEXT2, str);
            prev_status = sensor[1][STATUSLSB];
            sensor_status[1].alarm1=0;
            sensor_status[1].alarm2=0;
            sensor_status[1].alarm3=0;
            return;
        }else if(sensor[1][STATUSLSB]==POLL_SENSOR_STATUS_WARMUP){
            system_SetLed(1, SYSTEM_LED_GREEN, 1,ch2Active);
            Nextion_ObjVisible(DISP_OBJ_SYS_READING2, 0);
            Nextion_ObjVisible(DISP_OBJ_TEXT2, 1);
            Nextion_SetObjAttr(DISP_OBJ_STATUS2,"pic",WARMUP_ICON);
            Nextion_SetObjAttr(DISP_OBJ_RING2,"pic",RING_DOUBLE_OK_ICON);
            CyDelay(200);
            sensor_status[1].alarm1=0;
            sensor_status[1].alarm2=0;
            sensor_status[1].alarm3=0;
            
        }else if(sensor[1][STATUSLSB]== RUNTIME_SENSOR_OVERRANGE){
            alarm_mask[1]=(1<<3);
            Set_relay_Fault();
            RecalculateAlarms(1);
        }else if(sensor[1][STATUSLSB]== RUNTIME_SENSOR_UNDERRANGE){
            alarm_mask[1]=(1<<3);
            alarm_status[1]=SYSREADING_FAULT;
            system_SetLed(1, SYSTEM_LED_BLUE, 1,ch2Active);
            Nextion_ObjVisible(DISP_OBJ_SYS_READING2, 1);
            Nextion_ObjVisible(DISP_OBJ_TEXT1, 0);
            Nextion_SetObjAttr(DISP_OBJ_STATUS2,"pic",FAULT_ICON);
            Nextion_SetObjAttr(DISP_OBJ_RING2,"pic",RING_DOUBLE_FAULT_ICON);
            CyDelay(200);
        
            Set_relay_Fault();
        }
        else if (sensor[1][STATUSLSB]==POLL_SENSOR_STATUS_OK){
            Nextion_ObjVisible(DISP_OBJ_SYS_READING2, 1);
            Nextion_ObjVisible(DISP_OBJ_TEXT2, 0);
            if(!relay_present || ( relay_present && !relay[3].latched))
                Nextion_SetObjAttr(DISP_OBJ_STATUS2,"pic",OK_ICON);
            RecalculateAlarms(1);    
                  
        }
    }
    
    if(sensor[1][STATUSLSB]==POLL_SENSOR_STATUS_WARMUP){
        system_SetLed(1, SYSTEM_LED_GREEN, 1,ch2Active);
        warmupcnt = (uint16_t)(sensor[1][WARMUPTIMERMSB]<<8)+sensor[1][WARMUPTIMERLSB];
        uint16_t h= warmupcnt/3600;
        sprintf(str, "%02d:%02d:%02d",h,(warmupcnt - (h*3600))/60, warmupcnt%60  );
        Nextion_SetText(DISP_OBJ_TEXT2, str);
        prev_status = sensor[1][STATUSLSB];
        return;
    }
    

    
    if(!sensor[1][SERIALNUMBER1]&&!sensor[1][SERIALNUMBER2]&&!sensor[1][SERIALNUMBER3]&&
       !sensor[1][SERIALNUMBER4]&&!sensor[1][SERIALNUMBER5]&&!sensor[1][SERIALNUMBER6])
        wait =1;
    else{
        if(wait)
            RecalculateAlarms(1);
        wait =0;
    }
    
    if (sensor[1][STATUSLSB]==POLL_SENSOR_STATUS_OK && !wait){
        alarm_status[1] =CheckAlarms(1); 
        if(prev_alarm!= alarm_status[1]){
                switch(alarm_status[1]){
                    case SYSREADING_ALARM1: 
                        Nextion_SetObjAttr(DISP_OBJ_RING2,"pic",RING_DOUBLE_ALARM1_ICON);
                        Set_relay_Alarm1();
                        sprintf(log_txt,"%02d/%02d/%02d at %02d:%02d CH2 ALARM1",date_time.month, date_time.day, date_time.year, date_time.hours, date_time.minutes);
                        SaveLogEntry(&log_last_entry_idx, log_txt);
                    break;
                    case SYSREADING_ALARM2: 
                        Nextion_SetObjAttr(DISP_OBJ_RING2,"pic",RING_DOUBLE_ALARM2_ICON);  
                        Set_relay_Alarm2();
                        sprintf(log_txt,"%02d/%02d/%02d at %02d:%02d CH2 ALARM2",date_time.month, date_time.day, date_time.year, date_time.hours, date_time.minutes);
                        SaveLogEntry(&log_last_entry_idx, log_txt);
                    break;
                    case SYSREADING_ALARM3: 
                        Nextion_SetObjAttr(DISP_OBJ_RING2,"pic",RING_DOUBLE_ALARM3_ICON);   
                        Set_relay_Alarm3();
                        sprintf(log_txt,"%02d/%02d/%02d at %02d:%02d CH2 ALARM3",date_time.month, date_time.day, date_time.year, date_time.hours, date_time.minutes);
                        SaveLogEntry(&log_last_entry_idx, log_txt);
                    break;
                    case SYSREADING_OK:
                        if(!ef2){
                            Nextion_SetObjAttr(DISP_OBJ_RING2,"pic",RING_DOUBLE_FAULT_ICON);
                        }
                        else{
                            Nextion_SetObjAttr(DISP_OBJ_RING2,"pic",RING_DOUBLE_OK_ICON);
                        }
                    break;
                }
            }
        prev_alarm= alarm_status[1];
    }
    
    // no alrams - check EF
    if(ef2){
        Nextion_ObjVisible(DISP_OBJ_EF2,0);
    }else{
        Nextion_ObjVisible(DISP_OBJ_EF2,1);
        Set_relay_Fault();
    }
    
    if(!alarm_status[1] && ef_changed){
        if(!ef2){
            Nextion_SetObjAttr(display_obj_status[1],"pic",FAULT_ICON);
            Nextion_SetObjAttr(DISP_OBJ_RING2,"pic",RING_DOUBLE_FAULT_ICON);
            CyDelay(200);
            Set_relay_Fault();
        }else{
            if(!relay_present || (relay_present && !relay[3].latched) )
                Nextion_SetObjAttr(display_obj_status[1],"pic",OK_ICON);
            Nextion_SetObjAttr(DISP_OBJ_RING2,"pic",RING_DOUBLE_OK_ICON);
            CyDelay(200);
        }
    }
    prev_ef2 = ef2;
    
    // now check lowest priorities
    if(status_changed){
        if(sensor[1][STATUSLSB]==RUNTIME_SENSOR_CAL_FAULT){
            Nextion_ObjVisible(DISP_OBJ_SYS_READING2, 0);
            Nextion_ObjVisible(DISP_OBJ_TEXT2, 1);
            Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_DOUBLE_FAULT_ICON);
            CyDelay(200);
            Set_relay_Fault();
            sprintf(str, "CAL FAULT");
            Nextion_SetText(DISP_OBJ_TEXT2, str);
        }else if(sensor[1][STATUSLSB]==RUNTIME_SENSOR_IN_CALIBRATION){
            cal_wait=0;
            Nextion_ObjVisible(DISP_OBJ_SYS_READING2, 0);
            Nextion_ObjVisible(DISP_OBJ_TEXT2, 1);
            if(sensor[1][CALSTATUSLSB]==RUNTIME_SMART_SENSOR_ZERO_CAL_INPROGRESS){
                Nextion_SetObjAttr(DISP_OBJ_RING2,"pic",RING_DOUBLE_ZEROCAL_ICON); 
            }
            if(sensor[1][CALSTATUSLSB]==RUNTIME_SMART_SENSOR_SPAN_CAL_INPROGRESS){
                Nextion_SetObjAttr(DISP_OBJ_RING2,"pic",RING_DOUBLE_SPANCAL_ICON); 
            }
            CyDelay(200);
            sprintf(str, "CALIBRATION");
            Nextion_SetText(DISP_OBJ_TEXT2, str);
        }else if((sensor[1][STATUSLSB]==SYSREADING_OK)&& !alarm_status[1]){
            Nextion_SetObjAttr(DISP_OBJ_RING2,"pic",RING_DOUBLE_OK_ICON);
            if(relay_present && relay[3].latched){
                uint8_t realy_state = GetRelayMask();
                if ( (!relay[3].energ && (realy_state>>3)==1) || (relay[3].energ && (realy_state>>3)==0))
                    Nextion_SetObjAttr(DISP_OBJ_STATUS2,"pic",FAULT_ICON);
            }
               
                
            CyDelay(200);
        }
        
        if(prev_cal_status!=sensor[1][CALSTATUSLSB]){
            if(sensor[1][CALSTATUSLSB] == RUNTIME_SMART_SENSOR_ZERO_CAL_COMPLETE){
                zero_cal_complete=1;
            }
            if(sensor[1][CALSTATUSLSB] == RUNTIME_SMART_SENSOR_SPAN_CAL_COMPLETE){
                span_cal_complete=1;
            }
        }
    }    
    
    if(zero_cal_complete){
        if(cal_wait>8){
            zero_cal_complete=0;
            SaveZeroCalDate(1);
        }
        cal_wait++;
    }
    
    if(span_cal_complete){
        if(cal_wait>8){
            span_cal_complete=0;
            SaveSpanCalDate(1);
        }
        cal_wait++;
    }
    
          
     if ( (sensor[1][STATUSLSB]==POLL_SENSOR_STATUS_OK)||(sensor[1][STATUSLSB]== RUNTIME_SENSOR_UNDERRANGE)||(sensor[1][STATUSLSB]== RUNTIME_SENSOR_OVERRANGE)){
        //display sys reading here and 
        tmp[3]=sensor[1][SYSREADING1];
        tmp[2]=sensor[1][SYSREADING2];
        tmp[1]=sensor[1][SYSREADING3];
        tmp[0]=sensor[1][SYSREADING4];
        float fval;
        memcpy(&fval,tmp,sizeof(float)) ;
        uint8_t res1 = sensor[1][RESOLUTIONLSB];
        Nextion_ObjVisible(DISP_OBJ_SYS_READING2, 1);
        Nextion_SendFloat(DISP_OBJ_SYS_READING2, (int32_t)(fval*pow(10, res1)),  res1);   
        prev_status = sensor[1][STATUSLSB];  
        return;
    }   
    
    prev_status = sensor[1][STATUSLSB];
    
}


void HandleRelays(void){

    if(!relay[0].latched){
        if(!sensor_status[0].alarm1&&!sensor_status[1].alarm1){
            Reset_relay_Alarm1();
            alarm_mask[0] &=~(1<<0); 
            alarm_mask[1] &=~(1<<0);
        }
    }
    if(!relay[1].latched){
        if(!sensor_status[0].alarm2&&!sensor_status[1].alarm2){
            Reset_relay_Alarm2();
            alarm_mask[0] &=~(1<<1); 
            alarm_mask[1] &=~(1<<1);
        }
    }
    if(!relay[2].latched){
        if(!sensor_status[0].alarm3&&!sensor_status[1].alarm3){
            Reset_relay_Alarm3();
            alarm_mask[0] &=~(1<<2); 
            alarm_mask[1] &=~(1<<2);
        }
    }
    if(!relay[3].latched){
        if( ((sensor[0][STATUSLSB]==POLL_SENSOR_STATUS_OK)|| (sensor[0][STATUSLSB]==POLL_SENSOR_STATUS_WARMUP))&&
            ((sensor[1][STATUSLSB]==POLL_SENSOR_STATUS_OK) || (sensor[1][STATUSLSB]==POLL_SENSOR_STATUS_WARMUP))
            && ef1 && ef2){
            Reset_relay_Fault();
            alarm_mask[0] &=~(1<<3); 
            alarm_mask[1] &=~(1<<3);
        }
            
    }
    
    if(reset_alarms){
        reset_alarms=0;
        if(  ( ((sensor_status[0].alarm3asc) && (sysReading[0]< sensor_status[0].alarm3fval)) ||
             ((!sensor_status[0].alarm3asc) && (sysReading[0]> sensor_status[0].alarm3fval))) &&
        
            ( ((sensor_status[1].alarm3asc) && (sysReading[1]< sensor_status[1].alarm3fval)) ||
             ((!sensor_status[1].alarm3asc) && (sysReading[1]> sensor_status[1].alarm3fval)))
        ){
            Reset_relay_Alarm3();
            
        }    
         
        if(  ( ((sensor_status[0].alarm2asc) && (sysReading[0]< sensor_status[0].alarm2fval)) ||
             ((!sensor_status[0].alarm2asc) && (sysReading[0]> sensor_status[0].alarm2fval))) &&
        
            ( ((sensor_status[1].alarm2asc) && (sysReading[1]< sensor_status[1].alarm2fval)) ||
             ((!sensor_status[1].alarm2asc) && (sysReading[1]> sensor_status[1].alarm2fval)))
        ){
            Reset_relay_Alarm2();
        }
        
        if(  ( ((sensor_status[0].alarm1asc) && (sysReading[0]< sensor_status[0].alarm1fval)) ||
             ((!sensor_status[0].alarm1asc) && (sysReading[0]> sensor_status[0].alarm1fval))) &&
        
            ( ((sensor_status[1].alarm1asc) && (sysReading[1]< sensor_status[1].alarm1fval)) ||
             ((!sensor_status[1].alarm1asc) && (sysReading[1]> sensor_status[1].alarm1fval)))
        ){
            Reset_relay_Alarm1();
        }
        
        if( ((sensor[0][STATUSLSB]==POLL_SENSOR_STATUS_OK)||(sensor[0][STATUSLSB]==POLL_SENSOR_STATUS_WARMUP))&&
            ((sensor[1][STATUSLSB]==POLL_SENSOR_STATUS_OK)|| (sensor[1][STATUSLSB]==POLL_SENSOR_STATUS_WARMUP)) &&
            ef1 && ef2){
            Reset_relay_Fault();
            alarm_mask[0] &=~(1<<3); 
            alarm_mask[1] &=~(1<<3);
        }
    }
}

void Handle_Icons(){
    if( ((alarm_mask[0]>>3)&0x1) || ((alarm_mask[1]>>3)&0x1) || ((alarm_mask[0]>>4)&0x1) || ((alarm_mask[1]>>4)&0x1) ){
        if( ((alarm_mask[0]>>3)&0x1) || ((alarm_mask[0]>>4)&0x1) )
            Nextion_SetObjAttr(display_obj_status[0],"pic",FAULT_ICON);
        if( ((alarm_mask[1]>>3)&0x1) || ((alarm_mask[1]>>4)&0x1) )
            Nextion_SetObjAttr(display_obj_status[1],"pic",FAULT_ICON);
        return;    
    }
    
    if( (alarm_mask[0]>>2)&0x1 )
            Nextion_SetObjAttr(display_obj_status[0],"pic",ALARM3_ICON);
    else if ((alarm_mask[0]>>1)&0x1)         
            Nextion_SetObjAttr(display_obj_status[0],"pic",ALARM2_ICON);
    else if (alarm_mask[0]&0x1)        
            Nextion_SetObjAttr(display_obj_status[0],"pic",ALARM1_ICON);
    else
            Nextion_SetObjAttr(display_obj_status[0],"pic",OK_ICON);
            
            
    if( (alarm_mask[1]>>2)&0x1 )
            Nextion_SetObjAttr(display_obj_status[1],"pic",ALARM3_ICON);
    else if ((alarm_mask[1]>>1)&0x1)         
            Nextion_SetObjAttr(display_obj_status[1],"pic",ALARM2_ICON);
    else if (alarm_mask[1]&0x1)        
            Nextion_SetObjAttr(display_obj_status[1],"pic",ALARM1_ICON);
    else
            Nextion_SetObjAttr(display_obj_status[1],"pic",OK_ICON);        
}

void DisplayRealTime(uint8_t ch2Active){
    static uint8_t prev_num;
    static int16 val,val2;
    static float ival,ival2;
    static float fval, fval2;
    static uint8_t dir;    
    static uint8_t outer_guage_base;
    static uint8_t inner_guage_base;
    
    
    uint8_t tmp[4];
    
    uint32_t warmupcnt;
    char str[20];
    
    static uint8_t prev_status[2];
    
    if(ch2Active){
        //if((prev_status[1]!=sensor[1][STATUSLSB]) && ef2 ){
            if(sensor[1][STATUSLSB]==RUNTIME_SENSOR_NORMAL && ef2){
                Nextion_ObjVisible(DISP_OBJ_TEXT2, 0);
                Nextion_ObjVisible(DISP_OBJ_SYS_READING2, 1);
            }
            if(sensor[1][STATUSLSB]==RUNTIME_SENSOR_IN_WARMUP || 
               sensor[1][STATUSLSB]==RUNTIME_SENSOR_IN_CALIBRATION || sensor[1][STATUSLSB]==POLL_SENSOR_STATUS_ABSENT ){
                Nextion_ObjVisible(DISP_OBJ_SYS_READING2, 0);
                Nextion_ObjVisible(DISP_OBJ_TEXT2, 1);
                
            }
        //}
    }
    
    
    //if((prev_status[0]!=sensor[0][STATUSLSB])  ){
        
        if(sensor[0][STATUSLSB]==RUNTIME_SENSOR_NORMAL && ef1){
            Nextion_ObjVisible(DISP_OBJ_TEXT1, 0);
            Nextion_ObjVisible(DISP_OBJ_SYS_READING1, 1);
        }
        if(sensor[0][STATUSLSB]==RUNTIME_SENSOR_IN_WARMUP || 
           sensor[0][STATUSLSB]==RUNTIME_SENSOR_IN_CALIBRATION|| sensor[0][STATUSLSB]==POLL_SENSOR_STATUS_ABSENT){
            Nextion_ObjVisible(DISP_OBJ_SYS_READING1, 0);
            Nextion_ObjVisible(DISP_OBJ_TEXT1, 1);
        }
    //}
    
    
    if(!ef1){
        sprintf(str, "mA FAULT");
        Nextion_SetText(DISP_OBJ_TEXT1, str);
    }
    
    if(sensor[0][STATUSLSB]==RUNTIME_SENSOR_NORMAL && ef1 ){
        tmp[3]=sensor[0][SYSREADING1];
        tmp[2]=sensor[0][SYSREADING2];
        tmp[1]=sensor[0][SYSREADING3];
        tmp[0]=sensor[0][SYSREADING4];
        memcpy(&fval,tmp,sizeof(float)) ;
        uint8_t res1 = sensor[0][RESOLUTIONLSB];
        Nextion_ObjVisible(DISP_OBJ_SYS_READING1, 1);
        Nextion_SendFloat(DISP_OBJ_SYS_READING1, (int32_t)(fval*pow(10, res1)),  res1);   
        
    }
    if(sensor[0][STATUSLSB]==POLL_SENSOR_STATUS_ABSENT){
        
        sprintf(str, "NO SENSOR");
        Nextion_SetText(DISP_OBJ_TEXT1, str);
    
    }
    if(sensor[0][STATUSLSB]==RUNTIME_SENSOR_IN_CALIBRATION){
        sprintf(str, "CALIBRATION");
        Nextion_SetText(DISP_OBJ_TEXT1, str);
    
    }
    if(sensor[0][STATUSLSB]==RUNTIME_SENSOR_CAL_FAULT){
        sprintf(str, "CAL FAULT");
        Nextion_SetText(DISP_OBJ_TEXT1, str);
    
    }
    if(sensor[0][STATUSLSB]==RUNTIME_SENSOR_IN_WARMUP){
        warmupcnt = (uint16_t)(sensor[0][WARMUPTIMERMSB]<<8)+sensor[0][WARMUPTIMERLSB];
        uint8_t h = warmupcnt/3600;
        sprintf(str, "%02d:%02d:%02d",h,(warmupcnt - (h*3600))/60, warmupcnt%60  );
        Nextion_SetText(DISP_OBJ_TEXT1, str);
    }
    
    
    
    prev_status[0] = sensor[0][STATUSLSB];
    
    if(ch2Active){
        if(!ef2){
            sprintf(str, "mA FAULT");
            Nextion_SetText(DISP_OBJ_TEXT2, str);
        }
        if(sensor[1][STATUSLSB]==RUNTIME_SENSOR_NORMAL && ef2){
            tmp[3]=sensor[1][SYSREADING1];
            tmp[2]=sensor[1][SYSREADING2];
            tmp[1]=sensor[1][SYSREADING3];
            tmp[0]=sensor[1][SYSREADING4];
            memcpy(&fval2,tmp,sizeof(float)) ;
            uint8_t res2 = sensor[1][RESOLUTIONLSB];
            Nextion_ObjVisible(DISP_OBJ_SYS_READING2, 1);
            Nextion_SendFloat(DISP_OBJ_SYS_READING2, (int32_t)(fval2*pow(10, res2)),  res2); 
        }
        if(sensor[1][STATUSLSB]==POLL_SENSOR_STATUS_ABSENT){
            sprintf(str, "NO SENSOR");
            Nextion_SetText(DISP_OBJ_TEXT2, str);
        }
        if(sensor[1][STATUSLSB]==RUNTIME_SENSOR_IN_CALIBRATION){
            sprintf(str, "CALIBRATION");
            Nextion_SetText(DISP_OBJ_TEXT2, str);
        }
        
        if(sensor[1][STATUSLSB]==RUNTIME_SENSOR_CAL_FAULT){
            sprintf(str, "CAL FAULT");
            Nextion_SetText(DISP_OBJ_TEXT2, str);
        }
        
        if(sensor[1][STATUSLSB]==RUNTIME_SENSOR_IN_WARMUP){
            warmupcnt = (uint16_t)(sensor[1][WARMUPTIMERMSB]<<8)+sensor[1][WARMUPTIMERLSB];
            uint16_t h = warmupcnt/3600;
            sprintf(str, "%02d:%02d:%02d",h,(warmupcnt-(h*3600))/60, warmupcnt%60  );
            Nextion_SetText(DISP_OBJ_TEXT2, str);
        }
        
        
        
    }
    prev_status[1] = sensor[1][STATUSLSB];
                 
}


//void HandleSensorData(uint8_t ch){
//    char str[10];
//    
//    if(sensor[ch][STATUSLSB]==POLL_SENSOR_STATUS_OK)
//        alarm_status[ch] = CheckAlarms(ch);
//    else if(sensor[ch][STATUSLSB]==POLL_SENSOR_STATUS_ABSENT)     
//        alarm_status[ch] = SYSREADING_FAULT;
//    
//    // Blinkwers for Alarm1 and Alarm2
//        
//    if(!system_IsLEDInBreathMode(ch)&&(!SW1_Pressed)&&(!SW2_Pressed)&&(!SW4_Pressed)){    
//        if(alarm_status[ch]==SYSREADING_ALARM1){
//            if(!blinker)
//                system_SetLed(ch, SYSTEM_LED_YELLOW, 0,ch2Active);
//            else
//               system_SetLed(ch, SYSTEM_LED_OFF, 0,ch2Active);
//        }else if (alarm_status[ch]==SYSREADING_ALARM2){
//            if(!blinker)
//                system_SetLed(ch, SYSTEM_LED_RED, 0,ch2Active);
//            else
//               system_SetLed(ch, SYSTEM_LED_OFF, 0,ch2Active);
//        }
//    }
//    
//    
//    if( (prev_status[ch]!= sensor[ch][STATUSLSB])|| (prev_alarm_status[ch] != alarm_status[ch])){
//            if(ch==0){
//                switch(sensor[0][STATUSLSB]){
//                case POLL_SENSOR_STATUS_OK:
//                        if (alarm_status[0]==SYSREADING_OK ){
//                            if(prev_status[0] ==POLL_SENSOR_STATUS_WARMUP){
//                                if(ef1)
//                                    Nextion_SetObjAttr(DISP_OBJ_STATUS1,"pic",OK_ICON);
//                                else    
//                                    Nextion_SetObjAttr(DISP_OBJ_STATUS1,"pic",FAULT_ICON);
//                            }
//                            if(ef1){
//                                if(ch2Active){
//                                    Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_DOUBLE_OK_ICON);
//                                }
//                                else{
//                                    Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_SINGLE_OK_ICON);
//                                }
//                            system_SetLed(0, SYSTEM_LED_GREEN, 0,ch2Active);
//                            }else{
//                                if(ch2Active){
//                                    Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_DOUBLE_FAULT_ICON);
//                                }
//                                else{
//                                    Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_SINGLE_FAULT_ICON);
//                                }
//                            
//                            
//                            }
//                            
//                            
//                            
//                        }else if(alarm_status[0]==SYSREADING_ALARM1){
//                            //Nextion_SetObjAttr(DISP_OBJ_STATUS1,"pic",ALARM1_ICON);
//                            if(ch2Active){
//                                Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_DOUBLE_ALARM1_ICON);
//                            }
//                            else{
//                                Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_SINGLE_ALARM1_ICON);
//                            }
//                        }else if(alarm_status[0]==SYSREADING_ALARM2){
//                            //Nextion_SetObjAttr(DISP_OBJ_STATUS1,"pic",ALARM2_ICON);
//                            if(ch2Active){
//                                Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_DOUBLE_ALARM2_ICON);
//                            }
//                            else{
//                                Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_SINGLE_ALARM2_ICON);
//                            }
//                                
//                        }else if(alarm_status[0]==SYSREADING_ALARM3){
//                            Nextion_ObjVisible(DISP_OBJ_STATUS1,1);
//                            //Nextion_SetObjAttr(DISP_OBJ_STATUS1,"pic",ALARM3_ICON);
//                            if(ch2Active){
//                                Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_DOUBLE_ALARM3_ICON);
//                            }
//                            else{
//                                Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_SINGLE_ALARM3_ICON);
//                            }
//                            system_SetLed(0, SYSTEM_LED_RED, 0,ch2Active);
//                        }
//                break;    
//                case POLL_SENSOR_STATUS_ABSENT:
//                        Nextion_SetObjAttr(DISP_OBJ_STATUS1,"pic",FAULT_ICON);
//                        if(ch2Active){
//                            Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_DOUBLE_FAULT_ICON);
//                        }
//                        else{
//                            Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_SINGLE_FAULT_ICON);
//                        }
//                        system_SetLed(0, SYSTEM_LED_BLUE, 0,ch2Active);
//                        
//                break;
//                case POLL_SENSOR_STATUS_WARMUP:
//                    Nextion_SetObjAttr(DISP_OBJ_STATUS1,"pic",WARMUP_ICON);
//                    alarm_status[0]=0;
//                    if(ch2Active){
//                        Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_DOUBLE_OK_ICON);
//                    }
//                    else{
//                        Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_SINGLE_OK_ICON);
//                    }
//                    system_SetLed(0, SYSTEM_LED_GREEN, 1,ch2Active);
//                break;    
//            }
//        }
//        if(ch==1){
//            switch(sensor[1][STATUSLSB]){
//                case POLL_SENSOR_STATUS_OK:
//                        if (alarm_status[1]==SYSREADING_OK ){
//                            if(prev_status[1] ==POLL_SENSOR_STATUS_WARMUP){
//                                if(ef2)
//                                    Nextion_SetObjAttr(DISP_OBJ_STATUS2,"pic",OK_ICON);
//                                else    
//                                    Nextion_SetObjAttr(DISP_OBJ_STATUS2,"pic",FAULT_ICON);
//                            }
//                            if(ef2)        
//                                Nextion_SetObjAttr(DISP_OBJ_RING2,"pic",RING_DOUBLE_OK_ICON);
//                            else 
//                                Nextion_SetObjAttr(DISP_OBJ_RING2,"pic",RING_DOUBLE_FAULT_ICON);
//                            system_SetLed(1, SYSTEM_LED_GREEN, 0, ch2Active);
//                            
//                            
//                        }else if(alarm_status[1]==SYSREADING_ALARM1){
//                            //Nextion_SetObjAttr(DISP_OBJ_STATUS2,"pic",ALARM1_ICON);
//                            Nextion_SetObjAttr(DISP_OBJ_RING2,"pic",RING_DOUBLE_ALARM1_ICON);
//                        }else if(alarm_status[1]==SYSREADING_ALARM2){
//                            //Nextion_SetObjAttr(DISP_OBJ_STATUS2,"pic",ALARM2_ICON);
//                            Nextion_SetObjAttr(DISP_OBJ_RING2,"pic",RING_DOUBLE_ALARM2_ICON);
//                        }else if(alarm_status[1]==SYSREADING_ALARM3){
//                            //Nextion_SetObjAttr(DISP_OBJ_STATUS2,"pic",ALARM2_ICON);
//                            Nextion_SetObjAttr(DISP_OBJ_RING2,"pic",RING_DOUBLE_ALARM3_ICON);
//                            system_SetLed(1, SYSTEM_LED_RED, 0, ch2Active);
//                        }else if (alarm_status[1]==SYSREADING_420FAULT){
//                            
//                        }
//                break;
//                        case POLL_SENSOR_STATUS_ABSENT:
//                        Nextion_SetObjAttr(DISP_OBJ_STATUS2,"pic",FAULT_ICON);
//                        Nextion_SetObjAttr(DISP_OBJ_RING2,"pic",RING_DOUBLE_FAULT_ICON);
//                        system_SetLed(1, SYSTEM_LED_BLUE, 0, ch2Active);
//                break;
//                case POLL_SENSOR_STATUS_WARMUP:
//                    Nextion_SetObjAttr(DISP_OBJ_STATUS2,"pic",WARMUP_ICON);
//                    Nextion_SetObjAttr(DISP_OBJ_RING2,"pic",RING_DOUBLE_OK_ICON);
//                    system_SetLed(1, SYSTEM_LED_GREEN, 1,ch2Active);
//                    alarm_status[1]=0;
//                break;
//                
//            }
//        }
//        memset(str, 0, sizeof(str));
//        if(sensor[ch][STATUSLSB]==POLL_SENSOR_STATUS_ABSENT){
//            sprintf(log_txt,"%02d/%02d/%02d at %02d:%02d CH%d FAULT",date_time.day,date_time.month, date_time.year, date_time.hours, date_time.minutes, ch+1);
//            SaveLogEntry(&log_last_entry_idx, log_txt);
//        }
//        if(alarm_status[ch] == SYSREADING_ALARM1) {
//            sprintf(log_txt,"%02d/%02d/%02d at %02d:%02d CH%d ALARM1",date_time.day,date_time.month, date_time.year, date_time.hours, date_time.minutes, ch+1);
//            SaveLogEntry(&log_last_entry_idx, log_txt);
//        }
//        if(alarm_status[ch] == SYSREADING_ALARM2){
//            sprintf(log_txt,"%02d/%02d/%02d at %02d:%02d CH%d ALARM2",date_time.day,date_time.month, date_time.year, date_time.hours, date_time.minutes, ch+1);
//            SaveLogEntry(&log_last_entry_idx, log_txt);
//        }
//        if(alarm_status[ch] == SYSREADING_ALARM3){
//            sprintf(log_txt,"%02d/%02d/%02d at %02d:%02d CH%d ALARM3",date_time.day,date_time.month, date_time.year, date_time.hours, date_time.minutes, ch+1);
//            SaveLogEntry(&log_last_entry_idx, log_txt);
//        }    
//    }
//    
//    prev_status[ch]= sensor[ch][STATUSLSB];
//    prev_alarm_status[ch] = alarm_status[ch];
//}


uint8_t CheckAlarms(uint8_t ch){
    uint8_t tmp[4];
    tmp[3]= sensor[ch][SYSREADING1];
    tmp[2]= sensor[ch][SYSREADING2];
    tmp[1]= sensor[ch][SYSREADING3];
    tmp[0]= sensor[ch][SYSREADING4];
    memcpy(&sysReading[ch], &tmp, sizeof(float));    
    
    // Starting with Alarm3 check, then go further if no alarm
    // If below alarm level and not latched reset alarm
    
    //if(sysReading[ch]> sensor_status[ch].fullScaleRange)
    //    return SYSREADING_OVERRANGE;
    
    if(sysReading[ch]<  0)
        return SYSREADING_UNDERRANGE;
    
    if( ((sensor_status[ch].alarm3asc) && (sysReading[ch]>= sensor_status[ch].alarm3fval)) ||
        ((!sensor_status[ch].alarm3asc) && (sysReading[ch]<= sensor_status[ch].alarm3fval)) )  
    {   
        if(!sensor_status[ch].alarm3)
        {
            sensor_status[ch].alarm3=1;
            newEvent=1;
            alarm_mask[0]=(1<<2);
            Nextion_SetObjAttr(display_obj_status[ch],"pic",ALARM3_ICON);
        }
        
        if(!newEvent && ack_received && relay[2].ack)
            Nextion_SetObjAttr(display_obj_status[ch],"pic",OK_ICON);
        return SYSREADING_ALARM3;
    }else{
        if((!relay[2].latched && sensor_status[ch].alarm3)|| (relay[2].latched && reset_alarms) ){
            sensor_status[ch].alarm3=0;
            Nextion_SetObjAttr(display_obj_status[ch],"pic",OK_ICON);
            //Reset_relay_Alarm3();
        }
    }
     
    if( ((sensor_status[ch].alarm2asc) && (sysReading[ch]>= sensor_status[ch].alarm2fval)) ||
        ((!sensor_status[ch].alarm2asc) && (sysReading[ch]<= sensor_status[ch].alarm2fval)) )  
    {
        if(!sensor_status[ch].alarm2 )
        {
            sensor_status[ch].alarm2=1;
            newEvent=1;
            alarm_mask[0]=(1<<1);
        }
        
        if(relay[2].latched){
            if(!sensor_status[ch].alarm3){
                Nextion_SetObjAttr(display_obj_status[ch],"pic",ALARM2_ICON);   
            }
        }else{
            Nextion_SetObjAttr(display_obj_status[ch],"pic",ALARM2_ICON);   
        }
        
                
        
        if(!newEvent && ack_received && relay[1].ack)
            Nextion_SetObjAttr(display_obj_status[ch],"pic",OK_ICON);
        return SYSREADING_ALARM2;
    }else{
        if((!relay[1].latched && sensor_status[ch].alarm2 )|| (relay[1].latched && reset_alarms)){
            sensor_status[ch].alarm2=0;
            Nextion_SetObjAttr(display_obj_status[ch],"pic",OK_ICON);
            //Reset_relay_Alarm2();
        }
    }
    
    if( ((sensor_status[ch].alarm1asc) && (sysReading[ch]>= sensor_status[ch].alarm1fval)) ||
        ((!sensor_status[ch].alarm1asc) && (sysReading[ch]<= sensor_status[ch].alarm1fval)) )  
    {
        if(!sensor_status[ch].alarm1 ){
            sensor_status[ch].alarm1=1;
            newEvent=1;
            
            
            alarm_mask[0]=(1<<0);
        }
        
        if(relay[1].latched){
            if(!sensor_status[ch].alarm2){
                Nextion_SetObjAttr(display_obj_status[ch],"pic",ALARM1_ICON);
            }
        }else{
            Nextion_SetObjAttr(display_obj_status[ch],"pic",ALARM1_ICON);
        }
        
                
        if(!newEvent && ack_received && relay[0].ack)
            Nextion_SetObjAttr(display_obj_status[ch],"pic",OK_ICON);
        return SYSREADING_ALARM1;
    }else{
        if((!relay[0].latched && sensor_status[ch].alarm1 )||(relay[0].latched && reset_alarms)){
            sensor_status[ch].alarm1=0;
            Nextion_SetObjAttr(display_obj_status[ch],"pic",OK_ICON);
            //Reset_relay_Alarm1();
        }
            
    }
    
    
    if(!sensor_status[ch].alarm1 && !sensor_status[ch].alarm2 && !sensor_status[ch].alarm3)
        return SYSREADING_OK;
    
    return SYSREADING_OK; 
}


void CheckEF(){
    uint8_t cur_ef1 =EF1_Read();
    
    if(ef1!= cur_ef1){
        if(!cur_ef1){
            Nextion_ObjVisible(DISP_OBJ_SYS_READING1, 0);
            Nextion_ObjVisible(DISP_OBJ_TEXT1, 1);
            Nextion_ObjVisible(DISP_OBJ_EF1,1);
            Nextion_SetObjAttr(DISP_OBJ_STATUS1,"pic",FAULT_ICON);
            if(ch2Active)
                Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_DOUBLE_FAULT_ICON);   
            else
                Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_SINGLE_FAULT_ICON);   
                
        }
        else{
            Nextion_ObjVisible(DISP_OBJ_SYS_READING1, 1);
            Nextion_ObjVisible(DISP_OBJ_TEXT1, 0);
            Nextion_ObjVisible(DISP_OBJ_EF1,0);
            Nextion_SetObjAttr(DISP_OBJ_STATUS1,"pic",OK_ICON);
            if(ch2Active)
                Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_DOUBLE_OK_ICON);
            else    
                Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_SINGLE_OK_ICON);
        }
       ef1= cur_ef1;     
    }
    
    uint8_t cur_ef2 =EF2_Read();
    if(ef2!= cur_ef2){
        if(!cur_ef2){
            Nextion_ObjVisible(DISP_OBJ_SYS_READING2, 0);
            Nextion_ObjVisible(DISP_OBJ_TEXT2, 1);
            Nextion_ObjVisible(DISP_OBJ_EF2,1);
            Nextion_SetObjAttr(DISP_OBJ_STATUS2,"pic",FAULT_ICON);
            Nextion_SetObjAttr(DISP_OBJ_RING2,"pic",RING_DOUBLE_FAULT_ICON);    
        }
        else{
            Nextion_ObjVisible(DISP_OBJ_SYS_READING2, 1);
            Nextion_ObjVisible(DISP_OBJ_TEXT2, 0);
            Nextion_SetObjAttr(DISP_OBJ_STATUS2,"pic",OK_ICON);
            Nextion_SetObjAttr(DISP_OBJ_RING2,"pic",RING_DOUBLE_OK_ICON);
            Nextion_ObjVisible(DISP_OBJ_EF2,0);
        }
        ef2= cur_ef2; 
    }
    
    if(ch2Active){    
        if(!SW1_Pressed && !SW4_Pressed){
            if(!ef1 && (alarm_status[0]==SYSREADING_OK) && (sensor[0][STATUSLSB]!=RUNTIME_SENSOR_IN_WARMUP)){
                if(!strobe)
                    system_SetLed(0, SYSTEM_LED_BLUE, 0,ch2Active);
                else    
                    system_SetLed(0, SYSTEM_LED_OFF, 0,ch2Active);
                
                            
            }else{
                if(alarm_status[0]==SYSREADING_OK){
                    if(sensor[0][STATUSLSB]==RUNTIME_SENSOR_IN_WARMUP)
                        system_SetLed(0, SYSTEM_LED_GREEN, 1,ch2Active);
                    else if(sensor[0][STATUSLSB]==POLL_SENSOR_STATUS_ABSENT) 
                        system_SetLed(0, SYSTEM_LED_BLUE, 0,ch2Active);
                    else
                        system_SetLed(0, SYSTEM_LED_GREEN, 0,ch2Active);
                }
                
                
            }
        }
        if(!SW2_Pressed && !SW4_Pressed ){
            if(!ef2 && (alarm_status[1]==SYSREADING_OK)&& (sensor[1][STATUSLSB]!=RUNTIME_SENSOR_IN_WARMUP)){
                if(!strobe)
                    system_SetLed(1, SYSTEM_LED_BLUE, 0,ch2Active);
                else    
                    system_SetLed(1, SYSTEM_LED_OFF, 0,ch2Active);
                
            }else {
                if(alarm_status[1]==SYSREADING_OK){
                    if(sensor[1][STATUSLSB]==RUNTIME_SENSOR_IN_WARMUP)
                        system_SetLed(1, SYSTEM_LED_GREEN, 1,ch2Active);
                    else if(sensor[1][STATUSLSB]==POLL_SENSOR_STATUS_ABSENT) 
                        system_SetLed(1, SYSTEM_LED_BLUE, 0,ch2Active);
                    else    
                        system_SetLed(1, SYSTEM_LED_GREEN, 0,ch2Active);
                }
                
                
            }
        }
    }else{
        if(!SW1_Pressed && !SW2_Pressed && !SW4_Pressed){
            if(!ef1 && (alarm_status[0]==SYSREADING_OK)&& (sensor[0][STATUSLSB]!=RUNTIME_SENSOR_IN_WARMUP)){
                if(!strobe)
                    system_SetLed(0, SYSTEM_LED_BLUE, 0,ch2Active);
                else    
                    system_SetLed(0, SYSTEM_LED_OFF, 0,ch2Active);
            }else{
                if(alarm_status[0]==SYSREADING_OK){
                    if(sensor[0][STATUSLSB]==RUNTIME_SENSOR_IN_WARMUP)
                        system_SetLed(0, SYSTEM_LED_GREEN, 1,ch2Active);
                    else if(sensor[0][STATUSLSB]==POLL_SENSOR_STATUS_ABSENT) 
                        system_SetLed(0, SYSTEM_LED_BLUE, 0,ch2Active);
                    else                    
                        system_SetLed(0, SYSTEM_LED_GREEN, 0,ch2Active);
                }
                
            }
        }
    }

}


void runtimeRealTimeCheckSwitches(void){
    //ch2Active=1;
    static uint8_t warmup[2];
    
   
    
    if(SW1_Pressed){
        uint32 cnt= SWTimer_ReadCounter();
        if(!warmup[0] && (sensor[0][STATUSLSB]!=RUNTIME_SENSOR_IN_WARMUP) ){
            if(cnt<SW_HOLD_PERIOD*1){
                system_SetLed(0 , SYSTEM_LED_GREEN, 1, ch2Active);   
            }else if(cnt<SW_HOLD_PERIOD*2){
                
                if((sensor[0][ZEROCALDATEMSB] || sensor[0][ZEROCALDATELSB]) )
                    system_SetLed(0 , SYSTEM_LED_WHITE, 0, ch2Active);
                else 
                    system_SetLed(0 , SYSTEM_LED_GREEN, 1, ch2Active);
                    
            }else if(cnt<SW_HOLD_PERIOD*3){
                system_SetLed(0 , SYSTEM_LED_GREEN, 1, ch2Active);
            }else if(cnt<SW_HOLD_PERIOD*4){
                if((sensor[0][SPANCALDATEMSB] || sensor[0][SPANCALDATELSB]) )
                    system_SetLed(0 , SYSTEM_LED_MAGENTA, 0, ch2Active);
            }else {
                system_SetLed(0 , SYSTEM_LED_GREEN, 1, ch2Active);    
            }
        }else{
            warmup[0]=1;
            if(cnt>SW_HOLD_PERIOD*1){
                system_SetLed(0 , SYSTEM_LED_GREEN, 0,  ch2Active);    
                AbortWarmup(0);    
               // Nextion_SetObjAttr(DISP_OBJ_STATUS1,"pic",OK_ICON);
            }
        }
        
    }
        
    if(SW1_Pressed && SW1_2_Released){
        SW1_Pressed=0;
        uint32 cnt= SWTimer_ReadCounter();
        while(!SW1_2_Released){};
        
        if(!warmup[0]){
            if(cnt<SW_HOLD_PERIOD*1){
                //GoRealtime(ch2Active);
            }else if(cnt<SW_HOLD_PERIOD*2){
                if((sensor[0][ZEROCALDATEMSB] || sensor[0][ZEROCALDATELSB]) )
                    setMode(RUNTIME_ZEROCAL_MODE1);
            }else if(cnt<SW_HOLD_PERIOD*3){
                //GoRealtime(ch2Active);
            }else if(cnt<SW_HOLD_PERIOD*4){
                if((sensor[0][SPANCALDATEMSB] || sensor[0][SPANCALDATELSB]) )
                    setMode(RUNTIME_SPANCAL_MODE1);
            }else {
                //GoRealtime(ch2Active);
            }
        }else{
            warmup[0]=0;
        }
        ResetSWTimer();
    }
    
    
#ifndef SINGLE_SENSOR_SYSTEM   
    
    if(SW2_Pressed && SW1_2_Released){
        SW2_Pressed=0;
        uint32 cnt= SWTimer_ReadCounter();
        while(!SW1_2_Released){};
        
        if(ch2Active){
            if(sensor[1][STATUSLSB]!=POLL_SENSOR_STATUS_ABSENT){
            //if(ch2Active&& sensor[1][STATUSLSB]!=POLL_SENSOR_STATUS_ABSENT){
                if(!warmup[1]){
                    if(cnt<SW_HOLD_PERIOD*1){
                        //GoRealtime(ch2Active);
                    }else if(cnt<SW_HOLD_PERIOD*2){
                        if((sensor[1][ZEROCALDATEMSB] || sensor[1][ZEROCALDATELSB]) )
                            setMode(RUNTIME_ZEROCAL_MODE2);
                       else if(!sensor[1][ZEROCALDATEMSB]&& !sensor[1][ZEROCALDATELSB] && !sensor[1][SPANCALDATEMSB] && !sensor[1][SPANCALDATELSB] ) {
                            ch2Active=0;
                            Nextion_SetObjectAttr("bt2", "val", 0);
                            GoRealtime(ch2Active);
                            EEPROM_SetCh2Activity(ch2Active);// CH2 deactivated
                       } 

                    }else if(cnt<SW_HOLD_PERIOD*3){
                        //GoRealtime(ch2Active);
                    }else if(cnt<SW_HOLD_PERIOD*4){
                        if((sensor[1][SPANCALDATEMSB] || sensor[1][SPANCALDATELSB]) )
                            setMode(RUNTIME_SPANCAL_MODE2);
                    }else if(cnt<SW_HOLD_PERIOD*5){
                        //GoRealtime(ch2Active);
                    }else if(cnt<SW_HOLD_PERIOD*6){
                        ch2Active=0;
                        Nextion_SetObjectAttr("bt2", "val", 0);
                        GoRealtime(ch2Active);
                        EEPROM_SetCh2Activity(ch2Active);// CH2 deactivated
                    }else{
                        //GoRealtime(ch2Active);
                    }
                }else{
                    warmup[1]=0;
                    //GoRealtime(ch2Active);
                }
            }else if(sensor[1][STATUSLSB]==POLL_SENSOR_STATUS_ABSENT){
                if(cnt< SW_HOLD_PERIOD*1){
                
                }
                else if(cnt< SW_HOLD_PERIOD*2){
                    ch2Active=0;
                    Nextion_SetObjectAttr("bt2", "val", 0);
                    GoRealtime(ch2Active);
                    EEPROM_SetCh2Activity(ch2Active);// CH2 deactivated
                    sensor[1][STATUSLSB]=0; // resetting status
                }
            }
        }
        else {
            if(cnt<SW_HOLD_PERIOD*1){
                //GoRealtime(ch2Active);
                uint8_t temp = 10;
            }else if(( cnt>SW_HOLD_PERIOD*1)&&( cnt<SW_HOLD_PERIOD*2)){// && (cnt<SW_HOLD_PERIOD*2)){
                ch2Active=1;
                Nextion_SetObjectAttr(DISP_OBJ_CH2_ACTIVE, "pco", DISP_YELLOW);
                EEPROM_SetCh2Activity(ch2Active); // CH2 Activated
                GoRealtime(ch2Active);
            }else{
                uint8_t temp = 20;
                //GoRealtime(ch2Active);
            }
        }

        ResetSWTimer();
    }
    
    
    
    if(SW2_Pressed){
        uint32 cnt= SWTimer_ReadCounter();
        
//        if(!(sensor[1][ZEROCALDATEMSB]&& !sensor[1][ZEROCALDATELSB])&& !(sensor[1][SPANCALDATEMSB] && !sensor[1][SPANCALDATELSB])){
//            Nextion_ObjVisible(DISP_OBJ_SYS_READING2, 0);
//            Nextion_ObjVisible(DISP_OBJ_TEXT2, 1);
//            Nextion_SetText(DISP_OBJ_TEXT2, "CALIBRATION N/A");
//        }
        
        if(ch2Active && sensor[1][STATUSLSB]!=POLL_SENSOR_STATUS_ABSENT){
            if(!warmup[1]&& (sensor[1][STATUSLSB]!=RUNTIME_SENSOR_IN_WARMUP) ){
                if(cnt<SW_HOLD_PERIOD*1){
                    system_SetLed(1 , SYSTEM_LED_GREEN, 1, ch2Active);
                }else if(cnt<SW_HOLD_PERIOD*2){
                    if((sensor[1][ZEROCALDATEMSB] || sensor[1][ZEROCALDATELSB]) )
                        system_SetLed(1 , SYSTEM_LED_WHITE, 0, ch2Active);
                    else if(!(sensor[1][ZEROCALDATEMSB]&& !sensor[1][ZEROCALDATELSB])&& !(sensor[1][SPANCALDATEMSB] && !sensor[1][SPANCALDATELSB]))
                        system_SetLed(1 , SYSTEM_LED_CYAN, 0,ch2Active);
                    else
                        system_SetLed(1 , SYSTEM_LED_GREEN, 1, ch2Active);
                }else if(cnt<SW_HOLD_PERIOD*3){
                    system_SetLed(1 , SYSTEM_LED_GREEN, 1, ch2Active);
                }else if(cnt<SW_HOLD_PERIOD*4){
                    if((sensor[1][SPANCALDATEMSB] || sensor[1][SPANCALDATELSB]) )
                        system_SetLed(1 , SYSTEM_LED_MAGENTA, 0, ch2Active);
                    else    
                        system_SetLed(1 , SYSTEM_LED_GREEN, 1,ch2Active);
                }else if(cnt<SW_HOLD_PERIOD*5){
                    system_SetLed(1 , SYSTEM_LED_GREEN, 1,ch2Active);
                }else if(cnt<SW_HOLD_PERIOD*6){
                    system_SetLed(1 , SYSTEM_LED_CYAN, 0,ch2Active);
                }else{
                    system_SetLed(1 , SYSTEM_LED_GREEN, 1,ch2Active);
                }
            }else{
                warmup[1]=1;
                if(cnt>SW_HOLD_PERIOD*1){
                    system_SetLed(1 , SYSTEM_LED_GREEN, 0, ch2Active);
                    AbortWarmup(1);
                    Nextion_SetObjAttr(DISP_OBJ_STATUS2,"pic",OK_ICON);
                }
            }
        }else{
            if(cnt<SW_HOLD_PERIOD*1){
                system_SetLed(1 , SYSTEM_LED_GREEN, 1, ch2Active);
            }else if((cnt>SW_HOLD_PERIOD*1+100)&&(cnt<SW_HOLD_PERIOD*2)) {
                system_SetLed(1 , SYSTEM_LED_CYAN, 0, ch2Active);
            }else{
                system_SetLed(1 , SYSTEM_LED_GREEN, 1, ch2Active);
            }
        }
    }

    
    
    
    
    if(SW3_Pressed){
        uint32 cnt= SWTimer_ReadCounter();
        if(cnt>SW_HOLD_PERIOD*1){
            ack_received=1;
            if(relay[0].ack)Reset_relay_Alarm1();
            if(relay[1].ack)Reset_relay_Alarm2();
            if(relay[2].ack)Reset_relay_Alarm3();
            if(relay[3].ack)Reset_relay_Fault();
        }else if(cnt>2000){
            reset_alarms=1;
            if(sensor[0][STATUSLSB]==RUNTIME_SENSOR_CAL_FAULT){
                runtime_CtrlSensor(0 , SENSOR_SYS_REMOTE_REG, RUNTIME_SMART_SENSOR_SYS_REMOTE_RESET_CAL_FAULTS);
            }
            if(sensor[1][STATUSLSB]==RUNTIME_SENSOR_CAL_FAULT){
                runtime_CtrlSensor(1 , SENSOR_SYS_REMOTE_REG, RUNTIME_SMART_SENSOR_SYS_REMOTE_RESET_CAL_FAULTS);
            }
        }
    }
#else
    if(SW2_Pressed){
        ResetSWTimer();
    }
    
    if(SW2_Pressed && SW1_2_Released){
        SW2_Pressed=0;
        while(!SW1_2_Released){};
    }
#endif


    if(SW3_Pressed && SW3_4_Released){
        SW3_Pressed=0;
        uint32 cnt= SWTimer_ReadCounter();
        while(!SW3_4_Released){};
        
        if(cnt>SW_HOLD_PERIOD*1){
            ack_received=0;
            newEvent=0;
        }else if(cnt>2000){
            reset_alarms=0;
        }
        
        ResetSWTimer();
    }
    
    if(SW4_Pressed){
        uint32 cnt= SWTimer_ReadCounter();
        if(cnt<SW_HOLD_PERIOD){
            
            system_SetLed(0 , SYSTEM_LED_GREEN, 1, 0);
            //system_SetLed(0 , SYSTEM_LED_GREEN, 2, 0);
        }else{
            system_SetLed(0 , SYSTEM_LED_CYAN, 0, 0);
            //system_SetLed(0 , SYSTEM_LED_CYAN, 2, 0);
        }
    }
    
    if(SW4_Pressed && SW3_4_Released){
        SW4_Pressed=0;
        while(!SW3_4_Released){};
        uint32 cnt= SWTimer_ReadCounter();
        if(cnt<SW_HOLD_PERIOD){
            //GoRealtime(ch2Active);
        }else{
            setMode(RUNTIME_SETTINGS_MODE);CySysWdtDisable( CY_SYS_WDT_COUNTER1_MASK );
        }
        ResetSWTimer();
    }
}

void runtimeZeroCal(uint8_t ch){
    char str[10];
    char units[4];
    uint16_t encoded_date;
    uint8_t res;
    uint8_t cal_in_progeress;
    static uint8_t wait;
    
    
    if(newMode){
        newMode=0;
        wait=0;
        runtime_CtrlSensor(ch , SENSOR_SYS_REMOTE_REG, RUNTIME_SMART_SENSOR_SYS_REMOTE_START_ZERO_CAL);
        
        Nextion_SetPage(PAGE_ZERO_CAL);
        Nextion_ObjVisible("b1",0);
        Nextion_SetText(DISP_OBJ_ZERO_CAL_STATUS,ZERO_CAL_INITIATED);
        system_SetLed(ch , SYSTEM_LED_WHITE, 1, ch2Active);
        LED_Update();
        Set_Output_mA(ch, 2);
        
        memset(str , 0 , sizeof(str));
        remove_trailing_spaces(str, (char*) &sensor[ch][COMPAUND1]);
        Nextion_SetText(DISP_OBJ_ZERO_CAL_GAS_TYPE,str);
        
        memset(str , 0 , sizeof(str));
        strncpy(units, (char *)&sensor[ch][UNIT1], 4);
        remove_leading_spaces((char *)units);
        remove_trailing_spaces(str, (char*) units);
        
        Nextion_SetText(DISP_OBJ_ZERO_CAL_ENG_UNITS,str);
        CyDelay(250);
    }
    
    SysWdtReset();
    
    uint32_t now = msTick;
    
    if(now- last_sensor_time>= TASK_PERIOD_SENSOR_MS){
        last_sensor_time = now;
        LoadSensorInfo(ch); // Load first sensor inf here
        if(sensor[ch][STATUSLSB] == POLL_SENSOR_STATUS_ABSENT){
            Nextion_SetText(DISP_OBJ_ZERO_CAL_STATUS,RUNTIME_SMART_SENSOR_ZERO_CAL_NOSENSOR_TXT);
            CyDelay(3000);
            GoRealtime(ch2Active);
        }
        // Update Calibration status on display here
        switch(sensor[ch][CALSTATUSLSB]){
            case RUNTIME_SMART_SENSOR_ZERO_CAL_INPROGRESS:
                Nextion_SetText(DISP_OBJ_ZERO_CAL_STATUS,RUNTIME_SMART_SENSOR_ZERO_CAL_INPROGRESS_TXT);
                //Set_Output_mA(ch, 2);
            break;
            case RUNTIME_SMART_SENSOR_ZERO_CAL_COMPLETE:
                if(!wait){
                    Nextion_SetText(DISP_OBJ_ZERO_CAL_STATUS,RUNTIME_SMART_SENSOR_ZERO_CAL_COMPLETE_TXT);
                    Set_Output_mA(ch, 4);
                }else if(wait>8){
                    
                    SaveZeroCalDate(ch );
//                    encoded_date = encodeCurrentDate();
//                    runtime_UpdateSensor(ch , (ZEROCALDATEMSB-4), encoded_date);
//                    runtime_CtrlSensor(ch , SENSOR_SYS_REMOTE_REG, RUNTIME_SMART_SENSOR_SYS_REMOTE_SAVE_TO_EEPROM);
//#ifndef SINGLE_SENSOR_SYSTEM                   
//                    sprintf(log_txt,"%02d/%02d/%02d at %02d:%02d CH%d ZERO CAL",date_time.month, date_time.day, date_time.year, date_time.hours, date_time.minutes, ch+1);
//#else
//                    sprintf(log_txt,"%02d/%02d/%02d at %02d:%02d ZERO CAL",date_time.month, date_time.day, date_time.year, date_time.hours, date_time.minutes);
//#endif
                    //SaveLogEntry(&log_last_entry_idx, log_txt);
                    GoRealtime(ch2Active);
                }
                wait++;
            break;
            case RUNTIME_SMART_SENSOR_ZERO_CAL_FAILED:
                Nextion_SetText(DISP_OBJ_ZERO_CAL_STATUS,RUNTIME_SMART_SENSOR_ZERO_CAL_FAILED_TXT);
                Set_Output_mA(ch, 0);
                system_SetLed(ch , SYSTEM_LED_ORANGE, 1, ch2Active);
                //LED_Update();
                //CyDelay(3000);
                GoRealtime(ch2Active);
            break;
        }
        //LED_Update();
        // temporary changing endian
        uint8_t tmp[4];
        float fval;
        tmp[3]=sensor[ch][SYSREADING1];
        tmp[2]=sensor[ch][SYSREADING2];
        tmp[1]=sensor[ch][SYSREADING3];
        tmp[0]=sensor[ch][SYSREADING4];
        memcpy(&fval,tmp,sizeof(float)) ;
        res = sensor[ch][RESOLUTIONLSB];
        Nextion_SendFloat(DISP_OBJ_ZERO_CAL_SYS_READING, (int32_t)(fval*pow(10, res)),  res);
    }
    
    
    
    /*Modbus request here*/
    if(modbusRequestReceived){
        modbusRequestReceived=0;
        runtime_ProcessRequest();// Response to Modbus
        bytesIn =0;
    }
    
    // Block Timer on  button press
    if(SW2_Pressed){
        SW2_Pressed=0;
        ResetSWTimer();
    }
    
    if(SW3_Pressed){
        SW3_Pressed=0;
        ResetSWTimer();
    }
    
    if(SW4_Pressed){
        SW3_Pressed=0;
        ResetSWTimer();
    }
    
    if(now- last_led_time>= TASK_PERIOD_LED_MS){
        last_led_time=now;
        LED_Update();
    }
}


void runtimeSpanCal(uint8_t ch){
    char str[20];
    char units[4];
    uint8_t tmp[4];
    uint16_t encoded_date;
    uint8_t res;
    float as_found;
    static uint8_t wait;
    static uint8_t prev_status;
    uint8_t new_state;
    static uint8_t retry_reset_en;
    
    uint32_t now = msTick;
    if(newMode){
        newMode=0;
        wait =0;
        retry_reset_en=0;
        
        if(sensor[ch][STATUSLSB]==RUNTIME_SENSOR_CAL_FAULT){
            Reset_relay_Fault();
            runtime_CtrlSensor(ch , SENSOR_SYS_REMOTE_REG, RUNTIME_SMART_SENSOR_SYS_REMOTE_RESET_CAL_FAULTS);
        }
        
        
        Nextion_SetPage(PAGE_SPAN_CAL);
        
        
        Nextion_ObjVisible(span_reset_obj,0);
        Nextion_ObjVisible(retry_obj[0],0);
        Nextion_ObjVisible(retry_obj[1],0);
        Nextion_ObjVisible(DISP_OBJ_SPAN_CAL_AS_FOUND,0);
        
        system_SetLed(ch , SYSTEM_LED_MAGENTA, 0, ch2Active);
        LED_Update();
        Set_Output_mA(ch, 2);
        
        memset(str , 0 , sizeof(str));
        remove_trailing_spaces(str, (char*) &sensor[ch][COMPAUND1]);
        Nextion_SetText(DISP_OBJ_ZERO_CAL_GAS_TYPE,str);
        memset(str , 0 , sizeof(str));
        strncpy(units, (char *)&sensor[ch][UNIT1], 4);
        remove_leading_spaces((char *)units);
        remove_trailing_spaces(str, (char*) units);
        Nextion_SetText(DISP_OBJ_SPAN_CAL_ENG_UNITS,str);
            
        runtime_CtrlSensor(ch , SENSOR_SYS_REMOTE_REG, RUNTIME_SMART_SENSOR_SYS_REMOTE_START_SPAN_CAL);
        Nextion_SetText(DISP_OBJ_ZERO_CAL_STATUS,ZERO_CAL_INITIATED);
        CyDelay(150);
        
        
    }
    SysWdtReset();
    
    if(now- last_led_time>= TASK_PERIOD_LED_MS){
        last_led_time=now;
        switch(sensor[ch][CALSTATUSLSB]){
            case RUNTIME_SMART_SENSOR_SPAN_CAL_INPROGRESS: 
                if(blinker)
                    system_SetLed(ch, SYSTEM_LED_MAGENTA, 0, ch2Active);
                else
                    system_SetLed(ch, SYSTEM_LED_OFF, 0, ch2Active);
            break;
            case RUNTIME_SMART_SENSOR_SPAN_CAL_NOGASAPPLIED:
                if(blinker)
                    system_SetLed(ch, SYSTEM_LED_ORANGE, 0, ch2Active);
                else
                    system_SetLed(ch, SYSTEM_LED_OFF, 0, ch2Active);        
            break; 
            case RUNTIME_SMART_SENSOR_SPAN_CAL_GASDETECTED:
                system_SetLed(ch, SYSTEM_LED_MAGENTA, 0, ch2Active);
            break;
            case RUNTIME_SMART_SENSOR_SPAN_CAL_AS_FOUND_SAVED:
                system_SetLed(ch, SYSTEM_LED_MAGENTA, 0, ch2Active);    
            break; 
            case RUNTIME_SMART_SENSOR_SPAN_CAL_ADJUSTED:
                system_SetLed(ch, SYSTEM_LED_MAGENTA, 0, ch2Active);
            break; 
            case RUNTIME_SMART_SENSOR_SPAN_CAL_STABILITYCHECKSTARTED:
                system_SetLed(ch, SYSTEM_LED_MAGENTA, 1, ch2Active);    
            break; 
            case RUNTIME_SMART_SENSOR_SPAN_CAL_STABILITYCHECKFAILED: 
                if(blinker)
                    system_SetLed(ch, SYSTEM_LED_ORANGE, 0, ch2Active);
                else
                    system_SetLed(ch, SYSTEM_LED_OFF, 0, ch2Active);   
            break;
            case RUNTIME_SMART_SENSOR_SPAN_CAL_REMOVEGAS:
                if(blinker)
                    system_SetLed(ch, SYSTEM_LED_MAGENTA, 0, ch2Active);
                 else
                    system_SetLed(ch, SYSTEM_LED_OFF, 0, ch2Active);        
            break;
            case RUNTIME_SMART_SENSOR_SPAN_CAL_COMPLETE:break;
            case RUNTIME_SMART_SENSOR_SPAN_CAL_CLEARINGFAILED:
                if(blinker)
                    system_SetLed(ch, SYSTEM_LED_ORANGE, 0, ch2Active);
                else
                    system_SetLed(ch, SYSTEM_LED_OFF, 0, ch2Active);
            break;
            
        }
        if(retry_reset_en)
            system_SetLed(ch , SYSTEM_LED_MAGENTA, 0, ch2Active);
        LED_Update();    
    }    
    
    
    
    
    if(now- last_sensor_time>= TASK_PERIOD_SENSOR_MS){
        last_sensor_time = now;
        LoadSensorInfo(ch); // Load first sensor inf here
        
        // Gettting Relays State;
        if(relay_present)
            relays_state = GetRelayState();
        else     
            relays_state =0;
        // Update Calibration status on display here
        if(sensor[ch][STATUSLSB] == POLL_SENSOR_STATUS_ABSENT){
            Nextion_SetText(DISP_OBJ_ZERO_CAL_STATUS,RUNTIME_SMART_SENSOR_ZERO_CAL_NOSENSOR_TXT);
            CyDelay(3000);
            GoRealtime(ch2Active);
        }
            if(prev_status!=sensor[ch][CALSTATUSLSB]) new_state=1;
            switch(sensor[ch][CALSTATUSLSB]){
                case RUNTIME_SMART_SENSOR_SPAN_CAL_INPROGRESS:
                    if(new_state){
                        new_state=0;
                        Nextion_SetText(DISP_OBJ_ZERO_CAL_STATUS,RUNTIME_SMART_SENSOR_SPAN_CAL_INPROGRESS_TXT);
                        Set_Output_mA(ch, 2);
                    }
                break;
                case RUNTIME_SMART_SENSOR_SPAN_CAL_NOGASAPPLIED:
                    if(new_state){
                        new_state=0;
                        Nextion_ObjVisible(span_reset_obj,1);
                        Nextion_ObjVisible(retry_obj[ch],1);
                        Nextion_SetText(DISP_OBJ_ZERO_CAL_STATUS,RUNTIME_SMART_SENSOR_SPAN_CAL_NOGASAPPLIED_TXT);
                        Set_Output_mA(ch, 0);
                    }
                break;
                case RUNTIME_SMART_SENSOR_SPAN_CAL_GASDETECTED:
                     if(new_state){
                        new_state=0;   
                        Nextion_SetText(DISP_OBJ_ZERO_CAL_STATUS,RUNTIME_SMART_SENSOR_SPAN_CAL_GASDETECTED_TXT);
                        Set_Output_mA(ch, 2);  
                    }
                break;
                case RUNTIME_SMART_SENSOR_SPAN_CAL_AS_FOUND_SAVED:
                    if(new_state){
                        new_state=0;
                        Nextion_SetText(DISP_OBJ_ZERO_CAL_STATUS,RUNTIME_SMART_SENSOR_SPAN_CAL_AS_FOUND_SAVED_TXT);
                        Nextion_ObjVisible(DISP_OBJ_SPAN_CAL_AS_FOUND,1);
                        Set_Output_mA(ch, 2);    
                     }   
                        memcpy(str, 0 , sizeof(str));
                        tmp[3] = sensor[ch][SPANASFOUND1];
                        tmp[2] = sensor[ch][SPANASFOUND2];
                        tmp[1] = sensor[ch][SPANASFOUND3];
                        tmp[0] = sensor[ch][SPANASFOUND4];
                        memcpy(&as_found, tmp,4);
                        
                        memset(str, 0 , sizeof(str));
                        memset(units, 0 , sizeof(units));
                        strncpy(str, (char *)&sensor[ch][UNIT1], 4);
                        remove_leading_spaces((char *)str);
                        remove_trailing_spaces(units, (char*) str);
                        
                        memset(str, 0 , sizeof(str));
                        sprintf(str,"%.*f %s", sensor[ch][RESOLUTIONLSB], as_found, units );
                        Nextion_SetText(DISP_OBJ_SPAN_CAL_AS_FOUND,str);
                        
                    
                break;
                case RUNTIME_SMART_SENSOR_SPAN_CAL_ADJUSTED:
                    if(new_state){
                        new_state=0;
                        Nextion_SetText(DISP_OBJ_ZERO_CAL_STATUS,RUNTIME_SMART_SENSOR_SPAN_CAL_ADJUSTED_TXT);
                        Set_Output_mA(ch, 2);
                    }    

                        memset(str, 0 , sizeof(str));
                        memset(units, 0 , sizeof(units));
                        strncpy(str, (char *)&sensor[ch][UNIT1], 4);
                        remove_leading_spaces((char *)str);
                        remove_trailing_spaces(units, (char*) str);
                        
                        memset(str, 0 , sizeof(str));
                        uint32_t cal_level =  (uint32_t)(sensor[ch][CALLEVEL1]<<24)+
                                              (uint32_t)(sensor[ch][CALLEVEL2]<<16)+
                                              (uint32_t)(sensor[ch][CALLEVEL3]<<8)+
                                              (uint32_t)sensor[ch][CALLEVEL4];
                        uint32_t range =  (uint32_t)(sensor[ch][FULLSCALERANGE1]<<24)+
                                              (uint32_t)(sensor[ch][FULLSCALERANGE2]<<16)+
                                              (uint32_t)(sensor[ch][FULLSCALERANGE3]<<8)+
                                              (uint32_t)sensor[ch][FULLSCALERANGE4];                    
                        float level= (cal_level*range)/10000.0;
                        sprintf(str,"%0.f %s",  level, units );
                        Nextion_SetText(DISP_OBJ_SPAN_CAL_AS_FOUND,str);
                          
                    
                break;
                case RUNTIME_SMART_SENSOR_SPAN_CAL_STABILITYCHECKSTARTED:
                    if(new_state){
                        new_state=0;
                        Nextion_SetText(DISP_OBJ_ZERO_CAL_STATUS,RUNTIME_SMART_SENSOR_SPAN_CAL_STABILITYCHECKSTARTED_TXT);
                        Set_Output_mA(ch, 2);    
                    }
                break;    
                case RUNTIME_SMART_SENSOR_SPAN_CAL_STABILITYCHECKFAILED:
                     if(new_state){
                        new_state=0; 
                        Nextion_ObjVisible(span_reset_obj,1);
                        Nextion_ObjVisible(retry_obj[ch],1);
                        Nextion_SetText(DISP_OBJ_ZERO_CAL_STATUS,RUNTIME_SMART_SENSOR_SPAN_CAL_STABILITYCHECKFAILED_TXT);
                        Set_Output_mA(ch, 0);
                    }
                break;
                case RUNTIME_SMART_SENSOR_SPAN_CAL_REMOVEGAS:
                    if(new_state){
                        new_state=0;     
                        Nextion_SetText(DISP_OBJ_ZERO_CAL_STATUS,RUNTIME_SMART_SENSOR_SPAN_CAL_REMOVEGAS_TXT);
                        Set_Output_mA(ch, 2);    
                    }
                break;
                case RUNTIME_SMART_SENSOR_SPAN_CAL_COMPLETE:
                    if(new_state){
                        new_state=0; 
                    }
                    if(!wait){
                        Nextion_SetText(DISP_OBJ_SPAN_CAL_STATUS,RUNTIME_SMART_SENSOR_SPAN_CAL_COMPLETE_TXT);
                        Set_Output_mA(ch, 4);
                    }
                    else if(wait>8){
                        SaveSpanCalDate(ch );
//                        encoded_date = encodeCurrentDate();
//                        runtime_UpdateSensor(ch , SPANCALDATEMSB-4, encoded_date);
//                        runtime_CtrlSensor(ch , SENSOR_SYS_REMOTE_REG, RUNTIME_SMART_SENSOR_SYS_REMOTE_SAVE_TO_EEPROM);
//#ifndef SINGLE_SENSOR_SYSTEM                        
//                        sprintf(log_txt,"%02d/%02d/%02d at %02d:%02d CH%d SPAN CAL",date_time.month, date_time.day, date_time.year, date_time.hours, date_time.minutes, ch+1);
//#else
//                        sprintf(log_txt,"%02d/%02d/%02d at %02d:%02d SPAN CAL",date_time.month, date_time.day, date_time.year, date_time.hours, date_time.minutes);
//#endif    
//                        SaveLogEntry(&log_last_entry_idx, log_txt);
                        GoRealtime(ch2Active);
                    }
                    wait++;
                break;    
                case RUNTIME_SMART_SENSOR_SPAN_CAL_CLEARINGFAILED:
                    if(new_state){
                        new_state=0; 
                        Nextion_ObjVisible(span_reset_obj,1);
                        Nextion_ObjVisible(retry_obj[ch],1);
                        Nextion_SetText(DISP_OBJ_ZERO_CAL_STATUS,RUNTIME_SMART_SENSOR_SPAN_CAL_CLEARINGFAILED_TXT);
                        //Set_Output_mA(ch, 0);
                    }
                break;     
            }
            prev_status=sensor[ch][CALSTATUSLSB];
        
        // temporary changing endian
        uint8_t tmp[4];
        float fval;
        tmp[3]=sensor[ch][SYSREADING1];
        tmp[2]=sensor[ch][SYSREADING2];
        tmp[1]=sensor[ch][SYSREADING3];
        tmp[0]=sensor[ch][SYSREADING4];
        memcpy(&fval,tmp,sizeof(float)) ;
        res = sensor[ch][RESOLUTIONLSB];
        Nextion_SendFloat(DISP_OBJ_SPAN_CAL_SYS_READING, (int32_t)(fval*pow(10, res)),  res);
    }
    
    /*Modbus request here*/
    if(modbusRequestReceived){
        modbusRequestReceived=0;
        runtime_ProcessRequest();// Response to Modbus
        bytesIn =0;
    }
    
    // Check button
    if(sensor[ch][STATUSLSB]==RUNTIME_SENSOR_CAL_FAULT)
        Set_relay_Fault(); 
    if( (sensor[ch][CALSTATUSLSB]==RUNTIME_SMART_SENSOR_SPAN_CAL_NOGASAPPLIED)||
        (sensor[ch][CALSTATUSLSB]== RUNTIME_SMART_SENSOR_SPAN_CAL_STABILITYCHECKFAILED)||
        (sensor[ch][CALSTATUSLSB]==RUNTIME_SMART_SENSOR_SPAN_CAL_CLEARINGFAILED) ) {
        
        if(ch==0){
            if(SW1_Pressed){
                uint32 cnt= SWTimer_ReadCounter();
                if((cnt>SW_HOLD_PERIOD_SETTINGS*3)){
                    Nextion_SetObjectAttr(DISP_OBJ_SPAN_CAL_RETRY_BTN1, "bco", DISP_YELLOW);
                    
                    retry_reset_en=1;
                    
                    //system_SetLed(ch , SYSTEM_LED_MAGENTA, 0, ch2Active);
                    //LED_Update();
                }
            }
            if(SW1_Pressed && SW1_2_Released){
                SW1_Pressed=0;
                while(!SW1_2_Released){};
                retry_reset_en=0;
                
                uint32 cnt= SWTimer_ReadCounter();
                ResetSWTimer();
                Nextion_SetObjectAttr(DISP_OBJ_SPAN_CAL_RETRY_BTN1, "bco", DISP_GREY_BTN);
                if((cnt>SW_HOLD_PERIOD_SETTINGS*3)){
                    runtime_CtrlSensor(ch , SENSOR_SYS_REMOTE_REG, RUNTIME_SMART_SENSOR_SYS_REMOTE_RESET_CAL_FAULTS);
                    setMode(RUNTIME_SPANCAL_MODE1);
                }
            }
            if(SW1_Pressed && SW1_2_Released)
            {
                SW1_Pressed=0; 
                while(!SW1_2_Released){};
                ResetSWTimer();
            }
        }
        if(ch==1){
            if(SW2_Pressed){
                uint32 cnt= SWTimer_ReadCounter();
                if((cnt>SW_HOLD_PERIOD_SETTINGS*3)){
                    Nextion_SetObjectAttr(DISP_OBJ_SPAN_CAL_RETRY_BTN2, "bco", DISP_YELLOW);
                    retry_reset_en=1;
                    //system_SetLed(ch , SYSTEM_LED_MAGENTA, 0, ch2Active);
                    //LED_Update();
                }
            }
            if(SW2_Pressed && SW1_2_Released){
                SW2_Pressed=0;
                while(!SW1_2_Released){};
                retry_reset_en=0;
                uint32 cnt= SWTimer_ReadCounter();
                ResetSWTimer();
                Nextion_SetObjectAttr(DISP_OBJ_SPAN_CAL_RETRY_BTN2, "bco", DISP_GREY_BTN);
                if((cnt>SW_HOLD_PERIOD_SETTINGS*3)){
                    runtime_CtrlSensor(ch , SENSOR_SYS_REMOTE_REG, RUNTIME_SMART_SENSOR_SYS_REMOTE_RESET_CAL_FAULTS);
                    setMode(RUNTIME_SPANCAL_MODE2);
                }
            }
            if(SW1_Pressed && SW1_2_Released)
            {
                SW1_Pressed=0;  
                while(!SW1_2_Released){};
                ResetSWTimer();
            }
        }
        if(SW3_Pressed){
            uint32 cnt= SWTimer_ReadCounter();
            if((cnt>SW_HOLD_PERIOD_SETTINGS*3)){
                Nextion_SetObjectAttr(DISP_OBJ_SPAN_CAL_RESET_BTN, "bco", DISP_YELLOW);
                retry_reset_en=1;
                //system_SetLed(ch , SYSTEM_LED_MAGENTA, 0, ch2Active);
                //LED_Update();
            }
        }
        if(SW3_Pressed && SW3_4_Released){
            SW3_Pressed=0;
            while(!SW3_4_Released){};
            retry_reset_en=0;
            uint32 cnt= SWTimer_ReadCounter();
            ResetSWTimer();
            Nextion_SetObjectAttr(DISP_OBJ_SPAN_CAL_RESET_BTN, "bco", DISP_GREY_BTN);
            if((cnt>SW_HOLD_PERIOD_SETTINGS*3)){
                runtime_CtrlSensor(ch , SENSOR_SYS_REMOTE_REG, RUNTIME_SMART_SENSOR_SYS_REMOTE_RESET_CAL_FAULTS);
                GoRealtime(ch2Active);
            }
        }
    }else{
        // Block Timer on  button press
        if(SW1_Pressed){
            SW1_Pressed=0;
            ResetSWTimer();
        }
        
        if(SW2_Pressed){
            SW2_Pressed=0;
            ResetSWTimer();
        }
        
        if(SW3_Pressed){
            SW3_Pressed=0;
            ResetSWTimer();
        }    
        
        if(SW4_Pressed){
            SW4_Pressed=0;
            ResetSWTimer();
        }
    
    }
    
    
    
    
}


void runtimeSesttings(void){
    static uint pos;
    
    if(newMode){
        newMode=0;
        ResetSWTimer();
        Nextion_SetPage(PAGE_SETTINGS);
        //system_SetLED(SYSTEM_LED_GREEN);
        system_SetLed(0 , SYSTEM_LED_GREEN, 0, 0);
        LED_Update();
        pos =0;
        Nextion_SetObjAttr(settings_obj[pos],"bco",DISP_BLUE);
        Restart_Inactivity_timer();
    }
    
    Check_Inactivity();
    
    /*Check Modbus request here*/
    if(modbusRequestReceived){
        modbusRequestReceived=0;
        runtime_ProcessRequest();// Response to Modbus
        bytesIn =0;
    }
    
    
    if(SW4_Pressed &&SW3_4_Released){
        SW4_Pressed=0;
        while(!SW3_4_Released){};
        ResetSWTimer();
    }
    
    if(SW3_Pressed &&SW3_4_Released){
        SW3_Pressed=0;
        while(!SW3_4_Released){};
        ResetSWTimer();
    }
    
    if(SW1_Pressed){
        uint32 cnt= SWTimer_ReadCounter();
        if(cnt>SW_HOLD_PERIOD_SETTINGS*3){
            Nextion_SetObjAttr(settings_obj[pos],"bco",DISP_YELLOW);
            Nextion_SetObjAttr(settings_obj[pos],"pco",DISP_BLUE);
        }
        Restart_Inactivity_timer();
    }
    
    if(SW1_Pressed && SW1_2_Released){
        SW1_Pressed=0;
        while(!SW1_2_Released){};
        
        uint32 cnt= SWTimer_ReadCounter();
        if(cnt < SW_HOLD_PERIOD_SETTINGS*3){
            if(pos>0){
                Nextion_SetObjAttr(settings_obj[pos],"bco",DISP_BLACK);
                pos--;
                Nextion_SetObjAttr(settings_obj[pos],"bco",DISP_BLUE);
            }
            else{
                Nextion_SetObjAttr(settings_obj[0],"bco",DISP_BLACK);
                pos=DISP_OBJ_SETTINGS_NUM-1;
                Nextion_SetObjAttr(settings_obj[pos],"bco",DISP_BLUE);
            }
        }else{
            switch(pos){
                case 0: setMode(RUNTIME_RELAYCONF_MODE);break; 
                case 1: setMode(RUNTIME_ALARMCONF_MODE);break; 
                case 2: setMode(RUNTIME_DATETIME_MODE);break;
                case 3: setMode(RUNTIME_SLEEPCONF_MODE);break;
                case 4: setMode(RUNTIME_MODBUS_MODE);break;
                case 5: setMode(RUNTIME_GPS_MODE);break;
                case 6: setMode(RUNTIME_LOG_MODE);break;
                case 7: setMode(RUNTIME_INFO_MODE);break;
            }
        }
        ResetSWTimer();
        
    }
    
    if(SW2_Pressed){
        uint32 cnt= SWTimer_ReadCounter();
        if(cnt>SW_HOLD_PERIOD_SETTINGS*3 ){
            Nextion_SetObjectAttr("b8", "bco", DISP_YELLOW);
            Nextion_SetText("b8","EXITING");
        }
        Restart_Inactivity_timer();
    }
    
    if(SW2_Pressed && SW1_2_Released){
        SW2_Pressed=0;
        while(!SW1_2_Released){};
        
        uint32 cnt= SWTimer_ReadCounter();
        if(cnt < SW_HOLD_PERIOD_SETTINGS*3){
            if(pos<(DISP_OBJ_SETTINGS_NUM-1)){
                Nextion_SetObjAttr(settings_obj[pos],"bco",DISP_BLACK);
                pos++;
                Nextion_SetObjAttr(settings_obj[pos],"bco",DISP_BLUE);
            }
            else{
                Nextion_SetObjAttr(settings_obj[DISP_OBJ_SETTINGS_NUM-1],"bco",DISP_BLACK);
                pos=0;
                Nextion_SetObjAttr(settings_obj[pos],"bco",DISP_BLUE);
            }
        }else{
            Nextion_SetObjectAttr("b8", "bco", DISP_GREY_BTN);
           GoRealtime(ch2Active);
        }
        ResetSWTimer();
    }
    
    
    
    
}


void runtimeRelayConfig(void){
    static uint8_t pos;
    static uint8_t edit;
    static uint8_t par[DISP_OBJ_RLY_NUM];// remove static and get relay data at newMode
    char str[5];
    
    if(newMode){
        newMode=0;
        Nextion_SetPage(PAGE_REALY_CONFIG);
        //system_SetLED(SYSTEM_LED_GREEN);
        system_SetLed(0 , SYSTEM_LED_GREEN, 0, 1);
        LED_Update();
        pos =0;
        Nextion_SetObjAttr(relay_obj[pos],"bco",DISP_BLUE);
        memcpy(par, relay, sizeof(relay));
        for(uint8_t i =0;i<DISP_OBJ_RLY_NUM;i++){
            memset(str, 0 , sizeof(str));
            switch(i){
                case 0: if(relay[0].energ==0)strcpy(str, "DNRGD");else strcpy(str, "ENRGD");break;
                case 1: if(relay[0].latched==0)strcpy(str, "NLTCH");else strcpy(str, "LATCH");break;
                case 2: if(relay[0].ack==0)strcpy(str, "NACK");else strcpy(str, "ACK");break;
                case 3: if(relay[1].energ==0)strcpy(str, "DNRGD");else strcpy(str, "ENRGD");break;
                case 4: if(relay[1].latched==0)strcpy(str, "NLTCH");else strcpy(str, "LATCH");break;
                case 5: if(relay[1].ack==0)strcpy(str, "NACK");else strcpy(str, "ACK");break;
                case 6: if(relay[2].energ==0)strcpy(str, "DNRGD");else strcpy(str, "ENRGD");break;
                case 7: if(relay[2].latched==0)strcpy(str, "NLTCH");else strcpy(str, "LATCH");break;
                case 8: if(relay[2].ack==0)strcpy(str, "NACK");else strcpy(str, "ACK");break;
                case 9: if(relay[3].energ==0)strcpy(str, "DNRGD");else strcpy(str, "ENRGD");break;
                case 10: if(relay[3].latched==0)strcpy(str, "NLTCH");else strcpy(str, "LATCH");break;
                case 11: if(relay[3].ack==0)strcpy(str, "NACK");else strcpy(str, "ACK");break;
            }
            Nextion_SetText(relay_obj[i],str);
        }
        // READ SETINGS HERE AND GIVE NAMES TO FIELDS
    }
    
    Check_Inactivity();
    
    /*Check Modbus request here*/
    if(modbusRequestReceived){
        modbusRequestReceived=0;
        runtime_ProcessRequest();// Response to Modbus
        bytesIn =0;
    }
    
    if(SW1_Pressed){
        uint32 cnt= SWTimer_ReadCounter();
        if(cnt>SW_HOLD_PERIOD_SETTINGS*3){
            if(!edit){
                Nextion_SetObjAttr(relay_obj[pos],"bco",DISP_YELLOW);
                Nextion_SetObjAttr(relay_obj[pos],"pco",DISP_BLUE);
                
            }else{
               Nextion_SetObjAttr(relay_obj[pos],"bco",DISP_BLUE);
               Nextion_SetObjAttr(relay_obj[pos],"pco",DISP_WHITE);
            }
        }
        Restart_Inactivity_timer();
    }
    
    if(SW1_Pressed && SW1_2_Released){
        
        while(!SW1_2_Released){};
        SW1_Pressed=0;
        uint32 cnt= SWTimer_ReadCounter();
        if(cnt < SW_HOLD_PERIOD_SETTINGS*3){
            if(!edit){
                if(pos>0){
                    Nextion_SetObjAttr(relay_obj[pos],"bco",DISP_GREY);
                    pos--;
                    Nextion_SetObjAttr(relay_obj[pos],"bco",DISP_BLUE);
                }
                else{
                    Nextion_SetObjAttr(relay_obj[0],"bco",DISP_GREY);
                    pos=DISP_OBJ_RLY_NUM-1;
                    Nextion_SetObjAttr(relay_obj[pos],"bco",DISP_BLUE);
                }
            }else{
                par[pos]^=1;
                switch(pos){
                case 0:
                case 3:
                case 6:
                case 9: 
                    if(!par[pos]) Nextion_SetText(relay_obj[pos],"DNRGD");else Nextion_SetText(relay_obj[pos],"ENRGD");break;
                case 1:
                case 4: 
                case 7:
                case 10: 
                    if(!par[pos]) Nextion_SetText(relay_obj[pos],"NLTCH");else Nextion_SetText(relay_obj[pos],"LATCH");break;
                case 2: 
                case 5:
                case 8:
                case 11: 
                    if(!par[pos]) Nextion_SetText(relay_obj[pos],"NACK");else Nextion_SetText(relay_obj[pos],"ACK");break;
                }
            }
        }else{
            if(!edit){
                 edit =1;
                Nextion_SetText("b0","SET");
                Nextion_ObjVisible("b1",0);
            }else{
                edit=0;
                Nextion_SetText("b0","EDIT");
                Nextion_ObjVisible("b1",1);
                 Nextion_SetObjAttr(relay_obj[pos],"pco",DISP_WHITE);
            }
            switch(pos){
                case 0:  break;
                case 1: break;
                case 2: break; 
                case 3: break; 
                case 4: break;
                case 5: break;
                case 6: break;
            }
        }
        ResetSWTimer();
    }
    
    if(SW2_Pressed){
        uint32 cnt= SWTimer_ReadCounter();
        if((cnt>SW_HOLD_PERIOD_SETTINGS*3) && !edit){
            Nextion_SetObjectAttr("b1", "bco", DISP_YELLOW);
            Nextion_SetText("b1","SAVING");
            // SAVE PARAMETERS HERE
            memcpy(relay,par, sizeof(relay));
            uint8_t send[sizeof(relay)+1];
            
            EEPROM_SetRelaySetting(EEPROM_RELAY1_ENERG_REG, relay[0].energ);
            EEPROM_SetRelaySetting(EEPROM_RELAY2_ENERG_REG, relay[1].energ);
            EEPROM_SetRelaySetting(EEPROM_RELAY3_ENERG_REG, relay[2].energ);
            EEPROM_SetRelaySetting(EEPROM_RELAY4_ENERG_REG, relay[3].energ);
            
            EEPROM_SetRelaySetting(EEPROM_RELAY1_LATCH_REG, relay[0].latched);
            EEPROM_SetRelaySetting(EEPROM_RELAY2_LATCH_REG, relay[1].latched);
            EEPROM_SetRelaySetting(EEPROM_RELAY3_LATCH_REG, relay[2].latched);
            EEPROM_SetRelaySetting(EEPROM_RELAY4_LATCH_REG, relay[3].latched);
            
            EEPROM_SetRelaySetting(EEPROM_RELAY1_ACK_REG, relay[0].ack);
            EEPROM_SetRelaySetting(EEPROM_RELAY2_ACK_REG, relay[1].ack);
            EEPROM_SetRelaySetting(EEPROM_RELAY3_ACK_REG, relay[2].ack);
            EEPROM_SetRelaySetting(EEPROM_RELAY4_ACK_REG, relay[3].ack);
            //EEPROM_WriteBytes(EEPROM_RELAY1_ENERG_REG, (uint8_t*)relay, sizeof(relay));
            
            //CyDelay(2000); // Mimicing saving
            // Return button text back and switch page
//            Nextion_SetObjectAttr("b1", "bco", DISP_GREY_BTN);
//            Nextion_SetText("b1","SAVE");
//            setMode(RUNTIME_SETTINGS_MODE);
        }
        Restart_Inactivity_timer();
    }
    
    if(SW2_Pressed && SW1_2_Released){
        while(!SW1_2_Released){};
        SW2_Pressed=0;
        uint32 cnt= SWTimer_ReadCounter();
        if(cnt < SW_HOLD_PERIOD_SETTINGS*3){
            if(!edit){
                if(pos<(DISP_OBJ_RLY_NUM-1)){
                    Nextion_SetObjAttr(relay_obj[pos],"bco",DISP_GREY);
                    pos++;
                    Nextion_SetObjAttr(relay_obj[pos],"bco",DISP_BLUE);
                }
                else{
                    Nextion_SetObjAttr(relay_obj[DISP_OBJ_RLY_NUM-1],"bco",DISP_GREY);
                    pos=0;
                    Nextion_SetObjAttr(relay_obj[pos],"bco",DISP_BLUE);
                }
            }else{
                par[pos]^=1;
                switch(pos){
                case 0:
                case 3:
                case 6:
                case 9: 
                    if(!par[pos]) Nextion_SetText(relay_obj[pos],"DNRGD");else Nextion_SetText(relay_obj[pos],"ENRGD");break;
                case 1:
                case 4: 
                case 7:
                case 10: 
                    if(!par[pos]) Nextion_SetText(relay_obj[pos],"NLTCH");else Nextion_SetText(relay_obj[pos],"LATCH");break;
                case 2: 
                case 5:
                case 8:
                case 11: 
                    if(!par[pos]) Nextion_SetText(relay_obj[pos],"NACK");else Nextion_SetText(relay_obj[pos],"ACK");break;
                }
            }
        }else{
            Nextion_SetObjectAttr("b1", "bco", DISP_GREY_BTN);
            Nextion_SetText("b1","SAVE");
            setMode(RUNTIME_SETTINGS_MODE);
        }
//        SWTimer_Stop();
//        SWTimer_WriteCounter(0);
        ResetSWTimer();
    }
    
    
    // Isolating unused switches
    if(SW3_Pressed||SW4_Pressed){
        ResetSWTimer();
    }
    
}


void runtimeGps(void){
    static uint8_t pos;
    static uint8_t edit;
    static uint8_t par[DISP_OBJ_GPS_NUM];
    uint32 number = 374567891;
    
    //int32_t latitude =  90345678;
    //int32_t longitude = 322345678;
    
    uint32_t abs_lat;
    uint32_t abs_lon;
    
    uint32_t powersOf10[9] = {
        100000000, 10000000, 1000000,
        100000, 10000, 1000,
        100, 10, 1
    };
    
    // Temporary value
    static uint16_t sleep_time;
    if(newMode){
        newMode=0;
        Nextion_SetPage(PAGE_GPS);
        system_SetLed(0 , SYSTEM_LED_GREEN, 0, 1);
        LED_Update();
        pos =0;
        Nextion_SetObjAttr(gps_obj[pos],"bco",DISP_BLUE);
        
        if(flatitude>=0){
            abs_lat= flatitude*1000000;
            par[0]=1;
        }else{
            abs_lat= flatitude*(-1000000);
            par[0]=0;
        }
        if(flongitude>=0){
            abs_lon= flongitude*1000000;
            par[9]=1;
        }else{
            abs_lon= (flongitude*(-1000000));
            par[9]=0;
        }
        
        //abs_lat = (uint32)flatitude*1;
        //abs_lon = (uint32)flongitude*1;
        for (int i = 1; i < 9; i++)  {
            par[i] = abs_lat / powersOf10[i];
            abs_lat %= powersOf10[i];
            Nextion_SendInt(gps_obj[i], par[i]);
        }
        for (int i = 0; i < 9; i++)  {
            par[i+10] = abs_lon / powersOf10[i];
            abs_lon %= powersOf10[i];
            Nextion_SendInt(gps_obj[i+10], par[i+10]);
        }
        // Display signs
        if(par[0]) Nextion_SetText(gps_obj[0],"+");else Nextion_SetText(gps_obj[0],"-");
        if(par[9]) Nextion_SetText(gps_obj[9],"+");else Nextion_SetText(gps_obj[9],"-");
        Restart_Inactivity_timer();
    }
    
    Check_Inactivity();
    
    /*Check Modbus request here*/
    if(modbusRequestReceived){
        modbusRequestReceived=0;
        runtime_ProcessRequest();// Response to Modbus
        bytesIn =0;
    }
    
    if(SW1_Pressed){
        uint32 cnt= SWTimer_ReadCounter();
        if(cnt>SW_HOLD_PERIOD_SETTINGS){
            if(!edit){
                Nextion_SetObjAttr(gps_obj[pos],"bco",DISP_YELLOW);
                Nextion_SetObjAttr(gps_obj[pos],"pco",DISP_BLUE);
                
            }else{
               Nextion_SetObjAttr(gps_obj[pos],"bco",DISP_BLUE);
               Nextion_SetObjAttr(gps_obj[pos],"pco",DISP_WHITE);
            }
        }
        Restart_Inactivity_timer();
    }
    
    
    if(SW1_Pressed && SW1_2_Released){
        CyDelay(20);
        while(!SW1_2_Released){};
        SW1_Pressed=0;
        uint32 cnt= SWTimer_ReadCounter();
        if(cnt < SW_HOLD_PERIOD_SETTINGS){
            if(!edit){
                if(pos>0){
                    Nextion_SetObjAttr(gps_obj[pos],"bco",DISP_GREY);
                    pos--;
                    Nextion_SetObjAttr(gps_obj[pos],"bco",DISP_BLUE);
                }
                else{
                    Nextion_SetObjAttr(gps_obj[0],"bco",DISP_GREY);
                    pos=DISP_OBJ_GPS_NUM-1;
                    Nextion_SetObjAttr(gps_obj[pos],"bco",DISP_BLUE);
                }
            }else{
                switch(pos){
                    case 0:
                    case 9:
                        if(par[pos]==0) par[pos]=1; else par[pos]=0;
                        if(par[pos]) Nextion_SetText(gps_obj[pos],"+");else Nextion_SetText(gps_obj[pos],"-");
                    break;
                    case 1 ... 8:
                    case 10 ... 18:
                        if(par[pos]==9) par[pos]=0; else par[pos]++;
                        Nextion_SetObjAttr(gps_obj[pos],"val",par[pos]);
                    break; 
                }
                
            }
        }else{
            if(!edit){
                 edit =1;
                Nextion_SetText("b0","SET");
                Nextion_ObjVisible("b1",0);
            }else{
                edit=0;
                Nextion_SetText("b0","EDIT");
                Nextion_ObjVisible("b1",1);
                Nextion_SetObjAttr(gps_obj[pos],"pco",DISP_WHITE);
            }
            switch(pos){
                case 0:  break;
                case 1: break;
                case 2: break; 
            }
        }
        ResetSWTimer();
    }
    
    if(SW2_Pressed){
        uint32 cnt= SWTimer_ReadCounter();
        if((cnt>SW_HOLD_PERIOD_SETTINGS*3) && !edit){
            Nextion_SetObjectAttr("b1", "bco", DISP_YELLOW);
            Nextion_SetText("b1","SAVING");
            
            latitude=0;
            for(int i = 1; i < 9; i++){
                latitude += par[i]*powersOf10[i];
            }
            if(par[0]==0) latitude*=-1;
            
            flatitude=(float)latitude/1000000.0f;
            uint8 tmp[4];
            uint8 inv[4];
            float invLat, invLong;
            memcpy(tmp, &flatitude, 4);
            for(uint8_t i=0;i<4;i++)
                inv[3-i]=tmp[i];
            memcpy(&invLat, inv, 4);    
            
            longitude=0;
            for(int i = 0; i < 9; i++){
                longitude += par[i+10]*powersOf10[i];
            }
            if(par[9]==0) longitude*=-1;
            
            flongitude = (float)longitude/1000000.0f;
            memcpy(tmp, &flongitude, 4);
            for(uint8_t i=0;i<4;i++)
                inv[3-i]=tmp[i];
            memcpy(&invLong, inv, 4);
            
            
            // SAVE PARAMETERS to EEPROM HERE
            EEPROM_SaveGPS(invLat, invLong );
            
            //CyDelay(2000); // Mimicing saving
            // Return button text back and switch page
//            Nextion_SetObjectAttr("b1", "bco", DISP_GREY_BTN);
//            Nextion_SetText("b1","SAVE");
//            setMode(RUNTIME_SETTINGS_MODE);
        }
        Restart_Inactivity_timer();
    }
    
    if(SW2_Pressed && SW1_2_Released){
        CyDelay(20);
        while(!SW1_2_Released){};
        SW2_Pressed=0;
        uint32 cnt= SWTimer_ReadCounter();
        if(cnt < SW_HOLD_PERIOD_SETTINGS*3){
            if(!edit){
                if(pos<(DISP_OBJ_GPS_NUM-1)){
                    Nextion_SetObjAttr(gps_obj[pos],"bco",DISP_GREY);
                    pos++;
                    Nextion_SetObjAttr(gps_obj[pos],"bco",DISP_BLUE);
                }
                else{
                    Nextion_SetObjAttr(gps_obj[DISP_OBJ_GPS_NUM-1],"bco",DISP_GREY);
                    pos=0;
                    Nextion_SetObjAttr(gps_obj[pos],"bco",DISP_BLUE);
                }
            }else{
                switch(pos){
                    case 0:
                    case 9:
                        if(par[pos]==0) par[pos]=1; else par[pos]=0;
                        if(par[pos]) Nextion_SetText(gps_obj[pos],"+");else Nextion_SetText(gps_obj[pos],"-");
                        break;
                    case 1 ... 8:
                    case 10 ... 18:
                        if(par[pos]<=0) par[pos]=9; else par[pos]--;
                        Nextion_SetObjAttr(gps_obj[pos],"val",par[pos]);    
                        break; 
                }
                
                
            }
        }else{
            Nextion_SetObjectAttr("b1", "bco", DISP_GREY_BTN);
            Nextion_SetText("b1","SAVE");
            setMode(RUNTIME_SETTINGS_MODE);
            
        }
//        SWTimer_Stop();
//        SWTimer_WriteCounter(0);
        ResetSWTimer();
    }
    
    
}

void runtimeModbusAddress(void){
    static uint8_t pos;
    static uint8_t edit;
    static uint8_t par[DISP_OBJ_MODBUS_NUM];
    
    // Temporary value
    static uint16_t sleep_time;
    if(newMode){
        newMode=0;
        Nextion_SetPage(PAGE_MODBUS_ADDR);
        //system_SetLED(SYSTEM_LED_GREEN);
        system_SetLed(0 , SYSTEM_LED_GREEN, 0, 1);
        LED_Update();
        pos =0;
        Nextion_SetObjAttr(modbus_obj[pos],"bco",DISP_BLUE);
        // READ MODBUS ADDRESS HERE AND GIVE VALUES TO FIELDS
        par[2]=(mySlaveAddr%100)%10;
        par[1]= (mySlaveAddr%100)/10;
        par[0]= mySlaveAddr/100;
        for(uint8_t i=0;i<3;i++)
            Nextion_SendInt(modbusaddr_obj[i],par[i]);
        
        Restart_Inactivity_timer();
    }
    
    Check_Inactivity();
    
    /*Check Modbus request here*/
    if(modbusRequestReceived){
        modbusRequestReceived=0;
        runtime_ProcessRequest();// Response to Modbus
        bytesIn =0;
    }
    
    if(SW1_Pressed){
        uint32 cnt= SWTimer_ReadCounter();
        if(cnt>SW_HOLD_PERIOD_SETTINGS*3){
            if(!edit){
                Nextion_SetObjAttr(modbus_obj[pos],"bco",DISP_YELLOW);
                Nextion_SetObjAttr(modbus_obj[pos],"pco",DISP_BLUE);
                
            }else{
               Nextion_SetObjAttr(modbus_obj[pos],"bco",DISP_BLUE);
               Nextion_SetObjAttr(modbus_obj[pos],"pco",DISP_WHITE);
            }
        }
        Restart_Inactivity_timer();
    }
    
    if(SW1_Pressed && SW1_2_Released){
        CyDelay(20);
        while(!SW1_2_Released){};
        SW1_Pressed=0;
        uint32 cnt= SWTimer_ReadCounter();
        if(cnt < SW_HOLD_PERIOD_SETTINGS*3){
            if(!edit){
                if(pos>0){
                    Nextion_SetObjAttr(modbus_obj[pos],"bco",DISP_GREY);
                    pos--;
                    Nextion_SetObjAttr(modbus_obj[pos],"bco",DISP_BLUE);
                }
                else{
                    Nextion_SetObjAttr(modbus_obj[0],"bco",DISP_GREY);
                    pos=DISP_OBJ_MODBUS_NUM-1;
                    Nextion_SetObjAttr(modbus_obj[pos],"bco",DISP_BLUE);
                }
            }else{
                switch(pos){
                    case 0:if(par[0]==2) par[0]=0; else par[0]++;break; 
                    case 1:if(par[1]==9) par[1]=0; else par[1]++;break; 
                    case 2:if(par[2]==9) par[2]=0; else par[2]++;break;     
                }

                Nextion_SetObjAttr(modbus_obj[pos],"val",par[pos]);
            }
        }else{
            if(!edit){
                 edit =1;
                Nextion_SetText("b0","SET");
                Nextion_ObjVisible("b1",0);
            }else{
                edit=0;
                Nextion_SetText("b0","EDIT");
                Nextion_ObjVisible("b1",1);
                Nextion_SetObjAttr(modbus_obj[pos],"pco",DISP_WHITE);
            }
            switch(pos){
                case 0:  break;
                case 1: break;
                case 2: break; 
            }
        }
//        SWTimer_Stop();
//        SWTimer_WriteCounter(0);
        ResetSWTimer();
    }
    
    if(SW2_Pressed){
        uint32 cnt= SWTimer_ReadCounter();
        if((cnt>SW_HOLD_PERIOD_SETTINGS*3) && !edit){
            Nextion_SetObjectAttr("b1", "bco", DISP_YELLOW);
            Nextion_SetText("b1","SAVING");
            // SAVE PARAMETERS to EEPROM HERE
            uint16_t chk=par[0]*100+par[1]*10+par[2];
            if(chk<MODBUS_MAXIMAL_ADDRESS && chk!= 0 ) 
                mySlaveAddr = (uint8_t)chk;
            EEPROM_SetMyAddress(&mySlaveAddr);
            //CyDelay(2000); // Mimicing saving
            // Return button text back and switch page
//            Nextion_SetObjectAttr("b1", "bco", DISP_GREY_BTN);
//            Nextion_SetText("b1","SAVE");
//            setMode(RUNTIME_SETTINGS_MODE);
        }
        Restart_Inactivity_timer();
    }
    
    if(SW2_Pressed && SW1_2_Released){
        CyDelay(20);
        while(!SW1_2_Released){};
        SW2_Pressed=0;
        uint32 cnt= SWTimer_ReadCounter();
        if(cnt < SW_HOLD_PERIOD_SETTINGS*3){
            if(!edit){
                if(pos<(DISP_OBJ_MODBUS_NUM-1)){
                    Nextion_SetObjAttr(modbus_obj[pos],"bco",DISP_GREY);
                    pos++;
                    Nextion_SetObjAttr(modbus_obj[pos],"bco",DISP_BLUE);
                }
                else{
                    Nextion_SetObjAttr(modbus_obj[DISP_OBJ_MODBUS_NUM-1],"bco",DISP_GREY);
                    pos=0;
                    Nextion_SetObjAttr(modbus_obj[pos],"bco",DISP_BLUE);
                }
            }else{
                switch(pos){
                    case 0:if(par[0]<=0) par[0]=2; else par[0]--;break; 
                    case 1:if(par[1]<=0) par[1]=9; else par[1]--;break; 
                    case 2:if(par[2]<=0) par[2]=9; else par[2]--;break;     
                }
                Nextion_SetObjAttr(modbus_obj[pos],"val",par[pos]);
            }
        }else{
            Nextion_SetObjectAttr("b1", "bco", DISP_GREY_BTN);
            Nextion_SetText("b1","SAVE");
            setMode(RUNTIME_SETTINGS_MODE);
            
        }
//        SWTimer_Stop();
//        SWTimer_WriteCounter(0);
        ResetSWTimer();
    }
    

}




void runtimeAlarmConfig(void){
    static uint8_t pos;
    static uint8_t edit;
    static uint8_t par[DISP_OBJ_ALARM_NUM];// remove static and get relay data at newMode
    static uint8_t curCh;
    uint16_t alarmLevel;
    char str[20];
    static char gas[2][10];
    static char units[2][4];
    static uint32_t range[2];
    // These vars are temporary
    static uint8_t alarm1[2];
    static uint8_t alarm2[2];
    static uint8_t alarm3[2];
    static uint8_t alarm1asc[2];
    static uint8_t alarm2asc[2];
    static uint8_t alarm3asc[2];
    static uint8_t cal_level[2];
    static uint8_t resp_factor[2];
    
    
    
    curCh = par[0];
    if(newMode){
        newMode=0;
        curCh =0;
        par[0]=0;
        for(uint8_t i=0;i<2;i++){
            alarm1[i]=0;
            alarm2[i]=0;
            alarm3[i]=0;
            alarm1asc[i]=0;
            alarm2asc[i]=0;
            alarm3asc[i]=0;
        }
        
        Nextion_SetPage(PAGE_ALARM_CONFIG);
        system_SetLed(0 , SYSTEM_LED_GREEN, 0, 1);
        LED_Update();
        pos =0;
        Nextion_SetObjAttr(alarm_obj[pos],"bco",DISP_BLUE);
        Restart_Inactivity_timer();
        
        // READ SETINGS HERE AND GIVE NAMES TO FIELDS
        
        alarm1[0] = (uint16_t)((sensor[0][ALARM1MSB]<<8)+sensor[0][ALARM1LSB])/100;
        alarm2[0] = (uint16_t)((sensor[0][ALARM2MSB]<<8)+sensor[0][ALARM2LSB])/100;
        alarm3[0] = (uint16_t)((sensor[0][ALARM3MSB]<<8)+sensor[0][ALARM3LSB])/100;
        alarm1asc[0] = sensor[0][ALARM1ASCLSB];
        alarm2asc[0] = sensor[0][ALARM2ASCLSB];
        alarm3asc[0] = sensor[0][ALARM3ASCLSB];
        cal_level[0] = ((sensor[0][CALLEVEL3]<<8)+sensor[0][CALLEVEL4])/100;
        //resp_factor[0] = ((sensor[0][RESPFACTORMSB]<<8)+sensor[0][RESPFACTORLSB])/100;
        range[0] = (uint32_t) (sensor[0][FULLSCALERANGE1]<<24)+        
                (uint32_t) (sensor[0][FULLSCALERANGE2]<<16)+
                (uint32_t) (sensor[0][FULLSCALERANGE3]<<8)+
                (uint32_t) sensor[0][FULLSCALERANGE4];
        if(ch2Active){
              range[1] = (uint32_t) (sensor[1][FULLSCALERANGE1]<<24)+        
                (uint32_t) (sensor[1][FULLSCALERANGE2]<<16)+
                (uint32_t) (sensor[1][FULLSCALERANGE3]<<8)+
                (uint32_t) sensor[1][FULLSCALERANGE4];
        }
        memset(str , 0 , sizeof(str));
        remove_trailing_spaces(gas[0], (char*) &sensor[0][COMPAUND1]);
        
        memset(str , 0 , sizeof(str));
        strncpy(str, (char *)&sensor[0][UNIT1], 4);
        remove_leading_spaces((char *)str);
        remove_trailing_spaces(units[0], (char*) str);
        
        memset(str , 0 , sizeof(str));
        sprintf(str,"%s 0-%d %s", gas[0],range[0], units[0] );
        Nextion_SetText(DISP_OBJ_CH,str);
        
        if(ch2Active){
            memset(str , 0 , sizeof(str));
            remove_trailing_spaces(gas[1], (char*) &sensor[1][COMPAUND1]);
            memset(str , 0 , sizeof(str));
            strncpy(str, (char *)&sensor[1][UNIT1], 4);
            remove_leading_spaces((char *)str);
            remove_trailing_spaces(units[1], (char*) str);
        }
        
        Nextion_SetObjAttr(DISP_OBJ_AL1,"val",alarm1[0]);
        Nextion_SetObjAttr(DISP_OBJ_AL2,"val",alarm2[0]);
        Nextion_SetObjAttr(DISP_OBJ_AL3,"val",alarm3[0]);
        Nextion_SetObjAttr(DISP_CAL_LEVEL,"val",cal_level[0]);
        //Nextion_SetObjAttr(DISP_RESP_FACTOR,"val",resp_factor[0]);
        
        if(!alarm1asc[0])
            Nextion_SetText(DISP_OBJ_AL1_ASC,"DESC");
        else
            Nextion_SetText(DISP_OBJ_AL1_ASC,"ASC");
        if(!alarm2asc[0])
            Nextion_SetText(DISP_OBJ_AL2_ASC,"DESC");
        else
            Nextion_SetText(DISP_OBJ_AL2_ASC,"ASC");
        if(!alarm3asc[0])
            Nextion_SetText(DISP_OBJ_AL3_ASC,"DESC");
        else
            Nextion_SetText(DISP_OBJ_AL3_ASC,"ASC");    
        if(ch2Active){
            alarm1[1] = (uint16_t)((sensor[1][ALARM1MSB]<<8)+sensor[1][ALARM1LSB])/100;
            alarm2[1] = (uint16_t)((sensor[1][ALARM2MSB]<<8)+sensor[1][ALARM2LSB])/100;
            alarm3[1] = (uint16_t)((sensor[1][ALARM3MSB]<<8)+sensor[1][ALARM3LSB])/100;
            alarm1asc[1] = sensor[1][ALARM1ASCLSB];
            alarm2asc[1] = sensor[1][ALARM2ASCLSB];
            alarm3asc[1] = sensor[1][ALARM3ASCLSB];
            cal_level[1] = ((sensor[1][CALLEVEL3]<<8)+sensor[1][CALLEVEL4])/100;
            resp_factor[1] = ((sensor[1][RESPFACTORMSB]<<8)+sensor[1][RESPFACTORLSB])/100;
        
        }
        
    }
    
    Check_Inactivity();
    
    /*Check Modbus request here*/
    if(modbusRequestReceived){
        modbusRequestReceived=0;
        runtime_ProcessRequest();// Response to Modbus
        bytesIn =0;
    }
    
    
    if(SW1_Pressed){
        uint32 cnt= SWTimer_ReadCounter();
        if(cnt>SW_HOLD_PERIOD_SETTINGS*2){
            if(!edit){
                Nextion_SetObjAttr(alarm_obj[pos],"bco",DISP_YELLOW);
                Nextion_SetObjAttr(alarm_obj[pos],"pco",DISP_BLUE);
                
            }else{
               Nextion_SetObjAttr(alarm_obj[pos],"bco",DISP_BLUE);
               Nextion_SetObjAttr(alarm_obj[pos],"pco",DISP_WHITE);
            }
        }
        Restart_Inactivity_timer();
    }
    
    if(SW1_Pressed && SW1_2_Released){
        //CyDelay(20);
        while(!SW1_2_Released){};
        SW1_Pressed=0;
        uint32 cnt= SWTimer_ReadCounter();
        if(cnt < SW_HOLD_PERIOD_SETTINGS*2){
            if(!edit){
                if(pos>0){
                    Nextion_SetObjAttr(alarm_obj[pos],"bco",DISP_GREY);
                    pos--;
                    Nextion_SetObjAttr(alarm_obj[pos],"bco",DISP_BLUE);
                }
                else{
                    Nextion_SetObjAttr(alarm_obj[0],"bco",DISP_GREY);
                    pos=DISP_OBJ_ALARM_NUM-1;
                    Nextion_SetObjAttr(alarm_obj[pos],"bco",DISP_BLUE);
                }
            }else{
                
                switch(pos){
                    case 0:
                        if(ch2Active){
                            par[0]^=1;
                            curCh =par[0];
                            
                            memset(str , 0 , sizeof(str));
                            sprintf(str,"%s 0-%d %s", gas[curCh],range[curCh], units[curCh] );
                            Nextion_SetText(DISP_OBJ_CH,str);
                            
                            Nextion_SetObjAttr(DISP_OBJ_AL1,"val",alarm1[curCh]);
                            Nextion_SetObjAttr(DISP_OBJ_AL2,"val",alarm2[curCh]);
                            Nextion_SetObjAttr(DISP_OBJ_AL3,"val",alarm3[curCh]);
                            if(!alarm1asc[curCh])
                                Nextion_SetText(DISP_OBJ_AL1_ASC,"DESC");
                            else
                                Nextion_SetText(DISP_OBJ_AL1_ASC,"ASC");
                            if(!alarm2asc[curCh])
                                Nextion_SetText(DISP_OBJ_AL2_ASC,"DESC");
                            else
                                Nextion_SetText(DISP_OBJ_AL2_ASC,"ASC");
                            if(!alarm3asc[curCh])
                                Nextion_SetText(DISP_OBJ_AL3_ASC,"DESC");
                            else
                                Nextion_SetText(DISP_OBJ_AL3_ASC,"ASC");    
                            Nextion_SetObjAttr(DISP_CAL_LEVEL,"val",cal_level[curCh]);
                        }
                    break;   
                    case 1:if(alarm1[curCh]<100)alarm1[curCh]++; Nextion_SetObjAttr(alarm_obj[pos],"val",alarm1[curCh]);break;
                    case 3:if(alarm2[curCh]<100)alarm2[curCh]++; Nextion_SetObjAttr(alarm_obj[pos],"val",alarm2[curCh]);break;
                    case 5:if(alarm3[curCh]<100)alarm3[curCh]++; Nextion_SetObjAttr(alarm_obj[pos],"val",alarm3[curCh]);break;
                    case 2:
                        alarm1asc[curCh]^=1;
                        if(alarm1asc[curCh]) Nextion_SetText(DISP_OBJ_AL1_ASC,"ASC");else Nextion_SetText(DISP_OBJ_AL1_ASC,"DESC"); 
                    break;
                    case 4:
                        alarm2asc[curCh]^=1;
                        if(alarm2asc[curCh]) Nextion_SetText(DISP_OBJ_AL2_ASC,"ASC");else Nextion_SetText(DISP_OBJ_AL2_ASC,"DESC"); 
                    break; 
                    case 6:
                        alarm3asc[curCh]^=1;
                        if(alarm3asc[curCh]) Nextion_SetText(DISP_OBJ_AL3_ASC,"ASC");else Nextion_SetText(DISP_OBJ_AL3_ASC,"DESC"); 
                    break;
                    case 7:if(cal_level[curCh]<100)cal_level[curCh]++; Nextion_SetObjAttr(alarm_obj[pos],"val",cal_level[curCh]);break;
                    //case 8:if(resp_factor[curCh]<100)resp_factor[curCh]++; Nextion_SetObjAttr(alarm_obj[pos],"val",resp_factor[curCh]);break;
                        
                }
            }
        }else{
            if(!edit){
                 edit =1;
                Nextion_SetText("b0","SET");
                Nextion_ObjVisible("b1",0);
            }else{
                edit=0;
                Nextion_SetText("b0","EDIT");
                Nextion_ObjVisible("b1",1);
                 Nextion_SetObjAttr(alarm_obj[pos],"pco",DISP_WHITE);
            }
            switch(pos){
                case 0:  break;
                case 1: break;
                case 2: break; 
                case 3: break; 
                case 4: break;
                case 5: break;
                case 6: break;
            }
        }
        ResetSWTimer();
    }
    
    if(SW2_Pressed){
        uint32 cnt= SWTimer_ReadCounter();
        if((cnt>SW_HOLD_PERIOD_SETTINGS*2)&&(cnt<SW_HOLD_PERIOD_SETTINGS*3) && !edit){
            // SAVE PARAMETERS HERE
            sensor[0][ALARM1ASCLSB] = alarm1asc[0];
            runtime_UpdateSensor(0 , ALARM1ASCMSB-4, alarm1asc[0]);
            sensor[0][ALARM2ASCLSB] = alarm2asc[0];
            runtime_UpdateSensor(0 , ALARM2ASCMSB-4, alarm2asc[0]);
            sensor[0][ALARM3ASCLSB] = alarm3asc[0];
            runtime_UpdateSensor(0 , ALARM3ASCMSB-4, alarm3asc[0]);
            sensor[0][ALARM1MSB] =  ((alarm1[0]*100)>>8)&0xFF;
            sensor[0][ALARM1LSB] =  (alarm1[0]*100)&0xFF;
            runtime_UpdateSensor(0 , ALARM1MSB-4, alarm1[0]*100);
            sensor[0][ALARM2MSB] =  ((alarm2[0]*100)>>8)&0xFF;
            sensor[0][ALARM2LSB] =  (alarm2[0]*100)&0xFF;
            runtime_UpdateSensor(0 , ALARM2MSB-4, alarm2[0]*100);
            sensor[0][ALARM3MSB] =  ((alarm3[0]*100)>>8)&0xFF;
            sensor[0][ALARM3LSB] =  (alarm3[0]*100)&0xFF;
            runtime_UpdateSensor(0 , ALARM3MSB-4, alarm3[0]*100);
            
            sensor[0][CALLEVEL3] =  ((cal_level[0]*100)>>8)&0xFF;
            sensor[0][CALLEVEL4] =  (cal_level[0]*100)&0xFF;
            runtime_UpdateSensor(0 , CALLEVEL3-4, cal_level[0]*100);
            
            
            runtime_CtrlSensor(0 , SENSOR_SYS_REMOTE_REG, RUNTIME_SMART_SENSOR_SYS_REMOTE_SAVE_TO_EEPROM);
            RecalculateAlarms(0);
            
            
            
        }else if ((cnt>SW_HOLD_PERIOD_SETTINGS*3) && !edit){
            Nextion_SetObjectAttr("b1", "bco", DISP_YELLOW);
            Nextion_SetText("b1","SAVING");
            if(ch2Active){
                sensor[1][ALARM1ASCLSB] = alarm1asc[1];
                runtime_UpdateSensor(1 , ALARM1ASCMSB-4, alarm1asc[1]);
                sensor[1][ALARM2ASCLSB] = alarm2asc[1];
                runtime_UpdateSensor(1 , ALARM2ASCMSB-4, alarm2asc[1]);
                sensor[1][ALARM3ASCLSB] = alarm3asc[1];
                runtime_UpdateSensor(1 , ALARM3ASCMSB-4, alarm3asc[1]);
                sensor[1][ALARM1MSB] =  ((alarm1[1]*100)>>8)&0xFF;
                sensor[1][ALARM1LSB] =  (alarm1[1]*100)&0xFF;
                runtime_UpdateSensor(1 , ALARM1MSB-4, alarm1[1]*100);
                sensor[1][ALARM2MSB] =  ((alarm2[1]*100)>>8)&0xFF;
                sensor[1][ALARM2LSB] =  (alarm2[1]*100)&0xFF;
                runtime_UpdateSensor(1 , ALARM2MSB-4, alarm2[1]*100);
                sensor[1][ALARM3MSB] =  ((alarm3[1]*100)>>8)&0xFF;
                sensor[1][ALARM3LSB] =  (alarm3[1]*100)&0xFF;
                runtime_UpdateSensor(1 , ALARM3MSB-4, alarm3[1]*100);
                
                sensor[1][CALLEVEL3] =  ((cal_level[1]*100)>>8)&0xFF;
                sensor[1][CALLEVEL4] =  (cal_level[1]*100)&0xFF;
                runtime_UpdateSensor(1 , CALLEVEL3-4, cal_level[1]*100);
                SelectChannel(1);
                
                runtime_CtrlSensor(1 , SENSOR_SYS_REMOTE_REG, RUNTIME_SMART_SENSOR_SYS_REMOTE_SAVE_TO_EEPROM);
                RecalculateAlarms(1);
                //CyDelay(100);
            
            }
        
        }
        Restart_Inactivity_timer();
    }
    
    if(SW2_Pressed && SW1_2_Released){
        //CyDelay(20);
        while(!SW1_2_Released){};
        SW2_Pressed=0;
        uint32 cnt= SWTimer_ReadCounter();
        if(cnt < SW_HOLD_PERIOD_SETTINGS*3){
            if(!edit){
                if(pos<(DISP_OBJ_ALARM_NUM-1)){
                    Nextion_SetObjAttr(alarm_obj[pos],"bco",DISP_GREY);
                    pos++;
                    Nextion_SetObjAttr(alarm_obj[pos],"bco",DISP_BLUE);
                }
                else{
                    Nextion_SetObjAttr(alarm_obj[DISP_OBJ_ALARM_NUM-1],"bco",DISP_GREY);
                    pos=0;
                    Nextion_SetObjAttr(alarm_obj[pos],"bco",DISP_BLUE);
                }
            }else{
                
                switch(pos){
                    case 0:
                        if(ch2Active){
                            par[0]^=1;
                            curCh =par[0];
                            
                            memset(str , 0 , sizeof(str));
                            sprintf(str,"%s 0-%d %s", gas[curCh],range[curCh], units[curCh] );
                            Nextion_SetText(DISP_OBJ_CH,str);
                            
                            
                            Nextion_SetObjAttr(DISP_OBJ_AL1,"val",alarm1[curCh]);
                            Nextion_SetObjAttr(DISP_OBJ_AL2,"val",alarm2[curCh]);
                            Nextion_SetObjAttr(DISP_OBJ_AL3,"val",alarm3[curCh]);
                            if(!alarm1asc[curCh])
                                Nextion_SetText(DISP_OBJ_AL1_ASC,"DESC");
                            else
                                Nextion_SetText(DISP_OBJ_AL1_ASC,"ASC");
                            if(!alarm2asc[curCh])
                                Nextion_SetText(DISP_OBJ_AL2_ASC,"DESC");
                            else
                                Nextion_SetText(DISP_OBJ_AL2_ASC,"ASC");
                            if(!alarm3asc[curCh])
                                Nextion_SetText(DISP_OBJ_AL3_ASC,"DESC");
                            else
                                Nextion_SetText(DISP_OBJ_AL3_ASC,"ASC");   
                            Nextion_SetObjAttr(DISP_CAL_LEVEL,"val",cal_level[curCh]);    
                        }
                    break; 
                    case 1:if(alarm1[curCh]>0)alarm1[curCh]--; Nextion_SetObjAttr(alarm_obj[pos],"val",alarm1[curCh]);break; 
                    case 3:if(alarm2[curCh]>0)alarm2[curCh]--; Nextion_SetObjAttr(alarm_obj[pos],"val",alarm2[curCh]);break; 
                    case 5:if(alarm3[curCh]>0)alarm3[curCh]--; Nextion_SetObjAttr(alarm_obj[pos],"val",alarm3[curCh]);break;      
                    case 2:
                        alarm1asc[curCh]^=1;
                        if(alarm1asc[curCh]) Nextion_SetText(DISP_OBJ_AL1_ASC,"ASC");else Nextion_SetText(DISP_OBJ_AL1_ASC,"DESC");
                    break;
                    case 4:
                        alarm2asc[curCh]^=1;
                        if(alarm2asc[curCh]) Nextion_SetText(DISP_OBJ_AL2_ASC,"ASC");else Nextion_SetText(DISP_OBJ_AL2_ASC,"DESC"); 
                    break; 
                    case 6:
                        alarm3asc[curCh]^=1;
                        if(alarm3asc[curCh]) Nextion_SetText(DISP_OBJ_AL3_ASC,"ASC");else Nextion_SetText(DISP_OBJ_AL3_ASC,"DESC"); 
                    break;  
                    case 7:if(cal_level[curCh]>0)cal_level[curCh]--; Nextion_SetObjAttr(alarm_obj[pos],"val",cal_level[curCh]);break;      
                    //case 8:if(resp_factor[curCh]>0)resp_factor[curCh]--; Nextion_SetObjAttr(alarm_obj[pos],"val",resp_factor[curCh]);break;      
                }
            }
        }else{

           Nextion_SetObjectAttr("b1", "bco", DISP_GREY_BTN);
            Nextion_SetText("b1","SAVE");
            setMode(RUNTIME_SETTINGS_MODE); 
            
        }
        ResetSWTimer();
    }
}

void runtimeDateTime(void){
    static uint8_t pos;
    static uint8_t edit;
    static uint8_t par[DISP_OBJ_DATETIME_NUM];// remove static and get relay data at newMode
    
    // Temporary values
    static uint8_t day;
    static uint8_t month;
    static uint8_t year;
    static uint8_t hour;
    static uint8_t minute;
    
    if(newMode){
        newMode=0;
        Nextion_SetPage(PAGE_DATE_TIME);
        //system_SetLED(SYSTEM_LED_GREEN);
        system_SetLed(0, SYSTEM_LED_GREEN, 0, 0);
        LED_Update();
        pos =0;
        Nextion_SetObjAttr(date_time_obj[pos],"bco",DISP_BLUE);
        Restart_Inactivity_timer();
        // READ SETINGS HERE AND GIVE NAMES TO FIELDS
        month=date_time.month;
        Nextion_SendInt(date_time_obj[0],month);
        day = date_time.day;
        Nextion_SendInt(date_time_obj[1], day);
        year = date_time.year;
        Nextion_SendInt(date_time_obj[2],year);
        hour = date_time.hours;
        Nextion_SendInt(date_time_obj[3],hour);
        minute= date_time.minutes;
        Nextion_SendInt(date_time_obj[4],minute);
    }
    
    Check_Inactivity();
    
    /*Check Modbus request here*/
    if(modbusRequestReceived){
        modbusRequestReceived=0;
        runtime_ProcessRequest();// Response to Modbus
        bytesIn =0;
    }
    
    if(SW1_Pressed){
        uint32 cnt= SWTimer_ReadCounter();
        if(cnt>SW_HOLD_PERIOD_SETTINGS*3){
            if(!edit){
                Nextion_SetObjAttr(date_time_obj[pos],"bco",DISP_YELLOW);
                Nextion_SetObjAttr(date_time_obj[pos],"pco",DISP_BLUE);
                
            }else{
               Nextion_SetObjAttr(date_time_obj[pos],"bco",DISP_BLUE);
               Nextion_SetObjAttr(date_time_obj[pos],"pco",DISP_WHITE);
            }
        }
        Restart_Inactivity_timer();
    }
    
    if(SW1_Pressed && SW1_2_Released){
        CyDelay(20);
        //while(!SW1_2_Released){};
        SW1_Pressed=0;
        uint32 cnt= SWTimer_ReadCounter();
        if(cnt < SW_HOLD_PERIOD_SETTINGS*3){
            if(!edit){
                if(pos>0){
                    Nextion_SetObjAttr(date_time_obj[pos],"bco",DISP_GREY);
                    pos--;
                    Nextion_SetObjAttr(date_time_obj[pos],"bco",DISP_BLUE);
                }
                else{
                    Nextion_SetObjAttr(date_time_obj[0],"bco",DISP_GREY);
                    pos=DISP_OBJ_DATETIME_NUM-1;
                    Nextion_SetObjAttr(date_time_obj[pos],"bco",DISP_BLUE);
                }
            }else{
                par[pos]^=1;
                switch(pos){
                    
                    case 0: if(month<12) month++;else month=1; Nextion_SetObjAttr(date_time_obj[pos],"val",month); break;   
                    case 1: if(day<31)day++;else day=1; Nextion_SetObjAttr(date_time_obj[pos],"val",day);break; 
                    case 2: year++; Nextion_SetObjAttr(date_time_obj[pos],"val",year); break;
                    case 3:if(hour<23) hour++;else hour=0; Nextion_SetObjAttr(date_time_obj[pos],"val",hour); break; 
                    case 4:if(minute<59) minute++;else minute=0; Nextion_SetObjAttr(date_time_obj[pos],"val",minute); break; 
                }

            }
        }else{
            if(!edit){
                 edit =1;
                Nextion_SetText("b0","SET");
                Nextion_ObjVisible("b1",0);
            }else{
                edit=0;
                Nextion_SetText("b0","EDIT");
                Nextion_ObjVisible("b1",1);
                 Nextion_SetObjAttr(date_time_obj[pos],"pco",DISP_WHITE);
            }
            switch(pos){
                case 0:  break;
                case 1: break;
                case 2: break; 
                case 3: break; 
                case 4: break;
            }
        }
//        SWTimer_Stop();
//        SWTimer_WriteCounter(0);
        ResetSWTimer();
    }
    
    if(SW2_Pressed){
        uint32 cnt= SWTimer_ReadCounter();
        if((cnt>SW_HOLD_PERIOD_SETTINGS*3) && !edit){
            Nextion_SetObjectAttr("b1", "bco", DISP_YELLOW);
            Nextion_SetText("b1","SAVING");
            // SAVE PARAMETERS HERE
            date_time.day = day;
            date_time.month = month;
            date_time.year = year;
            date_time.hours = hour;
            date_time.minutes = minute;
            DS3231_SetDateTime(&date_time);
            //CyDelay(2000); // Mimicing saving
            // Return button text back and switch page
//            Nextion_SetObjectAttr("b1", "bco", DISP_GREY_BTN);
//            Nextion_SetText("b1","SAVE");
//            setMode(RUNTIME_SETTINGS_MODE);
        }
        Restart_Inactivity_timer();
    }
    
    if(SW2_Pressed && SW1_2_Released){
        CyDelay(20);
        //while(!SW1_2_Released){};
        SW2_Pressed=0;
        uint32 cnt= SWTimer_ReadCounter();
        if(cnt < SW_HOLD_PERIOD_SETTINGS*3){
            if(!edit){
                if(pos<(DISP_OBJ_DATETIME_NUM-1)){
                    Nextion_SetObjAttr(date_time_obj[pos],"bco",DISP_GREY);
                    pos++;
                    Nextion_SetObjAttr(date_time_obj[pos],"bco",DISP_BLUE);
                }
                else{
                    Nextion_SetObjAttr(date_time_obj[DISP_OBJ_DATETIME_NUM-1],"bco",DISP_GREY);
                    pos=0;
                    Nextion_SetObjAttr(date_time_obj[pos],"bco",DISP_BLUE);
                }
            }else{
            par[pos]^=1;
                switch(pos){
                    
                    case 0:if(month>1) month--;else month=12; Nextion_SetObjAttr(date_time_obj[pos],"val",month);break; 
                    case 1: if(day>1)day--; else day=31; Nextion_SetObjAttr(date_time_obj[pos],"val",day); break; 
                    case 2:year--; Nextion_SetObjAttr(date_time_obj[pos],"val",year); break; 
                    case 3:if(hour>0) hour--;else hour=23; Nextion_SetObjAttr(date_time_obj[pos],"val",hour); break; 
                    case 4:if(minute>0) minute--;else minute=59; Nextion_SetObjAttr(date_time_obj[pos],"val",minute); break; 
                }
                
            }
        }else{
            Nextion_SetObjectAttr("b1", "bco", DISP_GREY_BTN);
            Nextion_SetText("b1","SAVE");
            setMode(RUNTIME_SETTINGS_MODE); 
            
        }
//        SWTimer_Stop();
//        SWTimer_WriteCounter(0);
        ResetSWTimer();
    }
    
    if(SW3_Pressed||SW3_Pressed){
        ResetSWTimer();
    }
    
    
}

void runtimePollRate(void){
    static uint8_t edit;
    
    // Temporary value
    static uint16_t rate;
    if(newMode){
        newMode=0;
        Nextion_SetPage(PAGE_POLL_RATE);
        system_SetLed(0, SYSTEM_LED_GREEN, 0, 0);
        LED_Update();
        Nextion_SetObjAttr(DISP_OBJ_POLLRATE_VALUE,"bco",DISP_BLUE);
        Restart_Inactivity_timer();
        // READ SETINGS HERE AND GIVE NAMES TO FIELDS
        rate = i2cPeriod;
        Nextion_SendInt(pollrate_obj[0],i2cPeriod);
    }
    
    Check_Inactivity();
    
    /*Check Modbus request here*/
    if(modbusRequestReceived){
        modbusRequestReceived=0;
        runtime_ProcessRequest();// Response to Modbus
        bytesIn =0;
    }
    
    if(SW1_Pressed){
        uint32 cnt= SWTimer_ReadCounter();
        if(cnt>SW_HOLD_PERIOD_SETTINGS){
            if(!edit){
                Nextion_SetObjAttr(DISP_OBJ_POLLRATE_VALUE,"bco",DISP_YELLOW);
                Nextion_SetObjAttr(DISP_OBJ_POLLRATE_VALUE,"pco",DISP_BLUE);
            }else{
               Nextion_SetObjAttr(DISP_OBJ_POLLRATE_VALUE,"bco",DISP_BLUE);
               Nextion_SetObjAttr(DISP_OBJ_POLLRATE_VALUE,"pco",DISP_WHITE);
            }
        }
        Restart_Inactivity_timer();
    }
    
    if(SW1_Pressed && SW1_2_Released){
        CyDelay(20);
        //while(!SW1_2_Released){};
        SW1_Pressed=0;
        uint32 cnt= SWTimer_ReadCounter();
        if(cnt < SW_HOLD_PERIOD_SETTINGS){
            if(!edit){
                
            }else{
                if(rate>100)rate-=50;; Nextion_SetObjAttr(DISP_OBJ_POLLRATE_VALUE,"val",rate);
            }
        }else{
            if(!edit){
                 edit =1;
                Nextion_SetText("b0","SET");
                Nextion_ObjVisible("b1",0);
            }else{
                edit=0;
                Nextion_SetText("b0","EDIT");
                Nextion_ObjVisible("b1",1);
            }
        }
//        SWTimer_Stop();
//        SWTimer_WriteCounter(0);
        ResetSWTimer();
    }
    
    if(SW2_Pressed){
        uint32 cnt= SWTimer_ReadCounter();
        if((cnt>SW_HOLD_PERIOD_SETTINGS) && !edit){
            Nextion_SetObjectAttr("b1", "bco", DISP_YELLOW);
            Nextion_SetText("b1","SAVING");
            // SAVE PARAMETERS HERE
            i2cPeriod = rate;
            EEPROM_SetI2CPeriod(i2cPeriod);
            CyDelay(2000); // Mimicing saving
            // Return button text back and switch page
            Nextion_SetObjectAttr("b1", "bco", DISP_GREY_BTN);
            Nextion_SetText("b1","SAVE");
            setMode(RUNTIME_SETTINGS_MODE);
        }
        Restart_Inactivity_timer();
    }
    
    if(SW2_Pressed && SW1_2_Released){
        CyDelay(20);
        //while(!SW1_2_Released){};
        SW2_Pressed=0;
        uint32 cnt= SWTimer_ReadCounter();
        if(cnt < SW_HOLD_PERIOD_SETTINGS){
            if(!edit){
            }else{
                if(rate<MAXIMAL_DEFAULT_I2C_PERIOD)rate+=50; Nextion_SetObjAttr(DISP_OBJ_POLLRATE_VALUE,"val",rate);
            }
        }else{
            
            
        }
//        SWTimer_Stop();
//        SWTimer_WriteCounter(0);
        ResetSWTimer();
    }
}


void runtimeSleepTime(void){
    static uint8_t edit;
    
    // Temporary value
    static uint16_t sleep_time;
    if(newMode){
        newMode=0;
        Nextion_SetPage(PAGE_SLEEP_TIME);
        system_SetLed(0, SYSTEM_LED_GREEN, 0, 0);
        LED_Update();
        Nextion_SetObjAttr(DISP_OBJ_SLEEP_VALUE,"bco",DISP_BLUE);
        Restart_Inactivity_timer();
        sleep_time = sleepPeriod/1000;
        if(sleep_time>60){
            Nextion_SetText("t1","MINUTES");
            Nextion_SetObjAttr(DISP_OBJ_SLEEP_VALUE,"val",sleep_time/60);
        }
        else{    
            Nextion_SetText("t1","SECONDS");
            Nextion_SetObjAttr(DISP_OBJ_SLEEP_VALUE,"val",sleep_time);
        }
        // READ SETINGS HERE AND GIVE NAMES TO FIELDS
    }
    
    Check_Inactivity();
    
    /*Check Modbus request here*/
    if(modbusRequestReceived){
        modbusRequestReceived=0;
        runtime_ProcessRequest();// Response to Modbus
        bytesIn =0;
    }
    
    if(SW1_Pressed){
        uint32 cnt= SWTimer_ReadCounter();
        if(cnt>SW_HOLD_PERIOD_SETTINGS){
            if(!edit){
                Nextion_SetObjAttr(DISP_OBJ_SLEEP_VALUE,"bco",DISP_YELLOW);
                Nextion_SetObjAttr(DISP_OBJ_SLEEP_VALUE,"pco",DISP_BLUE);
            }else{
               Nextion_SetObjAttr(DISP_OBJ_SLEEP_VALUE,"bco",DISP_BLUE);
               Nextion_SetObjAttr(DISP_OBJ_SLEEP_VALUE,"pco",DISP_WHITE);
            }
        }
        Restart_Inactivity_timer();
    }
    
    if(SW1_Pressed && SW1_2_Released){
        CyDelay(20);
        //while(!SW1_2_Released){};
        SW1_Pressed=0;
        uint32 cnt= SWTimer_ReadCounter();
        if(cnt < SW_HOLD_PERIOD_SETTINGS){
            if(!edit){
                
            }else{
                
                if(sleep_time<MINIMAL_SLEEP_PERIOD/1000){
                    sleep_time=MINIMAL_SLEEP_PERIOD/1000;
                    Nextion_SetObjAttr(DISP_OBJ_SLEEP_VALUE,"val",sleep_time);
                }
                else if(sleep_time<60 ){
                    sleep_time+=1;
                    Nextion_SetObjAttr(DISP_OBJ_SLEEP_VALUE,"val",sleep_time);
                }
                else{
                    sleep_time+=60;
                    Nextion_SetObjAttr(DISP_OBJ_SLEEP_VALUE,"val",sleep_time/60);
                    Nextion_SetText("t1","MINUTES");
                }
            }
        }else{
            if(!edit){
                 edit =1;
                Nextion_SetText("b0","SET");
                Nextion_ObjVisible("b1",0);
            }else{
                edit=0;
                Nextion_SetText("b0","EDIT");
                Nextion_ObjVisible("b1",1);
            }
        }
        ResetSWTimer();
    }
    
    if(SW2_Pressed){
        uint32 cnt= SWTimer_ReadCounter();
        if((cnt>SW_HOLD_PERIOD_SETTINGS*3) && !edit){
            Nextion_SetObjectAttr("b1", "bco", DISP_YELLOW);
            Nextion_SetText("b1","SAVING");
            // SAVE PARAMETERS HERE
            sleepPeriod = sleep_time*1000;
            EEPROM_SetSleepPeriod(sleepPeriod);
            //CyDelay(2000); // Mimicing saving
            // Return button text back and switch page
            //Nextion_SetObjectAttr("b1", "bco", DISP_GREY_BTN);
            //Nextion_SetText("b1","SAVE");
            //setMode(RUNTIME_SETTINGS_MODE);
        }
        Restart_Inactivity_timer();
    }
    
    if(SW2_Pressed && SW1_2_Released){
        CyDelay(20);
        //while(!SW1_2_Released){};
        SW2_Pressed=0;
        uint32 cnt= SWTimer_ReadCounter();
        if(cnt < SW_HOLD_PERIOD_SETTINGS*3){
            if(!edit){
            }else{
                if(sleep_time>60){
                    sleep_time-=60;
                    Nextion_SetText("t1","MINUTES");
                    Nextion_SetObjAttr(DISP_OBJ_SLEEP_VALUE,"val",sleep_time/60);
                }else if(sleep_time>MINIMAL_SLEEP_PERIOD/1000){
                    sleep_time-=1; 
                    Nextion_SetText("t1","SECONDS");
                    Nextion_SetObjAttr(DISP_OBJ_SLEEP_VALUE,"val",sleep_time);
                }else{
                    sleep_time=0;
                    Nextion_SetText("t1","SECONDS");
                    Nextion_SetObjAttr(DISP_OBJ_SLEEP_VALUE,"val",sleep_time);
                }
            }
        }else{
            Nextion_SetObjectAttr("b1", "bco", DISP_GREY_BTN);
            Nextion_SetText("b1","SAVE");
            setMode(RUNTIME_SETTINGS_MODE);    
            
        }
//        SWTimer_Stop();
//        SWTimer_WriteCounter(0);
        ResetSWTimer();
    }
}


void runtimeLog(void){
    if(newMode){
        newMode=0;
        Nextion_SetPage(PAGE_LOG);
        system_SetLed(0, SYSTEM_LED_GREEN, 0, 0);
        LED_Update();
        Restart_Inactivity_timer();
        
        for(uint8_t i=0;i<LOG_MAX_ENTRIES;i++ )
            GetLogEntry((log_last_entry_idx+i)%LOG_MAX_ENTRIES, log_record[i]);
        
        Nextion_SetPage(PAGE_LOG);
        for(uint8_t i=0;i<LOG_MAX_ENTRIES;i++){
            Nextion_SetText(log_obj[i],log_record[(LOG_MAX_ENTRIES-1)-i]);
            char *found;
            found = strstr(log_record[(LOG_MAX_ENTRIES-1)-i], "ALARM1"); 
            if(found){
                Nextion_SetColor(log_obj[i],  DISP_YELLOW);
                Nextion_SetTextColor(log_obj[i],DISP_BLACK);
            }
            found = strstr(log_record[(LOG_MAX_ENTRIES-1)-i], "ALARM2"); 
            if(found){
                Nextion_SetColor(log_obj[i],  DISP_RED);
                Nextion_SetTextColor(log_obj[i],DISP_WHITE);
            } 
            found = strstr(log_record[(LOG_MAX_ENTRIES-1)-i], "ALARM3"); 
            if(found){
                Nextion_SetColor(log_obj[i],  DISP_RED);
                Nextion_SetTextColor(log_obj[i],DISP_WHITE);
            }
            found = strstr(log_record[(LOG_MAX_ENTRIES-1)-i], "FAULT"); 
            if(found){
                Nextion_SetColor(log_obj[i],  DISP_BLUE);
                Nextion_SetTextColor(log_obj[i],DISP_WHITE);
            }
            found = strstr(log_record[(LOG_MAX_ENTRIES-1)-i], "ZERO");
            if(found){
                Nextion_SetColor(log_obj[i],  DISP_WHITE);
                Nextion_SetTextColor(log_obj[i],DISP_BLACK);
            }
            found = strstr(log_record[(LOG_MAX_ENTRIES-1)-i], "SPAN");
            if(found){
                Nextion_SetColor(log_obj[i],  DISP_PURPLE);
                Nextion_SetTextColor(log_obj[i],DISP_WHITE);
            }
        }
    }
    Check_Inactivity();
    
    /*Check Modbus request here*/
    if(modbusRequestReceived){
        modbusRequestReceived=0;
        runtime_ProcessRequest();// Response to Modbus
        bytesIn =0;
    }
    
    if(SW2_Pressed){
        uint32 cnt= SWTimer_ReadCounter();
        if((cnt>SW_HOLD_PERIOD_SETTINGS*2)){
            Nextion_SetObjectAttr("b1", "bco", DISP_YELLOW);
            Nextion_SetText("b1","EXITING");
        }
        Restart_Inactivity_timer();
    }
    
    if(SW2_Pressed && SW1_2_Released){
        while(!SW1_2_Released){};
        SW2_Pressed=0;
        Nextion_SetObjectAttr("b1", "bco", DISP_GREY_BTN);
        Nextion_SetText("b1","EXIT");
        ResetSWTimer();
        setMode(RUNTIME_SETTINGS_MODE);
    }
    
}

void runtimeInfo(void){
    char str[50];
    char gas[10];
    char units[4];
    char sn[6];
     
    if(newMode){
        
        newMode=0;
        Nextion_SetPage(PAGE_INFO);
        system_SetLed(0, SYSTEM_LED_GREEN, 0, 0);
        LED_Update();
        Restart_Inactivity_timer();
        
        memset(str , 0 , sizeof(str));
        
        memset(gas , 0 , sizeof(gas));
        remove_trailing_spaces(gas, (char*) &sensor[0][COMPAUND1]);
        
        memset(str, 0 , sizeof(str));
        memset(units, 0 , sizeof(units));
        strncpy(str, (char *)&sensor[0][UNIT1], 4);
        remove_leading_spaces((char *)str);
        remove_trailing_spaces(units, (char*) str);
        
        memset(str, 0 , sizeof(str));
        
        memset(sn , 0 , sizeof(sn));
        memcpy(sn, (char *)&sensor[0][SERIALNUMBER1], sizeof(sn));

#ifndef  SINGLE_SENSOR_SYSTEM        
        sprintf(str, "CH1: %s 0 - %d %s", gas, sensor_status[0].fullScaleRange, units);
#else
        sprintf(str, "%s 0 - %d %s", gas, sensor_status[0].fullScaleRange, units);
#endif    
        Nextion_SetText(DISP_OBJ_CH1,str);
        
        sprintf(str, "SA2 S/N: %s", sn);
        Nextion_SetText(DISP_OBJ_CH1_2,str);
        
        
        memset(str , 0 , sizeof(str));
        if(!sensor[0][ZEROCALDATEMSB]&& !sensor[0][ZEROCALDATELSB])
            sprintf(str, "ZERO CAL DATE: N/A ");
        else
            sprintf(str, "ZERO CAL DATE: %02d/%02d/%02d ", sensor_status[0].zerocalmonth, sensor_status[0].zerocalday, sensor_status[0].zerocalyear );
        Nextion_SetText(DISP_OBJ_CH1_ZERO_CAL_DATE,str);
        memset(str , 0 , sizeof(str));
        if(!sensor[0][SPANCALDATEMSB]&& !sensor[0][SPANCALDATELSB])
            sprintf(str, "ZERO CAL DATE: N/A ");
        else
            sprintf(str, "SPAN CAL DATE: %02d/%02d/%02d ", sensor_status[0].spancalmonth, sensor_status[0].spancalday, sensor_status[0].spancalyear );
        Nextion_SetText(DISP_OBJ_CH1_SPAN_CAL_DATE,str);
        
        sprintf(str, "GPS:%f LAT,%f LON", flatitude, flongitude);
        Nextion_SetText(DISP_OBJ_GPS,str);
        
        
        if(ch2Active){
            memset(str , 0 , sizeof(str));
            
            memset(gas , 0 , sizeof(gas));
            remove_trailing_spaces(gas, (char*) &sensor[1][COMPAUND1]);
            
            memset(str, 0 , sizeof(str));
            memset(units, 0 , sizeof(units));
            strncpy(str, (char *)&sensor[0][UNIT1], 4);
            remove_leading_spaces((char *)str);
            remove_trailing_spaces(units, (char*) str);
        
            memset(str, 0 , sizeof(str));
            
            memset(sn , 0 , sizeof(sn));
            memcpy(sn, (char *)&sensor[1][SERIALNUMBER1], sizeof(sn));
            
            sprintf(str, "CH2: %s 0 - %d %s", gas, sensor_status[1].fullScaleRange, units);
            Nextion_SetText(DISP_OBJ_CH2,str);
        
            sprintf(str, "SA2 S/N: %s", sn);
            Nextion_SetText(DISP_OBJ_CH2_2,str);
        
            memset(str , 0 , sizeof(str));
            if(!sensor[1][ZEROCALDATEMSB]&& !sensor[1][ZEROCALDATELSB])
                sprintf(str, "ZERO CAL DATE: N/A ");
            else
                sprintf(str, "ZERO CAL DATE: %02d/%02d/%02d ", sensor_status[1].zerocalmonth, sensor_status[1].zerocalday,sensor_status[1].zerocalyear );
            Nextion_SetText(DISP_OBJ_CH2_ZERO_CAL_DATE,str);
            memset(str , 0 , sizeof(str));
            if(!sensor[1][SPANCALDATEMSB]&& !sensor[1][SPANCALDATELSB])
                sprintf(str, "ZERO CAL DATE: N/A ");
            else
                sprintf(str, "SPAN CAL DATE: %02d/%02d/%02d ", sensor_status[1].spancalmonth, sensor_status[1].spancalday,sensor_status[1].spancalyear );
            Nextion_SetText(DISP_OBJ_CH2_SPAN_CAL_DATE,str);
        }else{
            Nextion_ObjVisible(DISP_OBJ_CH2, 0);
            Nextion_ObjVisible(DISP_OBJ_CH2_ZERO_CAL_DATE, 0);
            Nextion_ObjVisible(DISP_OBJ_CH2_SPAN_CAL_DATE, 0);
            Nextion_ObjVisible(DISP_OBJ_CH2_2, 0);
        }
        
    }
    Check_Inactivity();
    
    /*Check Modbus request here*/
    if(modbusRequestReceived){
        modbusRequestReceived=0;
        runtime_ProcessRequest();// Response to Modbus
        bytesIn =0;
    }
    
    
    if(SW2_Pressed){
        uint32 cnt= SWTimer_ReadCounter();
        if((cnt>SW_HOLD_PERIOD_SETTINGS*2)){
            Nextion_SetObjectAttr("b1", "bco", DISP_YELLOW);
            Nextion_SetText("b1","EXITING");
        }
        Restart_Inactivity_timer();
    }
    
    
    
    if(SW2_Pressed && SW1_2_Released){
        while(!SW1_2_Released){};
        SW2_Pressed=0;
        ResetSWTimer();
        Nextion_SetObjectAttr("b1", "bco", DISP_GREY_BTN);
        Nextion_SetText("b1","EXIT");
        setMode(RUNTIME_SETTINGS_MODE);
    }
    
    if(SW1_Pressed||SW3_Pressed||SW4_Pressed){
        ResetSWTimer();
    }
}


void GoRealtime(uint8_t num){
    if(!num)
        setMode(RUNTIME_REALTIME_MODE1);
    else    
        setMode(RUNTIME_REALTIME_MODE2);
}

void SetRealTimePage(uint8_t num){
    if(!num)
        Nextion_SetPage(PAGE_REAL_TIME1);
    else    
        Nextion_SetPage(PAGE_REAL_TIME2);
    CyDelay(150);
}



//uint32_t counter32=0;

void Check_Inactivity(void){
    
    
    if(ScreenCounter_ReadCounter() >= INACTIVE_USER_TIMEUOT/2*1000){  // prescaler x2 is used for 16-bit timer with max value of 65535
        Reset_Inactivity_timer();
        GoRealtime(ch2Active);
    }
}

void Restart_Inactivity_timer(void){
    ScreenCounter_Stop();
    ScreenCounter_WriteCounter(0);
    ScreenCounter_Start();
}

void Reset_Inactivity_timer(void){
    ScreenCounter_Stop();
    ScreenCounter_WriteCounter(0);
    
}

void ResetSWTimer(void){
    SWTimer_Stop();
    SWTimer_WriteCounter(0);
}

void ShowCalculatedWarmupTime(uint8_t ch){
    uint16_t elapsed_time;
    uint32_t warmupcnt = (uint16_t)(sensor[ch][WARMUPTIMERMSB]<<8)+sensor[ch][WARMUPTIMERLSB];
    
    
    
    if(warmupcnt>3600){
        elapsed_time = warmupcnt/3600;
        Nextion_SetText(warmup_obj[ch],"HRS");
    }else if(warmupcnt>60){
        elapsed_time = warmupcnt/60;
        Nextion_SetText(warmup_obj[ch],"MIN");
    }else{
        elapsed_time = warmupcnt;
        Nextion_SetText(warmup_obj[ch],"SEC");
    }
    if(ch==0)
        Nextion_SendFloat(DISP_OBJ_SYS_READING1, warmupcnt,  0);
    else    
        Nextion_SendFloat(DISP_OBJ_SYS_READING2, warmupcnt,  0);
    //sprintf((char *)scratchText," WU %2d%4s %-6s",elapsed_time,ctime, runtime_sensorModule[ch].sensingType);
}

void AbortWarmup(uint8_t ch){
    runtime_CtrlSensor(ch , SENSOR_SYS_REMOTE_REG, RUNTIME_SMART_SENSOR_SYS_REMOTE_ABORT_WARMUP);
}

uint16_t encodeCurrentDate(void){
    uint16_t encodedDate;
    encodedDate = (date_time.year<<9)| (date_time.month<<5)|date_time.day;
    return encodedDate;
}


void decodeLastCalDate(uint8_t ch){
    uint16_t encoded_date;
    
    encoded_date=(uint16_t) (sensor[ch][ZEROCALDATEMSB]<<8)+sensor[ch][ZEROCALDATELSB];
    sensor_status[ch].zerocalyear = encoded_date>>9;
    sensor_status[ch].zerocalmonth = (encoded_date>>5)&15;
    sensor_status[ch].zerocalday = encoded_date&31;
    
    encoded_date=(uint16_t) (sensor[ch][SPANCALDATEMSB]<<8)+sensor[ch][SPANCALDATELSB];
    sensor_status[ch].spancalyear = encoded_date>>9;
    sensor_status[ch].spancalmonth = (encoded_date>>5)&15;
    sensor_status[ch].spancalday = encoded_date&31;

}


void UpdateWholeScreen(){
    char str[10];
    I2C_Recover();
    memset(str , 0 , sizeof(str));
        remove_trailing_spaces(str, (char*) &sensor[0][COMPAUND1]);
        Nextion_SetText(DISP_OBJ_GAS_TYPE1,str);
        memset(str , 0 , sizeof(str));
        char units[4];
        strncpy(units, (char *)&sensor[0][UNIT1], 4);
        remove_leading_spaces((char *)units);
        remove_trailing_spaces(str, (char*) units);
        Nextion_SetText(DISP_OBJ_ENG_UNIT1,str);
        Nextion_SetObjAttr(DISP_OBJ_STATUS1,"pic",OK_ICON);
        Nextion_SetObjectAttr(DISP_OBJ_CH1_ACTIVE, "pco", DISP_YELLOW);
        
        
        if(ch2Active){
            memset(str , 0 , sizeof(str));
            remove_trailing_spaces(str, (char*) &sensor[1][COMPAUND1]);
            Nextion_SetText(DISP_OBJ_GAS_TYPE2,str);
            memset(str , 0 , sizeof(str));
            char units[4];
            strncpy(units, (char *)&sensor[1][UNIT1], 4);
            remove_leading_spaces((char *)units);
            remove_trailing_spaces(str, (char*) units);
            Nextion_SetText(DISP_OBJ_ENG_UNIT2,str);
            Nextion_SetObjAttr(DISP_OBJ_STATUS2,"pic",OK_ICON);
            Nextion_SetObjectAttr(DISP_OBJ_CH2_ACTIVE, "pco", DISP_YELLOW);
            Nextion_SetObjAttr(DISP_OBJ_RING2,"pic",RING_DOUBLE_OK_ICON);
            
        }
        else    
            Nextion_SetObjectAttr(DISP_OBJ_CH2_ACTIVE, "pco", DISP_GREY);
        
        CyDelay(300);    
        if(ch2Active){
                Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_DOUBLE_OK_ICON);
        }
        else{
            Nextion_SetObjAttr(DISP_OBJ_RING1,"pic",RING_SINGLE_OK_ICON);
        }    
        CyDelay(300);
        
        
                
}


void Set_relay_Alarm1(){
    uint8_t set;
    set = (relay[0].energ)?0:1; 
    TCA9534_SetRelay(0 , set);
}

void Set_relay_Alarm2(){
uint8_t set;
    set = (relay[1].energ)?0:1; 
    TCA9534_SetRelay(1 , set);
    set = (relay[0].energ)?0:1; 
    TCA9534_SetRelay(0 , set);
}

void Set_relay_Alarm3(){
uint8_t set;
    set = (relay[2].energ)?0:1; 
    TCA9534_SetRelay(2 , set);
    set = (relay[1].energ)?0:1; 
    TCA9534_SetRelay(1 , set);
    set = (relay[0].energ)?0:1; 
    TCA9534_SetRelay(0 , set);
}

void Set_relay_Fault(){
    uint8_t set;
    set = (relay[3].energ)?0:1; 
    TCA9534_SetRelay(3 , set);
}



void Reset_relay_Alarm1(){
    uint8_t set;
    set = (relay[0].energ)?1:0; 
    TCA9534_SetRelay(0 , set);
}

void Reset_relay_Alarm2(){
uint8_t set;
    set = (relay[1].energ)?1:0; 
    TCA9534_SetRelay(1 , set);
}

void Reset_relay_Alarm3(){
uint8_t set;
    set = (relay[2].energ)?1:0; 
    TCA9534_SetRelay(2 , set);
}

void Reset_relay_Fault(){
    uint8_t set;
    set = (relay[3].energ)?1:0; 
    TCA9534_SetRelay(3 , set);
}

void Ack_realys(){
    uint8_t set;
    if(relay[0].ack){
        set = (relay[0].energ)?1:0;
    }
    if(relay[1].ack){
       if(relay[1].energ)
            set |= (1<<1);
       else     
            set &= ~(1<<1);
    }
    
    if(relay[2].ack){
       if(relay[2].energ)
            set |= (1<<2);
       else     
            set &= ~(1<<2);
    }
    if(relay[3].ack){
       if(relay[3].energ)
            set |= (1<<3);
       else     
            set &= ~(1<<3);
    }
    TCA9534_WriteReg(set);
}

void Reset_relays(){
    uint8_t set;
    set = (relay[0].energ)?1:0;
    if(relay[1].energ)
        set |= (1<<1);
    else     
        set &= ~(1<<1);
    if(relay[2].energ)
        set |= (1<<2);
    else     
        set &= ~(1<<2);
    if(relay[3].energ)
        set |= (1<<3);
    else     
        set &= ~(1<<3);    
    TCA9534_WriteReg(set);    
}



/////////////////////////////////   TEMPORARY BLE CALIBRATION SHIT
#define SYS_REMOTE_IDLE                     ( 0x00u )
#define SYS_REMOTE_CAL_ZERO                 ( 0x10u )
#define SYS_REMOTE_CAL_SPAN                 ( 0x20u )
#define SYS_REMOTE_CAL_WORKING              ( 0x21u )
#define SYS_REMOTE_CAL_STABILITY            ( 0x22u )
#define SYS_REMOTE_CAL_REMOVE_GAS           ( 0x23u )
#define SYS_REMOTE_SET_BRIDGE               ( 0x30u )
#define SYS_REMOTE_READ_PARAMS              ( 0x40u )
#define SYS_REMOTE_WIPE_SENSOR              ( 0x50u )
#define SYS_REMOTE_SET_IR_GAIN              ( 0x60u )
#define SYS_REMOTE_LED_TEST                 ( 0x77u )
#define SYS_REMOTE_CAL_420_4                ( 0x80u )
#define SYS_REMOTE_CAL_420_20               ( 0x81u )
#define SYS_REMOTE_CAL_420_SAVE             ( 0x82u )
#define SYS_REMOTE_DISABLE_I2C              ( 0xF0u )
#define SYS_REMOTE_ENABLE_I2C               ( 0xF1u )
#define SYS_REMOTE_CAL_FLT_RESET            ( 0xF9u )
#define SYS_REMOTE_WDT_RESET                ( 0xFAu )
#define SYS_REMOTE_CAN_UPDATE               ( 0xFCu )




/* System Cal Status' */
#define SYS_CAL_CLEAR_ALL_MASK              ( 0x00u )
#define SYS_CAL_AS_FOUND_MASK               ( 0x01u )
#define SYS_CAL_ADJUSTED_MASK               ( 0x02u )
#define SYS_CAL_SPEED_MASK                  ( 0x04u )
#define SYS_CAL_STABILITY_MASK              ( 0x08u )
#define SYS_CAL_LIFE_MASK                   ( 0x10u )
#define SYS_CAL_CLEARING_MASK               ( 0x20u )
#define SYS_CAL_COMPLETE_MASK               ( 0x40u )
#define SYS_CAL_CLEAR_7LSBS                 ( 0x80u )
#define SYS_CAL_DELAY                       ( 3u )


/* System Cal Steps */
#define SYS_CAL_STEP_START                  ( 100u )
#define SYS_CAL_STEP_AS_FOUND               ( 110u )
#define SYS_CAL_STEP_ADJUSTED               ( 120u )
#define SYS_CAL_STEP_SPEED                  ( 130u )
#define SYS_CAL_STEP_STABILITY              ( 140u )
#define SYS_CAL_STEP_LIFE                   ( 150u )
#define SYS_CAL_STEP_CLEARING               ( 160u )

#define STATUS_CAL_FAULT                    ( 0x02 )


void handleBle(void){
      uint8_t sysrem; 
      static uint8_t sysrem_prev;  
      Sensor.Reading = iReading;
      static uint8_t sensorlife;
      static uint8_t speed;
      static uint8_t stab_check;
      static uint16 stab_cnt;
      static uint16 delay_sim;
     static uint8_t zerodelay;
     static uint8_t span_complete_delay;
     static uint8_t remove_gas_delay;
     static uint8_t remove_gas;
     static uint8_t as_found_delay;
     static uint8_t sensor_life_delay;
      
    sysrem= sensor[0][CALSTATUSLSB]; // ==========================
    
    
    if(speed){
        static uint8_t sec;
            speed =0;
            System.CalibrationStatus |= SYS_CAL_SPEED_MASK;
            System.CalStep = SYS_CAL_STEP_SPEED;
            updateOurBleData();
    }
    
    if(sensorlife){
        static uint8_t sec;
        sec++; 
        //if(sec>12){// simulating 3 seconds as in SA-100 (12 x 250ms)
        if(sec>0){
            sec=0;
            sensorlife=0;
            Sensor.Life = sensor[0][SENSORLIFELSB];
            System.CalibrationStatus |= SYS_CAL_LIFE_MASK;
            System.CalStep = SYS_CAL_STEP_LIFE;
            updateOurBleData();
            delay_sim=1;
        }
    }
    
    
    //simultating delay
    if(delay_sim){
        static uint8_t cnt;
        if(cnt<6){
            cnt++;
        }else{
            cnt=0;
            delay_sim=0;
        }
    }
    
    
    if(zerodelay){
        static uint8_t cnt;
        if(cnt<120){
            cnt++;
        }else{
            if(sysrem==0x12){
                cnt=0;
                System.BleControl = SYS_REMOTE_IDLE;
                System.CalZeroError = FALSE;
                updateOurBleData();
                zerodelay=0;
            }
        }
    }
    
    
    if(as_found_delay){
        static uint8_t cnt;
        if(cnt<4){
            cnt++;
        }else{
            cnt=0;
            as_found_delay=0;
            System.CalibrationStatus |= SYS_CAL_AS_FOUND_MASK;
            System.CalStep = SYS_CAL_STEP_AS_FOUND;
            updateOurBleData();
        }
    }
                
    if(sensor_life_delay){
        static uint8_t cnt;
        if(cnt<8){
            cnt++;
        }else{
            cnt=0;
            sensor_life_delay=0;
            Sensor.Life = sensor[0][SENSORLIFELSB];
            System.CalStep = SYS_CAL_STEP_LIFE;
            System.CalibrationStatus |=SYS_CAL_LIFE_MASK;
            updateOurBleData();
            span_complete_delay=1;
        }
    }
    
    if(span_complete_delay){
        static uint8_t cnt;
        if(cnt<8){
            cnt++;
        }else{
            span_complete_delay=0;
            cnt=0;
           
            System.CalSpanError=FALSE;
            System.CalibrationStatus |= SYS_CAL_COMPLETE_MASK;
           
            System.BleControl = SYS_REMOTE_IDLE;
            updateOurBleData();
            
        }
    }
    
    
    if(remove_gas){
        static uint16_t cnt;
        if(cnt<2){
            cnt++;
        }else{
            cnt=0;
            remove_gas=0;
            System.BleControl = SYS_REMOTE_CAL_REMOVE_GAS;
            updateOurBleData();
        }
    
    
    }
    
    if(sysrem_prev != sysrem){
        switch(sysrem){  // temporary  re-assgnmnet  ===============================================================
            case 0x10:
                System.BleControl = SYS_REMOTE_CAL_ZERO;
                System.Status=0;
                updateOurBleData();
                zerodelay=1;
                sysrem_prev = sysrem;
            break;
            case 0x12:
                System.CalibrationStatus |= SYS_CAL_COMPLETE_MASK;
                updateOurBleData();
                sysrem_prev = sysrem;
            break;
            case 0x13:
                System.CalZeroError = TRUE;
                updateOurBleData();
                sysrem_prev = sysrem;
            break;
            
//            case 0xF2:
//                System.CalibrationStatus |= SYS_CAL_COMPLETE_MASK;
//                updateOurBleData();
//                sysrem_prev = sysrem;
//            break;

            case 0x0:
                System.BleControl = SYS_REMOTE_IDLE;
                sysrem_prev = sysrem;
            break;
            case 0x20: // SPAN IN PROGRESS
                System.BleControl = SYS_REMOTE_CAL_SPAN;
                System.CalStep = SYS_CAL_STEP_START;
                System.CalSpeed = 0;
                System.CalAsFound = 0;
                System.CalAdjusted = 0;
                System.CalCounts = 0;
                System.CalClearing = 0;
                System.CalSpanError = FALSE;
                System.CalibrationStatus &= SYS_CAL_CLEAR_ALL_MASK;
                System.Status = 0;
                Sensor.Life=0;
                System.AlertStatus=0;
                System.AlertBits =0;
                Sensor.CalResult = 0 ; 
                sensorlife=0;
                speed=0;
                stab_check=0;
                stab_cnt=0;
                delay_sim=0;
                span_complete_delay=0;
                sensor_life_delay=0;
                updateOurBleData();
                sysrem_prev = sysrem;    
            break; 
            
            case 0x21: // NO GAS APPLIED (FAULT)
                System.BleControl = SYS_REMOTE_IDLE;
                System.Status = STATUS_CAL_FAULT;
                System.CalSpanError=TRUE;
                Sensor.CalResult = STATUS_CAL_FAULT ; 
                updateOurBleData();
                sysrem_prev = sysrem;
            break;
            case 0x22: // GAS DETECTED
                System.BleControl = SYS_REMOTE_CAL_WORKING;
                updateOurBleData();
                sysrem_prev = sysrem;
            break;
            case 0x24: // AS FOUND READING SAVED
                as_found_delay =1;
                sysrem_prev = sysrem;
            break;  
            case 0x26: // CAL ADJUSTED
                System.CalAdjusted = (int8)iReading;
                System.CalibrationStatus |= SYS_CAL_ADJUSTED_MASK;
                System.CalStep = SYS_CAL_STEP_ADJUSTED;
                updateOurBleData();
                sysrem_prev = sysrem;
                speed=1;
            break;  
            case 0x28: //STABILITY CHECK STARTED
                System.BleControl = SYS_REMOTE_CAL_STABILITY;  
                updateOurBleData();
                sysrem_prev = sysrem;
            break;
                
            case 0x29: // STABILITY CHECK FAILED (FAULT)
                System.BleControl = SYS_REMOTE_IDLE;
                System.CalSpanError=TRUE;
                System.Status = STATUS_CAL_FAULT;
                updateOurBleData();
                sysrem_prev = sysrem;
            break;    
            case 0x2A: // REMOVE GAS
                System.CalClearing=10;//100;
                System.CalibrationStatus |=SYS_CAL_STABILITY_MASK;
                System.CalStep = SYS_CAL_STEP_STABILITY;
                remove_gas=1;
                updateOurBleData();
                sysrem_prev = sysrem;
            break;
            case 0x2C: // SPAN COMPLETE
                System.CalibrationStatus |= SYS_CAL_CLEARING_MASK;
                System.CalStep = SYS_CAL_STEP_CLEARING;
                sensor_life_delay=1;
                Sensor.Life = sensor[0][SENSORLIFELSB];
                updateOurBleData();
                sysrem_prev = sysrem;
                
            break;
            case 0x2D: // CLEARING FAILED(FAULT)
                System.CalSpanError=TRUE;
                updateOurBleData();
                sysrem_prev = sysrem;
            break;    
        }
    }
    
    
}


void SaveZeroCalDate(uint8_t ch ){
    uint16_t encoded_date = encodeCurrentDate();
    runtime_UpdateSensor(ch , (ZEROCALDATEMSB-4), encoded_date);
    runtime_CtrlSensor(ch , SENSOR_SYS_REMOTE_REG, RUNTIME_SMART_SENSOR_SYS_REMOTE_SAVE_TO_EEPROM);
#ifndef SINGLE_SENSOR_SYSTEM                   
    sprintf(log_txt,"%02d/%02d/%02d at %02d:%02d CH%d ZERO CAL",date_time.month, date_time.day, date_time.year, date_time.hours, date_time.minutes, ch+1);
#else
    sprintf(log_txt,"%02d/%02d/%02d at %02d:%02d ZERO CAL",date_time.month, date_time.day, date_time.year, date_time.hours, date_time.minutes);
#endif
    SaveLogEntry(&log_last_entry_idx, log_txt);


}

void SaveSpanCalDate(uint8_t ch ){
   uint16_t encoded_date = encodeCurrentDate();
   runtime_UpdateSensor(ch , SPANCALDATEMSB-4, encoded_date);
   runtime_CtrlSensor(ch , SENSOR_SYS_REMOTE_REG, RUNTIME_SMART_SENSOR_SYS_REMOTE_SAVE_TO_EEPROM);
#ifndef SINGLE_SENSOR_SYSTEM                        
   sprintf(log_txt,"%02d/%02d/%02d at %02d:%02d CH%d SPAN CAL",date_time.month, date_time.day, date_time.year, date_time.hours, date_time.minutes, ch+1);
#else
   sprintf(log_txt,"%02d/%02d/%02d at %02d:%02d SPAN CAL",date_time.month, date_time.day, date_time.year, date_time.hours, date_time.minutes);
#endif    
   SaveLogEntry(&log_last_entry_idx, log_txt);


}

void ReloadSensorGasType(uint8_t ch){
    static char str[10];
    static char units[5];
            
            memset(str , 0 , sizeof(str));
            remove_trailing_spaces(str, (char*) &sensor[ch][COMPAUND1]);
            if(ch==0)
                Nextion_SetText(DISP_OBJ_GAS_TYPE1,str);
            else
                Nextion_SetText(DISP_OBJ_GAS_TYPE2,str);
            
            memset(str , 0 , sizeof(str));
            strncpy(units, (char *)&sensor[ch][UNIT1], 4);
            units[4] = '\0';
            remove_leading_spaces((char *)units);
            remove_trailing_spaces(str, (char*) units);
            
            if(ch==0)
                Nextion_SetText(DISP_OBJ_ENG_UNIT1,str);
            else
                Nextion_SetText(DISP_OBJ_ENG_UNIT2,str);
}




/* [] END OF FILE */
