/*******************************************************************************
* File Name: ScreenCounter_PM.c
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

#include "ScreenCounter.h"

static ScreenCounter_BACKUP_STRUCT ScreenCounter_backup;


/*******************************************************************************
* Function Name: ScreenCounter_SaveConfig
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
void ScreenCounter_SaveConfig(void)
{

}


/*******************************************************************************
* Function Name: ScreenCounter_Sleep
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
void ScreenCounter_Sleep(void)
{
    if(0u != (ScreenCounter_BLOCK_CONTROL_REG & ScreenCounter_MASK))
    {
        ScreenCounter_backup.enableState = 1u;
    }
    else
    {
        ScreenCounter_backup.enableState = 0u;
    }

    ScreenCounter_Stop();
    ScreenCounter_SaveConfig();
}


/*******************************************************************************
* Function Name: ScreenCounter_RestoreConfig
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
void ScreenCounter_RestoreConfig(void)
{

}


/*******************************************************************************
* Function Name: ScreenCounter_Wakeup
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
void ScreenCounter_Wakeup(void)
{
    ScreenCounter_RestoreConfig();

    if(0u != ScreenCounter_backup.enableState)
    {
        ScreenCounter_Enable();
    }
}


/* [] END OF FILE */
