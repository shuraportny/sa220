/*******************************************************************************
* File Name: SWTimer_PM.c
* Version 2.10
*
* Description:
*  This file contains the setup, control, and status commands to support
*  the component operations in the low power mode.
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

#include "SWTimer.h"

static SWTimer_BACKUP_STRUCT SWTimer_backup;


/*******************************************************************************
* Function Name: SWTimer_SaveConfig
********************************************************************************
*
* Summary:
*  All configuration registers are retention. Nothing to save here.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void SWTimer_SaveConfig(void)
{

}


/*******************************************************************************
* Function Name: SWTimer_Sleep
********************************************************************************
*
* Summary:
*  Stops the component operation and saves the user configuration.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void SWTimer_Sleep(void)
{
    if(0u != (SWTimer_BLOCK_CONTROL_REG & SWTimer_MASK))
    {
        SWTimer_backup.enableState = 1u;
    }
    else
    {
        SWTimer_backup.enableState = 0u;
    }

    SWTimer_Stop();
    SWTimer_SaveConfig();
}


/*******************************************************************************
* Function Name: SWTimer_RestoreConfig
********************************************************************************
*
* Summary:
*  All configuration registers are retention. Nothing to restore here.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void SWTimer_RestoreConfig(void)
{

}


/*******************************************************************************
* Function Name: SWTimer_Wakeup
********************************************************************************
*
* Summary:
*  Restores the user configuration and restores the enable state.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void SWTimer_Wakeup(void)
{
    SWTimer_RestoreConfig();

    if(0u != SWTimer_backup.enableState)
    {
        SWTimer_Enable();
    }
}


/* [] END OF FILE */
