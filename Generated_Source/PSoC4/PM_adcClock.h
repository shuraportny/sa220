/*******************************************************************************
* File Name: PM_adcClock.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_PM_adcClock_H)
#define CY_CLOCK_PM_adcClock_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void PM_adcClock_StartEx(uint32 alignClkDiv);
#define PM_adcClock_Start() \
    PM_adcClock_StartEx(PM_adcClock__PA_DIV_ID)

#else

void PM_adcClock_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void PM_adcClock_Stop(void);

void PM_adcClock_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 PM_adcClock_GetDividerRegister(void);
uint8  PM_adcClock_GetFractionalDividerRegister(void);

#define PM_adcClock_Enable()                         PM_adcClock_Start()
#define PM_adcClock_Disable()                        PM_adcClock_Stop()
#define PM_adcClock_SetDividerRegister(clkDivider, reset)  \
    PM_adcClock_SetFractionalDividerRegister((clkDivider), 0u)
#define PM_adcClock_SetDivider(clkDivider)           PM_adcClock_SetDividerRegister((clkDivider), 1u)
#define PM_adcClock_SetDividerValue(clkDivider)      PM_adcClock_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define PM_adcClock_DIV_ID     PM_adcClock__DIV_ID

#define PM_adcClock_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define PM_adcClock_CTRL_REG   (*(reg32 *)PM_adcClock__CTRL_REGISTER)
#define PM_adcClock_DIV_REG    (*(reg32 *)PM_adcClock__DIV_REGISTER)

#define PM_adcClock_CMD_DIV_SHIFT          (0u)
#define PM_adcClock_CMD_PA_DIV_SHIFT       (8u)
#define PM_adcClock_CMD_DISABLE_SHIFT      (30u)
#define PM_adcClock_CMD_ENABLE_SHIFT       (31u)

#define PM_adcClock_CMD_DISABLE_MASK       ((uint32)((uint32)1u << PM_adcClock_CMD_DISABLE_SHIFT))
#define PM_adcClock_CMD_ENABLE_MASK        ((uint32)((uint32)1u << PM_adcClock_CMD_ENABLE_SHIFT))

#define PM_adcClock_DIV_FRAC_MASK  (0x000000F8u)
#define PM_adcClock_DIV_FRAC_SHIFT (3u)
#define PM_adcClock_DIV_INT_MASK   (0xFFFFFF00u)
#define PM_adcClock_DIV_INT_SHIFT  (8u)

#else 

#define PM_adcClock_DIV_REG        (*(reg32 *)PM_adcClock__REGISTER)
#define PM_adcClock_ENABLE_REG     PM_adcClock_DIV_REG
#define PM_adcClock_DIV_FRAC_MASK  PM_adcClock__FRAC_MASK
#define PM_adcClock_DIV_FRAC_SHIFT (16u)
#define PM_adcClock_DIV_INT_MASK   PM_adcClock__DIVIDER_MASK
#define PM_adcClock_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_PM_adcClock_H) */

/* [] END OF FILE */
