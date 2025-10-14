/*******************************************************************************
* File Name: AIN.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_AIN_ALIASES_H) /* Pins AIN_ALIASES_H */
#define CY_PINS_AIN_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define AIN_0			(AIN__0__PC)
#define AIN_0_PS		(AIN__0__PS)
#define AIN_0_PC		(AIN__0__PC)
#define AIN_0_DR		(AIN__0__DR)
#define AIN_0_SHIFT	(AIN__0__SHIFT)
#define AIN_0_INTR	((uint16)((uint16)0x0003u << (AIN__0__SHIFT*2u)))

#define AIN_INTR_ALL	 ((uint16)(AIN_0_INTR))


#endif /* End Pins AIN_ALIASES_H */


/* [] END OF FILE */
