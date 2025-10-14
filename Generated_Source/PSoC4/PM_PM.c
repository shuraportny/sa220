/*******************************************************************************
* File Name: PM_PM.c
* Version 2.60
*
* Description:
*  This file provides Sleep/WakeUp APIs functionality.
*
* Note:
*
********************************************************************************
* Copyright 2008-2017, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "PM.h"


/***************************************
* Local data allocation
***************************************/

static PM_BACKUP_STRUCT  PM_backup =
{
    PM_DISABLED,
    0u    
};


/*******************************************************************************
* Function Name: PM_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void PM_SaveConfig(void)
{
    /* All configuration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: PM_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void PM_RestoreConfig(void)
{
    /* All configuration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: PM_Sleep
********************************************************************************
*
* Summary:
*  Stops the ADC operation and saves the configuration registers and component
*  enable state. Should be called just prior to entering sleep.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  PM_backup - modified.
*
*******************************************************************************/
void PM_Sleep(void)
{
    /* During deepsleep/ hibernate mode keep SARMUX active, i.e. do not open
    *   all switches (disconnect), to be used for ADFT
    */
    PM_backup.dftRegVal = PM_SAR_DFT_CTRL_REG & (uint32)~PM_ADFT_OVERRIDE;
    PM_SAR_DFT_CTRL_REG |= PM_ADFT_OVERRIDE;
    if((PM_SAR_CTRL_REG  & PM_ENABLE) != 0u)
    {
        if((PM_SAR_SAMPLE_CTRL_REG & PM_CONTINUOUS_EN) != 0u)
        {
            PM_backup.enableState = PM_ENABLED | PM_STARTED;
        }
        else
        {
            PM_backup.enableState = PM_ENABLED;
        }
        PM_StopConvert();
        PM_Stop();
        
        /* Disable the SAR internal pump before entering the chip low power mode */
        if((PM_SAR_CTRL_REG & PM_BOOSTPUMP_EN) != 0u)
        {
            PM_SAR_CTRL_REG &= (uint32)~PM_BOOSTPUMP_EN;
            PM_backup.enableState |= PM_BOOSTPUMP_ENABLED;
        }
    }
    else
    {
        PM_backup.enableState = PM_DISABLED;
    }
}


/*******************************************************************************
* Function Name: PM_Wakeup
********************************************************************************
*
* Summary:
*  Restores the component enable state and configuration registers.
*  This should be called just after awaking from sleep mode.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  PM_backup - used.
*
*******************************************************************************/
void PM_Wakeup(void)
{
    PM_SAR_DFT_CTRL_REG = PM_backup.dftRegVal;
    if(PM_backup.enableState != PM_DISABLED)
    {
        /* Enable the SAR internal pump  */
        if((PM_backup.enableState & PM_BOOSTPUMP_ENABLED) != 0u)
        {
            PM_SAR_CTRL_REG |= PM_BOOSTPUMP_EN;
        }
        PM_Enable();
        if((PM_backup.enableState & PM_STARTED) != 0u)
        {
            PM_StartConvert();
        }
    }
}
/* [] END OF FILE */
