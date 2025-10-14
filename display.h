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

/* [] END OF FILE */
#include <project.h>
#include <stdio.h>

// Color Codes
#define DISP_BLACK      0
#define DISP_GREEN      2016
#define DISP_BLUE       1055
#define DISP_PURPLE     53810//33816
#define DISP_GREY       29582//50712
#define DISP_RED        63488
#define DISP_YELLOW     65504
#define DISP_WHITE      65535


#define DISP_ORANGE     65088
#define DISP_GREY_BTN   50712

//

// DISPLAY PAGES
#define PAGE_POWER_UP      "powerup"
#define PAGE_REAL_TIME1      "realtime1"
#define PAGE_REAL_TIME2      "realtime2"
#define PAGE_SETTINGS       "settings"
#define PAGE_ZERO_CAL       "zero_cal"
#define PAGE_SPAN_CAL       "span_cal"
#define PAGE_ZERO_CAL_SEL   "zerocal_sel"
#define PAGE_SPAN_CAL_SEL   "spancal_sel"
#define PAGE_REALY_CONFIG   "relay_config"
#define PAGE_ALARM_CONFIG   "alarm_config"
#define PAGE_DATE_TIME      "date_time"
#define PAGE_POLL_RATE      "poll_rate"
#define PAGE_SLEEP_TIME     "sleep_time"
#define PAGE_MODBUS_ADDR    "modbus"
#define PAGE_GPS            "gps"
#define PAGE_LOG            "log"
#define PAGE_INFO           "info"

// Objects names
// Power Up mode objects
#define DISP_OBJ_FIRMWARE_VERSION  "t1"

//Real time mode objects
#define DISP_OBJ_SYS_READING1    "x0"
#define DISP_OBJ_GAS_TYPE1       "t0"
#define DISP_OBJ_ENG_UNIT1       "t2"
#define DISP_OBJ_STATUS1         "p2"
#define DISP_OBJ_RING1           "p0"
#define DISP_OBJ_WARMUP1         "t8"

#define DISP_OBJ_SYS_READING2    "x1"
#define DISP_OBJ_GAS_TYPE2       "t1"
#define DISP_OBJ_ENG_UNIT2       "t3"
#define DISP_OBJ_STATUS2         "p3"
#define DISP_OBJ_CH1_ACTIVE      "bt1"
#define DISP_OBJ_CH2_ACTIVE      "bt2"
#define DISP_OBJ_BLE_CONNECTED   "p4"
#define DISP_OBJ_RING2           "p1"
#define DISP_OBJ_WARMUP2         "t9"

#define DISP_OBJ_TEXT1           "t10"
#define DISP_OBJ_TEXT2           "t11"

#define DISP_OBJ_EF1         "t4"
#define DISP_OBJ_EF2         "t5"

#define DISP_OBJ_DATE           "t6"
#define DISP_OBJ_TIME           "t7"

//SETTINGS DISPLAY OBJECTS
#define DISP_OBJ_SETTINGS_NUM     8
#define DISP_OBJ_ZERO_CAL         "b0"
#define DISP_OBJ_SPAN_CAL         "b1"
#define DISP_OBJ_RELAY_CONF       "b2"
#define DISP_OBJ_ALARM_CONF       "b3"
#define DISP_OBJ_DATE_TIME        "b4"
#define DISP_OBJ_POLL_RATE        "b5"
#define DISP_OBJ_SLEEP_CONF       "b6"
#define DISP_OBJ_MODBUS_ADDR      "b9"
#define DISP_OBJ_GPS_CONF         "b10"
#define DISP_OBJ_LOG              "b11"
#define DISP_OBJ_INFO             "b12"


// ZERO CAL DISPLAY OBJECTS
#define DISP_OBJ_ZERO_CAL_SYS_READING   "x0"
#define DISP_OBJ_ZERO_CAL_GAS_TYPE      "t0"
#define DISP_OBJ_ZERO_CAL_ENG_UNITS     "t2"
#define DISP_OBJ_ZERO_CAL_STATUS        "t4"


// SPAN CAL DISPLAY OBJECTS
#define DISP_OBJ_SPAN_CAL_SYS_READING   "x0"
#define DISP_OBJ_SPAN_CAL_GAS_TYPE      "t0"
#define DISP_OBJ_SPAN_CAL_ENG_UNITS     "t2"
#define DISP_OBJ_SPAN_CAL_STATUS        "t4"
#define DISP_OBJ_SPAN_CAL_RESET_BTN     "b0"
#define DISP_OBJ_SPAN_CAL_RETRY_BTN1    "b1"
#define DISP_OBJ_SPAN_CAL_RETRY_BTN2    "b2"
#define DISP_OBJ_SPAN_CAL_AS_FOUND      "t3"

//ALARM CONFIG DISPLAY OBJECTS 
#define DISP_OBJ_ALARM_NUM     8
#define DISP_OBJ_CH           "bt1"
#define DISP_OBJ_AL1           "n0"
#define DISP_OBJ_AL1_ASC       "t3"
#define DISP_OBJ_AL2           "n1"
#define DISP_OBJ_AL2_ASC       "t6"
#define DISP_OBJ_AL3           "n2"
#define DISP_OBJ_AL3_ASC       "t9"
#define DISP_CAL_LEVEL         "n3"
//#define DISP_RESP_FACTOR       "n4"

//RELAY CONFIG DISPLAY OBJECTS 
#define DISP_OBJ_RLY_NUM          12 
#define DISP_OBJ_RLY_ENRGD1       "t3"
#define DISP_OBJ_RLY_LATCH1       "t8"
#define DISP_OBJ_RLY_ACK1         "t13"
#define DISP_OBJ_RLY_ENRGD2       "t6"
#define DISP_OBJ_RLY_LATCH2       "t10"
#define DISP_OBJ_RLY_ACK2         "t14"
#define DISP_OBJ_RLY_ENRGD3       "t9"
#define DISP_OBJ_RLY_LATCH3       "t11"
#define DISP_OBJ_RLY_ACK3         "t15"
#define DISP_OBJ_RLY_ENRGD4       "t5"
#define DISP_OBJ_RLY_LATCH4       "t12"
#define DISP_OBJ_RLY_ACK4         "t16"


//DATE&TIME CONFIG DISPLAY OBJECTS 
#define DISP_OBJ_DATETIME_NUM     5 
#define DISP_OBJ_DAY             "n0"
#define DISP_OBJ_MONTH           "n1"
#define DISP_OBJ_YEAR            "n2"
#define DISP_OBJ_HOUR            "n3"
#define DISP_OBJ_MINUTE          "n4"

//POLL RATE DISPLAY OBJECT 
#define DISP_OBJ_POLLRATE_VALUE  "n0"

//SLEEP TIME DISPLAY OBJECT 
#define DISP_OBJ_SLEEP_VALUE  "n0"

//MODBUS_DISPLAY OBJECT 
#define DISP_OBJ_MODBUS_ADDR_100  "n0"
#define DISP_OBJ_MODBUS_ADDR_10  "n1"
#define DISP_OBJ_MODBUS_ADDR_1  "n2"

//MODSBUS DISPLAY OBJECTs 
#define DISP_OBJ_MODBUS_NUM     3
#define DISP_OBJ_MODBUS_HUNDREDS  "n0"
#define DISP_OBJ_MODBUS_TENS      "n1"
#define DISP_OBJ_MODBUS_ONES      "n2"


//GPS DISPLAY OBJECTs 
#define DISP_OBJ_GPS_NUM     19

#define DISP_OBJ_GPS_LAT_SIGN     "t3"
#define DISP_OBJ_GPS_LAT_1        "n0"
#define DISP_OBJ_GPS_LAT_2        "n1"
#define DISP_OBJ_GPS_LAT_3        "n2"
#define DISP_OBJ_GPS_LAT_4        "n3"
#define DISP_OBJ_GPS_LAT_5        "n4"
#define DISP_OBJ_GPS_LAT_6        "n5"
#define DISP_OBJ_GPS_LAT_7        "n6"
#define DISP_OBJ_GPS_LAT_8        "n7"

//GPS DISPLAY OBJECTS
#define DISP_OBJ_GPS_LON_SIGN     "t4"
#define DISP_OBJ_GPS_LON_1        "n8"
#define DISP_OBJ_GPS_LON_2        "n9"
#define DISP_OBJ_GPS_LON_3        "n10"
#define DISP_OBJ_GPS_LON_4        "n11"
#define DISP_OBJ_GPS_LON_5        "n12"
#define DISP_OBJ_GPS_LON_6        "n13"
#define DISP_OBJ_GPS_LON_7        "n14"
#define DISP_OBJ_GPS_LON_8        "n15"
#define DISP_OBJ_GPS_LON_9        "n16"





//LOG DISPLAY OBJECTS
#define DISP__OBJ_EVENT_CNT         10
#define DISP__OBJ_EVENT1            "t1"
#define DISP__OBJ_EVENT2            "t2"
#define DISP__OBJ_EVENT3            "t3"
#define DISP__OBJ_EVENT4            "t4"
#define DISP__OBJ_EVENT5            "t5"
#define DISP__OBJ_EVENT6            "t6"
#define DISP__OBJ_EVENT7            "t7"
#define DISP__OBJ_EVENT8            "t8"
#define DISP__OBJ_EVENT9            "t9"
#define DISP__OBJ_EVENT10           "t10"

//INFO DISPLAY OBJECTS
#define DISP_OBJ_CH1                "t1"
#define DISP_OBJ_CH1_2              "t8"
#define DISP_OBJ_CH1_ZERO_CAL_DATE  "t2"
#define DISP_OBJ_CH1_SPAN_CAL_DATE  "t6"
#define DISP_OBJ_CH2                "t3"
#define DISP_OBJ_CH2_2              "t9"
#define DISP_OBJ_CH2_ZERO_CAL_DATE  "t4"
#define DISP_OBJ_CH2_SPAN_CAL_DATE  "t5"
#define DISP_OBJ_GPS                "t7"

#define ALARM1_ICON   3
#define ALARM2_ICON   16
#define ALARM3_ICON   4
#define FAULT_ICON    5
#define WARMUP_ICON   6
#define OK_ICON       7

#define RING_SINGLE_OK_ICON       8
#define RING_SINGLE_ALARM1_ICON   9
#define RING_SINGLE_ALARM2_ICON   10
#define RING_SINGLE_ALARM3_ICON   10
#define RING_SINGLE_FAULT_ICON    11
#define RING_SINGLE_ZEROCAL_ICON  17
#define RING_SINGLE_SPANCAL_ICON  19

#define RING_DOUBLE_OK_ICON       12
#define RING_DOUBLE_ALARM1_ICON   13
#define RING_DOUBLE_ALARM2_ICON   14
#define RING_DOUBLE_ALARM3_ICON   14
#define RING_DOUBLE_FAULT_ICON    15
#define RING_DOUBLE_ZEROCAL_ICON  18
#define RING_DOUBLE_SPANCAL_ICON  20

#define ZERO_CAL_INITIATED        "INITIATED"
#define ZERO_CAL_IN_PROGRESS      "IN PROGRESS"
#define ZERO_CAL_COMPLETE         "COMPLETE"

typedef struct {
    int16_t x;
    int16_t y;
} Point;
#define NUM_POINTS 102
#define M_PI 3.14


// Define RGB struct
typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} RGB888;


void Nextion_Crop( );

void Nextion_SendEndOfCommand();
void Nextion_DisplayPicture(uint16_t x , uint16_t y , uint8_t id );
void Nextion_ClearScreen();
void Nextion_SetPage(char* id);
void Nextion_SetObjAttr(char* obj,char* attr, uint32_t val);
void Nextion_Sleep(void);
void Nextion_WakeUp(void);

void Nextion_DeepSleep();
void Nextion_DeepSleepWakeUp();

void Nextion_SendFloat(char* obj, int32 val,  uint8_t res);
void Nextion_SendInt(char* obj, uint32_t val);
void Nextion_SetText(char* obj,char* txt);
void Nextion_SetTextMultiline(char* obj,char* txt);
void Nextion_SetButton(char* obj,uint8_t val);
void Nextion_SetColor(char* obj,uint32_t val);
void Nextion_SetTextColor(char* obj,uint32_t val);
void Nextion_SetObjectAttr(char* obj, char* attr, uint32_t val);
void Nextion_SetTextIncognito(char* page, char* obj, char* text);
void Nextion_ObjVisibleIncognito(char* page, char* obj, uint8_t val);

void Display_Date(char* obj, uint8_t dd,uint8_t mm, uint8_t yy );
void Display_Time(char* obj, uint8_t hh,uint8_t mm );
void Display_Select_Object(char* obj, uint8_t sel);
void Nextion_ObjVisible(char* obj, uint8_t val);

void Nextion_Refresh_Object(char* obj);

void DisplayCommTest(void);

void Nextion_InitNeedleCoordinates();
uint16_t RGB565(uint8_t r, uint8_t g, uint8_t b);