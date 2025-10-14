/*******************************************************************************
* File Name: TBD.h  
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

#if !defined(CY_PINS_TBD_ALIASES_H) /* Pins TBD_ALIASES_H */
#define CY_PINS_TBD_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define TBD_0			(TBD__0__PC)
#define TBD_0_PS		(TBD__0__PS)
#define TBD_0_PC		(TBD__0__PC)
#define TBD_0_DR		(TBD__0__DR)
#define TBD_0_SHIFT	(TBD__0__SHIFT)
#define TBD_0_INTR	((uint16)((uint16)0x0003u << (TBD__0__SHIFT*2u)))

#define TBD_INTR_ALL	 ((uint16)(TBD_0_INTR))


#endif /* End Pins TBD_ALIASES_H */


/* [] END OF FILE */
