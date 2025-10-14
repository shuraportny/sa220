/*******************************************************************************
* File Name: TBD.h  
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

#if !defined(CY_PINS_TBD_H) /* Pins TBD_H */
#define CY_PINS_TBD_H

#include "cytypes.h"
#include "cyfitter.h"
#include "TBD_aliases.h"


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
} TBD_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   TBD_Read(void);
void    TBD_Write(uint8 value);
uint8   TBD_ReadDataReg(void);
#if defined(TBD__PC) || (CY_PSOC4_4200L) 
    void    TBD_SetDriveMode(uint8 mode);
#endif
void    TBD_SetInterruptMode(uint16 position, uint16 mode);
uint8   TBD_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void TBD_Sleep(void); 
void TBD_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(TBD__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define TBD_DRIVE_MODE_BITS        (3)
    #define TBD_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - TBD_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the TBD_SetDriveMode() function.
         *  @{
         */
        #define TBD_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define TBD_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define TBD_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define TBD_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define TBD_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define TBD_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define TBD_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define TBD_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define TBD_MASK               TBD__MASK
#define TBD_SHIFT              TBD__SHIFT
#define TBD_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in TBD_SetInterruptMode() function.
     *  @{
     */
        #define TBD_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define TBD_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define TBD_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define TBD_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(TBD__SIO)
    #define TBD_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(TBD__PC) && (CY_PSOC4_4200L)
    #define TBD_USBIO_ENABLE               ((uint32)0x80000000u)
    #define TBD_USBIO_DISABLE              ((uint32)(~TBD_USBIO_ENABLE))
    #define TBD_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define TBD_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define TBD_USBIO_ENTER_SLEEP          ((uint32)((1u << TBD_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << TBD_USBIO_SUSPEND_DEL_SHIFT)))
    #define TBD_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << TBD_USBIO_SUSPEND_SHIFT)))
    #define TBD_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << TBD_USBIO_SUSPEND_DEL_SHIFT)))
    #define TBD_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(TBD__PC)
    /* Port Configuration */
    #define TBD_PC                 (* (reg32 *) TBD__PC)
#endif
/* Pin State */
#define TBD_PS                     (* (reg32 *) TBD__PS)
/* Data Register */
#define TBD_DR                     (* (reg32 *) TBD__DR)
/* Input Buffer Disable Override */
#define TBD_INP_DIS                (* (reg32 *) TBD__PC2)

/* Interrupt configuration Registers */
#define TBD_INTCFG                 (* (reg32 *) TBD__INTCFG)
#define TBD_INTSTAT                (* (reg32 *) TBD__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define TBD_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(TBD__SIO)
    #define TBD_SIO_REG            (* (reg32 *) TBD__SIO)
#endif /* (TBD__SIO_CFG) */

/* USBIO registers */
#if !defined(TBD__PC) && (CY_PSOC4_4200L)
    #define TBD_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define TBD_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define TBD_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define TBD_DRIVE_MODE_SHIFT       (0x00u)
#define TBD_DRIVE_MODE_MASK        (0x07u << TBD_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins TBD_H */


/* [] END OF FILE */
