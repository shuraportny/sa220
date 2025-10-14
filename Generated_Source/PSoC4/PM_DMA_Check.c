/*******************************************************************************
* File Name: PM_DMA_Check.h
* Version 1.0
*
* Description:
*  This file contains all of the channel DMA API functions.
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "PM_DMA_Check.h"

uint8 PM_DMA_Check_initVar = 0u;


/*******************************************************************************
* Function Name: PM_DMA_Check_Start
********************************************************************************
*
* Summary:
*  Calls Init() to configure the DMA channel based on the customizer settings if
*  the channel has not been initialized before.
*  Sets source and destination address, validates descriptor 0 and enables the
*  channel. After calling this function the DMA channel is active and waiting
*  for a trigger.
*
* Parameters:
*  srcAddress: Address of DMA transfer source for descriptor 0.
*  dstAddress: Address of DMA transfer destination for descriptor 0.
*                   
* Return:
*  None.
*
*******************************************************************************/
void PM_DMA_Check_Start(void * srcAddress, void * dstAddress)
{
    if(0u == PM_DMA_Check_initVar)
    {
        PM_DMA_Check_Init();
        PM_DMA_Check_initVar = 1u;
    }
    CyDmaEnable();
    PM_DMA_Check_SetSrcAddress(0, srcAddress);
    PM_DMA_Check_SetDstAddress(0, dstAddress);
    PM_DMA_Check_ValidateDescriptor(0);
    PM_DMA_Check_ChEnable();
}


/*******************************************************************************
* Function Name: PM_DMA_Check_Init
********************************************************************************
*
* Summary:
*  Initializes the DMA channel based on the parameters set in the component
*  customizer. It is not necessary to call PM_DMA_Check_Init() because the
*  PM_DMA_Check_Start() API calls this function, which is the preferred method
*  to begin component operation.
*
* Parameters:
*  None.
*                   
* Return:
*  None.
*
* Side effect:
*  The first descriptor is set to descriptor 0. This function should not be
*  called while the channel is enabled. All settings will be reset to their
*  initial values.
*
*******************************************************************************/
void PM_DMA_Check_Init(void)
{
    CYDMA_CH_CTL_BASE.ctl[PM_DMA_Check_CHANNEL] = PM_DMA_Check_CHANNEL_CFG;
    CYDMA_DESCR_BASE.descriptor[PM_DMA_Check_CHANNEL][0].ctl = PM_DMA_Check_DESCR0_CFG;
    CYDMA_DESCR_BASE.descriptor[PM_DMA_Check_CHANNEL][1].ctl = PM_DMA_Check_DESCR1_CFG;

    /* If the Generate IRQ option is selected for descriptor 0 or 1, set the
    * channel bit field in the DMA interrupt mask register.
    */
    #if (0u != ((PM_DMA_Check_DESCR0_CFG | PM_DMA_Check_DESCR1_CFG) & CYDMA_GENERATE_IRQ))
        CYDMA_INTR_MASK_REG |= PM_DMA_Check_CHANNEL_MASK;
    #endif /* (0u != ((PM_DMA_Check_DESCR0_CFG | PM_DMA_Check_DESCR1_CFG) & CYDMA_GENERATE_IRQ)) */
}


/* [] END OF FILE */
