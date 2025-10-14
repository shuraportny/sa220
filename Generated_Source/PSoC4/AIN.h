/*******************************************************************************
* File Name: AIN.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_AIN_H) /* Pins AIN_H */
#define CY_PINS_AIN_H

#include "cytypes.h"
#include "cyfitter.h"
#include "AIN_aliases.h"


/***************************************
*     Data Struct Definitions
***************************************/

/**
* \addtogroup group_structures
* @{
*/
    
/* Structure for sleep mode support */
typedef struct
{
    uint32 pcState; /**< State of the port control register */
    uint32 sioState; /**< State of the SIO configuration */
    uint32 usbState; /**< State of the USBIO regulator */
} AIN_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   AIN_Read(void);
void    AIN_Write(uint8 value);
uint8   AIN_ReadDataReg(void);
#if defined(AIN__PC) || (CY_PSOC4_4200L) 
    void    AIN_SetDriveMode(uint8 mode);
#endif
void    AIN_SetInterruptMode(uint16 position, uint16 mode);
uint8   AIN_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void AIN_Sleep(void); 
void AIN_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(AIN__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define AIN_DRIVE_MODE_BITS        (3)
    #define AIN_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - AIN_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the AIN_SetDriveMode() function.
         *  @{
         */
        #define AIN_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define AIN_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define AIN_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define AIN_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define AIN_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define AIN_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define AIN_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define AIN_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define AIN_MASK               AIN__MASK
#define AIN_SHIFT              AIN__SHIFT
#define AIN_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in AIN_SetInterruptMode() function.
     *  @{
     */
        #define AIN_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define AIN_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define AIN_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define AIN_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(AIN__SIO)
    #define AIN_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(AIN__PC) && (CY_PSOC4_4200L)
    #define AIN_USBIO_ENABLE               ((uint32)0x80000000u)
    #define AIN_USBIO_DISABLE              ((uint32)(~AIN_USBIO_ENABLE))
    #define AIN_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define AIN_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define AIN_USBIO_ENTER_SLEEP          ((uint32)((1u << AIN_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << AIN_USBIO_SUSPEND_DEL_SHIFT)))
    #define AIN_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << AIN_USBIO_SUSPEND_SHIFT)))
    #define AIN_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << AIN_USBIO_SUSPEND_DEL_SHIFT)))
    #define AIN_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(AIN__PC)
    /* Port Configuration */
    #define AIN_PC                 (* (reg32 *) AIN__PC)
#endif
/* Pin State */
#define AIN_PS                     (* (reg32 *) AIN__PS)
/* Data Register */
#define AIN_DR                     (* (reg32 *) AIN__DR)
/* Input Buffer Disable Override */
#define AIN_INP_DIS                (* (reg32 *) AIN__PC2)

/* Interrupt configuration Registers */
#define AIN_INTCFG                 (* (reg32 *) AIN__INTCFG)
#define AIN_INTSTAT                (* (reg32 *) AIN__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define AIN_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(AIN__SIO)
    #define AIN_SIO_REG            (* (reg32 *) AIN__SIO)
#endif /* (AIN__SIO_CFG) */

/* USBIO registers */
#if !defined(AIN__PC) && (CY_PSOC4_4200L)
    #define AIN_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define AIN_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define AIN_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define AIN_DRIVE_MODE_SHIFT       (0x00u)
#define AIN_DRIVE_MODE_MASK        (0x07u << AIN_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins AIN_H */


/* [] END OF FILE */
