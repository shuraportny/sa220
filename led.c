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
#include "led.h"

typedef enum {
    MODE_OFF = 0,
    MODE_STATIC,
    MODE_BREATHING,
    MODE_STROBE
} LEDMode;

typedef struct {
    uint8 led_index;
    uint8 r_base, g_base, b_base;
    LEDMode mode;
    uint8 step;
    int8 direction;
} RGBLED;

//RGBLED leds[4];


RGBLED leds[4] = {
            {0, 255, 0, 255, MODE_BREATHING, 0, 1},
            {1, 255, 192, 0, MODE_BREATHING, 0, 1},
            {2, 0, 0, 255, MODE_BREATHING, 0, 1},
            {3, 255, 255, 255, MODE_BREATHING, 0, 1}
        };


//#define SIN_CNT 64
//uint8_t sineTable[SIN_CNT] = {
//    0, 6, 12, 19, 25, 31, 37, 43,
//    49, 56, 62, 68, 74, 80, 86, 91,
//    97, 103, 108, 113, 118, 123, 128, 132,
//    137, 141, 145, 149, 152, 156, 159, 162,
//    165, 167, 170, 172, 174, 175, 177, 178,
//    179, 180, 180, 180, 180, 179, 178, 177,
//    175, 174, 172, 170, 167, 165, 162, 159,
//    156, 152, 149, 145, 141, 137, 132, 128
//};




//uint8_t sineTable[SIN_CNT] = {
//    0, 6, 12, 19, 25, 31, 37, 43,
//    49, 56, 62, 68, 74, 80, 86, 91,
//    97, 103, 108, 113, 118, 123, 128, 132,
//    137, 141, 145, 149, 152, 156, 159, 162,
//    165, 167, 170, 172, 174, 175, 177, 178,
//    179, 180, 180, 180, 180, 179, 178, 177,
//    175, 174, 172, 170, 167, 165, 162, 159,
//    156, 152, 149, 145, 141, 137, 132, 128
//};

//#define SIN_CNT 23
//uint8_t sineTable[SIN_CNT] = {
//    0,  12,  25,  37, 
//    49, 62,  74,  86, 
//    97,  108,  118,  128, 
//    137,  145,  152,  159, 
//    165,  170,  174,  177, 
//    179,  180,  180 
//};


#define SIN_CNT 20
uint8_t sineTable[SIN_CNT] = {
    6, 12,  25,  37, 
    49, 62,  74,  86, 
    97,  108,  118,  128, 
    137,  145,  152,  159, 
    165,  170,  174,  177 
     
};

#define STROBE_CNT 20
uint8_t strobeTable[SIN_CNT] = {
    152, 152,  12,  12, 
    152, 152,  12,  12,  
    152, 152,  12,  12, 
    152, 152,  12,  12, 
    152, 152,  12,  12, 
     
};


//#define SIN_CNT 12
//uint8_t sineTable[SIN_CNT] = {
//    0,    25,   
//    49,   74,   
//    97,    118,   
//    137,    152,   
//    165,    174,   
//    179,    180   
//     
//};

//#define SIN_CNT 28
//uint8_t sineTable[SIN_CNT] = {
//    0, 6, 12, 19, 25, 31, 37, 43,
//    49, 56, 62, 68, 74, 80, 86, 91,
//    97, 103, 108, 113, 118, 123, 128, 132,
//    137, 141, 145, 149 
//    
//};



uint8 TLC_ReadReg(uint8 reg)
{
    uint8 value = 0;

    // Send register address
    I2C_I2CMasterWriteBuf(I2C_LED_SLAVE_ADDR, &reg, 1, I2C_I2C_MODE_COMPLETE_XFER);
    while (I2C_I2CMasterStatus() & I2C_I2C_MSTAT_XFER_INP);

    // Read 1 byte from that register
    I2C_I2CMasterReadBuf(I2C_LED_SLAVE_ADDR, &value, 1, I2C_I2C_MODE_COMPLETE_XFER);
    while (I2C_I2CMasterStatus() & I2C_I2C_MSTAT_XFER_INP);

    return value;
}

void TLC_WriteReg(uint8 reg, uint8 value) {
    uint8 buf[2] = {reg, value};
    I2C_I2CMasterWriteBuf(I2C_LED_SLAVE_ADDR, buf, 2, I2C_I2C_MODE_COMPLETE_XFER);
    while(I2C_I2CMasterStatus() & I2C_I2C_MSTAT_XFER_INP);
}


void TLC_Init(void) {
    

    // MODE1: Normal mode (AI=1 for auto-increment)
    TLC_WriteReg(0x00, 0x01);
    // MODE2: Output change on STOP, totem pole structure
    TLC_WriteReg(0x01, 0x00);
    
    // LEDOUTx: Set all channels to PWM mode (0b10)
    TLC_WriteReg(0x14, 0xAA); // OUT0-3
    TLC_WriteReg(0x15, 0xAA); // OUT4-7
    TLC_WriteReg(0x16, 0xAA); // OUT8-11
    TLC_WriteReg(0x17, 0xAA); // OUT12-15
    
    
    for(uint8_t i=0 ;i<4; i++){
        leds[i].direction=1;
    }
}



void TLC_SetChannel(uint8 channel, uint8 brightness) {
    if (channel > 17) return;
    TLC_WriteReg(channel, brightness);
}

void TLC_SetRGB(uint8 led_index, uint8 r, uint8 g, uint8 b) {
    uint8 base_channel = led_index * 3;
    
    switch(led_index){
        case 0: base_channel = 10;break;
        case 1: base_channel = 14;break;
        case 2: base_channel = 6;break;
        case 3: base_channel = 2; break;
    }
    
//    TLC_SetChannel(base_channel, r);
//    TLC_SetChannel(base_channel + 1, g);
//    TLC_SetChannel(base_channel + 2, b);
    
    TLC_SetChannel(base_channel, b);
    TLC_SetChannel(base_channel + 1, g);
    TLC_SetChannel(base_channel + 2, r);
    
}


void TLC_UpdateRGBLED(RGBLED* led) {
    switch (led->mode) {
        case MODE_OFF:
            TLC_SetRGB(led->led_index, 0, 0, 0);
            break;

        case MODE_STATIC:
            TLC_SetRGB(led->led_index, led->r_base, led->g_base, led->b_base);
            break;

        case MODE_BREATHING: {
            uint8 scale = sineTable[led->step];
            TLC_SetRGB(led->led_index,
                       (led->r_base * scale) / 255,
                       (led->g_base * scale) / 255,
                       (led->b_base * scale) / 255);

            led->step += led->direction;
            if ( led->step == (SIN_CNT-1) || led->step == 0) {
                led->direction *= -1;
            }
            break;
        }
        case MODE_STROBE:{ 
            uint8 scale = strobeTable[led->step];
            TLC_SetRGB(led->led_index,
                       (led->r_base * scale) / 255,
                       (led->g_base * scale) / 255,
                       (led->b_base * scale) / 255);

            led->step += led->direction;
            if ( led->step == (STROBE_CNT-1) || led->step == 0) {
                led->direction *= -1;
            }
            break;
        }
    }
}



/*    Example Usage
 
RGBLED leds[4] = {
    {0, 255, 0, 0, MODE_BREATHING, 0, 1},
    {1, 0, 255, 0, MODE_STATIC, 0, 1},
    {2, 0, 0, 255, MODE_OFF, 0, 1},
    {3, 255, 255, 0, MODE_BREATHING, 32, -1}
};

int main(void) {
    CyGlobalIntEnable;
    TLC_Init();

    for (;;) {
        for (int i = 0; i < 4; i++) {
            TLC_UpdateRGBLED(&leds[i]);
        }

        // Add timer-based speed control or delay if needed
        // and continue polling sensors or updating UI here
    }
}



*/

void LED_Update(){
    for (int i = 0; i < 4; i++) {
        TLC_UpdateRGBLED(&leds[i]);
        
    }
}


void SetLEDmode(uint8_t num , uint8_t mode, uint8_t ch2Active){
    if(ch2Active){
        if(num==0){
            if(mode==0){
                leds[2].mode = MODE_STATIC;
                leds[3].mode = MODE_STATIC;
            }else if(mode==1){
                leds[2].mode = MODE_BREATHING;
                leds[3].mode = MODE_BREATHING;
            }else if(mode ==2){
                leds[2].mode = MODE_STROBE;
                leds[3].mode = MODE_STROBE;
            }
        }
        if(num==1){
                if(mode==0){
                leds[0].mode = MODE_STATIC;
                leds[1].mode = MODE_STATIC;
            }else if (mode ==1){
                leds[0].mode = MODE_BREATHING;
                leds[1].mode = MODE_BREATHING;
            }else if (mode ==2){
                leds[0].mode = MODE_STROBE;
                leds[1].mode = MODE_STROBE;
            }
        }
        
    }else{
        if(mode==0){
                leds[0].mode = MODE_STATIC;
                leds[1].mode = MODE_STATIC;
                leds[2].mode = MODE_STATIC;
                leds[3].mode = MODE_STATIC;
            }else if (mode ==1){
                leds[0].mode = MODE_BREATHING;
                leds[1].mode = MODE_BREATHING;
                leds[2].mode = MODE_BREATHING;
                leds[3].mode = MODE_BREATHING;
            }else if (mode ==2){
                leds[0].mode = MODE_STROBE;
                leds[1].mode = MODE_STROBE;
                leds[2].mode = MODE_STROBE;
                leds[3].mode = MODE_STROBE;
            }
    }
    
}






void system_SetLed(uint8_t num , uint8_t color, uint8_t breath, uint8_t doubleCh)
{   
    if(!doubleCh){
        if(breath==1){
            for(uint8_t i =0 ;i<4; i++){
                leds[i].mode = MODE_BREATHING;
            }
        }else if (breath==2){
            for(uint8_t i =0 ;i<4; i++){
                leds[i].mode = MODE_STROBE;
            }
        }else {
            for(uint8_t i =0 ;i<4; i++){
                leds[i].mode = MODE_STATIC;
                leds[i].step=SIN_CNT-1;
                leds[i].direction=-1;
            }
        }
        switch(color){
            case  SYSTEM_LED_OFF: 
                leds[0].mode = MODE_OFF;
                leds[1].mode = MODE_OFF;
                leds[2].mode = MODE_OFF;
                leds[3].mode = MODE_OFF;
            break;
            case  SYSTEM_LED_GREEN:
                leds[0].r_base =0;leds[0].g_base =225;leds[0].b_base =0;
                leds[1].r_base =0;leds[1].g_base =225;leds[1].b_base =0;
                leds[2].r_base =0;leds[2].g_base =225;leds[2].b_base =0;
                leds[3].r_base =0;leds[3].g_base =225;leds[3].b_base =0;
            break;
            case  SYSTEM_LED_YELLOW:
                leds[0].r_base =100;leds[0].g_base =200;leds[0].b_base =0;
                leds[1].r_base =100;leds[1].g_base =200;leds[1].b_base =0;
                leds[2].r_base =100;leds[2].g_base =200;leds[2].b_base =0;
                leds[3].r_base =100;leds[3].g_base =200;leds[3].b_base =0;
            break; 
            case  SYSTEM_LED_RED:
                leds[0].r_base =255;leds[0].g_base =0;leds[0].b_base =0;
                leds[1].r_base =255;leds[1].g_base =0;leds[1].b_base =0; 
                leds[2].r_base =255;leds[2].g_base =0;leds[2].b_base =0;
                leds[3].r_base =255;leds[3].g_base =0;leds[3].b_base =0;
            break;    
            case  SYSTEM_LED_BLUE:
                leds[0].r_base =0;leds[0].g_base =0;leds[0].b_base =255;
                leds[1].r_base =0;leds[1].g_base =0;leds[1].b_base =255;
                leds[2].r_base =0;leds[2].g_base =0;leds[2].b_base =255;
                leds[3].r_base =0;leds[3].g_base =0;leds[3].b_base =255;
            break;
            case  SYSTEM_LED_MAGENTA:
                leds[0].r_base =255;leds[0].g_base =0;leds[0].b_base =255;
                leds[1].r_base =255;leds[1].g_base =0;leds[1].b_base =255;
                leds[2].r_base =255;leds[2].g_base =0;leds[2].b_base =255;
                leds[3].r_base =255;leds[3].g_base =0;leds[3].b_base =255;
            break;  
            case  SYSTEM_LED_WHITE:
                leds[0].r_base =30;leds[0].g_base =128;leds[0].b_base =100;
                leds[1].r_base =30;leds[1].g_base =128;leds[1].b_base =100;
                leds[2].r_base =30;leds[2].g_base =128;leds[2].b_base =100;
                leds[3].r_base =30;leds[3].g_base =128;leds[3].b_base =100;
            break;   
                case  SYSTEM_LED_ORANGE:
                leds[0].r_base =250;leds[0].g_base =100;leds[0].b_base =0;
                leds[1].r_base =250;leds[1].g_base =100;leds[1].b_base =0;
                leds[2].r_base =250;leds[2].g_base =100;leds[2].b_base =0;
                leds[3].r_base =250;leds[3].g_base =100;leds[3].b_base =0;
            break;  
                case  SYSTEM_LED_CYAN:
                leds[0].r_base =12;leds[0].g_base =250;leds[0].b_base =245;
                leds[1].r_base =12;leds[1].g_base =250;leds[1].b_base =245;
                leds[2].r_base =12;leds[2].g_base =250;leds[2].b_base =245;
                leds[3].r_base =12;leds[3].g_base =250;leds[3].b_base =245;
            break;  
        } 
    }else{
        if(breath){
            if(num==1){
                leds[0].mode = MODE_BREATHING;
                leds[1].mode = MODE_BREATHING;
            }else{
                leds[2].mode = MODE_BREATHING;
                leds[3].mode = MODE_BREATHING;
            }
        }else{
            if(num==1){
                leds[0].mode = MODE_STATIC;leds[0].step=SIN_CNT-1;leds[0].direction=-1;
                leds[1].mode = MODE_STATIC;leds[1].step=SIN_CNT-1;leds[1].direction=-1;
            }else{
                leds[2].mode = MODE_STATIC;leds[2].step=SIN_CNT-1;leds[2].direction=-1;
                leds[3].mode = MODE_STATIC;leds[3].step=SIN_CNT-1;leds[3].direction=-1;
            }
        }
        switch(color){
            case  SYSTEM_LED_OFF: 
                if(num==1){
                    leds[0].mode = MODE_OFF;
                    leds[1].mode = MODE_OFF;
                }else{
                    leds[2].mode = MODE_OFF;
                    leds[3].mode = MODE_OFF;
                }
            break;
            case  SYSTEM_LED_GREEN:
                if(num==1){
                    leds[0].r_base =0;leds[0].g_base =225;leds[0].b_base =0;
                    leds[1].r_base =0;leds[1].g_base =225;leds[1].b_base =0;
                }else{
                    leds[2].r_base =0;leds[2].g_base =225;leds[2].b_base =0;
                    leds[3].r_base =0;leds[3].g_base =225;leds[3].b_base =0;
                }
            break;
            case  SYSTEM_LED_YELLOW:
                if(num==1){
                    leds[0].r_base =100;leds[0].g_base =200;leds[0].b_base =0;
                    leds[1].r_base =100;leds[1].g_base =200;leds[1].b_base =0;
                }else{
                    leds[2].r_base =100;leds[2].g_base =192;leds[2].b_base =0;
                    leds[3].r_base =100;leds[3].g_base =192;leds[3].b_base =0;
                }
            break; 
            case  SYSTEM_LED_RED:
                 if(num==1){
                    leds[0].r_base =255;leds[0].g_base =0;leds[0].b_base =0;
                    leds[1].r_base =255;leds[1].g_base =0;leds[1].b_base =0;
                }else{
                    leds[2].r_base =255;leds[2].g_base =0;leds[2].b_base =0;
                    leds[3].r_base =255;leds[3].g_base =0;leds[3].b_base =0;
                }
            break;     
            case  SYSTEM_LED_BLUE:
                 if(num==1){
                    leds[0].r_base =0;leds[0].g_base =0;leds[0].b_base =255;
                    leds[1].r_base =0;leds[1].g_base =0;leds[1].b_base =255;
                }else{
                    leds[2].r_base =0;leds[2].g_base =0;leds[2].b_base =255;
                    leds[3].r_base =0;leds[3].g_base =0;leds[3].b_base =255;
                }
            break;
            case  SYSTEM_LED_MAGENTA:
                if(num==1){
                    leds[0].r_base =255;leds[0].g_base =0;leds[0].b_base =255;
                    leds[1].r_base =255;leds[1].g_base =0;leds[1].b_base =255;
                }else{
                    leds[2].r_base =255;leds[2].g_base =0;leds[2].b_base =255;
                    leds[3].r_base =255;leds[3].g_base =0;leds[3].b_base =255;
                }
            break;  
            case  SYSTEM_LED_WHITE:
                if(num==1){
                    leds[0].r_base =64;leds[0].g_base =128;leds[0].b_base =128;
                    leds[1].r_base =64;leds[1].g_base =128;leds[1].b_base =128;
                }else{
                    leds[2].r_base =64;leds[2].g_base =128;leds[2].b_base =128;
                    leds[3].r_base =64;leds[3].g_base =128;leds[3].b_base =128;
                }
            break;
                case  SYSTEM_LED_ORANGE:
                if(num==1){
                    leds[0].r_base =250;leds[0].g_base =100;leds[0].b_base =0;
                    leds[1].r_base =250;leds[1].g_base =100;leds[1].b_base =0;
                }else{
                    leds[2].r_base =250;leds[2].g_base =100;leds[2].b_base =0;
                    leds[3].r_base =250;leds[3].g_base =100;leds[3].b_base =0;
                }
            break;  
                case  SYSTEM_LED_CYAN:
                if(num==1){
                    leds[0].r_base =12;leds[0].g_base =250;leds[0].b_base =245;
                    leds[1].r_base =12;leds[1].g_base =250;leds[1].b_base =245;
                }else{
                    leds[2].r_base =12;leds[2].g_base =250;leds[2].b_base =245;
                    leds[3].r_base =12;leds[3].g_base =250;leds[3].b_base =245;
                }
            break;  
        }
    
    } // else
   
    
}


uint8_t system_IsLEDInBreathMode(uint8_t ch){
    if(leds[ch].mode==MODE_BREATHING)
        return 1;
    else
        return 0;

}
/* [] END OF FILE */
