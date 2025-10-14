/*******************************************************************************
* File Name: EF1.h  
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

#if !defined(CY_PINS_EF1_ALIASES_H) /* Pins EF1_ALIASES_H */
#define CY_PINS_EF1_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define EF1_0			(EF1__0__PC)
#define EF1_0_PS		(EF1__0__PS)
#define EF1_0_PC		(EF1__0__PC)
#define EF1_0_DR		(EF1__0__DR)
#define EF1_0_SHIFT	(EF1__0__SHIFT)
#define EF1_0_INTR	((uint16)((uint16)0x0003u << (EF1__0__SHIFT*2u)))

#define EF1_INTR_ALL	 ((uint16)(EF1_0_INTR))


#endif /* End Pins EF1_ALIASES_H */


/* [] END OF FILE */
