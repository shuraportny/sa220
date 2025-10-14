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
#include "rtc.h"


static uint8_t DecToBCD(uint8_t val) {
    return ((val / 10) << 4) | (val % 10);
}

// Helper: Convert BCD to Decimal
static uint8_t BCDToDec(uint8_t val) {
    return (val >> 4) * 10 + (val & 0x0F);
}

void DS3231_EnableDateTime(void ){
    uint8_t data[2];
    data[0] = 0x00; // Start at seconds register
    data[1] = 0x00;
    
    I2C_I2CMasterWriteBuf(I2C_RTC_SLAVE_ADDR, data, sizeof(data), I2C_I2C_MODE_COMPLETE_XFER);
    while (I2C_I2CMasterStatus() & I2C_I2C_MSTAT_XFER_INP);
}

void DS3231_SetDateTime(const S_DateTime* dt) {
    uint8_t data[8];
    
    data[0] = 0x00; // Start at seconds register
    data[1] = DecToBCD(dt->seconds);
    data[2] = DecToBCD(dt->minutes);
    data[3] = DecToBCD(dt->hours);
    data[4] = DecToBCD(dt->dayOfWeek);
    data[5] = DecToBCD(dt->day);
    data[6] = DecToBCD(dt->month);
    data[7] = DecToBCD(dt->year);
   
    
    I2C_I2CMasterWriteBuf(I2C_RTC_SLAVE_ADDR, data, sizeof(data)+1, I2C_I2C_MODE_COMPLETE_XFER);
    while (I2C_I2CMasterStatus() & I2C_I2C_MSTAT_XFER_INP);

}

void DS3231_GetDateTime(S_DateTime* dt) {
    uint8_t reg = 0x00;
    uint8_t data[7];

    // Send memory address
    I2C_I2CMasterWriteBuf(I2C_RTC_SLAVE_ADDR, &reg, 1, I2C_I2C_MODE_COMPLETE_XFER);
    while (I2C_I2CMasterStatus() & I2C_I2C_MSTAT_XFER_INP);

    // Read data
    I2C_I2CMasterReadBuf(I2C_RTC_SLAVE_ADDR, data, 7, I2C_I2C_MODE_COMPLETE_XFER);
    while (I2C_I2CMasterStatus() & I2C_I2C_MSTAT_XFER_INP);
    
    dt->seconds    = BCDToDec(data[0]);
    dt->minutes    = BCDToDec(data[1]);
    dt->hours      = BCDToDec(data[2] & 0x3F);
    dt->dayOfWeek  = BCDToDec(data[3]);
    dt->day        = BCDToDec(data[4]);
    dt->month      = BCDToDec(data[5] & 0x1F); // remove century bit
    dt->year       = BCDToDec(data[6]);
    
    
}


volatile uint32_t write_result = 0;
volatile uint32_t read_result = 0;
uint32 result = 0;

void read_RTC( S_DateTime *rtc) {
    
    uint8 WriteBuf[1] = {0};
    WriteBuf[0] = 0;
    S_DateTime chip_rtc;
   
    
    
    write_result = I2C_I2CMasterWriteBuf(I2C_RTC_SLAVE_ADDR, (uint8 *) WriteBuf, 1, I2C_I2C_MODE_NO_STOP ); // I2CM_I2C_MODE_NO_STOP
    while(((result = I2C_I2CMasterStatus()) & I2C_I2C_MSTAT_WR_CMPLT) == 0) {
        write_result = result;
    }
    
    //CyDelay(10);
    
    read_result = I2C_I2CMasterReadBuf(I2C_RTC_SLAVE_ADDR, (uint8 *) &chip_rtc, sizeof( S_DateTime), I2C_I2C_MODE_REPEAT_START ); // I2CM_I2C_MODE_REPEAT_START
    while(((result = I2C_I2CMasterStatus()) & I2C_I2C_MSTAT_RD_CMPLT) == 0) {};
    
    
    // convert chip date to real rtc info
    rtc->seconds = ((chip_rtc.seconds >> 4) * 10) + (chip_rtc.seconds & 0x0F);
    rtc->minutes =  ((chip_rtc.minutes >> 4) * 10) + (chip_rtc.minutes & 0x0F);
    //fix hour check with AM/PM vs 24hr bit
    // rtc->hours = (((chip_rtc.hours & 0x1F) >> 4) * 10) + (chip_rtc.hours & 0x0F); (Wrong!!)_
    

    // TODO check 12/24n bit (0x40)
    if (chip_rtc.hours & 0x20) rtc->hours = 20; // 20hr Flag?
    else if (chip_rtc.hours & 0x10) rtc->hours = 10;  // 10hr Flag ?
    else rtc->hours = 0; // no flags.

    rtc->hours +=  (chip_rtc.hours & 0x0F);

//     rtc->date = (((chip_rtc.date & 0x3F) >> 4) * 10) + (chip_rtc.date & 0x0F);
    
    rtc->day = ((chip_rtc.day >> 4) * 10) + (chip_rtc.day & 0x0F);
    //if (chip_rtc.date & 0x30) rtc->date = 30;       // 30 day Flag?
    //else if (chip_rtc.date & 0x20) rtc->date = 20;  // 20 day Flag?
    //else if (chip_rtc.date & 0x10) rtc->date = 10;  // 10 day Flag ?
    //else rtc->date = 0; // no flags.

    //rtc->date +=  (chip_rtc.date & 0x0F);    
    
    
    //check century bit in month byte?
    rtc->month = (((chip_rtc.month & 0x1F) >> 4) * 10) + (chip_rtc.month & 0x0F);
    rtc->year = ((chip_rtc.year >> 4) * 10) + (chip_rtc.year & 0x0F); // + 2000;  // Chip isnt designed for Centuries..
    
    return;   
    
}


void write_RTC( S_DateTime *rtc) {
    
    uint8 WriteBuf[8] = {0};
    WriteBuf[0] = 0;
    
    S_DateTime chip_rtc;
    uint16 year, modulus;

    // convert real rtc info to Chip data
    chip_rtc.seconds = ((uint8)(rtc->seconds / 10) << 4 )  | (rtc->seconds%10);
    chip_rtc.minutes = ((uint8)(rtc->minutes / 10) << 4 )  | (rtc->minutes%10);
   // chip_rtc.hours = ((uint8)(rtc->hours / 10) << 4 )  | (rtc->hours%10);
    chip_rtc.day = 7;

    if (rtc->hours >= 20) {
        chip_rtc.hours = 0x20; // set 20hr Flag?
        rtc->hours -= 20;
    } else if (rtc->hours >= 10) { 
        chip_rtc.hours = 0x10;  // set 10hr Flag ?
        rtc->hours -= 10;   
    }
    else chip_rtc.hours  = 0; // clear flags.
    
    chip_rtc.hours |= rtc->hours;
    
    
       
    chip_rtc.day = ((uint8)(rtc->day / 10) << 4 )  | (rtc->day % 10);
               
//    if (rtc->date >  30) {             
//            chip_rtc.date = 0x30; //  Set 30 day Flag
//            rtc->date -= 30;
//    }
//    else if (rtc->date >  20) {             
//            chip_rtc.date = 0x20; //  Set 20 day Flag
//            rtc->date -= 20;
//    }    
//    else if (rtc->date >= 10) { 
//        chip_rtc.date = 0x10;  // set 10 day Flag ?
//        rtc->date -= 10;   
//    }
//    else chip_rtc.date  = 0; // clear flags.
    
//    chip_rtc.date |= rtc->date;
        
    
    chip_rtc.month = rtc->month;
 
    year = rtc->year; // Chip isnt designed for Centuries..    
    chip_rtc.year = ((uint8)(year / 10) << 4 )  | (year%10);
    
    // copy buffer 
    
    memcpy(&WriteBuf[1],&chip_rtc, sizeof(S_DateTime));
    
    
   // I2CM_I2CMasterWriteBuf(DS3231MZ_ADDR, (uint8 *) WriteBuf, 0, I2CM_I2C_MODE_COMPLETE_XFER);  // I2CM_I2C_MODE_NO_STOP
   // while((I2CM_I2CMasterStatus() & I2CM_I2C_MSTAT_WR_CMPLT) == 0) {};

    //CyDelay(10);
    
    I2C_I2CMasterWriteBuf(I2C_RTC_SLAVE_ADDR, (uint8 *) &WriteBuf[0],  sizeof(S_DateTime)+1 , I2C_I2C_MODE_COMPLETE_XFER); // I2CM_I2C_MODE_NO_STOP
    //I2CM_I2CMasterWriteBuf(DS3231MZ_ADDR, (uint8 *) &chip_rtc,  sizeof(struct rtc_date_struct) , I2CM_I2C_MODE_COMPLETE_XFER); // I2CM_I2C_MODE_NO_STOP
//    I2CM_I2CMasterWriteBuf(DS3231MZ_ADDR, (uint8 *) &chip_rtc,  sizeof(struct ds3231mz_date_struct), I2CM_I2C_MODE_NO_STOP);
    while((I2C_I2CMasterStatus() & I2C_I2C_MSTAT_WR_CMPLT) == 0) {};
    
    return;   
    
}


uint8_t read_RTCReg( uint8_t reg){
    //uint8_t reg = 0x0E;
    uint8_t data;
    write_result = I2C_I2CMasterWriteBuf(I2C_RTC_SLAVE_ADDR, (uint8 *) &reg, 1, I2C_I2C_MODE_NO_STOP ); // I2CM_I2C_MODE_NO_STOP
    while(((result = I2C_I2CMasterStatus()) & I2C_I2C_MSTAT_WR_CMPLT) == 0) {
        write_result = result;
    }
    
    read_result = I2C_I2CMasterReadBuf(I2C_RTC_SLAVE_ADDR, (uint8 *) &data, 1 , I2C_I2C_MODE_REPEAT_START ); // I2CM_I2C_MODE_REPEAT_START
    while(((result = I2C_I2CMasterStatus()) & I2C_I2C_MSTAT_RD_CMPLT) == 0) {};
    
    return data;
}

/* [] END OF FILE */
