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
#include "display.h"
#include "sensor.h"
#include <math.h>

char settings_obj[DISP_OBJ_SETTINGS_NUM][10]={DISP_OBJ_RELAY_CONF,DISP_OBJ_ALARM_CONF,DISP_OBJ_DATE_TIME,
                                              DISP_OBJ_SLEEP_CONF,DISP_OBJ_MODBUS_ADDR, DISP_OBJ_GPS_CONF, DISP_OBJ_LOG, DISP_OBJ_INFO};
char relay_obj[DISP_OBJ_RLY_NUM][10]={DISP_OBJ_RLY_ENRGD1,DISP_OBJ_RLY_LATCH1,DISP_OBJ_RLY_ACK1,
                        DISP_OBJ_RLY_ENRGD2,DISP_OBJ_RLY_LATCH2,DISP_OBJ_RLY_ACK2,
                        DISP_OBJ_RLY_ENRGD3,DISP_OBJ_RLY_LATCH3,DISP_OBJ_RLY_ACK3,
                        DISP_OBJ_RLY_ENRGD4,DISP_OBJ_RLY_LATCH4,DISP_OBJ_RLY_ACK4};
char alarm_obj[DISP_OBJ_ALARM_NUM][10]={ DISP_OBJ_CH,DISP_OBJ_AL1,
                                         DISP_OBJ_AL1_ASC,DISP_OBJ_AL2,
                                         DISP_OBJ_AL2_ASC,DISP_OBJ_AL3,
                                         DISP_OBJ_AL3_ASC, DISP_CAL_LEVEL};
char date_time_obj[DISP_OBJ_DATETIME_NUM][10]={DISP_OBJ_DAY,DISP_OBJ_MONTH,DISP_OBJ_YEAR,DISP_OBJ_HOUR,DISP_OBJ_MINUTE};
char modbus_obj[DISP_OBJ_MODBUS_NUM][10]={DISP_OBJ_MODBUS_HUNDREDS, DISP_OBJ_MODBUS_TENS,DISP_OBJ_MODBUS_ONES};

char gps_obj[DISP_OBJ_GPS_NUM][10]={DISP_OBJ_GPS_LAT_SIGN,
                                    DISP_OBJ_GPS_LAT_1,DISP_OBJ_GPS_LAT_2,DISP_OBJ_GPS_LAT_3,DISP_OBJ_GPS_LAT_4,DISP_OBJ_GPS_LAT_5,
                                    DISP_OBJ_GPS_LAT_6,DISP_OBJ_GPS_LAT_7,DISP_OBJ_GPS_LAT_8,
                                    DISP_OBJ_GPS_LON_SIGN,
                                    DISP_OBJ_GPS_LON_1, DISP_OBJ_GPS_LON_2,DISP_OBJ_GPS_LON_3,DISP_OBJ_GPS_LON_4,DISP_OBJ_GPS_LON_5,  
                                    DISP_OBJ_GPS_LON_6, DISP_OBJ_GPS_LON_7,DISP_OBJ_GPS_LON_8,DISP_OBJ_GPS_LON_9
                                    };
char pollrate_obj[1][10] = {DISP_OBJ_POLLRATE_VALUE};

char modbusaddr_obj[3][10]= {DISP_OBJ_MODBUS_ADDR_100, DISP_OBJ_MODBUS_ADDR_10, DISP_OBJ_MODBUS_ADDR_1};

char status_obj[2][10]={DISP_OBJ_STATUS1,DISP_OBJ_STATUS2};
char warmup_obj[2][10]={DISP_OBJ_WARMUP1,DISP_OBJ_WARMUP2 };
char retry_obj[2][10]={DISP_OBJ_SPAN_CAL_RETRY_BTN1, DISP_OBJ_SPAN_CAL_RETRY_BTN2};
char span_reset_obj[10] ={DISP_OBJ_SPAN_CAL_RESET_BTN};

char log_obj[DISP__OBJ_EVENT_CNT][10] = {DISP__OBJ_EVENT1, DISP__OBJ_EVENT2,DISP__OBJ_EVENT3,DISP__OBJ_EVENT4, DISP__OBJ_EVENT5, 
                                         DISP__OBJ_EVENT6, DISP__OBJ_EVENT7,DISP__OBJ_EVENT8,DISP__OBJ_EVENT9, DISP__OBJ_EVENT10, 
                                        }; 

char display_obj_text[2][10]={DISP_OBJ_TEXT1,DISP_OBJ_TEXT2};
char display_obj_status[2][10]={DISP_OBJ_STATUS1,DISP_OBJ_STATUS2};

Point circlePoints[NUM_POINTS];
Point circlePoints2[NUM_POINTS];
    const double radius = 63;
    const int16_t cx = 11; //po x =15  140x140
    const int16_t cy = 48; //po y= 51

    const int16_t cx2 = 162; //p1 x =  166  140x140
    const int16_t cy2 = 48; // p1 y= 51



uint16_t RGB565(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0xF8) << 8) |   // Top 5 bits of red
           ((g & 0xFC) << 3) |   // Top 6 bits of green
           (b >> 3);             // Top 5 bits of blue
}



// Decode function
RGB888 decodeRGB565(uint16_t color) {
    RGB888 rgb;

    rgb.r = ((color >> 11) & 0x1F) * 255 / 31;
    rgb.g = ((color >> 5) & 0x3F) * 255 / 63;
    rgb.b = (color & 0x1F) * 255 / 31;

    return rgb;
}



//void Nextion_InitNeedleCoordinates(void) {
//   
//    double centerx = cx+radius;
//    double centery = cy+radius;
//    
//    double centerx2 = cx2+radius;
//    double centery2 = cy2+radius;
//    
//    
//    for (int p = 0; p <= 100; ++p) {
//        double angle_deg = -40.0 + p * 2.6;               // -20° to 200°
//        double angle_rad = angle_deg * (M_PI / 180.0);   // NEGATE for clockwise
//
//        double x = centerx - radius * cos(angle_rad);  // <-- Subtract for clockwise
//        double y = centery - radius * sin(angle_rad);  // Y stays as before (clockwise upper arc)
//        
//        circlePoints[p].x = (int16_t)(x + 0.5);
//        circlePoints[p].y = (int16_t)(y + 0.5);
//        
//        double x2 = centerx2 - radius * cos(angle_rad);  // <-- Subtract for clockwise
//        double y2 = centery2 - radius * sin(angle_rad);  // Y stays as before (clockwise upper arc)
//        
//        
//        circlePoints2[p].x = (int16_t)(x2 + 0.5);
//        circlePoints2[p].y = (int16_t)(y2 + 0.5);
//    }
//}

void Nextion_SendEndOfCommand(){
    UART2_PutChar(0xFF);
    UART2_PutChar(0xFF);
    UART2_PutChar(0xFF);
}

void Nextion_DisplayPicture(uint16_t x , uint16_t y , uint8_t id ){
    char buf[50];
    memset(buf, 0 , sizeof(buf));
    uint32 len = sprintf(buf, "pic %d,%d,%d",x,y,id) ;
    UART2_PutString(buf);
    Nextion_SendEndOfCommand();
}


void Nextion_Crop( ){
    char buf[50];
    memset(buf, 0 , sizeof(buf));
    uint32 len = sprintf(buf, "xpic %d,%d,%d,%d,%d,%d,%d",80,100,50,50,15,15,0) ;
    UART2_PutString(buf);
    Nextion_SendEndOfCommand();
}

void Nextion_ClearScreen(){
    char buf[10];
    memset(buf, 0 , sizeof(buf));
    uint32 len = sprintf(buf, "cls %d",DISP_BLACK) ;
    UART2_PutString(buf);
    Nextion_SendEndOfCommand();
}
//
void Nextion_SetPage(char* id){
    char buf[50];
    memset(buf, 0 , sizeof(buf));
    uint32 len = sprintf(buf, "page %s",id) ;
    UART2_PutString(buf);
    Nextion_SendEndOfCommand();
}

void Nextion_ObjVisible(char* obj, uint8_t val){
    char buf[50];
    if(val)val=1;
    memset(buf, 0 , sizeof(buf));
    uint32 len = sprintf(buf, "vis %s,%d",obj,val) ;
    UART2_PutString(buf);
    Nextion_SendEndOfCommand();
}

void Nextion_ObjVisibleIncognito(char* page, char* obj, uint8_t val){
    char buf[50];
    if(val)val=1;
    memset(buf, 0 , sizeof(buf));
    uint32 len = sprintf(buf, "%s %s,%d",page, obj,val) ;
    UART2_PutString(buf);
    Nextion_SendEndOfCommand();
}


void Nextion_Refresh_Object(char* obj){
    char buf[50];
    memset(buf, 0 , sizeof(buf));
    uint32 len = sprintf(buf, "ref %s", obj) ;
    UART2_PutString(buf);
    Nextion_SendEndOfCommand();
}

void Nextion_SetObjAttr(char* obj,char* attr, uint32_t val){
    char buf[50];
    CyDelay(10);
    memset(buf, 0 , sizeof(buf));
    sprintf(buf, "%s.%s=%d",obj,attr, val) ;
    UART2_PutString(buf);
    Nextion_SendEndOfCommand();

}

void Nextion_SetTextIncognito(char* page, char* obj, char* text){
    char buf[50];
    memset(buf, 0 , sizeof(buf));
    sprintf(buf, "%s.%s.txt=\"%s\"",page ,obj, text) ;
    UART2_PutString(buf);
    Nextion_SendEndOfCommand();

}


void Nextion_Sleep(void){
    char buf[50];
    
    memset(buf, 0 , sizeof(buf));
    uint32 len = sprintf(buf, "lowpower=0") ;
    UART2_PutString(buf);
    Nextion_SendEndOfCommand();
    CyDelay(10);
    
    
    memset(buf, 0 , sizeof(buf));
    sprintf(buf, "sleep=1") ;
    UART2_PutString(buf);
    Nextion_SendEndOfCommand();
}


void Nextion_DeepSleep(void){
    char buf[50];
    memset(buf, 0 , sizeof(buf));
    uint32 len = sprintf(buf, "lowpower=1") ;
    UART2_PutString(buf);
    Nextion_SendEndOfCommand();
    CyDelay(10);
    
    memset(buf, 0 , sizeof(buf));
    sprintf(buf, "sleep=1") ;
    UART2_PutString(buf);
    Nextion_SendEndOfCommand();
}



void Nextion_WakeUp(void){
    char buf[50];
    memset(buf, 0 , sizeof(buf));
    uint32 len = sprintf(buf, "sleep=0") ;
    UART2_PutString(buf);
    Nextion_SendEndOfCommand();
}


void Nextion_DeepSleepWakeUp(void){
    char buf[50];
    memset(buf, 0 , sizeof(buf));
    uint32 len = sprintf(buf, "lowpower=0") ;
    UART2_PutString(buf);
    Nextion_SendEndOfCommand();
    
}

//
void Nextion_SendFloat(char* obj, int32 val,  uint8 res){
    char buf[50];
    memset(buf, 0 , sizeof(buf));
    sprintf(buf, "%s.vvs1=%d",obj,res) ;
    UART2_PutString(buf);
    Nextion_SendEndOfCommand();
    memset(buf, 0 , sizeof(buf));
    sprintf(buf, "%s.val=%ld",obj,val) ;
    UART2_PutString(buf);
    Nextion_SendEndOfCommand();
}

void Nextion_SendInt(char* obj, uint32_t val){
    char buf[50];
    memset(buf, 0 , sizeof(buf));
    sprintf(buf, "%s.val=%u",obj,val) ;
    UART2_PutString(buf);
    Nextion_SendEndOfCommand();
}

void Nextion_SetText(char* obj,char* txt){
    char buf[50];
    memset(buf, 0 , sizeof(buf));
    sprintf(buf, "%s.txt=\"%s\"",obj,txt) ;
    UART2_PutString(buf);
    Nextion_SendEndOfCommand();

}

void Nextion_SetTextMultiline(char* obj,char* txt){
    char buf[50];
    memset(buf, 0 , sizeof(buf));
    sprintf(buf, "%s.txt=\"%s\"",obj,txt) ;
    UART2_PutString(buf);
    Nextion_SendEndOfCommand();

}


void Nextion_SetButton(char* obj,uint8_t val){
    char buf[50];
    memset(buf, 0 , sizeof(buf));
    sprintf(buf, "%s.val=%d",obj,val) ;
    UART2_PutString(buf);
    Nextion_SendEndOfCommand();

}

void Nextion_SetColor(char* obj,uint32_t val){
    char buf[50];
    memset(buf, 0 , sizeof(buf));
    sprintf(buf, "%s.bco=%d",obj,val) ;
    UART2_PutString(buf);
    Nextion_SendEndOfCommand();

}

void Nextion_SetTextColor(char* obj,uint32_t val){
    char buf[50];
    memset(buf, 0 , sizeof(buf));
    sprintf(buf, "%s.pco=%d",obj,val) ;
    UART2_PutString(buf);
    Nextion_SendEndOfCommand();

}

void Nextion_SetObjectAttr(char* obj, char* attr, uint32_t val){
    char buf[50];
    memset(buf, 0 , sizeof(buf));
    sprintf(buf, "%s.%s=%d",obj,attr, val) ;
    UART2_PutString(buf);
    Nextion_SendEndOfCommand();

}


void Display_Date(char* obj, uint8_t dd,uint8_t mm, uint8_t yy ){
    char buf[50];
    memset(buf, 0 , sizeof(buf));
    sprintf(buf, "%s.txt=\"%02d/%02d/%02d\"",obj,dd,mm,yy) ;
    UART2_PutString(buf);
    Nextion_SendEndOfCommand();
}

void Display_Time(char* obj, uint8_t hh,uint8_t mm ){
    char buf[50];
    memset(buf, 0 , sizeof(buf));
    sprintf(buf, "%s.txt=\"%02d:%02d\"",obj,hh,mm) ;
    UART2_PutString(buf);
    Nextion_SendEndOfCommand();
}

void Display_Select_Object(char* obj, uint8_t sel){
    char buf[50];
    uint32_t color = DISP_BLACK;
    if(sel)color = DISP_BLUE;
    memset(buf, 0 , sizeof(buf));
    Nextion_SetObjectAttr(obj, "bco", color);
    //sprintf(buf, "%s.bco=%d",obj,color) ;
    //UART2_PutString(buf);
    //Nextion_SendEndOfCommand();
}



void DisplayCommTest(){
    for(;;){
        CyDelay(500);
        Nextion_SetPage("realtime1");
        CyDelay(500);
        Nextion_SetPage("realtime2");
        CyDelay(500);
        Nextion_SetPage("realtimeg1");
        CyDelay(5000);
        Nextion_SetPage("settings");
        CyDelay(500);
        Nextion_SetPage("zerocal_sel");
        CyDelay(500);
        Nextion_SetPage("spancal_sel");
        CyDelay(500);
        Nextion_SetPage("zero_cal");
        CyDelay(500);
        Nextion_SetPage("span_cal");
        CyDelay(500);
        Nextion_SetPage("relay_config");
        CyDelay(500);
        Nextion_SetPage("alarm_config");
        CyDelay(500);
        Nextion_SetPage("sleep_time");
        CyDelay(500);
        Nextion_SetPage("poll_rate");
        CyDelay(500);
        Nextion_SetPage("date_time");
        
        CyDelay(3000);
        Nextion_Sleep();
        CyDelay(5000);
        Nextion_WakeUp();
        
        Nextion_SetPage("realtime1");
        for(uint8_t i=0 ;i <10; i++){
            CyDelay(200);
            Nextion_SetButton("bt0", 1);
            CyDelay(200);
            Nextion_SetButton("bt0", 0);
        }
        
        Nextion_SetColor("x0", 0 );
        Nextion_SetTextColor("x0", 65535 );
        uint8_t res=0;
        for(uint8_t i=0 ;i <=100; i++){
            CyDelay(200);
            Nextion_SendFloat("x0", i,  res);
            if(i>30)res=1;
            if(i>60)res=2;
        }
        
        for(uint8_t i=0 ;i <2; i++){
            CyDelay(500);
            Nextion_SetColor("x0",0);//black
            CyDelay(500);
            Nextion_SetColor("x0",65535);//white
            CyDelay(500);
            Nextion_SetColor("x0",2016);//green
            CyDelay(500);
            Nextion_SetColor("x0",1055);//blue
            CyDelay(500);
            Nextion_SetColor("x0",65504);
            CyDelay(500);
            Nextion_SetColor("x0",63488);
            CyDelay(500);
            Nextion_SetColor("x0",0);
            
        }
        
        CyDelay(500);
        Nextion_SetPage("zero_cal");
        CyDelay(500);
        Nextion_SetText("t4", RUNTIME_SMART_SENSOR_ZERO_CAL_INPROGRESS_TXT);
        CyDelay(500);
        Nextion_SetText("t4", RUNTIME_SMART_SENSOR_ZERO_CAL_COMPLETE_TXT);
        CyDelay(500);
        Nextion_SetText("t4", RUNTIME_SMART_SENSOR_ZERO_CAL_FAILED_TXT);
        
        Nextion_SetPage("span_cal");
        CyDelay(500);
        Nextion_SetText("t4", RUNTIME_SMART_SENSOR_SPAN_CAL_INPROGRESS_TXT);
        CyDelay(500);
        Nextion_SetText("t4", RUNTIME_SMART_SENSOR_SPAN_CAL_NOGASAPPLIED_TXT);
        CyDelay(500);
        Nextion_SetText("t4", RUNTIME_SMART_SENSOR_SPAN_CAL_GASDETECTED_TXT);
        CyDelay(500);
        Nextion_SetText("t4", RUNTIME_SMART_SENSOR_SPAN_CAL_AS_FOUND_SAVED_TXT);
        CyDelay(500);
        Nextion_SetText("t4", RUNTIME_SMART_SENSOR_SPAN_CAL_ADJUSTED_TXT);
        CyDelay(500);
        Nextion_SetText("t4", RUNTIME_SMART_SENSOR_SPAN_CAL_STABILITYCHECKSTARTED_TXT);
        CyDelay(500);
        Nextion_SetText("t4", RUNTIME_SMART_SENSOR_SPAN_CAL_STABILITYCHECKFAILED_TXT);
        CyDelay(500);
        Nextion_SetText("t4", RUNTIME_SMART_SENSOR_SPAN_CAL_REMOVEGAS_TXT);
        CyDelay(500);
        Nextion_SetText("t4", RUNTIME_SMART_SENSOR_SPAN_CAL_COMPLETE_TXT);
        CyDelay(500);
        Nextion_SetText("t4", RUNTIME_SMART_SENSOR_SPAN_CAL_CLEARINGFAILED_TXT);
    
    
    
        CyDelay(500);
        Nextion_SetPage("zero_cal");
        
        Nextion_SetPage("alarm_config");
        CyDelay(500);
        
        for(uint8_t a=0;a<DISP_OBJ_ALARM_NUM;a++){
            for(uint8_t i=0;i<DISP_OBJ_ALARM_NUM;i++){
                Display_Select_Object((char*)alarm_obj[i],0);
            }
            Display_Select_Object((char*)alarm_obj[a],1);
            CyDelay(500);    
        }     
        Nextion_SetPage("relay_config");
        CyDelay(500);
        for(uint8_t a=0;a<DISP_OBJ_RLY_NUM;a++){
            for(uint8_t i=0;i<DISP_OBJ_RLY_NUM;i++){
                Display_Select_Object((char*)relay_obj[i],0);
            }
            Display_Select_Object((char*)relay_obj[a],1);
            CyDelay(500);        
        }
    }

}



/* [] END OF FILE */
