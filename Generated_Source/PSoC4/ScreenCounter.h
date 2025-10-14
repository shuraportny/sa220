/*******************************************************************************
* File Name: ScreenCounter.h
* Version 2.10
*
* Description:
*  This file provides constants and parameter values for the ScreenCounter
*  component.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2013-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_TCPWM_ScreenCounter_H)
#define CY_TCPWM_ScreenCounter_H


#include "CyLib.h"
#include "cytypes.h"
#include "cyfitter.h"


/*******************************************************************************
* Internal Type defines
*******************************************************************************/

/* Structure to save state before go to sleep */
typedef struct
{
    uint8  enableState;
} ScreenCounter_BACKUP_STRUCT;


/*******************************************************************************
* Variables
*******************************************************************************/
extern uint8  ScreenCounter_initVar;


/***************************************
*   Conditional Compilation Parameters
****************************************/

#define ScreenCounter_CY_TCPWM_V2                    (CYIPBLOCK_m0s8tcpwm_VERSION == 2u)
#define ScreenCounter_CY_TCPWM_4000                  (CY_PSOC4_4000)

/* TCPWM Configuration */
#define ScreenCounter_CONFIG                         (1lu)

/* Quad Mode */
/* Parameters */
#define ScreenCounter_QUAD_ENCODING_MODES            (0lu)
#define ScreenCounter_QUAD_AUTO_START                (1lu)

/* Signal modes */
#define ScreenCounter_QUAD_INDEX_SIGNAL_MODE         (0lu)
#define ScreenCounter_QUAD_PHIA_SIGNAL_MODE          (3lu)
#define ScreenCounter_QUAD_PHIB_SIGNAL_MODE          (3lu)
#define ScreenCounter_QUAD_STOP_SIGNAL_MODE          (0lu)

/* Signal present */
#define ScreenCounter_QUAD_INDEX_SIGNAL_PRESENT      (0lu)
#define ScreenCounter_QUAD_STOP_SIGNAL_PRESENT       (0lu)

/* Interrupt Mask */
#define ScreenCounter_QUAD_INTERRUPT_MASK            (1lu)

/* Timer/Counter Mode */
/* Parameters */
#define ScreenCounter_TC_RUN_MODE                    (0lu)
#define ScreenCounter_TC_COUNTER_MODE                (0lu)
#define ScreenCounter_TC_COMP_CAP_MODE               (2lu)
#define ScreenCounter_TC_PRESCALER                   (1lu)

/* Signal modes */
#define ScreenCounter_TC_RELOAD_SIGNAL_MODE          (0lu)
#define ScreenCounter_TC_COUNT_SIGNAL_MODE           (3lu)
#define ScreenCounter_TC_START_SIGNAL_MODE           (0lu)
#define ScreenCounter_TC_STOP_SIGNAL_MODE            (0lu)
#define ScreenCounter_TC_CAPTURE_SIGNAL_MODE         (0lu)

/* Signal present */
#define ScreenCounter_TC_RELOAD_SIGNAL_PRESENT       (0lu)
#define ScreenCounter_TC_COUNT_SIGNAL_PRESENT        (0lu)
#define ScreenCounter_TC_START_SIGNAL_PRESENT        (0lu)
#define ScreenCounter_TC_STOP_SIGNAL_PRESENT         (0lu)
#define ScreenCounter_TC_CAPTURE_SIGNAL_PRESENT      (0lu)

/* Interrupt Mask */
#define ScreenCounter_TC_INTERRUPT_MASK              (1lu)

/* PWM Mode */
/* Parameters */
#define ScreenCounter_PWM_KILL_EVENT                 (0lu)
#define ScreenCounter_PWM_STOP_EVENT                 (0lu)
#define ScreenCounter_PWM_MODE                       (4lu)
#define ScreenCounter_PWM_OUT_N_INVERT               (0lu)
#define ScreenCounter_PWM_OUT_INVERT                 (0lu)
#define ScreenCounter_PWM_ALIGN                      (0lu)
#define ScreenCounter_PWM_RUN_MODE                   (0lu)
#define ScreenCounter_PWM_DEAD_TIME_CYCLE            (0lu)
#define ScreenCounter_PWM_PRESCALER                  (0lu)

/* Signal modes */
#define ScreenCounter_PWM_RELOAD_SIGNAL_MODE         (0lu)
#define ScreenCounter_PWM_COUNT_SIGNAL_MODE          (3lu)
#define ScreenCounter_PWM_START_SIGNAL_MODE          (0lu)
#define ScreenCounter_PWM_STOP_SIGNAL_MODE           (0lu)
#define ScreenCounter_PWM_SWITCH_SIGNAL_MODE         (0lu)

/* Signal present */
#define ScreenCounter_PWM_RELOAD_SIGNAL_PRESENT      (0lu)
#define ScreenCounter_PWM_COUNT_SIGNAL_PRESENT       (0lu)
#define ScreenCounter_PWM_START_SIGNAL_PRESENT       (0lu)
#define ScreenCounter_PWM_STOP_SIGNAL_PRESENT        (0lu)
#define ScreenCounter_PWM_SWITCH_SIGNAL_PRESENT      (0lu)

/* Interrupt Mask */
#define ScreenCounter_PWM_INTERRUPT_MASK             (1lu)


/***************************************
*    Initial Parameter Constants
***************************************/

/* Timer/Counter Mode */
#define ScreenCounter_TC_PERIOD_VALUE                (65535lu)
#define ScreenCounter_TC_COMPARE_VALUE               (65535lu)
#define ScreenCounter_TC_COMPARE_BUF_VALUE           (65535lu)
#define ScreenCounter_TC_COMPARE_SWAP                (0lu)

/* PWM Mode */
#define ScreenCounter_PWM_PERIOD_VALUE               (65535lu)
#define ScreenCounter_PWM_PERIOD_BUF_VALUE           (65535lu)
#define ScreenCounter_PWM_PERIOD_SWAP                (0lu)
#define ScreenCounter_PWM_COMPARE_VALUE              (65535lu)
#define ScreenCounter_PWM_COMPARE_BUF_VALUE          (65535lu)
#define ScreenCounter_PWM_COMPARE_SWAP               (0lu)


/***************************************
*    Enumerated Types and Parameters
***************************************/

#define ScreenCounter__LEFT 0
#define ScreenCounter__RIGHT 1
#define ScreenCounter__CENTER 2
#define ScreenCounter__ASYMMETRIC 3

#define ScreenCounter__X1 0
#define ScreenCounter__X2 1
#define ScreenCounter__X4 2

#define ScreenCounter__PWM 4
#define ScreenCounter__PWM_DT 5
#define ScreenCounter__PWM_PR 6

#define ScreenCounter__INVERSE 1
#define ScreenCounter__DIRECT 0

#define ScreenCounter__CAPTURE 2
#define ScreenCounter__COMPARE 0

#define ScreenCounter__TRIG_LEVEL 3
#define ScreenCounter__TRIG_RISING 0
#define ScreenCounter__TRIG_FALLING 1
#define ScreenCounter__TRIG_BOTH 2

#define ScreenCounter__INTR_MASK_TC 1
#define ScreenCounter__INTR_MASK_CC_MATCH 2
#define ScreenCounter__INTR_MASK_NONE 0
#define ScreenCounter__INTR_MASK_TC_CC 3

#define ScreenCounter__UNCONFIG 8
#define ScreenCounter__TIMER 1
#define ScreenCounter__QUAD 3
#define ScreenCounter__PWM_SEL 7

#define ScreenCounter__COUNT_UP 0
#define ScreenCounter__COUNT_DOWN 1
#define ScreenCounter__COUNT_UPDOWN0 2
#define ScreenCounter__COUNT_UPDOWN1 3


/* Prescaler */
#define ScreenCounter_PRESCALE_DIVBY1                ((uint32)(0u << ScreenCounter_PRESCALER_SHIFT))
#define ScreenCounter_PRESCALE_DIVBY2                ((uint32)(1u << ScreenCounter_PRESCALER_SHIFT))
#define ScreenCounter_PRESCALE_DIVBY4                ((uint32)(2u << ScreenCounter_PRESCALER_SHIFT))
#define ScreenCounter_PRESCALE_DIVBY8                ((uint32)(3u << ScreenCounter_PRESCALER_SHIFT))
#define ScreenCounter_PRESCALE_DIVBY16               ((uint32)(4u << ScreenCounter_PRESCALER_SHIFT))
#define ScreenCounter_PRESCALE_DIVBY32               ((uint32)(5u << ScreenCounter_PRESCALER_SHIFT))
#define ScreenCounter_PRESCALE_DIVBY64               ((uint32)(6u << ScreenCounter_PRESCALER_SHIFT))
#define ScreenCounter_PRESCALE_DIVBY128              ((uint32)(7u << ScreenCounter_PRESCALER_SHIFT))

/* TCPWM set modes */
#define ScreenCounter_MODE_TIMER_COMPARE             ((uint32)(ScreenCounter__COMPARE         <<  \
                                                                  ScreenCounter_MODE_SHIFT))
#define ScreenCounter_MODE_TIMER_CAPTURE             ((uint32)(ScreenCounter__CAPTURE         <<  \
                                                                  ScreenCounter_MODE_SHIFT))
#define ScreenCounter_MODE_QUAD                      ((uint32)(ScreenCounter__QUAD            <<  \
                                                                  ScreenCounter_MODE_SHIFT))
#define ScreenCounter_MODE_PWM                       ((uint32)(ScreenCounter__PWM             <<  \
                                                                  ScreenCounter_MODE_SHIFT))
#define ScreenCounter_MODE_PWM_DT                    ((uint32)(ScreenCounter__PWM_DT          <<  \
                                                                  ScreenCounter_MODE_SHIFT))
#define ScreenCounter_MODE_PWM_PR                    ((uint32)(ScreenCounter__PWM_PR          <<  \
                                                                  ScreenCounter_MODE_SHIFT))

/* Quad Modes */
#define ScreenCounter_MODE_X1                        ((uint32)(ScreenCounter__X1              <<  \
                                                                  ScreenCounter_QUAD_MODE_SHIFT))
#define ScreenCounter_MODE_X2                        ((uint32)(ScreenCounter__X2              <<  \
                                                                  ScreenCounter_QUAD_MODE_SHIFT))
#define ScreenCounter_MODE_X4                        ((uint32)(ScreenCounter__X4              <<  \
                                                                  ScreenCounter_QUAD_MODE_SHIFT))

/* Counter modes */
#define ScreenCounter_COUNT_UP                       ((uint32)(ScreenCounter__COUNT_UP        <<  \
                                                                  ScreenCounter_UPDOWN_SHIFT))
#define ScreenCounter_COUNT_DOWN                     ((uint32)(ScreenCounter__COUNT_DOWN      <<  \
                                                                  ScreenCounter_UPDOWN_SHIFT))
#define ScreenCounter_COUNT_UPDOWN0                  ((uint32)(ScreenCounter__COUNT_UPDOWN0   <<  \
                                                                  ScreenCounter_UPDOWN_SHIFT))
#define ScreenCounter_COUNT_UPDOWN1                  ((uint32)(ScreenCounter__COUNT_UPDOWN1   <<  \
                                                                  ScreenCounter_UPDOWN_SHIFT))

/* PWM output invert */
#define ScreenCounter_INVERT_LINE                    ((uint32)(ScreenCounter__INVERSE         <<  \
                                                                  ScreenCounter_INV_OUT_SHIFT))
#define ScreenCounter_INVERT_LINE_N                  ((uint32)(ScreenCounter__INVERSE         <<  \
                                                                  ScreenCounter_INV_COMPL_OUT_SHIFT))

/* Trigger modes */
#define ScreenCounter_TRIG_RISING                    ((uint32)ScreenCounter__TRIG_RISING)
#define ScreenCounter_TRIG_FALLING                   ((uint32)ScreenCounter__TRIG_FALLING)
#define ScreenCounter_TRIG_BOTH                      ((uint32)ScreenCounter__TRIG_BOTH)
#define ScreenCounter_TRIG_LEVEL                     ((uint32)ScreenCounter__TRIG_LEVEL)

/* Interrupt mask */
#define ScreenCounter_INTR_MASK_TC                   ((uint32)ScreenCounter__INTR_MASK_TC)
#define ScreenCounter_INTR_MASK_CC_MATCH             ((uint32)ScreenCounter__INTR_MASK_CC_MATCH)

/* PWM Output Controls */
#define ScreenCounter_CC_MATCH_SET                   (0x00u)
#define ScreenCounter_CC_MATCH_CLEAR                 (0x01u)
#define ScreenCounter_CC_MATCH_INVERT                (0x02u)
#define ScreenCounter_CC_MATCH_NO_CHANGE             (0x03u)
#define ScreenCounter_OVERLOW_SET                    (0x00u)
#define ScreenCounter_OVERLOW_CLEAR                  (0x04u)
#define ScreenCounter_OVERLOW_INVERT                 (0x08u)
#define ScreenCounter_OVERLOW_NO_CHANGE              (0x0Cu)
#define ScreenCounter_UNDERFLOW_SET                  (0x00u)
#define ScreenCounter_UNDERFLOW_CLEAR                (0x10u)
#define ScreenCounter_UNDERFLOW_INVERT               (0x20u)
#define ScreenCounter_UNDERFLOW_NO_CHANGE            (0x30u)

/* PWM Align */
#define ScreenCounter_PWM_MODE_LEFT                  (ScreenCounter_CC_MATCH_CLEAR        |   \
                                                         ScreenCounter_OVERLOW_SET           |   \
                                                         ScreenCounter_UNDERFLOW_NO_CHANGE)
#define ScreenCounter_PWM_MODE_RIGHT                 (ScreenCounter_CC_MATCH_SET          |   \
                                                         ScreenCounter_OVERLOW_NO_CHANGE     |   \
                                                         ScreenCounter_UNDERFLOW_CLEAR)
#define ScreenCounter_PWM_MODE_ASYM                  (ScreenCounter_CC_MATCH_INVERT       |   \
                                                         ScreenCounter_OVERLOW_SET           |   \
                                                         ScreenCounter_UNDERFLOW_CLEAR)

#if (ScreenCounter_CY_TCPWM_V2)
    #if(ScreenCounter_CY_TCPWM_4000)
        #define ScreenCounter_PWM_MODE_CENTER                (ScreenCounter_CC_MATCH_INVERT       |   \
                                                                 ScreenCounter_OVERLOW_NO_CHANGE     |   \
                                                                 ScreenCounter_UNDERFLOW_CLEAR)
    #else
        #define ScreenCounter_PWM_MODE_CENTER                (ScreenCounter_CC_MATCH_INVERT       |   \
                                                                 ScreenCounter_OVERLOW_SET           |   \
                                                                 ScreenCounter_UNDERFLOW_CLEAR)
    #endif /* (ScreenCounter_CY_TCPWM_4000) */
#else
    #define ScreenCounter_PWM_MODE_CENTER                (ScreenCounter_CC_MATCH_INVERT       |   \
                                                             ScreenCounter_OVERLOW_NO_CHANGE     |   \
                                                             ScreenCounter_UNDERFLOW_CLEAR)
#endif /* (ScreenCounter_CY_TCPWM_NEW) */

/* Command operations without condition */
#define ScreenCounter_CMD_CAPTURE                    (0u)
#define ScreenCounter_CMD_RELOAD                     (8u)
#define ScreenCounter_CMD_STOP                       (16u)
#define ScreenCounter_CMD_START                      (24u)

/* Status */
#define ScreenCounter_STATUS_DOWN                    (1u)
#define ScreenCounter_STATUS_RUNNING                 (2u)


/***************************************
*        Function Prototypes
****************************************/

void   ScreenCounter_Init(void);
void   ScreenCounter_Enable(void);
void   ScreenCounter_Start(void);
void   ScreenCounter_Stop(void);

void   ScreenCounter_SetMode(uint32 mode);
void   ScreenCounter_SetCounterMode(uint32 counterMode);
void   ScreenCounter_SetPWMMode(uint32 modeMask);
void   ScreenCounter_SetQDMode(uint32 qdMode);

void   ScreenCounter_SetPrescaler(uint32 prescaler);
void   ScreenCounter_TriggerCommand(uint32 mask, uint32 command);
void   ScreenCounter_SetOneShot(uint32 oneShotEnable);
uint32 ScreenCounter_ReadStatus(void);

void   ScreenCounter_SetPWMSyncKill(uint32 syncKillEnable);
void   ScreenCounter_SetPWMStopOnKill(uint32 stopOnKillEnable);
void   ScreenCounter_SetPWMDeadTime(uint32 deadTime);
void   ScreenCounter_SetPWMInvert(uint32 mask);

void   ScreenCounter_SetInterruptMode(uint32 interruptMask);
uint32 ScreenCounter_GetInterruptSourceMasked(void);
uint32 ScreenCounter_GetInterruptSource(void);
void   ScreenCounter_ClearInterrupt(uint32 interruptMask);
void   ScreenCounter_SetInterrupt(uint32 interruptMask);

void   ScreenCounter_WriteCounter(uint32 count);
uint32 ScreenCounter_ReadCounter(void);

uint32 ScreenCounter_ReadCapture(void);
uint32 ScreenCounter_ReadCaptureBuf(void);

void   ScreenCounter_WritePeriod(uint32 period);
uint32 ScreenCounter_ReadPeriod(void);
void   ScreenCounter_WritePeriodBuf(uint32 periodBuf);
uint32 ScreenCounter_ReadPeriodBuf(void);

void   ScreenCounter_WriteCompare(uint32 compare);
uint32 ScreenCounter_ReadCompare(void);
void   ScreenCounter_WriteCompareBuf(uint32 compareBuf);
uint32 ScreenCounter_ReadCompareBuf(void);

void   ScreenCounter_SetPeriodSwap(uint32 swapEnable);
void   ScreenCounter_SetCompareSwap(uint32 swapEnable);

void   ScreenCounter_SetCaptureMode(uint32 triggerMode);
void   ScreenCounter_SetReloadMode(uint32 triggerMode);
void   ScreenCounter_SetStartMode(uint32 triggerMode);
void   ScreenCounter_SetStopMode(uint32 triggerMode);
void   ScreenCounter_SetCountMode(uint32 triggerMode);

void   ScreenCounter_SaveConfig(void);
void   ScreenCounter_RestoreConfig(void);
void   ScreenCounter_Sleep(void);
void   ScreenCounter_Wakeup(void);


/***************************************
*             Registers
***************************************/

#define ScreenCounter_BLOCK_CONTROL_REG              (*(reg32 *) ScreenCounter_cy_m0s8_tcpwm_1__TCPWM_CTRL )
#define ScreenCounter_BLOCK_CONTROL_PTR              ( (reg32 *) ScreenCounter_cy_m0s8_tcpwm_1__TCPWM_CTRL )
#define ScreenCounter_COMMAND_REG                    (*(reg32 *) ScreenCounter_cy_m0s8_tcpwm_1__TCPWM_CMD )
#define ScreenCounter_COMMAND_PTR                    ( (reg32 *) ScreenCounter_cy_m0s8_tcpwm_1__TCPWM_CMD )
#define ScreenCounter_INTRRUPT_CAUSE_REG             (*(reg32 *) ScreenCounter_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE )
#define ScreenCounter_INTRRUPT_CAUSE_PTR             ( (reg32 *) ScreenCounter_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE )
#define ScreenCounter_CONTROL_REG                    (*(reg32 *) ScreenCounter_cy_m0s8_tcpwm_1__CTRL )
#define ScreenCounter_CONTROL_PTR                    ( (reg32 *) ScreenCounter_cy_m0s8_tcpwm_1__CTRL )
#define ScreenCounter_STATUS_REG                     (*(reg32 *) ScreenCounter_cy_m0s8_tcpwm_1__STATUS )
#define ScreenCounter_STATUS_PTR                     ( (reg32 *) ScreenCounter_cy_m0s8_tcpwm_1__STATUS )
#define ScreenCounter_COUNTER_REG                    (*(reg32 *) ScreenCounter_cy_m0s8_tcpwm_1__COUNTER )
#define ScreenCounter_COUNTER_PTR                    ( (reg32 *) ScreenCounter_cy_m0s8_tcpwm_1__COUNTER )
#define ScreenCounter_COMP_CAP_REG                   (*(reg32 *) ScreenCounter_cy_m0s8_tcpwm_1__CC )
#define ScreenCounter_COMP_CAP_PTR                   ( (reg32 *) ScreenCounter_cy_m0s8_tcpwm_1__CC )
#define ScreenCounter_COMP_CAP_BUF_REG               (*(reg32 *) ScreenCounter_cy_m0s8_tcpwm_1__CC_BUFF )
#define ScreenCounter_COMP_CAP_BUF_PTR               ( (reg32 *) ScreenCounter_cy_m0s8_tcpwm_1__CC_BUFF )
#define ScreenCounter_PERIOD_REG                     (*(reg32 *) ScreenCounter_cy_m0s8_tcpwm_1__PERIOD )
#define ScreenCounter_PERIOD_PTR                     ( (reg32 *) ScreenCounter_cy_m0s8_tcpwm_1__PERIOD )
#define ScreenCounter_PERIOD_BUF_REG                 (*(reg32 *) ScreenCounter_cy_m0s8_tcpwm_1__PERIOD_BUFF )
#define ScreenCounter_PERIOD_BUF_PTR                 ( (reg32 *) ScreenCounter_cy_m0s8_tcpwm_1__PERIOD_BUFF )
#define ScreenCounter_TRIG_CONTROL0_REG              (*(reg32 *) ScreenCounter_cy_m0s8_tcpwm_1__TR_CTRL0 )
#define ScreenCounter_TRIG_CONTROL0_PTR              ( (reg32 *) ScreenCounter_cy_m0s8_tcpwm_1__TR_CTRL0 )
#define ScreenCounter_TRIG_CONTROL1_REG              (*(reg32 *) ScreenCounter_cy_m0s8_tcpwm_1__TR_CTRL1 )
#define ScreenCounter_TRIG_CONTROL1_PTR              ( (reg32 *) ScreenCounter_cy_m0s8_tcpwm_1__TR_CTRL1 )
#define ScreenCounter_TRIG_CONTROL2_REG              (*(reg32 *) ScreenCounter_cy_m0s8_tcpwm_1__TR_CTRL2 )
#define ScreenCounter_TRIG_CONTROL2_PTR              ( (reg32 *) ScreenCounter_cy_m0s8_tcpwm_1__TR_CTRL2 )
#define ScreenCounter_INTERRUPT_REQ_REG              (*(reg32 *) ScreenCounter_cy_m0s8_tcpwm_1__INTR )
#define ScreenCounter_INTERRUPT_REQ_PTR              ( (reg32 *) ScreenCounter_cy_m0s8_tcpwm_1__INTR )
#define ScreenCounter_INTERRUPT_SET_REG              (*(reg32 *) ScreenCounter_cy_m0s8_tcpwm_1__INTR_SET )
#define ScreenCounter_INTERRUPT_SET_PTR              ( (reg32 *) ScreenCounter_cy_m0s8_tcpwm_1__INTR_SET )
#define ScreenCounter_INTERRUPT_MASK_REG             (*(reg32 *) ScreenCounter_cy_m0s8_tcpwm_1__INTR_MASK )
#define ScreenCounter_INTERRUPT_MASK_PTR             ( (reg32 *) ScreenCounter_cy_m0s8_tcpwm_1__INTR_MASK )
#define ScreenCounter_INTERRUPT_MASKED_REG           (*(reg32 *) ScreenCounter_cy_m0s8_tcpwm_1__INTR_MASKED )
#define ScreenCounter_INTERRUPT_MASKED_PTR           ( (reg32 *) ScreenCounter_cy_m0s8_tcpwm_1__INTR_MASKED )


/***************************************
*       Registers Constants
***************************************/

/* Mask */
#define ScreenCounter_MASK                           ((uint32)ScreenCounter_cy_m0s8_tcpwm_1__TCPWM_CTRL_MASK)

/* Shift constants for control register */
#define ScreenCounter_RELOAD_CC_SHIFT                (0u)
#define ScreenCounter_RELOAD_PERIOD_SHIFT            (1u)
#define ScreenCounter_PWM_SYNC_KILL_SHIFT            (2u)
#define ScreenCounter_PWM_STOP_KILL_SHIFT            (3u)
#define ScreenCounter_PRESCALER_SHIFT                (8u)
#define ScreenCounter_UPDOWN_SHIFT                   (16u)
#define ScreenCounter_ONESHOT_SHIFT                  (18u)
#define ScreenCounter_QUAD_MODE_SHIFT                (20u)
#define ScreenCounter_INV_OUT_SHIFT                  (20u)
#define ScreenCounter_INV_COMPL_OUT_SHIFT            (21u)
#define ScreenCounter_MODE_SHIFT                     (24u)

/* Mask constants for control register */
#define ScreenCounter_RELOAD_CC_MASK                 ((uint32)(ScreenCounter_1BIT_MASK        <<  \
                                                                            ScreenCounter_RELOAD_CC_SHIFT))
#define ScreenCounter_RELOAD_PERIOD_MASK             ((uint32)(ScreenCounter_1BIT_MASK        <<  \
                                                                            ScreenCounter_RELOAD_PERIOD_SHIFT))
#define ScreenCounter_PWM_SYNC_KILL_MASK             ((uint32)(ScreenCounter_1BIT_MASK        <<  \
                                                                            ScreenCounter_PWM_SYNC_KILL_SHIFT))
#define ScreenCounter_PWM_STOP_KILL_MASK             ((uint32)(ScreenCounter_1BIT_MASK        <<  \
                                                                            ScreenCounter_PWM_STOP_KILL_SHIFT))
#define ScreenCounter_PRESCALER_MASK                 ((uint32)(ScreenCounter_8BIT_MASK        <<  \
                                                                            ScreenCounter_PRESCALER_SHIFT))
#define ScreenCounter_UPDOWN_MASK                    ((uint32)(ScreenCounter_2BIT_MASK        <<  \
                                                                            ScreenCounter_UPDOWN_SHIFT))
#define ScreenCounter_ONESHOT_MASK                   ((uint32)(ScreenCounter_1BIT_MASK        <<  \
                                                                            ScreenCounter_ONESHOT_SHIFT))
#define ScreenCounter_QUAD_MODE_MASK                 ((uint32)(ScreenCounter_3BIT_MASK        <<  \
                                                                            ScreenCounter_QUAD_MODE_SHIFT))
#define ScreenCounter_INV_OUT_MASK                   ((uint32)(ScreenCounter_2BIT_MASK        <<  \
                                                                            ScreenCounter_INV_OUT_SHIFT))
#define ScreenCounter_MODE_MASK                      ((uint32)(ScreenCounter_3BIT_MASK        <<  \
                                                                            ScreenCounter_MODE_SHIFT))

/* Shift constants for trigger control register 1 */
#define ScreenCounter_CAPTURE_SHIFT                  (0u)
#define ScreenCounter_COUNT_SHIFT                    (2u)
#define ScreenCounter_RELOAD_SHIFT                   (4u)
#define ScreenCounter_STOP_SHIFT                     (6u)
#define ScreenCounter_START_SHIFT                    (8u)

/* Mask constants for trigger control register 1 */
#define ScreenCounter_CAPTURE_MASK                   ((uint32)(ScreenCounter_2BIT_MASK        <<  \
                                                                  ScreenCounter_CAPTURE_SHIFT))
#define ScreenCounter_COUNT_MASK                     ((uint32)(ScreenCounter_2BIT_MASK        <<  \
                                                                  ScreenCounter_COUNT_SHIFT))
#define ScreenCounter_RELOAD_MASK                    ((uint32)(ScreenCounter_2BIT_MASK        <<  \
                                                                  ScreenCounter_RELOAD_SHIFT))
#define ScreenCounter_STOP_MASK                      ((uint32)(ScreenCounter_2BIT_MASK        <<  \
                                                                  ScreenCounter_STOP_SHIFT))
#define ScreenCounter_START_MASK                     ((uint32)(ScreenCounter_2BIT_MASK        <<  \
                                                                  ScreenCounter_START_SHIFT))

/* MASK */
#define ScreenCounter_1BIT_MASK                      ((uint32)0x01u)
#define ScreenCounter_2BIT_MASK                      ((uint32)0x03u)
#define ScreenCounter_3BIT_MASK                      ((uint32)0x07u)
#define ScreenCounter_6BIT_MASK                      ((uint32)0x3Fu)
#define ScreenCounter_8BIT_MASK                      ((uint32)0xFFu)
#define ScreenCounter_16BIT_MASK                     ((uint32)0xFFFFu)

/* Shift constant for status register */
#define ScreenCounter_RUNNING_STATUS_SHIFT           (30u)


/***************************************
*    Initial Constants
***************************************/

#define ScreenCounter_CTRL_QUAD_BASE_CONFIG                                                          \
        (((uint32)(ScreenCounter_QUAD_ENCODING_MODES     << ScreenCounter_QUAD_MODE_SHIFT))       |\
         ((uint32)(ScreenCounter_CONFIG                  << ScreenCounter_MODE_SHIFT)))

#define ScreenCounter_CTRL_PWM_BASE_CONFIG                                                           \
        (((uint32)(ScreenCounter_PWM_STOP_EVENT          << ScreenCounter_PWM_STOP_KILL_SHIFT))   |\
         ((uint32)(ScreenCounter_PWM_OUT_INVERT          << ScreenCounter_INV_OUT_SHIFT))         |\
         ((uint32)(ScreenCounter_PWM_OUT_N_INVERT        << ScreenCounter_INV_COMPL_OUT_SHIFT))   |\
         ((uint32)(ScreenCounter_PWM_MODE                << ScreenCounter_MODE_SHIFT)))

#define ScreenCounter_CTRL_PWM_RUN_MODE                                                              \
            ((uint32)(ScreenCounter_PWM_RUN_MODE         << ScreenCounter_ONESHOT_SHIFT))
            
#define ScreenCounter_CTRL_PWM_ALIGN                                                                 \
            ((uint32)(ScreenCounter_PWM_ALIGN            << ScreenCounter_UPDOWN_SHIFT))

#define ScreenCounter_CTRL_PWM_KILL_EVENT                                                            \
             ((uint32)(ScreenCounter_PWM_KILL_EVENT      << ScreenCounter_PWM_SYNC_KILL_SHIFT))

#define ScreenCounter_CTRL_PWM_DEAD_TIME_CYCLE                                                       \
            ((uint32)(ScreenCounter_PWM_DEAD_TIME_CYCLE  << ScreenCounter_PRESCALER_SHIFT))

#define ScreenCounter_CTRL_PWM_PRESCALER                                                             \
            ((uint32)(ScreenCounter_PWM_PRESCALER        << ScreenCounter_PRESCALER_SHIFT))

#define ScreenCounter_CTRL_TIMER_BASE_CONFIG                                                         \
        (((uint32)(ScreenCounter_TC_PRESCALER            << ScreenCounter_PRESCALER_SHIFT))       |\
         ((uint32)(ScreenCounter_TC_COUNTER_MODE         << ScreenCounter_UPDOWN_SHIFT))          |\
         ((uint32)(ScreenCounter_TC_RUN_MODE             << ScreenCounter_ONESHOT_SHIFT))         |\
         ((uint32)(ScreenCounter_TC_COMP_CAP_MODE        << ScreenCounter_MODE_SHIFT)))
        
#define ScreenCounter_QUAD_SIGNALS_MODES                                                             \
        (((uint32)(ScreenCounter_QUAD_PHIA_SIGNAL_MODE   << ScreenCounter_COUNT_SHIFT))           |\
         ((uint32)(ScreenCounter_QUAD_INDEX_SIGNAL_MODE  << ScreenCounter_RELOAD_SHIFT))          |\
         ((uint32)(ScreenCounter_QUAD_STOP_SIGNAL_MODE   << ScreenCounter_STOP_SHIFT))            |\
         ((uint32)(ScreenCounter_QUAD_PHIB_SIGNAL_MODE   << ScreenCounter_START_SHIFT)))

#define ScreenCounter_PWM_SIGNALS_MODES                                                              \
        (((uint32)(ScreenCounter_PWM_SWITCH_SIGNAL_MODE  << ScreenCounter_CAPTURE_SHIFT))         |\
         ((uint32)(ScreenCounter_PWM_COUNT_SIGNAL_MODE   << ScreenCounter_COUNT_SHIFT))           |\
         ((uint32)(ScreenCounter_PWM_RELOAD_SIGNAL_MODE  << ScreenCounter_RELOAD_SHIFT))          |\
         ((uint32)(ScreenCounter_PWM_STOP_SIGNAL_MODE    << ScreenCounter_STOP_SHIFT))            |\
         ((uint32)(ScreenCounter_PWM_START_SIGNAL_MODE   << ScreenCounter_START_SHIFT)))

#define ScreenCounter_TIMER_SIGNALS_MODES                                                            \
        (((uint32)(ScreenCounter_TC_CAPTURE_SIGNAL_MODE  << ScreenCounter_CAPTURE_SHIFT))         |\
         ((uint32)(ScreenCounter_TC_COUNT_SIGNAL_MODE    << ScreenCounter_COUNT_SHIFT))           |\
         ((uint32)(ScreenCounter_TC_RELOAD_SIGNAL_MODE   << ScreenCounter_RELOAD_SHIFT))          |\
         ((uint32)(ScreenCounter_TC_STOP_SIGNAL_MODE     << ScreenCounter_STOP_SHIFT))            |\
         ((uint32)(ScreenCounter_TC_START_SIGNAL_MODE    << ScreenCounter_START_SHIFT)))
        
#define ScreenCounter_TIMER_UPDOWN_CNT_USED                                                          \
                ((ScreenCounter__COUNT_UPDOWN0 == ScreenCounter_TC_COUNTER_MODE)                  ||\
                 (ScreenCounter__COUNT_UPDOWN1 == ScreenCounter_TC_COUNTER_MODE))

#define ScreenCounter_PWM_UPDOWN_CNT_USED                                                            \
                ((ScreenCounter__CENTER == ScreenCounter_PWM_ALIGN)                               ||\
                 (ScreenCounter__ASYMMETRIC == ScreenCounter_PWM_ALIGN))               
        
#define ScreenCounter_PWM_PR_INIT_VALUE              (1u)
#define ScreenCounter_QUAD_PERIOD_INIT_VALUE         (0x8000u)



#endif /* End CY_TCPWM_ScreenCounter_H */

/* [] END OF FILE */
