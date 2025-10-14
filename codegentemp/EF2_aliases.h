/*******************************************************************************
* File Name: EF2.h  
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

#if !defined(CY_PINS_EF2_ALIASES_H) /* Pins EF2_ALIASES_H */
#define CY_PINS_EF2_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define EF2_0			(EF2__0__PC)
#define EF2_0_PS		(EF2__0__PS)
#define EF2_0_PC		(EF2__0__PC)
#define EF2_0_DR		(EF2__0__DR)
#define EF2_0_SHIFT	(EF2__0__SHIFT)
#define EF2_0_INTR	((uint16)((uint16)0x0003u << (EF2__0__SHIFT*2u)))

#define EF2_INTR_ALL	 ((uint16)(EF2_0_INTR))


#endif /* End Pins EF2_ALIASES_H */


/* [] END OF FILE */
