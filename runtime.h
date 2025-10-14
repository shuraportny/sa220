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
#include "led.h"


#define DEEPSLEEP


#define DEV_TYPE                220
#define FIRMWARE_VISION         "01.00.00"
#define HARDWARE_VISION         "R1"


// RUNTIME MODES DEFINITIONS
#define RUNTIME_POWERUP_MODE        0
#define RUNTIME_REALTIME_MODE1      1
#define RUNTIME_REALTIME_MODE2      2
#define RUNTIME_ZEROCAL_MODE1       3
#define RUNTIME_SPANCAL_MODE1       4
#define RUNTIME_RELAYCONF_MODE      5
#define RUNTIME_ALARMCONF_MODE      6
#define RUNTIME_DATETIME_MODE       7
#define RUNTIME_POLLRATE_MODE       8
#define RUNTIME_SLEEPCONF_MODE      9

#define RUNTIME_ZEROCAL_MODE2       10
#define RUNTIME_SPANCAL_MODE2       11
#define RUNTIME_SETTINGS_MODE       12
#define RUNTIME_MODBUS_MODE         13
#define RUNTIME_GPS_MODE            14
#define RUNTIME_LOG_MODE            15
#define RUNTIME_INFO_MODE           16


#define SENSOR_READINGS_DATA        0
#define SENSOR_CAL_DATA             10
#define SENSOR_STATIC_DATA          20
#define SENSOR_CFG_DATA             50
#define SENSOR_DEBUG_DATA           60

#define SENSOR_READINGS_LENGTH      10
#define SENSOR_CAL_DATA_LENGTH      10
#define SENSOR_STATIC__DATA_LENGTH  20
#define SENSOR_CFG_DATA_LENGTH      10
#define SENSOR_DEBUG_DATA_LENGTH    10


#define POLL_SENSOR_STATUS_OK       0
#define POLL_SENSOR_STATUS_ABSENT   0x08
#define POLL_SENSOR_STATUS_WARMUP   0x40


#define SYSREADING_OK               0
#define SYSREADING_ALARM1           1
#define SYSREADING_ALARM2           2
#define SYSREADING_ALARM3           3
#define SYSREADING_FAULT            4
#define CAL_FAULT                  5

#define SYSREADING_420FAULT         5
#define SYSREADING_OVERRANGE        6
#define SYSREADING_UNDERRANGE       7


#define POLL_SENSOR_PERIOD_MS       250 
#define POLL_SENSOR_PERIOD_S        2 

#define LOG_RECORDS_NUM             10 

// Modbus definitions
#define MODBUS_MY_DEFAULT_ADDRESS   1
#define MODBUS_MAXIMAL_ADDRESS      240//247
#define MODBUS_BROADCAST_ADDRESS    254

// Admin registers 
#define MODBUS_ADDRESS_REG          2
#define MODBUS_SLEEP_PERIOD_REG     3
#define MODBUS_SYS_REMOTE_CTRL_REG  4
#define MODBUS_LATITUDE_REG         5  
#define MODBUS_LONGITUDE_REG        7
#define MODBUS_MONTH_REG            10
#define MODBUS_DAY_REG              11
#define MODBUS_YEAR_REG             12
#define MODBUS_HOURS_REG            13
#define MODBUS_MINUTES_REG          14

#define MODBUS_UNIX_TIME_REG        10


//#define MODBUS_DEBUG_REG            103
//#define MODBUS_FM_VERSION_REG       104
//#define MODBUS_I2C_PERIOD_REG       11


#define MODBUS_SYS_REMOTE_REG       11
#define MODBUS_ZERO_CAL_DATE_REG    13
#define MODBUS_SPAN_CAL_DATE_REG    14

#define MODBUS_ALARM1_REG           36
#define MODBUS_ALARM1_ASC_REG       37
#define MODBUS_ALARM2_REG           38
#define MODBUS_ALARM2_ASC_REG       39
#define MODBUS_ALARM3_REG           40
#define MODBUS_ALARM3_ASC_REG       41
#define MODBUS_CAL_LEVEL_REG        43
#define MODBUS_RESP_FACTOR_REG      44



//I2C definitions
#define MINIMAL_DEFAULT_I2C_PERIOD   250
#define MAXIMAL_DEFAULT_I2C_PERIOD   1000

//Sleep Mode definitions
#define MINIMAL_SLEEP_PERIOD        10000
#define MAXIMAL_SLEEP_PERIOD        60000//300000 // 5 minutes for now
#define DEFAULT_SLEEP_PERIOD        20000

// Channel 2 Activity DEFAULT
#define CHANNEL2_ACTIVE_DEFAULT     0


#define  SW_HOLD_PERIOD             5000//3000
#define  SW_HOLD_PERIOD_SETTINGS    1000



#define BUFFER_SIZE 100
 


#define RS232_RX_BUFFER_SIZE 8

#define TASK_PERIOD_RTC_MS     1000//15000
#define TASK_PERIOD_RELAY_MS   1000
#define TASK_PERIOD_LED_MS     50
#define TASK_PERIOD_SENSOR_MS  250
#define TASK_PERIOD_DISPLAY_MS  500

/* WDT counter time configuration.
 * The time is based on LFCLK (32768hz).
 * Time is 7.999 seconds ((7FFF x 8) / 32768)
*/
#define WDT_COUNT0_MATCH                    ( 0x7FFFu )         /* 1 second @ 32768hz */
#define WDT_COUNT1_MATCH                    ( 0x000Au )         /* 8 seconds ( 8 * 32768hz ) */
#define WDT_INIT                            SysWdtInit( );    /* Initialize the WDT */
#define WDT_RESET                           SysWdtReset( );   /* Reset the WDT */

#define INACTIVE_USER_TIMEUOT   120


// HELPERS
#define ARRtoINT32(arr,idx) (uint32_t)(arr[idx]<<24)+(uint32_t)(arr[idx+1]<<16)+(uint32_t)(arr[idx+2]<<8)+(uint32_t)arr[idx+3]
#define ARRtoINT16(arr,idx) (uint32_t)(arr[idx]<<8)+(uint32_t)arr[idx+1]


/* End WDT defines */
void SysWdtInit( void );
void SysWdtReset( void );
CY_ISR_PROTO( SysWdtIsrHandler );


extern uint8_t mySlaveAddr,msa;






// RUNTIME FUNCTIONS
void runtimePowerUp(void);
void runtimeRealTime(void);
void runtimeRealTimeCheckSwitches(void);
void runtimeZeroCal(uint8_t ch);
void runtimeSpanCal(uint8_t ch); 
void runtimeSesttings(void);
void runtimeRelayConfig(void);
void runtimeAlarmConfig(void);
void runtimeDateTime(void);
void runtimePollRate(void);
void runtimeSleepTime(void);
void runtimeModbusAddress(void);
void runtimeGps(void);
void runtimeLog(void);
void runtimeInfo(void);


void Test_Increment(uint8 reg , uint8 bytes_to_send);

void InitSystem(void);
int32_t LoadSensorInfo(uint8_t ch);
void HandleSensorData(uint8_t ch);
uint8_t CheckAlarms(uint8_t ch);
void CheckEF();
void SetStatus(uint8_t ch, uint8_t status);



void runtime_PollSensor(void);
void runtime_CtrlSensor(uint8_t ch, uint8_t reg , uint16_t data);
void runtime_UpdateSensor(uint8_t ch , uint8_t reg, uint16 data);

void RecalculateAlarms(uint8_t ch);

void runtime_ProcessRequest(void);
void runtime_RS232ProcessRequest(void);
void runtime_GetMySlaveAddress(void);

void GoToSleep(void);

void BLE_Start(void);
void ResetBLETimer(void);
void SetSleepPeriod(uint32_t period);



void isBLEDisconneted(void);
void updateData(void);
void updateOurBleData(void);
void Stack_Handler(uint32 eventCode, void *eventParam);

void remove_leading_spaces(char *str);
void remove_trailing_spaces(char* dst, char *str);

void setMode(uint8_t mode);


void Check_Inactivity(void);
void Restart_Inactivity_timer(void);
void Reset_Inactivity_timer(void);
void ResetSWTimer(void);

void GoRealtime(uint8_t num);
void SetRealTimePage(uint8_t num);

void DisplayRealTime(uint8_t );
void ShowCalculatedWarmupTime(uint8_t ch);
void AbortWarmup(uint8_t ch);

uint16_t encodeCurrentDate(void);
void decodeLastCalDate(uint8_t ch);

void UpdateWholeScreen();


void HandleSensors(uint8_t channels);
void HandleSensor1(uint8_t ch2Active, uint8_t forced);
void HandleSensor2(uint8_t forced);
//void HandleSensor2of2(void);


void SetLED1(void);
void SetLED2(void);

void Set_relay_Alarm1();
void Set_relay_Alarm2();
void Set_relay_Alarm3();
void Set_relay_Fault();

void Reset_relay_Alarm1();
void Reset_relay_Alarm2();
void Reset_relay_Alarm3();
void Reset_relay_Fault();

void Ack_realys();
void Reset_relays();

void HandleRelays();
void Handle_Icons();

void SaveZeroCalDate(uint8_t ch);
void SaveSpanCalDate(uint8_t ch);
void ReloadSensorGasType(uint8_t ch);

void handleBle();



/* [] END OF FILE */
