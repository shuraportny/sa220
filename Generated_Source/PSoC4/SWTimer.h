/*******************************************************************************
* File Name: SWTimer.h
* Version 2.10
*
* Description:
*  This file provides constants and parameter values for the SWTimer
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

#if !defined(CY_TCPWM_SWTimer_H)
#define CY_TCPWM_SWTimer_H


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
} SWTimer_BACKUP_STRUCT;


/*******************************************************************************
* Variables
*******************************************************************************/
extern uint8  SWTimer_initVar;


/***************************************
*   Conditional Compilation Parameters
****************************************/

#define SWTimer_CY_TCPWM_V2                    (CYIPBLOCK_m0s8tcpwm_VERSION == 2u)
#define SWTimer_CY_TCPWM_4000                  (CY_PSOC4_4000)

/* TCPWM Configuration */
#define SWTimer_CONFIG                         (1lu)

/* Quad Mode */
/* Parameters */
#define SWTimer_QUAD_ENCODING_MODES            (0lu)
#define SWTimer_QUAD_AUTO_START                (1lu)

/* Signal modes */
#define SWTimer_QUAD_INDEX_SIGNAL_MODE         (0lu)
#define SWTimer_QUAD_PHIA_SIGNAL_MODE          (3lu)
#define SWTimer_QUAD_PHIB_SIGNAL_MODE          (3lu)
#define SWTimer_QUAD_STOP_SIGNAL_MODE          (0lu)

/* Signal present */
#define SWTimer_QUAD_INDEX_SIGNAL_PRESENT      (0lu)
#define SWTimer_QUAD_STOP_SIGNAL_PRESENT       (0lu)

/* Interrupt Mask */
#define SWTimer_QUAD_INTERRUPT_MASK            (1lu)

/* Timer/Counter Mode */
/* Parameters */
#define SWTimer_TC_RUN_MODE                    (0lu)
#define SWTimer_TC_COUNTER_MODE                (0lu)
#define SWTimer_TC_COMP_CAP_MODE               (2lu)
#define SWTimer_TC_PRESCALER                   (0lu)

/* Signal modes */
#define SWTimer_TC_RELOAD_SIGNAL_MODE          (0lu)
#define SWTimer_TC_COUNT_SIGNAL_MODE           (3lu)
#define SWTimer_TC_START_SIGNAL_MODE           (0lu)
#define SWTimer_TC_STOP_SIGNAL_MODE            (0lu)
#define SWTimer_TC_CAPTURE_SIGNAL_MODE         (0lu)

/* Signal present */
#define SWTimer_TC_RELOAD_SIGNAL_PRESENT       (0lu)
#define SWTimer_TC_COUNT_SIGNAL_PRESENT        (0lu)
#define SWTimer_TC_START_SIGNAL_PRESENT        (0lu)
#define SWTimer_TC_STOP_SIGNAL_PRESENT         (0lu)
#define SWTimer_TC_CAPTURE_SIGNAL_PRESENT      (0lu)

/* Interrupt Mask */
#define SWTimer_TC_INTERRUPT_MASK              (1lu)

/* PWM Mode */
/* Parameters */
#define SWTimer_PWM_KILL_EVENT                 (0lu)
#define SWTimer_PWM_STOP_EVENT                 (0lu)
#define SWTimer_PWM_MODE                       (4lu)
#define SWTimer_PWM_OUT_N_INVERT               (0lu)
#define SWTimer_PWM_OUT_INVERT                 (0lu)
#define SWTimer_PWM_ALIGN                      (0lu)
#define SWTimer_PWM_RUN_MODE                   (0lu)
#define SWTimer_PWM_DEAD_TIME_CYCLE            (0lu)
#define SWTimer_PWM_PRESCALER                  (0lu)

/* Signal modes */
#define SWTimer_PWM_RELOAD_SIGNAL_MODE         (0lu)
#define SWTimer_PWM_COUNT_SIGNAL_MODE          (3lu)
#define SWTimer_PWM_START_SIGNAL_MODE          (0lu)
#define SWTimer_PWM_STOP_SIGNAL_MODE           (0lu)
#define SWTimer_PWM_SWITCH_SIGNAL_MODE         (0lu)

/* Signal present */
#define SWTimer_PWM_RELOAD_SIGNAL_PRESENT      (0lu)
#define SWTimer_PWM_COUNT_SIGNAL_PRESENT       (0lu)
#define SWTimer_PWM_START_SIGNAL_PRESENT       (0lu)
#define SWTimer_PWM_STOP_SIGNAL_PRESENT        (0lu)
#define SWTimer_PWM_SWITCH_SIGNAL_PRESENT      (0lu)

/* Interrupt Mask */
#define SWTimer_PWM_INTERRUPT_MASK             (1lu)


/***************************************
*    Initial Parameter Constants
***************************************/

/* Timer/Counter Mode */
#define SWTimer_TC_PERIOD_VALUE                (65535lu)
#define SWTimer_TC_COMPARE_VALUE               (65535lu)
#define SWTimer_TC_COMPARE_BUF_VALUE           (65535lu)
#define SWTimer_TC_COMPARE_SWAP                (0lu)

/* PWM Mode */
#define SWTimer_PWM_PERIOD_VALUE               (65535lu)
#define SWTimer_PWM_PERIOD_BUF_VALUE           (65535lu)
#define SWTimer_PWM_PERIOD_SWAP                (0lu)
#define SWTimer_PWM_COMPARE_VALUE              (65535lu)
#define SWTimer_PWM_COMPARE_BUF_VALUE          (65535lu)
#define SWTimer_PWM_COMPARE_SWAP               (0lu)


/***************************************
*    Enumerated Types and Parameters
***************************************/

#define SWTimer__LEFT 0
#define SWTimer__RIGHT 1
#define SWTimer__CENTER 2
#define SWTimer__ASYMMETRIC 3

#define SWTimer__X1 0
#define SWTimer__X2 1
#define SWTimer__X4 2

#define SWTimer__PWM 4
#define SWTimer__PWM_DT 5
#define SWTimer__PWM_PR 6

#define SWTimer__INVERSE 1
#define SWTimer__DIRECT 0

#define SWTimer__CAPTURE 2
#define SWTimer__COMPARE 0

#define SWTimer__TRIG_LEVEL 3
#define SWTimer__TRIG_RISING 0
#define SWTimer__TRIG_FALLING 1
#define SWTimer__TRIG_BOTH 2

#define SWTimer__INTR_MASK_TC 1
#define SWTimer__INTR_MASK_CC_MATCH 2
#define SWTimer__INTR_MASK_NONE 0
#define SWTimer__INTR_MASK_TC_CC 3

#define SWTimer__UNCONFIG 8
#define SWTimer__TIMER 1
#define SWTimer__QUAD 3
#define SWTimer__PWM_SEL 7

#define SWTimer__COUNT_UP 0
#define SWTimer__COUNT_DOWN 1
#define SWTimer__COUNT_UPDOWN0 2
#define SWTimer__COUNT_UPDOWN1 3


/* Prescaler */
#define SWTimer_PRESCALE_DIVBY1                ((uint32)(0u << SWTimer_PRESCALER_SHIFT))
#define SWTimer_PRESCALE_DIVBY2                ((uint32)(1u << SWTimer_PRESCALER_SHIFT))
#define SWTimer_PRESCALE_DIVBY4                ((uint32)(2u << SWTimer_PRESCALER_SHIFT))
#define SWTimer_PRESCALE_DIVBY8                ((uint32)(3u << SWTimer_PRESCALER_SHIFT))
#define SWTimer_PRESCALE_DIVBY16               ((uint32)(4u << SWTimer_PRESCALER_SHIFT))
#define SWTimer_PRESCALE_DIVBY32               ((uint32)(5u << SWTimer_PRESCALER_SHIFT))
#define SWTimer_PRESCALE_DIVBY64               ((uint32)(6u << SWTimer_PRESCALER_SHIFT))
#define SWTimer_PRESCALE_DIVBY128              ((uint32)(7u << SWTimer_PRESCALER_SHIFT))

/* TCPWM set modes */
#define SWTimer_MODE_TIMER_COMPARE             ((uint32)(SWTimer__COMPARE         <<  \
                                                                  SWTimer_MODE_SHIFT))
#define SWTimer_MODE_TIMER_CAPTURE             ((uint32)(SWTimer__CAPTURE         <<  \
                                                                  SWTimer_MODE_SHIFT))
#define SWTimer_MODE_QUAD                      ((uint32)(SWTimer__QUAD            <<  \
                                                                  SWTimer_MODE_SHIFT))
#define SWTimer_MODE_PWM                       ((uint32)(SWTimer__PWM             <<  \
                                                                  SWTimer_MODE_SHIFT))
#define SWTimer_MODE_PWM_DT                    ((uint32)(SWTimer__PWM_DT          <<  \
                                                                  SWTimer_MODE_SHIFT))
#define SWTimer_MODE_PWM_PR                    ((uint32)(SWTimer__PWM_PR          <<  \
                                                                  SWTimer_MODE_SHIFT))

/* Quad Modes */
#define SWTimer_MODE_X1                        ((uint32)(SWTimer__X1              <<  \
                                                                  SWTimer_QUAD_MODE_SHIFT))
#define SWTimer_MODE_X2                        ((uint32)(SWTimer__X2              <<  \
                                                                  SWTimer_QUAD_MODE_SHIFT))
#define SWTimer_MODE_X4                        ((uint32)(SWTimer__X4              <<  \
                                                                  SWTimer_QUAD_MODE_SHIFT))

/* Counter modes */
#define SWTimer_COUNT_UP                       ((uint32)(SWTimer__COUNT_UP        <<  \
                                                                  SWTimer_UPDOWN_SHIFT))
#define SWTimer_COUNT_DOWN                     ((uint32)(SWTimer__COUNT_DOWN      <<  \
                                                                  SWTimer_UPDOWN_SHIFT))
#define SWTimer_COUNT_UPDOWN0                  ((uint32)(SWTimer__COUNT_UPDOWN0   <<  \
                                                                  SWTimer_UPDOWN_SHIFT))
#define SWTimer_COUNT_UPDOWN1                  ((uint32)(SWTimer__COUNT_UPDOWN1   <<  \
                                                                  SWTimer_UPDOWN_SHIFT))

/* PWM output invert */
#define SWTimer_INVERT_LINE                    ((uint32)(SWTimer__INVERSE         <<  \
                                                                  SWTimer_INV_OUT_SHIFT))
#define SWTimer_INVERT_LINE_N                  ((uint32)(SWTimer__INVERSE         <<  \
                                                                  SWTimer_INV_COMPL_OUT_SHIFT))

/* Trigger modes */
#define SWTimer_TRIG_RISING                    ((uint32)SWTimer__TRIG_RISING)
#define SWTimer_TRIG_FALLING                   ((uint32)SWTimer__TRIG_FALLING)
#define SWTimer_TRIG_BOTH                      ((uint32)SWTimer__TRIG_BOTH)
#define SWTimer_TRIG_LEVEL                     ((uint32)SWTimer__TRIG_LEVEL)

/* Interrupt mask */
#define SWTimer_INTR_MASK_TC                   ((uint32)SWTimer__INTR_MASK_TC)
#define SWTimer_INTR_MASK_CC_MATCH             ((uint32)SWTimer__INTR_MASK_CC_MATCH)

/* PWM Output Controls */
#define SWTimer_CC_MATCH_SET                   (0x00u)
#define SWTimer_CC_MATCH_CLEAR                 (0x01u)
#define SWTimer_CC_MATCH_INVERT                (0x02u)
#define SWTimer_CC_MATCH_NO_CHANGE             (0x03u)
#define SWTimer_OVERLOW_SET                    (0x00u)
#define SWTimer_OVERLOW_CLEAR                  (0x04u)
#define SWTimer_OVERLOW_INVERT                 (0x08u)
#define SWTimer_OVERLOW_NO_CHANGE              (0x0Cu)
#define SWTimer_UNDERFLOW_SET                  (0x00u)
#define SWTimer_UNDERFLOW_CLEAR                (0x10u)
#define SWTimer_UNDERFLOW_INVERT               (0x20u)
#define SWTimer_UNDERFLOW_NO_CHANGE            (0x30u)

/* PWM Align */
#define SWTimer_PWM_MODE_LEFT                  (SWTimer_CC_MATCH_CLEAR        |   \
                                                         SWTimer_OVERLOW_SET           |   \
                                                         SWTimer_UNDERFLOW_NO_CHANGE)
#define SWTimer_PWM_MODE_RIGHT                 (SWTimer_CC_MATCH_SET          |   \
                                                         SWTimer_OVERLOW_NO_CHANGE     |   \
                                                         SWTimer_UNDERFLOW_CLEAR)
#define SWTimer_PWM_MODE_ASYM                  (SWTimer_CC_MATCH_INVERT       |   \
                                                         SWTimer_OVERLOW_SET           |   \
                                                         SWTimer_UNDERFLOW_CLEAR)

#if (SWTimer_CY_TCPWM_V2)
    #if(SWTimer_CY_TCPWM_4000)
        #define SWTimer_PWM_MODE_CENTER                (SWTimer_CC_MATCH_INVERT       |   \
                                                                 SWTimer_OVERLOW_NO_CHANGE     |   \
                                                                 SWTimer_UNDERFLOW_CLEAR)
    #else
        #define SWTimer_PWM_MODE_CENTER                (SWTimer_CC_MATCH_INVERT       |   \
                                                                 SWTimer_OVERLOW_SET           |   \
                                                                 SWTimer_UNDERFLOW_CLEAR)
    #endif /* (SWTimer_CY_TCPWM_4000) */
#else
    #define SWTimer_PWM_MODE_CENTER                (SWTimer_CC_MATCH_INVERT       |   \
                                                             SWTimer_OVERLOW_NO_CHANGE     |   \
                                                             SWTimer_UNDERFLOW_CLEAR)
#endif /* (SWTimer_CY_TCPWM_NEW) */

/* Command operations without condition */
#define SWTimer_CMD_CAPTURE                    (0u)
#define SWTimer_CMD_RELOAD                     (8u)
#define SWTimer_CMD_STOP                       (16u)
#define SWTimer_CMD_START                      (24u)

/* Status */
#define SWTimer_STATUS_DOWN                    (1u)
#define SWTimer_STATUS_RUNNING                 (2u)


/***************************************
*        Function Prototypes
****************************************/

void   SWTimer_Init(void);
void   SWTimer_Enable(void);
void   SWTimer_Start(void);
void   SWTimer_Stop(void);

void   SWTimer_SetMode(uint32 mode);
void   SWTimer_SetCounterMode(uint32 counterMode);
void   SWTimer_SetPWMMode(uint32 modeMask);
void   SWTimer_SetQDMode(uint32 qdMode);

void   SWTimer_SetPrescaler(uint32 prescaler);
void   SWTimer_TriggerCommand(uint32 mask, uint32 command);
void   SWTimer_SetOneShot(uint32 oneShotEnable);
uint32 SWTimer_ReadStatus(void);

void   SWTimer_SetPWMSyncKill(uint32 syncKillEnable);
void   SWTimer_SetPWMStopOnKill(uint32 stopOnKillEnable);
void   SWTimer_SetPWMDeadTime(uint32 deadTime);
void   SWTimer_SetPWMInvert(uint32 mask);

void   SWTimer_SetInterruptMode(uint32 interruptMask);
uint32 SWTimer_GetInterruptSourceMasked(void);
uint32 SWTimer_GetInterruptSource(void);
void   SWTimer_ClearInterrupt(uint32 interruptMask);
void   SWTimer_SetInterrupt(uint32 interruptMask);

void   SWTimer_WriteCounter(uint32 count);
uint32 SWTimer_ReadCounter(void);

uint32 SWTimer_ReadCapture(void);
uint32 SWTimer_ReadCaptureBuf(void);

void   SWTimer_WritePeriod(uint32 period);
uint32 SWTimer_ReadPeriod(void);
void   SWTimer_WritePeriodBuf(uint32 periodBuf);
uint32 SWTimer_ReadPeriodBuf(void);

void   SWTimer_WriteCompare(uint32 compare);
uint32 SWTimer_ReadCompare(void);
void   SWTimer_WriteCompareBuf(uint32 compareBuf);
uint32 SWTimer_ReadCompareBuf(void);

void   SWTimer_SetPeriodSwap(uint32 swapEnable);
void   SWTimer_SetCompareSwap(uint32 swapEnable);

void   SWTimer_SetCaptureMode(uint32 triggerMode);
void   SWTimer_SetReloadMode(uint32 triggerMode);
void   SWTimer_SetStartMode(uint32 triggerMode);
void   SWTimer_SetStopMode(uint32 triggerMode);
void   SWTimer_SetCountMode(uint32 triggerMode);

void   SWTimer_SaveConfig(void);
void   SWTimer_RestoreConfig(void);
void   SWTimer_Sleep(void);
void   SWTimer_Wakeup(void);


/***************************************
*             Registers
***************************************/

#define SWTimer_BLOCK_CONTROL_REG              (*(reg32 *) SWTimer_cy_m0s8_tcpwm_1__TCPWM_CTRL )
#define SWTimer_BLOCK_CONTROL_PTR              ( (reg32 *) SWTimer_cy_m0s8_tcpwm_1__TCPWM_CTRL )
#define SWTimer_COMMAND_REG                    (*(reg32 *) SWTimer_cy_m0s8_tcpwm_1__TCPWM_CMD )
#define SWTimer_COMMAND_PTR                    ( (reg32 *) SWTimer_cy_m0s8_tcpwm_1__TCPWM_CMD )
#define SWTimer_INTRRUPT_CAUSE_REG             (*(reg32 *) SWTimer_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE )
#define SWTimer_INTRRUPT_CAUSE_PTR             ( (reg32 *) SWTimer_cy_m0s8_tcpwm_1__TCPWM_INTR_CAUSE )
#define SWTimer_CONTROL_REG                    (*(reg32 *) SWTimer_cy_m0s8_tcpwm_1__CTRL )
#define SWTimer_CONTROL_PTR                    ( (reg32 *) SWTimer_cy_m0s8_tcpwm_1__CTRL )
#define SWTimer_STATUS_REG                     (*(reg32 *) SWTimer_cy_m0s8_tcpwm_1__STATUS )
#define SWTimer_STATUS_PTR                     ( (reg32 *) SWTimer_cy_m0s8_tcpwm_1__STATUS )
#define SWTimer_COUNTER_REG                    (*(reg32 *) SWTimer_cy_m0s8_tcpwm_1__COUNTER )
#define SWTimer_COUNTER_PTR                    ( (reg32 *) SWTimer_cy_m0s8_tcpwm_1__COUNTER )
#define SWTimer_COMP_CAP_REG                   (*(reg32 *) SWTimer_cy_m0s8_tcpwm_1__CC )
#define SWTimer_COMP_CAP_PTR                   ( (reg32 *) SWTimer_cy_m0s8_tcpwm_1__CC )
#define SWTimer_COMP_CAP_BUF_REG               (*(reg32 *) SWTimer_cy_m0s8_tcpwm_1__CC_BUFF )
#define SWTimer_COMP_CAP_BUF_PTR               ( (reg32 *) SWTimer_cy_m0s8_tcpwm_1__CC_BUFF )
#define SWTimer_PERIOD_REG                     (*(reg32 *) SWTimer_cy_m0s8_tcpwm_1__PERIOD )
#define SWTimer_PERIOD_PTR                     ( (reg32 *) SWTimer_cy_m0s8_tcpwm_1__PERIOD )
#define SWTimer_PERIOD_BUF_REG                 (*(reg32 *) SWTimer_cy_m0s8_tcpwm_1__PERIOD_BUFF )
#define SWTimer_PERIOD_BUF_PTR                 ( (reg32 *) SWTimer_cy_m0s8_tcpwm_1__PERIOD_BUFF )
#define SWTimer_TRIG_CONTROL0_REG              (*(reg32 *) SWTimer_cy_m0s8_tcpwm_1__TR_CTRL0 )
#define SWTimer_TRIG_CONTROL0_PTR              ( (reg32 *) SWTimer_cy_m0s8_tcpwm_1__TR_CTRL0 )
#define SWTimer_TRIG_CONTROL1_REG              (*(reg32 *) SWTimer_cy_m0s8_tcpwm_1__TR_CTRL1 )
#define SWTimer_TRIG_CONTROL1_PTR              ( (reg32 *) SWTimer_cy_m0s8_tcpwm_1__TR_CTRL1 )
#define SWTimer_TRIG_CONTROL2_REG              (*(reg32 *) SWTimer_cy_m0s8_tcpwm_1__TR_CTRL2 )
#define SWTimer_TRIG_CONTROL2_PTR              ( (reg32 *) SWTimer_cy_m0s8_tcpwm_1__TR_CTRL2 )
#define SWTimer_INTERRUPT_REQ_REG              (*(reg32 *) SWTimer_cy_m0s8_tcpwm_1__INTR )
#define SWTimer_INTERRUPT_REQ_PTR              ( (reg32 *) SWTimer_cy_m0s8_tcpwm_1__INTR )
#define SWTimer_INTERRUPT_SET_REG              (*(reg32 *) SWTimer_cy_m0s8_tcpwm_1__INTR_SET )
#define SWTimer_INTERRUPT_SET_PTR              ( (reg32 *) SWTimer_cy_m0s8_tcpwm_1__INTR_SET )
#define SWTimer_INTERRUPT_MASK_REG             (*(reg32 *) SWTimer_cy_m0s8_tcpwm_1__INTR_MASK )
#define SWTimer_INTERRUPT_MASK_PTR             ( (reg32 *) SWTimer_cy_m0s8_tcpwm_1__INTR_MASK )
#define SWTimer_INTERRUPT_MASKED_REG           (*(reg32 *) SWTimer_cy_m0s8_tcpwm_1__INTR_MASKED )
#define SWTimer_INTERRUPT_MASKED_PTR           ( (reg32 *) SWTimer_cy_m0s8_tcpwm_1__INTR_MASKED )


/***************************************
*       Registers Constants
***************************************/

/* Mask */
#define SWTimer_MASK                           ((uint32)SWTimer_cy_m0s8_tcpwm_1__TCPWM_CTRL_MASK)

/* Shift constants for control register */
#define SWTimer_RELOAD_CC_SHIFT                (0u)
#define SWTimer_RELOAD_PERIOD_SHIFT            (1u)
#define SWTimer_PWM_SYNC_KILL_SHIFT            (2u)
#define SWTimer_PWM_STOP_KILL_SHIFT            (3u)
#define SWTimer_PRESCALER_SHIFT                (8u)
#define SWTimer_UPDOWN_SHIFT                   (16u)
#define SWTimer_ONESHOT_SHIFT                  (18u)
#define SWTimer_QUAD_MODE_SHIFT                (20u)
#define SWTimer_INV_OUT_SHIFT                  (20u)
#define SWTimer_INV_COMPL_OUT_SHIFT            (21u)
#define SWTimer_MODE_SHIFT                     (24u)

/* Mask constants for control register */
#define SWTimer_RELOAD_CC_MASK                 ((uint32)(SWTimer_1BIT_MASK        <<  \
                                                                            SWTimer_RELOAD_CC_SHIFT))
#define SWTimer_RELOAD_PERIOD_MASK             ((uint32)(SWTimer_1BIT_MASK        <<  \
                                                                            SWTimer_RELOAD_PERIOD_SHIFT))
#define SWTimer_PWM_SYNC_KILL_MASK             ((uint32)(SWTimer_1BIT_MASK        <<  \
                                                                            SWTimer_PWM_SYNC_KILL_SHIFT))
#define SWTimer_PWM_STOP_KILL_MASK             ((uint32)(SWTimer_1BIT_MASK        <<  \
                                                                            SWTimer_PWM_STOP_KILL_SHIFT))
#define SWTimer_PRESCALER_MASK                 ((uint32)(SWTimer_8BIT_MASK        <<  \
                                                                            SWTimer_PRESCALER_SHIFT))
#define SWTimer_UPDOWN_MASK                    ((uint32)(SWTimer_2BIT_MASK        <<  \
                                                                            SWTimer_UPDOWN_SHIFT))
#define SWTimer_ONESHOT_MASK                   ((uint32)(SWTimer_1BIT_MASK        <<  \
                                                                            SWTimer_ONESHOT_SHIFT))
#define SWTimer_QUAD_MODE_MASK                 ((uint32)(SWTimer_3BIT_MASK        <<  \
                                                                            SWTimer_QUAD_MODE_SHIFT))
#define SWTimer_INV_OUT_MASK                   ((uint32)(SWTimer_2BIT_MASK        <<  \
                                                                            SWTimer_INV_OUT_SHIFT))
#define SWTimer_MODE_MASK                      ((uint32)(SWTimer_3BIT_MASK        <<  \
                                                                            SWTimer_MODE_SHIFT))

/* Shift constants for trigger control register 1 */
#define SWTimer_CAPTURE_SHIFT                  (0u)
#define SWTimer_COUNT_SHIFT                    (2u)
#define SWTimer_RELOAD_SHIFT                   (4u)
#define SWTimer_STOP_SHIFT                     (6u)
#define SWTimer_START_SHIFT                    (8u)

/* Mask constants for trigger control register 1 */
#define SWTimer_CAPTURE_MASK                   ((uint32)(SWTimer_2BIT_MASK        <<  \
                                                                  SWTimer_CAPTURE_SHIFT))
#define SWTimer_COUNT_MASK                     ((uint32)(SWTimer_2BIT_MASK        <<  \
                                                                  SWTimer_COUNT_SHIFT))
#define SWTimer_RELOAD_MASK                    ((uint32)(SWTimer_2BIT_MASK        <<  \
                                                                  SWTimer_RELOAD_SHIFT))
#define SWTimer_STOP_MASK                      ((uint32)(SWTimer_2BIT_MASK        <<  \
                                                                  SWTimer_STOP_SHIFT))
#define SWTimer_START_MASK                     ((uint32)(SWTimer_2BIT_MASK        <<  \
                                                                  SWTimer_START_SHIFT))

/* MASK */
#define SWTimer_1BIT_MASK                      ((uint32)0x01u)
#define SWTimer_2BIT_MASK                      ((uint32)0x03u)
#define SWTimer_3BIT_MASK                      ((uint32)0x07u)
#define SWTimer_6BIT_MASK                      ((uint32)0x3Fu)
#define SWTimer_8BIT_MASK                      ((uint32)0xFFu)
#define SWTimer_16BIT_MASK                     ((uint32)0xFFFFu)

/* Shift constant for status register */
#define SWTimer_RUNNING_STATUS_SHIFT           (30u)


/***************************************
*    Initial Constants
***************************************/

#define SWTimer_CTRL_QUAD_BASE_CONFIG                                                          \
        (((uint32)(SWTimer_QUAD_ENCODING_MODES     << SWTimer_QUAD_MODE_SHIFT))       |\
         ((uint32)(SWTimer_CONFIG                  << SWTimer_MODE_SHIFT)))

#define SWTimer_CTRL_PWM_BASE_CONFIG                                                           \
        (((uint32)(SWTimer_PWM_STOP_EVENT          << SWTimer_PWM_STOP_KILL_SHIFT))   |\
         ((uint32)(SWTimer_PWM_OUT_INVERT          << SWTimer_INV_OUT_SHIFT))         |\
         ((uint32)(SWTimer_PWM_OUT_N_INVERT        << SWTimer_INV_COMPL_OUT_SHIFT))   |\
         ((uint32)(SWTimer_PWM_MODE                << SWTimer_MODE_SHIFT)))

#define SWTimer_CTRL_PWM_RUN_MODE                                                              \
            ((uint32)(SWTimer_PWM_RUN_MODE         << SWTimer_ONESHOT_SHIFT))
            
#define SWTimer_CTRL_PWM_ALIGN                                                                 \
            ((uint32)(SWTimer_PWM_ALIGN            << SWTimer_UPDOWN_SHIFT))

#define SWTimer_CTRL_PWM_KILL_EVENT                                                            \
             ((uint32)(SWTimer_PWM_KILL_EVENT      << SWTimer_PWM_SYNC_KILL_SHIFT))

#define SWTimer_CTRL_PWM_DEAD_TIME_CYCLE                                                       \
            ((uint32)(SWTimer_PWM_DEAD_TIME_CYCLE  << SWTimer_PRESCALER_SHIFT))

#define SWTimer_CTRL_PWM_PRESCALER                                                             \
            ((uint32)(SWTimer_PWM_PRESCALER        << SWTimer_PRESCALER_SHIFT))

#define SWTimer_CTRL_TIMER_BASE_CONFIG                                                         \
        (((uint32)(SWTimer_TC_PRESCALER            << SWTimer_PRESCALER_SHIFT))       |\
         ((uint32)(SWTimer_TC_COUNTER_MODE         << SWTimer_UPDOWN_SHIFT))          |\
         ((uint32)(SWTimer_TC_RUN_MODE             << SWTimer_ONESHOT_SHIFT))         |\
         ((uint32)(SWTimer_TC_COMP_CAP_MODE        << SWTimer_MODE_SHIFT)))
        
#define SWTimer_QUAD_SIGNALS_MODES                                                             \
        (((uint32)(SWTimer_QUAD_PHIA_SIGNAL_MODE   << SWTimer_COUNT_SHIFT))           |\
         ((uint32)(SWTimer_QUAD_INDEX_SIGNAL_MODE  << SWTimer_RELOAD_SHIFT))          |\
         ((uint32)(SWTimer_QUAD_STOP_SIGNAL_MODE   << SWTimer_STOP_SHIFT))            |\
         ((uint32)(SWTimer_QUAD_PHIB_SIGNAL_MODE   << SWTimer_START_SHIFT)))

#define SWTimer_PWM_SIGNALS_MODES                                                              \
        (((uint32)(SWTimer_PWM_SWITCH_SIGNAL_MODE  << SWTimer_CAPTURE_SHIFT))         |\
         ((uint32)(SWTimer_PWM_COUNT_SIGNAL_MODE   << SWTimer_COUNT_SHIFT))           |\
         ((uint32)(SWTimer_PWM_RELOAD_SIGNAL_MODE  << SWTimer_RELOAD_SHIFT))          |\
         ((uint32)(SWTimer_PWM_STOP_SIGNAL_MODE    << SWTimer_STOP_SHIFT))            |\
         ((uint32)(SWTimer_PWM_START_SIGNAL_MODE   << SWTimer_START_SHIFT)))

#define SWTimer_TIMER_SIGNALS_MODES                                                            \
        (((uint32)(SWTimer_TC_CAPTURE_SIGNAL_MODE  << SWTimer_CAPTURE_SHIFT))         |\
         ((uint32)(SWTimer_TC_COUNT_SIGNAL_MODE    << SWTimer_COUNT_SHIFT))           |\
         ((uint32)(SWTimer_TC_RELOAD_SIGNAL_MODE   << SWTimer_RELOAD_SHIFT))          |\
         ((uint32)(SWTimer_TC_STOP_SIGNAL_MODE     << SWTimer_STOP_SHIFT))            |\
         ((uint32)(SWTimer_TC_START_SIGNAL_MODE    << SWTimer_START_SHIFT)))
        
#define SWTimer_TIMER_UPDOWN_CNT_USED                                                          \
                ((SWTimer__COUNT_UPDOWN0 == SWTimer_TC_COUNTER_MODE)                  ||\
                 (SWTimer__COUNT_UPDOWN1 == SWTimer_TC_COUNTER_MODE))

#define SWTimer_PWM_UPDOWN_CNT_USED                                                            \
                ((SWTimer__CENTER == SWTimer_PWM_ALIGN)                               ||\
                 (SWTimer__ASYMMETRIC == SWTimer_PWM_ALIGN))               
        
#define SWTimer_PWM_PR_INIT_VALUE              (1u)
#define SWTimer_QUAD_PERIOD_INIT_VALUE         (0x8000u)



#endif /* End CY_TCPWM_SWTimer_H */

/* [] END OF FILE */
