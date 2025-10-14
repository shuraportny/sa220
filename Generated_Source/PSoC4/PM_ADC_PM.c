/*******************************************************************************
* File Name: PM_ADC_PM.c
* Version 2.50
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

#include "PM_ADC.h"


/***************************************
* Local data allocation
***************************************/

static PM_ADC_BACKUP_STRUCT  PM_ADC_backup =
{
    PM_ADC_DISABLED,
    0u    
};


/*******************************************************************************
* Function Name: PM_ADC_SaveConfig
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
void PM_ADC_SaveConfig(void)
{
    /* All configuration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: PM_ADC_RestoreConfig
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
void PM_ADC_RestoreConfig(void)
{
    /* All configuration registers are marked as [reset_all_retention] */
}


/*******************************************************************************
* Function Name: PM_ADC_Sleep
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
*  PM_ADC_backup - modified.
*
*******************************************************************************/
void PM_ADC_Sleep(void)
{
    /* During deepsleep/ hibernate mode keep SARMUX active, i.e. do not open
    *   all switches (disconnect), to be used for ADFT
    */
    PM_ADC_backup.dftRegVal = PM_ADC_SAR_DFT_CTRL_REG & (uint32)~PM_ADC_ADFT_OVERRIDE;
    PM_ADC_SAR_DFT_CTRL_REG |= PM_ADC_ADFT_OVERRIDE;
    if((PM_ADC_SAR_CTRL_REG  & PM_ADC_ENABLE) != 0u)
    {
        if((PM_ADC_SAR_SAMPLE_CTRL_REG & PM_ADC_CONTINUOUS_EN) != 0u)
        {
            PM_ADC_backup.enableState = PM_ADC_ENABLED | PM_ADC_STARTED;
        }
        else
        {
            PM_ADC_backup.enableState = PM_ADC_ENABLED;
        }
        PM_ADC_StopConvert();
        PM_ADC_Stop();
        
        /* Disable the SAR internal pump before entering the chip low power mode */
        if((PM_ADC_SAR_CTRL_REG & PM_ADC_BOOSTPUMP_EN) != 0u)
        {
            PM_ADC_SAR_CTRL_REG &= (uint32)~PM_ADC_BOOSTPUMP_EN;
            PM_ADC_backup.enableState |= PM_ADC_BOOSTPUMP_ENABLED;
        }
    }
    else
    {
        PM_ADC_backup.enableState = PM_ADC_DISABLED;
    }
}


/*******************************************************************************
* Function Name: PM_ADC_Wakeup
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
*  PM_ADC_backup - used.
*
*******************************************************************************/
void PM_ADC_Wakeup(void)
{
    PM_ADC_SAR_DFT_CTRL_REG = PM_ADC_backup.dftRegVal;
    if(PM_ADC_backup.enableState != PM_ADC_DISABLED)
    {
        /* Enable the SAR internal pump  */
        if((PM_ADC_backup.enableState & PM_ADC_BOOSTPUMP_ENABLED) != 0u)
        {
            PM_ADC_SAR_CTRL_REG |= PM_ADC_BOOSTPUMP_EN;
        }
        PM_ADC_Enable();
        if((PM_ADC_backup.enableState & PM_ADC_STARTED) != 0u)
        {
            PM_ADC_StartConvert();
        }
    }
}
/* [] END OF FILE */
