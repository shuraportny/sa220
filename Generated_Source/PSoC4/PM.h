/*******************************************************************************
* File Name: PM.h
* Version 2.60
*
* Description:
*  This file contains the function prototypes and constants used in
*  the Sequencing Successive Approximation ADC Component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_ADC_SAR_SEQ_PM_H)
#define CY_ADC_SAR_SEQ_PM_H

#include "cytypes.h"
#include "CyLib.h"


/***************************************
*      Data Struct Definition
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 enableState;
    uint32 dftRegVal;
} PM_BACKUP_STRUCT;


/**************************************
*    Enumerated Types and Parameters
**************************************/

/*  Clock Source setting constants */
#define PM__EXTERNAL 0
#define PM__INTERNAL 1

/*  Sample Mode setting constants */
#define PM__FREERUNNING 0
#define PM__HARDWARESOC 1

/*  Reference type setting constants */
#define PM__VDDA_2 0
#define PM__VDDA 1
#define PM__INTERNAL1024 2
#define PM__INTERNAL1024BYPASSED 3
#define PM__INTERNALVREF 4
#define PM__INTERNALVREFBYPASSED 5
#define PM__VDDA_2BYPASSED 6
#define PM__EXTERNALVREF 7

/* Input buffer gain setting constants */
#define PM__DISABLED 0
#define PM__ONE 1
#define PM__TWO 2
#define PM__FOUR 3
#define PM__EIGHT 4
#define PM__SIXTEEN 5

/* Negative input setting sonstants in single ended mode */
#define PM__VSS 0
#define PM__VREF 1
#define PM__OTHER 2

/* Compare mode setting constants:
*    Mode0 - Disable
*    Mode1 - Result < Low_Limit
*    Mode2 - Low_Limit <= Result < High_Limit
*    Mode3 - High_Limit <= Result
*    Mode4 - (Result < Low_Limit) or (High_Limit <= Result)
*/
#define PM__MODE0 0
#define PM__MODE1 1
#define PM__MODE2 2
#define PM__MODE3 3

#define PM__RES8 0
#define PM__RES10 1

#define PM__RIGHT 0
#define PM__LEFT 1

#define PM__FSIGNED 1
#define PM__FUNSIGNED 0

#define PM__ACCUMULATE 0
#define PM__FIXEDRESOLUTION 1



/***************************************
*   Conditional Compilation Parameters
****************************************/ 

#define PM_CY_SAR_IP_VER0               	(0u)
#define PM_CY_SAR_IP_VER1               	(1u)

#if (CY_PSOC4_4100 || CY_PSOC4_4200)
    #define PM_CY_SAR_IP_VER              (PM_CY_SAR_IP_VER0)
#else /* Other devices */
    #define PM_CY_SAR_IP_VER              (PM_CY_SAR_IP_VER1)
#endif  /* (CY_PSOC4_4100 || CY_PSOC4_4200) */


/***************************************
*    Initial Parameter Constants
***************************************/
#define PM_DEFAULT_SAMPLE_MODE_SEL        (0u)
#define PM_DEFAULT_VREF_SEL               (1u)
#define PM_DEFAULT_NEG_INPUT_SEL          (0u)
#define PM_DEFAULT_ALT_RESOLUTION_SEL     (0u)
#define PM_DEFAULT_JUSTIFICATION_SEL      (0u)
#define PM_DEFAULT_DIFF_RESULT_FORMAT_SEL (1u)
#define PM_DEFAULT_SE_RESULT_FORMAT_SEL   (1u)
#define PM_DEFAULT_CLOCK_SOURCE           (1u)
#define PM_DEFAULT_VREF_MV_VALUE          (3300)
#define PM_DEFAULT_BUFFER_GAIN            (0u)
#define PM_DEFAULT_AVG_SAMPLES_NUM        (0u)
#define PM_DEFAULT_AVG_SAMPLES_DIV        (0u < 4u) ? (int16)(0x100u >> (7u - 0u)) : (int16)(0x100u >> 4u)
#define PM_DEFAULT_AVG_MODE               (1u)
#define PM_MAX_RESOLUTION                 (12u)
#define PM_DEFAULT_LOW_LIMIT              (0u)
#define PM_DEFAULT_HIGH_LIMIT             (2047u)
#define PM_DEFAULT_COMPARE_MODE           (0u)
#define PM_DEFAULT_ACLKS_NUM              (4u)
#define PM_DEFAULT_BCLKS_NUM              (4u)
#define PM_DEFAULT_CCLKS_NUM              (4u)
#define PM_DEFAULT_DCLKS_NUM              (4u)
#define PM_TOTAL_CHANNELS_NUM             (1u)
#define PM_SEQUENCED_CHANNELS_NUM         (1u)
#define PM_DEFAULT_EN_CHANNELS            (1u)
#define PM_NOMINAL_CLOCK_FREQ             (2000000)
#define PM_INJ_CHANNEL_ENABLED            (0u)
#define PM_IRQ_REMOVE                     (0u)

/* Determines whether the configuration contains external negative input. */
#define PM_SINGLE_PRESENT                 (0u)
#define PM_CHANNELS_MODE                  (0u)
#define PM_MAX_CHANNELS_EN_MASK           (0xffffu >> (16u - PM_SEQUENCED_CHANNELS_NUM))


/***************************************
*        Function Prototypes
***************************************/

void PM_Start(void);
void PM_Stop(void);
void PM_Init(void);
void PM_Enable(void);
void PM_StartConvert(void);
void PM_StopConvert(void);
uint32 PM_IsEndConversion(uint32 retMode);
int16 PM_GetResult16(uint32 chan);
void PM_SetChanMask(uint32 mask);
void PM_SetLowLimit(uint32 lowLimit);
void PM_SetHighLimit(uint32 highLimit);
void PM_SetLimitMask(uint32 mask);
void PM_SetSatMask(uint32 mask);
void PM_SetOffset(uint32 chan, int16 offset);
void PM_SetGain(uint32 chan, int32 adcGain);
#if(PM_INJ_CHANNEL_ENABLED)
    void PM_EnableInjection(void);
#endif /* PM_INJ_CHANNEL_ENABLED */
#if(PM_DEFAULT_JUSTIFICATION_SEL == PM__RIGHT)
    int16 PM_CountsTo_mVolts(uint32 chan, int16 adcCounts);
    int32 PM_CountsTo_uVolts(uint32 chan, int16 adcCounts);
    float32 PM_CountsTo_Volts(uint32 chan, int16 adcCounts);
#endif /* End PM_DEFAULT_JUSTIFICATION_SEL == PM__RIGHT */
void PM_Sleep(void);
void PM_Wakeup(void);
void PM_SaveConfig(void);
void PM_RestoreConfig(void);

CY_ISR_PROTO( PM_ISR );


/**************************************
*           API Constants
**************************************/
/* Constants for Sleep mode states */
#define PM_DISABLED                   (0x00u)
#define PM_ENABLED                    (0x01u)
#define PM_STARTED                    (0x02u)
#define PM_BOOSTPUMP_ENABLED          (0x04u)

/*   Constants for IsEndConversion() "retMode" parameter  */
#define PM_RETURN_STATUS              (0x01u)
#define PM_WAIT_FOR_RESULT            (0x02u)
#define PM_RETURN_STATUS_INJ          (0x04u)
#define PM_WAIT_FOR_RESULT_INJ        (0x08u)

#define PM_MAX_FREQUENCY              (18000000)       /*18Mhz*/

#define PM_RESOLUTION_12              (12u)
#define PM_RESOLUTION_10              (10u)
#define PM_RESOLUTION_8               (8u)

#define PM_10US_DELAY                 (10u)

#define PM_10V_COUNTS                 (10.0F)
#define PM_10MV_COUNTS                (10000)
#define PM_10UV_COUNTS                (10000000L)


/***************************************
* Global variables external identifier
***************************************/

extern uint8 PM_initVar;
extern volatile int16 PM_offset[PM_TOTAL_CHANNELS_NUM];
extern volatile int32 PM_countsPer10Volt[PM_TOTAL_CHANNELS_NUM];


/***************************************
*              Registers
***************************************/

#define PM_SAR_CTRL_REG                (*(reg32 *) PM_cy_psoc4_sar__SAR_CTRL )
#define PM_SAR_CTRL_PTR                ( (reg32 *) PM_cy_psoc4_sar__SAR_CTRL )

#define PM_SAR_SAMPLE_CTRL_REG         (*(reg32 *) PM_cy_psoc4_sar__SAR_SAMPLE_CTRL )
#define PM_SAR_SAMPLE_CTRL_PTR         ( (reg32 *) PM_cy_psoc4_sar__SAR_SAMPLE_CTRL )

#define PM_SAR_SAMPLE_TIME01_REG       (*(reg32 *) PM_cy_psoc4_sar__SAR_SAMPLE_TIME01 )
#define PM_SAR_SAMPLE_TIME01_PTR       ( (reg32 *) PM_cy_psoc4_sar__SAR_SAMPLE_TIME01 )

#define PM_SAR_SAMPLE_TIME23_REG       (*(reg32 *) PM_cy_psoc4_sar__SAR_SAMPLE_TIME23 )
#define PM_SAR_SAMPLE_TIME23_PTR       ( (reg32 *) PM_cy_psoc4_sar__SAR_SAMPLE_TIME23 )

#define PM_SAR_RANGE_THRES_REG         (*(reg32 *) PM_cy_psoc4_sar__SAR_RANGE_THRES )
#define PM_SAR_RANGE_THRES_PTR         ( (reg32 *) PM_cy_psoc4_sar__SAR_RANGE_THRES )

#define PM_SAR_RANGE_COND_REG          (*(reg32 *) PM_cy_psoc4_sar__SAR_RANGE_COND )
#define PM_SAR_RANGE_COND_PTR          ( (reg32 *) PM_cy_psoc4_sar__SAR_RANGE_COND )

#define PM_SAR_CHAN_EN_REG             (*(reg32 *) PM_cy_psoc4_sar__SAR_CHAN_EN )
#define PM_SAR_CHAN_EN_PTR             ( (reg32 *) PM_cy_psoc4_sar__SAR_CHAN_EN )

#define PM_SAR_START_CTRL_REG          (*(reg32 *) PM_cy_psoc4_sar__SAR_START_CTRL )
#define PM_SAR_START_CTRL_PTR          ( (reg32 *) PM_cy_psoc4_sar__SAR_START_CTRL )

#define PM_SAR_DFT_CTRL_REG            (*(reg32 *) PM_cy_psoc4_sar__SAR_DFT_CTRL )
#define PM_SAR_DFT_CTRL_PTR            ( (reg32 *) PM_cy_psoc4_sar__SAR_DFT_CTRL )

#define PM_SAR_CHAN_CONFIG_REG         (*(reg32 *) PM_cy_psoc4_sar__SAR_CHAN_CONFIG00 )
#define PM_SAR_CHAN_CONFIG_PTR         ( (reg32 *) PM_cy_psoc4_sar__SAR_CHAN_CONFIG00 )
#define PM_SAR_CHAN_CONFIG_IND         PM_cy_psoc4_sar__SAR_CHAN_CONFIG00

#define PM_SAR_CHAN_WORK_REG           (*(reg32 *) PM_cy_psoc4_sar__SAR_CHAN_WORK00 )
#define PM_SAR_CHAN_WORK_PTR           ( (reg32 *) PM_cy_psoc4_sar__SAR_CHAN_WORK00 )

#define PM_SAR_CHAN_RESULT_REG         (*(reg32 *) PM_cy_psoc4_sar__SAR_CHAN_RESULT00 )
#define PM_SAR_CHAN_RESULT_PTR         ( (reg32 *) PM_cy_psoc4_sar__SAR_CHAN_RESULT00 )
#define PM_SAR_CHAN_RESULT_IND         PM_cy_psoc4_sar__SAR_CHAN_RESULT00

#define PM_SAR_CHAN0_RESULT_REG         (*(reg32 *) PM_cy_psoc4_sar__SAR_CHAN_RESULT00 )
#define PM_SAR_CHAN0_RESULT_PTR         ( (reg32 *) PM_cy_psoc4_sar__SAR_CHAN_RESULT00 )

#define PM_SAR_CHAN1_RESULT_REG         (*(reg32 *) PM_cy_psoc4_sar__SAR_CHAN_RESULT01 )
#define PM_SAR_CHAN1_RESULT_PTR         ( (reg32 *) PM_cy_psoc4_sar__SAR_CHAN_RESULT01 )

#define PM_SAR_CHAN2_RESULT_REG         (*(reg32 *) PM_cy_psoc4_sar__SAR_CHAN_RESULT02 )
#define PM_SAR_CHAN2_RESULT_PTR         ( (reg32 *) PM_cy_psoc4_sar__SAR_CHAN_RESULT02 )

#define PM_SAR_CHAN3_RESULT_REG         (*(reg32 *) PM_cy_psoc4_sar__SAR_CHAN_RESULT03 )
#define PM_SAR_CHAN3_RESULT_PTR         ( (reg32 *) PM_cy_psoc4_sar__SAR_CHAN_RESULT03 )

#define PM_SAR_CHAN4_RESULT_REG         (*(reg32 *) PM_cy_psoc4_sar__SAR_CHAN_RESULT04 )
#define PM_SAR_CHAN4_RESULT_PTR         ( (reg32 *) PM_cy_psoc4_sar__SAR_CHAN_RESULT04 )

#define PM_SAR_CHAN5_RESULT_REG         (*(reg32 *) PM_cy_psoc4_sar__SAR_CHAN_RESULT05 )
#define PM_SAR_CHAN5_RESULT_PTR         ( (reg32 *) PM_cy_psoc4_sar__SAR_CHAN_RESULT05 )

#define PM_SAR_CHAN6_RESULT_REG         (*(reg32 *) PM_cy_psoc4_sar__SAR_CHAN_RESULT06 )
#define PM_SAR_CHAN6_RESULT_PTR         ( (reg32 *) PM_cy_psoc4_sar__SAR_CHAN_RESULT06 )

#define PM_SAR_CHAN7_RESULT_REG         (*(reg32 *) PM_cy_psoc4_sar__SAR_CHAN_RESULT07 )
#define PM_SAR_CHAN7_RESULT_PTR         ( (reg32 *) PM_cy_psoc4_sar__SAR_CHAN_RESULT07 )

#if(PM_CY_SAR_IP_VER != PM_CY_SAR_IP_VER0)
    #define PM_SAR_CHAN8_RESULT_REG     (*(reg32 *) PM_cy_psoc4_sar__SAR_CHAN_RESULT08 )
    #define PM_SAR_CHAN8_RESULT_PTR     ( (reg32 *) PM_cy_psoc4_sar__SAR_CHAN_RESULT08 )

    #define PM_SAR_CHAN9_RESULT_REG     (*(reg32 *) PM_cy_psoc4_sar__SAR_CHAN_RESULT09 )
    #define PM_SAR_CHAN9_RESULT_PTR     ( (reg32 *) PM_cy_psoc4_sar__SAR_CHAN_RESULT09 )

    #define PM_SAR_CHAN10_RESULT_REG    (*(reg32 *) PM_cy_psoc4_sar__SAR_CHAN_RESULT10 )
    #define PM_SAR_CHAN10_RESULT_PTR    ( (reg32 *) PM_cy_psoc4_sar__SAR_CHAN_RESULT10 )

    #define PM_SAR_CHAN11_RESULT_REG    (*(reg32 *) PM_cy_psoc4_sar__SAR_CHAN_RESULT11 )
    #define PM_SAR_CHAN11_RESULT_PTR    ( (reg32 *) PM_cy_psoc4_sar__SAR_CHAN_RESULT11 )

    #define PM_SAR_CHAN12_RESULT_REG    (*(reg32 *) PM_cy_psoc4_sar__SAR_CHAN_RESULT12 )
    #define PM_SAR_CHAN12_RESULT_PTR    ( (reg32 *) PM_cy_psoc4_sar__SAR_CHAN_RESULT12 )

    #define PM_SAR_CHAN13_RESULT_REG    (*(reg32 *) PM_cy_psoc4_sar__SAR_CHAN_RESULT13 )
    #define PM_SAR_CHAN13_RESULT_PTR    ( (reg32 *) PM_cy_psoc4_sar__SAR_CHAN_RESULT13 )

    #define PM_SAR_CHAN14_RESULT_REG    (*(reg32 *) PM_cy_psoc4_sar__SAR_CHAN_RESULT14 )
    #define PM_SAR_CHAN14_RESULT_PTR    ( (reg32 *) PM_cy_psoc4_sar__SAR_CHAN_RESULT14 )

    #define PM_SAR_CHAN15_RESULT_REG    (*(reg32 *) PM_cy_psoc4_sar__SAR_CHAN_RESULT15 )
    #define PM_SAR_CHAN15_RESULT_PTR    ( (reg32 *) PM_cy_psoc4_sar__SAR_CHAN_RESULT15 )
#endif /* (PM_CY_SAR_IP_VER != PM_CY_SAR_IP_VER0) */

#define PM_SAR_CHAN_WORK_VALID_REG     (*(reg32 *) PM_cy_psoc4_sar__SAR_CHAN_WORK_VALID)
#define PM_SAR_CHAN_WORK_VALID_PTR     ( (reg32 *) PM_cy_psoc4_sar__SAR_CHAN_WORK_VALID)

#define PM_SAR_CHAN_RESULT_VALID_REG  ( *(reg32 *) PM_cy_psoc4_sar__SAR_CHAN_RESULT_VALID )
#define PM_SAR_CHAN_RESULT_VALID_PTR  ( (reg32 *) PM_cy_psoc4_sar__SAR_CHAN_RESULT_VALID )

#define PM_SAR_STATUS_REG              (*(reg32 *) PM_cy_psoc4_sar__SAR_STATUS )
#define PM_SAR_STATUS_PTR              ( (reg32 *) PM_cy_psoc4_sar__SAR_STATUS )

#define PM_SAR_AVG_START_REG           (*(reg32 *) PM_cy_psoc4_sar__SAR_AVG_STAT )
#define PM_SAR_AVG_START_PTR           ( (reg32 *) PM_cy_psoc4_sar__SAR_AVG_STAT )

#define PM_SAR_INTR_REG                (*(reg32 *) PM_cy_psoc4_sar__SAR_INTR )
#define PM_SAR_INTR_PTR                ( (reg32 *) PM_cy_psoc4_sar__SAR_INTR )

#define PM_SAR_INTR_SET_REG            (*(reg32 *) PM_cy_psoc4_sar__SAR_INTR_SET )
#define PM_SAR_INTR_SET_PTR            ( (reg32 *) PM_cy_psoc4_sar__SAR_INTR_SET )

#define PM_SAR_INTR_MASK_REG           (*(reg32 *) PM_cy_psoc4_sar__SAR_INTR_MASK )
#define PM_SAR_INTR_MASK_PTR           ( (reg32 *) PM_cy_psoc4_sar__SAR_INTR_MASK )

#define PM_SAR_INTR_MASKED_REG         (*(reg32 *) PM_cy_psoc4_sar__SAR_INTR_MASKED )
#define PM_SAR_INTR_MASKED_PTR         ( (reg32 *) PM_cy_psoc4_sar__SAR_INTR_MASKED )

#define PM_SAR_SATURATE_INTR_REG       (*(reg32 *) PM_cy_psoc4_sar__SAR_SATURATE_INTR )
#define PM_SAR_SATURATE_INTR_PTR       ( (reg32 *) PM_cy_psoc4_sar__SAR_SATURATE_INTR )

#define PM_SAR_SATURATE_INTR_SET_REG   (*(reg32 *) PM_cy_psoc4_sar__SAR_SATURATE_INTR_SET )
#define PM_SAR_SATURATE_INTR_SET_PTR   ( (reg32 *) PM_cy_psoc4_sar__SAR_SATURATE_INTR_SET )

#define PM_SAR_SATURATE_INTR_MASK_REG (*(reg32 *) PM_cy_psoc4_sar__SAR_SATURATE_INTR_MASK )
#define PM_SAR_SATURATE_INTR_MASK_PTR ( (reg32 *) PM_cy_psoc4_sar__SAR_SATURATE_INTR_MASK )

#define PM_SAR_SATURATE_INTR_MASKED_REG \
                                                 (*(reg32 *) PM_cy_psoc4_sar__SAR_SATURATE_INTR_MASKED )
#define PM_SAR_SATURATE_INTR_MASKED_PTR \
                                                 ( (reg32 *) PM_cy_psoc4_sar__SAR_SATURATE_INTR_MASKED )

#define PM_SAR_RANGE_INTR_REG          (*(reg32 *) PM_cy_psoc4_sar__SAR_RANGE_INTR )
#define PM_SAR_RANGE_INTR_PTR          ( (reg32 *) PM_cy_psoc4_sar__SAR_RANGE_INTR )

#define PM_SAR_RANGE_INTR_SET_REG      (*(reg32 *) PM_cy_psoc4_sar__SAR_RANGE_INTR_SET )
#define PM_SAR_RANGE_INTR_SET_PTR      ( (reg32 *) PM_cy_psoc4_sar__SAR_RANGE_INTR_SET )

#define PM_SAR_RANGE_INTR_MASK_REG     (*(reg32 *) PM_cy_psoc4_sar__SAR_RANGE_INTR_MASK )
#define PM_SAR_RANGE_INTR_MASK_PTR     ( (reg32 *) PM_cy_psoc4_sar__SAR_RANGE_INTR_MASK )

#define PM_SAR_RANGE_INTR_MASKED_REG   (*(reg32 *) PM_cy_psoc4_sar__SAR_RANGE_INTR_MASKED )
#define PM_SAR_RANGE_INTR_MASKED_PTR   ( (reg32 *) PM_cy_psoc4_sar__SAR_RANGE_INTR_MASKED )

#define PM_SAR_INTR_CAUSE_REG          (*(reg32 *) PM_cy_psoc4_sar__SAR_INTR_CAUSE )
#define PM_SAR_INTR_CAUSE_PTR          ( (reg32 *) PM_cy_psoc4_sar__SAR_INTR_CAUSE )

#if(PM_INJ_CHANNEL_ENABLED)
    #define PM_SAR_INJ_CHAN_CONFIG_REG \
                                                 (*(reg32 *) PM_cy_psoc4_sarmux_8__SAR_INJ_CHAN_CONFIG )
    #define PM_SAR_INJ_CHAN_CONFIG_PTR    \
                                                 ( (reg32 *) PM_cy_psoc4_sarmux_8__SAR_INJ_CHAN_CONFIG )

    #define PM_SAR_INJ_RESULT_REG    (*(reg32 *) PM_cy_psoc4_sarmux_8__SAR_INJ_RESULT )
    #define PM_SAR_INJ_RESULT_PTR    ( (reg32 *) PM_cy_psoc4_sarmux_8__SAR_INJ_RESULT )
#endif /* PM_INJ_CHANNEL_ENABLED */
    
#define PM_MUX_SWITCH0_REG           (*(reg32 *)  PM_cy_psoc4_sar__SAR_MUX_SWITCH0 )
#define PM_MUX_SWITCH0_PTR           ( (reg32 *)  PM_cy_psoc4_sar__SAR_MUX_SWITCH0 )

#define PM_MUX_SWITCH_HW_CTRL_REG    (*(reg32 *)  PM_cy_psoc4_sar__SAR_MUX_SWITCH_HW_CTRL )
#define PM_MUX_SWITCH_HW_CTRL_PTR    ( (reg32 *)  PM_cy_psoc4_sar__SAR_MUX_SWITCH_HW_CTRL )

#define PM_PUMP_CTRL_REG             (*(reg32 *)  PM_cy_psoc4_sar__SAR_PUMP_CTRL )
#define PM_PUMP_CTRL_PTR             ( (reg32 *)  PM_cy_psoc4_sar__SAR_PUMP_CTRL )

#define PM_ANA_TRIM_REG              (*(reg32 *)  PM_cy_psoc4_sar__SAR_ANA_TRIM )
#define PM_ANA_TRIM_PTR              ( (reg32 *)  PM_cy_psoc4_sar__SAR_ANA_TRIM )

#define PM_WOUNDING_REG              (*(reg32 *)  PM_cy_psoc4_sar__SAR_WOUNDING )
#define PM_WOUNDING_PTR              ( (reg32 *)  PM_cy_psoc4_sar__SAR_WOUNDING )


/**************************************
*       Register Constants
**************************************/
#define PM_INTC_NUMBER                (PM_IRQ__INTC_NUMBER)
#define PM_INTC_PRIOR_NUMBER          (PM_IRQ__INTC_PRIOR_NUM)

/* defines for CTRL register */
#define PM_VREF_INTERNAL1024          (0x00000040Lu)
#define PM_VREF_EXTERNAL              (0x00000050Lu)
#define PM_VREF_VDDA_2                (0x00000060Lu)
#define PM_VREF_VDDA                  (0x00000070Lu)
#define PM_VREF_INTERNAL1024BYPASSED  (0x000000C0Lu)
#define PM_VREF_VDDA_2BYPASSED        (0x000000E0Lu)
#define PM_VREF_INTERNALVREF          (0x00000040Lu)
#define PM_VREF_INTERNALVREFBYPASSED  (0x000000C0Lu)

#define PM_NEG_VSSA_KELVIN            (0x00000000Lu)
#define PM_NEG_VSSA                   (0x00000200Lu)
#define PM_NEG_VREF                   (0x00000E00Lu)
#if(PM_TOTAL_CHANNELS_NUM > 1u)
    #define PM_NEG_OTHER              (uint16)((uint16)PM_cy_psoc4_sarmux_8__VNEG0 << 9u)
#else
    #define PM_NEG_OTHER              (0)
#endif /* PM_TOTAL_CHANNELS_NUM > 1u */

#define PM_SAR_HW_CTRL_NEGVREF        (0x00002000Lu)

#define PM_BOOSTPUMP_EN               (0x00100000Lu)

#define PM_NORMAL_PWR                 (0x00000000Lu)
#define PM_HALF_PWR                   (0x01000000Lu)
#define PM_MORE_PWR                   (0x02000000Lu)
#define PM_QUARTER_PWR                (0x03000000Lu)
#define PM_DEEPSLEEP_ON               (0x08000000Lu)

#define PM_DSI_SYNC_CONFIG            (0x10000000Lu)
#define PM_DSI_MODE                   (0x20000000Lu)
#define PM_SWITCH_DISABLE             (0x40000000Lu)
#define PM_ENABLE                     (0x80000000Lu)

/* defines for STATUS register */
#define PM_STATUS_BUSY                (0x80000000Lu)

/* defines for SAMPLE_CTRL register */
#define PM_ALT_RESOLUTION_10BIT       (0x00000001Lu)
#define PM_ALT_RESOLUTION_8BIT        (0x00000000Lu)

#define PM_DATA_ALIGN_LEFT            (0x00000002Lu)
#define PM_DATA_ALIGN_RIGHT           (0x00000000Lu)

#define PM_SE_SIGNED_RESULT           (0x00000004Lu)
#define PM_SE_UNSIGNED_RESULT         (0x00000000Lu)

#define PM_DIFF_SIGNED_RESULT         (0x00000008Lu)
#define PM_DIFF_UNSIGNED_RESULT       (0x00000000Lu)

#define PM_AVG_CNT_OFFSET             (4u)
#define PM_AVG_CNT_MASK               (0x00000070Lu)
#define PM_AVG_SHIFT                  (0x00000080Lu)

#define PM_CONTINUOUS_EN              (0x00010000Lu)
#define PM_DSI_TRIGGER_EN             (0x00020000Lu)
#define PM_DSI_TRIGGER_LEVEL          (0x00040000Lu)
#define PM_DSI_SYNC_TRIGGER           (0x00080000Lu)
#define PM_EOS_DSI_OUT_EN             (0x80000000Lu)

/* defines for SAMPLE_TIME01 / SAMPLE_TIME23 registers */
#define PM_SAMPLE_TIME13_OFFSET       (16u)
#define PM_SAMPLE_TIME02_MASK         (0x000003FFLu)
#define PM_SAMPLE_TIME13_MASK         (0x03FF0000Lu)

/* defines for RANGE_THRES registers */
#define PM_RANGE_HIGH_OFFSET          (16u)
#define PM_RANGE_HIGH_MASK            (0xFFFF0000Lu)
#define PM_RANGE_LOW_MASK             (0x0000FFFFLu)

/* defines for RANGE_COND register */
/* Compare mode setting constants:
*    BELOW   - Result < Low_Limit
*    INSIDE  - Low_Limit <= Result < High_Limit
*    ABOVE   - High_Limit <= Result
*    OUTSIDE - (Result < Low_Limit) or (High_Limit <= Result)
*/
#define PM_CMP_MODE_BELOW             (0x00000000Lu)
#define PM_CMP_MODE_INSIDE            (0x40000000Lu)
#define PM_CMP_MODE_ABOVE             (0x80000000Lu)
#define PM_CMP_MODE_OUTSIDE           (0xC0000000Lu)
#define PM_CMP_OFFSET                 (30u)

/* defines for _START_CTRL register */
#define PM_FW_TRIGGER                 (0x00000001Lu)

/* defines for DFT_CTRL register */
#define PM_DLY_INC                    (0x00000001Lu)
#define PM_HIZ                        (0x00000002Lu)
#define PM_DFT_INC_MASK               (0x000F0000Lu)
#define PM_DFT_OUTC_MASK              (0x00700000Lu)
#define PM_SEL_CSEL_DFT_MASK          (0x0F000000Lu)

/* configuration for clock speed > 9 Mhz based on
* characterization results
*/
#define PM_SEL_CSEL_DFT_CHAR          (0x03000000Lu)
#define PM_EN_CSEL_DFT                (0x10000000Lu)
#define PM_DCEN                       (0x20000000Lu)
#define PM_ADFT_OVERRIDE              (0x80000000Lu)

/* defines for CHAN_CONFIG / DIE_CHAN_CONFIG register
*  and channelsConfig parameter
*/
#define PM_SARMUX_VIRT_SELECT         (0x00000070Lu)
#define PM_DIFFERENTIAL_EN            (0x00000100Lu)
#define PM_ALT_RESOLUTION_ON          (0x00000200Lu)
#define PM_AVERAGING_EN               (0x00000400Lu)

#define PM_SAMPLE_TIME_SEL_SHIFT      (12u)
#define PM_SAMPLE_TIME_SEL_MASK       (0x00003000Lu)

#define PM_CHANNEL_CONFIG_MASK        (0x00003700Lu)

/* for CHAN_CONFIG only */
#define PM_DSI_OUT_EN                 (0x80000000Lu)

/* for INJ_CHAN_CONFIG only */
#define PM_INJ_TAILGATING             (0x40000000Lu)
#define PM_INJ_CHAN_EN                (0x80000000Lu)

/* defines for CHAN_WORK register */
#define PM_SAR_WRK_MAX_12BIT          (0x00001000Lu)
#define PM_SAR_WRK_MAX_10BIT          (0x00000400Lu)
#define PM_SAR_WRK_MAX_8BIT           (0x00000100Lu)

/* defines for CHAN_RESULT register */
#define PM_RESULT_MASK                (0x0000FFFFLu)
#define PM_SATURATE_INTR_MIR          (0x20000000Lu)
#define PM_RANGE_INTR_MIR             (0x40000000Lu)
#define PM_CHAN_RESULT_VALID_MIR      (0x80000000Lu)

/* defines for INTR_MASK register */
#define PM_EOS_MASK                   (0x00000001Lu)
#define PM_OVERFLOW_MASK              (0x00000002Lu)
#define PM_FW_COLLISION_MASK          (0x00000004Lu)
#define PM_DSI_COLLISION_MASK         (0x00000008Lu)
#define PM_INJ_EOC_MASK               (0x00000010Lu)
#define PM_INJ_SATURATE_MASK          (0x00000020Lu)
#define PM_INJ_RANGE_MASK             (0x00000040Lu)
#define PM_INJ_COLLISION_MASK         (0x00000080Lu)

/* defines for INJ_RESULT register */
#define PM_INJ_COLLISION_INTR_MIR     (0x10000000Lu)
#define PM_INJ_SATURATE_INTR_MIR      (0x20000000Lu)
#define PM_INJ_RANGE_INTR_MIR         (0x40000000Lu)
#define PM_INJ_EOC_INTR_MIR           (0x80000000Lu)

/* defines for MUX_SWITCH0 register */
#define PM_MUX_FW_VSSA_VMINUS         (0x00010000Lu)

/* defines for PUMP_CTRL register */
#define PM_PUMP_CTRL_ENABLED          (0x80000000Lu)

/* additional defines for channelsConfig parameter */
#define PM_IS_SATURATE_EN_MASK        (0x00000001Lu)
#define PM_IS_RANGE_CTRL_EN_MASK      (0x00000002Lu)

/* defines for WOUNDING register */
#define PM_WOUNDING_12BIT             (0x00000000Lu)
#define PM_WOUNDING_10BIT             (0x00000001Lu)
#define PM_WOUNDING_8BIT              (0x00000002Lu)

/* Trim value based on characterization */
#define PM_TRIM_COEF                  (2u)

#if(PM_MAX_RESOLUTION == PM_RESOLUTION_10)
    #define PM_ALT_WOUNDING           PM_WOUNDING_10BIT
#else
    #define PM_ALT_WOUNDING           PM_WOUNDING_8BIT
#endif /* PM_MAX_RESOLUTION == PM_RESOLUTION_10 */

#if(PM_DEFAULT_VREF_SEL == PM__VDDA_2)
    #define PM_DEFAULT_VREF_SOURCE    PM_VREF_VDDA_2
#elif(PM_DEFAULT_VREF_SEL == PM__VDDA)
    #define PM_DEFAULT_VREF_SOURCE    PM_VREF_VDDA
#elif(PM_DEFAULT_VREF_SEL == PM__INTERNAL1024)
    #define PM_DEFAULT_VREF_SOURCE    PM_VREF_INTERNAL1024
#elif(PM_DEFAULT_VREF_SEL == PM__INTERNAL1024BYPASSED)
    #define PM_DEFAULT_VREF_SOURCE    PM_VREF_INTERNAL1024BYPASSED
#elif(PM_DEFAULT_VREF_SEL == PM__INTERNALVREF)
    #define PM_DEFAULT_VREF_SOURCE    PM_VREF_INTERNALVREF
#elif(PM_DEFAULT_VREF_SEL == PM__INTERNALVREFBYPASSED)
    #define PM_DEFAULT_VREF_SOURCE    PM_VREF_INTERNALVREFBYPASSED
#elif(PM_DEFAULT_VREF_SEL == PM__VDDA_2BYPASSED)
    #define PM_DEFAULT_VREF_SOURCE    PM_VREF_VDDA_2BYPASSED
#else
    #define PM_DEFAULT_VREF_SOURCE    PM_VREF_EXTERNAL
#endif /* PM_DEFAULT_VREF_SEL == PM__VDDA_2 */

#if(PM_DEFAULT_NEG_INPUT_SEL == PM__VSS)
    /* Connect NEG input of SARADC to VSSA close to the SARADC for single channel mode */
    #if(PM_TOTAL_CHANNELS_NUM == 1u)
        #define PM_DEFAULT_SE_NEG_INPUT    PM_NEG_VSSA
    #else
        #define PM_DEFAULT_SE_NEG_INPUT    PM_NEG_VSSA_KELVIN
    #endif /* (PM_TOTAL_CHANNELS_NUM == 1u) */
    /* Do not connect VSSA to VMINUS when one channel in differential mode used */
    #if((PM_TOTAL_CHANNELS_NUM == 1u) && (PM_CHANNELS_MODE != 0u))
        #define PM_DEFAULT_MUX_SWITCH0     0u
    #else    /* miltiple channels or one single channel */
        #define PM_DEFAULT_MUX_SWITCH0     PM_MUX_FW_VSSA_VMINUS
    #endif /* (PM_TOTAL_CHANNELS_NUM == 1u) */
#elif(PM_DEFAULT_NEG_INPUT_SEL == PM__VREF)
    /* Do not connect VNEG to VREF when one channel in differential mode used */
    #if((PM_TOTAL_CHANNELS_NUM == 1u) && (PM_CHANNELS_MODE != 0u))
        #define PM_DEFAULT_SE_NEG_INPUT    0u
    #else    /* miltiple channels or one single channel */
        #define PM_DEFAULT_SE_NEG_INPUT    PM_NEG_VREF
    #endif /* (PM_TOTAL_CHANNELS_NUM == 1u) */
    #define PM_DEFAULT_MUX_SWITCH0     0u
#elif (PM_SINGLE_PRESENT != 0u)
    #define PM_DEFAULT_SE_NEG_INPUT    PM_NEG_OTHER
    #define PM_DEFAULT_MUX_SWITCH0     0u
#else
    #define PM_DEFAULT_SE_NEG_INPUT    0u
    #define PM_DEFAULT_MUX_SWITCH0     0u
#endif /* PM_DEFAULT_NEG_INPUT_SEL == PM__VREF */

/* If the SAR is configured for multiple channels, always set SAR_HW_CTRL_NEGVREF to 1 */
#if(PM_TOTAL_CHANNELS_NUM == 1u)
    #define PM_DEFAULT_HW_CTRL_NEGVREF 0u
#else
    #define PM_DEFAULT_HW_CTRL_NEGVREF PM_SAR_HW_CTRL_NEGVREF
#endif /* (PM_TOTAL_CHANNELS_NUM == 1u) */


#if(PM_DEFAULT_ALT_RESOLUTION_SEL == PM__RES8)
    #define PM_DEFAULT_ALT_RESOLUTION     (PM_ALT_RESOLUTION_8BIT)
    #define PM_DEFAULT_MAX_WRK_ALT        (PM_SAR_WRK_MAX_8BIT)
#else
    #define PM_DEFAULT_ALT_RESOLUTION     (PM_ALT_RESOLUTION_10BIT)
    #define PM_DEFAULT_MAX_WRK_ALT        (PM_SAR_WRK_MAX_10BIT)
#endif /* End PM_DEFAULT_ALT_RESOLUTION_SEL == PM__RES8 */

#if(PM_DEFAULT_JUSTIFICATION_SEL == PM__RIGHT)
    #define PM_DEFAULT_JUSTIFICATION  PM_DATA_ALIGN_RIGHT
#else
    #define PM_DEFAULT_JUSTIFICATION  PM_DATA_ALIGN_LEFT
#endif /* PM_DEFAULT_JUSTIFICATION_SEL == PM__RIGHT */

#if(PM_DEFAULT_DIFF_RESULT_FORMAT_SEL == PM__FSIGNED)
    #define PM_DEFAULT_DIFF_RESULT_FORMAT PM_DIFF_SIGNED_RESULT
#else
    #define PM_DEFAULT_DIFF_RESULT_FORMAT PM_DIFF_UNSIGNED_RESULT
#endif /* PM_DEFAULT_DIFF_RESULT_FORMAT_SEL == PM__FSIGNED */

#if(PM_DEFAULT_SE_RESULT_FORMAT_SEL == PM__FSIGNED)
    #define PM_DEFAULT_SE_RESULT_FORMAT PM_SE_SIGNED_RESULT
#else
    #define PM_DEFAULT_SE_RESULT_FORMAT PM_SE_UNSIGNED_RESULT
#endif /* PM_DEFAULT_SE_RESULT_FORMAT_SEL == PM__FSIGNED */

#if(PM_DEFAULT_SAMPLE_MODE_SEL == PM__FREERUNNING)
    #define PM_DSI_TRIGGER        0u
#else /* Firmware trigger */
    #define PM_DSI_TRIGGER        (PM_DSI_TRIGGER_EN | PM_DSI_SYNC_TRIGGER)
#endif /* End PM_DEFAULT_SAMPLE_MODE == PM__FREERUNNING */

#if(PM_INJ_CHANNEL_ENABLED)
    #define PM_SAR_INTR_MASK      (PM_EOS_MASK | PM_INJ_EOC_MASK)
#else
    #define PM_SAR_INTR_MASK      (PM_EOS_MASK)
#endif /* PM_INJ_CHANNEL_ENABLED*/

#if(PM_DEFAULT_AVG_MODE == PM__FIXEDRESOLUTION)
    #define PM_AVG_SHIFT_MODE     PM_AVG_SHIFT
#else
    #define PM_AVG_SHIFT_MODE     0u
#endif /* End PM_DEFAULT_AVG_MODE */

#define PM_COMPARE_MODE           (uint32)((uint32)(PM_DEFAULT_COMPARE_MODE) \
                                                << PM_CMP_OFFSET)

#if(PM_TOTAL_CHANNELS_NUM > 1u)
    #define PM_DEFAULT_SWITCH_CONF    0u
#else /* Disable SAR sequencer from enabling routing switches in single channel mode */
    #define PM_DEFAULT_SWITCH_CONF    PM_SWITCH_DISABLE
#endif /* End PM_TOTAL_CHANNELS_NUM > 1 */

#define PM_DEFAULT_POWER \
       ((PM_NOMINAL_CLOCK_FREQ > (PM_MAX_FREQUENCY / 4)) ? PM_NORMAL_PWR : \
       ((PM_NOMINAL_CLOCK_FREQ > (PM_MAX_FREQUENCY / 8)) ? PM_HALF_PWR : \
                                                                                       PM_QUARTER_PWR))

#define PM_DEFAULT_CTRL_REG_CFG       (PM_DEFAULT_VREF_SOURCE \
                                                   | PM_DEFAULT_SE_NEG_INPUT \
                                                   | PM_DEFAULT_HW_CTRL_NEGVREF \
                                                   | PM_DEFAULT_POWER \
                                                   | PM_DSI_SYNC_CONFIG \
                                                   | PM_DEFAULT_SWITCH_CONF)

#define PM_DEFAULT_SAMPLE_CTRL_REG_CFG (PM_DEFAULT_DIFF_RESULT_FORMAT \
                                                    | PM_DEFAULT_SE_RESULT_FORMAT \
                                                    | PM_DEFAULT_JUSTIFICATION \
                                                    | PM_DEFAULT_ALT_RESOLUTION \
                                           | (uint8)(PM_DEFAULT_AVG_SAMPLES_NUM \
                                                   << PM_AVG_CNT_OFFSET) \
                                                    | PM_AVG_SHIFT_MODE \
                                                    | PM_DSI_TRIGGER \
                                                    | PM_EOS_DSI_OUT_EN)

#define PM_DEFAULT_RANGE_THRES_REG_CFG (PM_DEFAULT_LOW_LIMIT \
            | (uint32)((uint32)PM_DEFAULT_HIGH_LIMIT << PM_RANGE_HIGH_OFFSET))

#define PM_DEFAULT_SAMPLE_TIME01_REG_CFG (PM_DEFAULT_ACLKS_NUM \
            | (uint32)((uint32)PM_DEFAULT_BCLKS_NUM << PM_SAMPLE_TIME13_OFFSET))

#define PM_DEFAULT_SAMPLE_TIME23_REG_CFG (PM_DEFAULT_CCLKS_NUM \
            | (uint32)((uint32)PM_DEFAULT_DCLKS_NUM << PM_SAMPLE_TIME13_OFFSET))


#endif /* End CY_ADC_SAR_SEQ_PM_H */


/* [] END OF FILE */
