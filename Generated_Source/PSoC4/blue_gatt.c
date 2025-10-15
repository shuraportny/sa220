/***************************************************************************//**
* \file CYBLE_gatt.c
* \version 3.40
* 
* \brief
*  This file contains the source code for the GATT API of the BLE Component.
* 
********************************************************************************
* \copyright
* Copyright 2014-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#include "blue_eventHandler.h"


/***************************************
* Global variables
***************************************/

CYBLE_STATE_T cyBle_state;

#if ((CYBLE_MODE_PROFILE) && (CYBLE_BONDING_REQUIREMENT == CYBLE_BONDING_YES))
    
#if(CYBLE_MODE_PROFILE)
    #if defined(__ARMCC_VERSION)
        CY_ALIGN(CYDEV_FLS_ROW_SIZE) const CY_BLE_FLASH_STORAGE cyBle_flashStorage CY_SECTION(".cy_checksum_exclude") =
    #elif defined (__GNUC__)
        const CY_BLE_FLASH_STORAGE cyBle_flashStorage CY_SECTION(".cy_checksum_exclude")
            CY_ALIGN(CYDEV_FLS_ROW_SIZE) =
    #elif defined (__ICCARM__)
        #pragma data_alignment=CY_FLASH_SIZEOF_ROW
        #pragma location=".cy_checksum_exclude"
        const CY_BLE_FLASH_STORAGE cyBle_flashStorage =
    #endif  /* (__ARMCC_VERSION) */
    {
        { 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
        0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u }, 
        {{
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x02u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
        },
        {
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x02u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
        },
        {
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x02u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
        },
        {
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x02u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
        },
        {
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x02u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
            0x00u, 0x00u,
        }}, 
        0x46u, /* CYBLE_GATT_DB_CCCD_COUNT */ 
        0x05u, /* CYBLE_GAP_MAX_BONDED_DEVICE */ 
    };
#endif /* (CYBLE_MODE_PROFILE) */

#endif  /* (CYBLE_MODE_PROFILE) && (CYBLE_BONDING_REQUIREMENT == CYBLE_BONDING_YES) */

#if(CYBLE_GATT_ROLE_SERVER)
    
    const CYBLE_GATTS_T cyBle_gatts =
{
    0x000Cu,    /* Handle of the GATT service */
    0x000Eu,    /* Handle of the Service Changed characteristic */
    0x000Fu,    /* Handle of the Client Characteristic Configuration descriptor */
};
    
    static uint8 cyBle_attValues[0x0496u] = {
    /* Device Name */
    (uint8)'A', (uint8)'L', (uint8)'E', (uint8)'X', (uint8)'_', (uint8)'P', (uint8)'O', (uint8)'R', (uint8)'T',
    (uint8)' ', (uint8)'0', (uint8)'1', (uint8)'2', (uint8)'3', (uint8)'4', (uint8)'5', (uint8)'6', (uint8)'7',
    (uint8)'_', (uint8)'_', (uint8)'_', (uint8)'_',

    /* Appearance */
    0x00u, 0x00u,

    /* Peripheral Preferred Connection Parameters */
    0x28u, 0x00u, 0x38u, 0x00u, 0x00u, 0x00u, 0x1Eu, 0x00u,

    /* Central Address Resolution */
    0x00u,

    /* Resolvable Private Address Only */
    0x00u,

    /* Service Changed */
    0x00u, 0x00u, 0x00u, 0x00u,

    /* SYS_REMOTE */
    0x00u,

    /* Characteristic User Description */
    (uint8)'S', (uint8)'Y', (uint8)'S', (uint8)'_', (uint8)'R', (uint8)'E', (uint8)'M', (uint8)'O', (uint8)'T',
    (uint8)'E',

    /* SYS_READING */
    0x00u, 0x00u,

    /* Characteristic User Description */
    (uint8)'S', (uint8)'Y', (uint8)'S', (uint8)'_', (uint8)'R', (uint8)'E', (uint8)'A', (uint8)'D', (uint8)'I',
    (uint8)'N', (uint8)'G',

    /* SYS_STATUS */
    0x00u, 0x00u,

    /* Characteristic User Description */
    (uint8)'S', (uint8)'Y', (uint8)'S', (uint8)'_', (uint8)'S', (uint8)'T', (uint8)'A', (uint8)'T', (uint8)'U',
    (uint8)'S',

    /* SYS_CAN_CHAN */
    0x00u,

    /* Characteristic User Description */
    (uint8)'S', (uint8)'Y', (uint8)'S', (uint8)'_', (uint8)'C', (uint8)'A', (uint8)'N', (uint8)'_', (uint8)'C',
    (uint8)'H', (uint8)'A', (uint8)'N',

    /* SYS_CAN_ID */
    0x00u,

    /* Characteristic User Description */
    (uint8)'S', (uint8)'Y', (uint8)'S', (uint8)'_', (uint8)'C', (uint8)'A', (uint8)'N', (uint8)'_', (uint8)'I',
    (uint8)'D',

    /* SYS_CAL_LOOP */
    0x00u, 0x00u,

    /* Characteristic User Description */
    (uint8)'S', (uint8)'Y', (uint8)'S', (uint8)'_', (uint8)'C', (uint8)'A', (uint8)'L', (uint8)'_', (uint8)'L',
    (uint8)'O', (uint8)'O', (uint8)'P',

    /* SYS_ADC */
    0x00u, 0x00u,

    /* Characteristic User Description */
    (uint8)'S', (uint8)'Y', (uint8)'S', (uint8)'_', (uint8)'A', (uint8)'D', (uint8)'C',

    /* SYS_ALERT_1 */
    0x00u, 0x00u,

    /* Characteristic User Description */
    (uint8)'S', (uint8)'Y', (uint8)'S', (uint8)'_', (uint8)'A', (uint8)'L', (uint8)'E', (uint8)'R', (uint8)'T',
    (uint8)'_', (uint8)'1',

    /* SYS_ALERT_2 */
    0x00u, 0x00u,

    /* Characteristic User Description */
    (uint8)'S', (uint8)'Y', (uint8)'S', (uint8)'_', (uint8)'A', (uint8)'L', (uint8)'E', (uint8)'R', (uint8)'T',
    (uint8)'_', (uint8)'2',

    /* SYS_ALERT_1_ASC */
    0x00u, 0x00u,

    /* Characteristic User Description */
    (uint8)'S', (uint8)'Y', (uint8)'S', (uint8)'_', (uint8)'A', (uint8)'L', (uint8)'E', (uint8)'R', (uint8)'T',
    (uint8)'_', (uint8)'1', (uint8)'_', (uint8)'A', (uint8)'S', (uint8)'C',

    /* SYS_ALERT_2_ASC */
    0x00u, 0x00u,

    /* Characteristic User Description */
    (uint8)'S', (uint8)'Y', (uint8)'S', (uint8)'_', (uint8)'A', (uint8)'L', (uint8)'E', (uint8)'R', (uint8)'T',
    (uint8)'_', (uint8)'2', (uint8)'_', (uint8)'A', (uint8)'S', (uint8)'C',

    /* SYS_FRACTIONAL */
    0x00u, 0x00u,

    /* Characteristic User Description */
    (uint8)'S', (uint8)'Y', (uint8)'S', (uint8)'_', (uint8)'F', (uint8)'R', (uint8)'A', (uint8)'C', (uint8)'T',
    (uint8)'I', (uint8)'O', (uint8)'N', (uint8)'A', (uint8)'L',

    /* RESOLUTION */
    0x00u,

    /* Characteristic User Description */
    (uint8)'R', (uint8)'E', (uint8)'S', (uint8)'O', (uint8)'L', (uint8)'U', (uint8)'T', (uint8)'I', (uint8)'O',
    (uint8)'N',

    /* RESOLUTION2 */
    0x00u,

    /* Characteristic User Description */
    (uint8)'R', (uint8)'E', (uint8)'S', (uint8)'O', (uint8)'L', (uint8)'U', (uint8)'T', (uint8)'I', (uint8)'O',
    (uint8)'N', (uint8)'2',

    /* RELAYS */
    0x00u, 0x00u, 0x00u, 0x00u,

    /* Characteristic User Description */
    (uint8)'R', (uint8)'E', (uint8)'L', (uint8)'A', (uint8)'Y', (uint8)'S',

    /* LOG1 */
    (uint8)'L', (uint8)'O', (uint8)'G', (uint8)'1', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',
    (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',
    (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',
    (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',
    (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',

    /* Characteristic User Description */
    (uint8)'L', (uint8)'O', (uint8)'G', (uint8)'1',

    /* LOG2 */
    (uint8)'L', (uint8)'O', (uint8)'G', (uint8)'2', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',
    (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',
    (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',
    (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',
    (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',

    /* Characteristic User Description */
    (uint8)'L', (uint8)'O', (uint8)'G', (uint8)'2',

    /* LOG3 */
    (uint8)'L', (uint8)'O', (uint8)'G', (uint8)'3', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',
    (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',
    (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',
    (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',
    (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',

    /* Characteristic User Description */
    (uint8)'L', (uint8)'O', (uint8)'G', (uint8)'3',

    /* LOG4 */
    (uint8)'L', (uint8)'O', (uint8)'G', (uint8)'4', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',
    (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',
    (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',
    (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',
    (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',

    /* Characteristic User Description */
    (uint8)'L', (uint8)'O', (uint8)'G', (uint8)'4',

    /* LOG5 */
    (uint8)'L', (uint8)'O', (uint8)'G', (uint8)'5', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',
    (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',
    (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',
    (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',
    (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',

    /* Characteristic User Description */
    (uint8)'L', (uint8)'O', (uint8)'G', (uint8)'5',

    /* LOG6 */
    (uint8)'L', (uint8)'O', (uint8)'G', (uint8)'6', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',
    (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',
    (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',
    (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',
    (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',

    /* Characteristic User Description */
    (uint8)'L', (uint8)'O', (uint8)'G', (uint8)'6',

    /* LOG7 */
    (uint8)'L', (uint8)'O', (uint8)'G', (uint8)'7', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',
    (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',
    (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',
    (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',
    (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',

    /* Characteristic User Description */
    (uint8)'L', (uint8)'O', (uint8)'G', (uint8)'7',

    /* LOG8 */
    (uint8)'L', (uint8)'O', (uint8)'G', (uint8)'8', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',
    (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',
    (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',
    (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',
    (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',

    /* Characteristic User Description */
    (uint8)'L', (uint8)'O', (uint8)'G', (uint8)'8',

    /* LOG9 */
    (uint8)'L', (uint8)'O', (uint8)'G', (uint8)'9', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',
    (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',
    (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',
    (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',
    (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',

    /* Characteristic User Description */
    (uint8)'L', (uint8)'O', (uint8)'G', (uint8)'9',

    /* LOG10 */
    (uint8)'L', (uint8)'O', (uint8)'G', (uint8)'1', (uint8)'0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',
    (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',
    (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',
    (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',
    (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',

    /* Characteristic User Description */
    (uint8)'L', (uint8)'O', (uint8)'G', (uint8)'1', (uint8)'0',

    /* SLEEP_TIME */
    0x00u, 0x00u,

    /* Characteristic User Description */
    (uint8)'S', (uint8)'L', (uint8)'E', (uint8)'E', (uint8)'P', (uint8)'_', (uint8)'T', (uint8)'I', (uint8)'M',
    (uint8)'E',

    /* GPS_LAT */
    0x00u, 0x00u, 0x00u, 0x00u,

    /* Characteristic User Description */
    (uint8)'G', (uint8)'P', (uint8)'S', (uint8)'_', (uint8)'L', (uint8)'A', (uint8)'T',

    /* GPS_LONG */
    0x00u, 0x00u, 0x00u, 0x00u,

    /* Characteristic User Description */
    (uint8)'G', (uint8)'P', (uint8)'S', (uint8)'_', (uint8)'L', (uint8)'O', (uint8)'N', (uint8)'G',

    /* DATE_TIME */
    0x00u, 0x00u, 0x00u, 0x00u, 0x00u,

    /* Characteristic User Description */
    (uint8)'D', (uint8)'A', (uint8)'T', (uint8)'E', (uint8)'_', (uint8)'T', (uint8)'I', (uint8)'M', (uint8)'E',

    /* SELECTED_CHANNEL */
    0x00u,

    /* Characteristic User Description */
    (uint8)'S', (uint8)'E', (uint8)'L', (uint8)'E', (uint8)'C', (uint8)'T', (uint8)'E', (uint8)'D', (uint8)'_',
    (uint8)'C', (uint8)'H', (uint8)'A', (uint8)'N', (uint8)'N', (uint8)'E', (uint8)'L',

    /* Manufacturer Name String */
    (uint8)'S', (uint8)'a', (uint8)'f', (uint8)'e', (uint8)'g', (uint8)'u', (uint8)'a', (uint8)'r', (uint8)'d',
    (uint8)' ', (uint8)'A', (uint8)'n', (uint8)'a', (uint8)'l', (uint8)'y', (uint8)'t', (uint8)'i', (uint8)'c',
    (uint8)'s',

    /* Model Number String */
    (uint8)'S', (uint8)'A', (uint8)'-', (uint8)'2', (uint8)'2', (uint8)'0',

    /* Serial Number String */
    (uint8)'S', (uint8)'N', (uint8)'0', (uint8)'0', (uint8)'1', (uint8)'\0', (uint8)'\0', (uint8)'\0', (uint8)'\0',
    (uint8)'\0', (uint8)'\0', (uint8)'\0',

    /* Hardware Revision String */
    (uint8)' ', (uint8)' ', (uint8)'R', (uint8)'2',

    /* Firmware Revision String */
    (uint8)'F', (uint8)'W', (uint8)'0', (uint8)'1',

    /* Software Revision String */
    (uint8)'N', (uint8)'A',

    /* System ID */
    0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,

    /* IEEE 11073-20601 Regulatory Certification Data List */
    0x00u,

    /* PnP ID */
    0x01u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,

    /* PIN */
    (uint8)'0', (uint8)'0', (uint8)'0', (uint8)'0',

    /* Characteristic User Description */
    (uint8)'P', (uint8)'I', (uint8)'N',

    /* SENSOR_TYPE */
    0x00u,

    /* Characteristic User Description */
    (uint8)'S', (uint8)'E', (uint8)'N', (uint8)'S', (uint8)'O', (uint8)'R', (uint8)'_', (uint8)'T', (uint8)'Y',
    (uint8)'P', (uint8)'E',

    /* SENSOR_TYPE2 */
    0x00u,

    /* Characteristic User Description */
    (uint8)'S', (uint8)'E', (uint8)'N', (uint8)'S', (uint8)'O', (uint8)'R', (uint8)'_', (uint8)'T', (uint8)'Y',
    (uint8)'P', (uint8)'E', (uint8)'2',

    /* SENSOR_RANGE */
    0x00u, 0x00u,

    /* Characteristic User Description */
    (uint8)'S', (uint8)'E', (uint8)'N', (uint8)'S', (uint8)'O', (uint8)'R', (uint8)'_', (uint8)'R', (uint8)'A',
    (uint8)'N', (uint8)'G', (uint8)'E',

    /* SENSOR_RANGE2 */
    0x00u, 0x00u,

    /* Characteristic User Description */
    (uint8)'S', (uint8)'E', (uint8)'N', (uint8)'S', (uint8)'O', (uint8)'R', (uint8)'_', (uint8)'R', (uint8)'A',
    (uint8)'N', (uint8)'G', (uint8)'E', (uint8)'2',

    /* SENSOR_GAS_NOTE */
    (uint8)'T', (uint8)'Y', (uint8)'P', (uint8)'U', (uint8)'N', (uint8)'K', (uint8)'N', (uint8)'O', (uint8)'W',
    (uint8)'N',

    /* Characteristic User Description */
    (uint8)'S', (uint8)'E', (uint8)'N', (uint8)'S', (uint8)'O', (uint8)'R', (uint8)'_', (uint8)'G', (uint8)'A',
    (uint8)'S', (uint8)'_', (uint8)'N', (uint8)'O', (uint8)'T', (uint8)'E',

    /* SENSOR_GAS_NOTE2 */
    (uint8)'T', (uint8)'Y', (uint8)'P', (uint8)'U', (uint8)'N', (uint8)'K', (uint8)'N', (uint8)'O', (uint8)'W',
    (uint8)'N',

    /* Characteristic User Description */
    (uint8)'S', (uint8)'E', (uint8)'N', (uint8)'S', (uint8)'O', (uint8)'R', (uint8)'_', (uint8)'G', (uint8)'A',
    (uint8)'S', (uint8)'_', (uint8)'N', (uint8)'O', (uint8)'T', (uint8)'E', (uint8)'2',

    /* SENSOR_TAG */
    (uint8)'S', (uint8)'A', (uint8)'F', (uint8)'E', (uint8)'G', (uint8)'U', (uint8)'A', (uint8)'R', (uint8)'D',
    (uint8)' ', (uint8)'A', (uint8)'N', (uint8)'A', (uint8)'L', (uint8)'Y', (uint8)'T', (uint8)'I', (uint8)'C',
    (uint8)'S', (uint8)'\0',

    /* Characteristic User Description */
    (uint8)'S', (uint8)'E', (uint8)'N', (uint8)'S', (uint8)'O', (uint8)'R', (uint8)'_', (uint8)'T', (uint8)'A',
    (uint8)'G',

    /* SENSOR_TEMPERATURE */
    0x00u,

    /* Characteristic User Description */
    (uint8)'S', (uint8)'E', (uint8)'N', (uint8)'S', (uint8)'O', (uint8)'R', (uint8)'_', (uint8)'T', (uint8)'E',
    (uint8)'M', (uint8)'P', (uint8)'E', (uint8)'R', (uint8)'A', (uint8)'T', (uint8)'U', (uint8)'R', (uint8)'E',

    /* SENSOR_CAL_LEVEL */
    0x00u, 0x00u,

    /* Characteristic User Description */
    (uint8)'S', (uint8)'E', (uint8)'N', (uint8)'S', (uint8)'O', (uint8)'R', (uint8)'_', (uint8)'C', (uint8)'A',
    (uint8)'L', (uint8)'_', (uint8)'L', (uint8)'E', (uint8)'V', (uint8)'E', (uint8)'L',

    /* CAL_Z_COUNTS */
    0x00u, 0x00u,

    /* Characteristic User Description */
    (uint8)'C', (uint8)'A', (uint8)'L', (uint8)'_', (uint8)'Z', (uint8)'_', (uint8)'C', (uint8)'O', (uint8)'U',
    (uint8)'N', (uint8)'T', (uint8)'S',

    /* ZERO_READING */
    0x00u, 0x00u,

    /* Characteristic User Description */
    (uint8)'Z', (uint8)'E', (uint8)'R', (uint8)'O', (uint8)'_', (uint8)'R', (uint8)'E', (uint8)'A', (uint8)'D',
    (uint8)'I', (uint8)'N', (uint8)'G',

    /* CAL_ZERO_ERROR */
    0x00u,

    /* Characteristic User Description */
    (uint8)'C', (uint8)'A', (uint8)'L', (uint8)'_', (uint8)'Z', (uint8)'E', (uint8)'R', (uint8)'O', (uint8)'_',
    (uint8)'E', (uint8)'R', (uint8)'R', (uint8)'O', (uint8)'R',

    /* CAL_STEP */
    0x00u,

    /* Characteristic User Description */
    (uint8)'C', (uint8)'A', (uint8)'L', (uint8)'_', (uint8)'S', (uint8)'T', (uint8)'E', (uint8)'P',

    /* CAL_SPEED */
    0x00u, 0x00u,

    /* Characteristic User Description */
    (uint8)'C', (uint8)'A', (uint8)'L', (uint8)'_', (uint8)'S', (uint8)'P', (uint8)'E', (uint8)'E', (uint8)'D',

    /* CAL_AS_FOUND */
    0x00u, 0x00u, 0x00u, 0x00u,

    /* Characteristic User Description */
    (uint8)'C', (uint8)'A', (uint8)'L', (uint8)'_', (uint8)'A', (uint8)'S', (uint8)'_', (uint8)'F', (uint8)'O',
    (uint8)'U', (uint8)'N', (uint8)'D',

    /* CAL_ADJUSTED */
    0x00u,

    /* Characteristic User Description */
    (uint8)'C', (uint8)'A', (uint8)'L', (uint8)'_', (uint8)'A', (uint8)'D', (uint8)'J', (uint8)'U', (uint8)'S',
    (uint8)'T', (uint8)'E', (uint8)'D',

    /* CAL_COUNTS */
    0x00u, 0x00u,

    /* Characteristic User Description */
    (uint8)'C', (uint8)'A', (uint8)'L', (uint8)'_', (uint8)'C', (uint8)'O', (uint8)'U', (uint8)'N', (uint8)'T',
    (uint8)'S',

    /* SENSOR_LIFE */
    0x00u,

    /* Characteristic User Description */
    (uint8)'S', (uint8)'E', (uint8)'N', (uint8)'S', (uint8)'O', (uint8)'R', (uint8)'_', (uint8)'L', (uint8)'I',
    (uint8)'F', (uint8)'E',

    /* CAL_CLEARING */
    0x00u, 0x00u,

    /* Characteristic User Description */
    (uint8)'C', (uint8)'A', (uint8)'L', (uint8)'_', (uint8)'C', (uint8)'L', (uint8)'E', (uint8)'A', (uint8)'R',
    (uint8)'I', (uint8)'N', (uint8)'G',

    /* CAL_SPAN_ERROR */
    0x00u,

    /* Characteristic User Description */
    (uint8)'C', (uint8)'A', (uint8)'L', (uint8)'_', (uint8)'S', (uint8)'P', (uint8)'A', (uint8)'N', (uint8)'_',
    (uint8)'E', (uint8)'R', (uint8)'R', (uint8)'O', (uint8)'R',

    /* CALIBRATION_STATUS */
    0x00u,

    /* Characteristic User Description */
    (uint8)'C', (uint8)'A', (uint8)'L', (uint8)'I', (uint8)'B', (uint8)'R', (uint8)'A', (uint8)'T', (uint8)'I',
    (uint8)'O', (uint8)'N', (uint8)'_', (uint8)'S', (uint8)'T', (uint8)'A', (uint8)'T', (uint8)'U', (uint8)'S',

    /* READING_SCALED */
    0x00u, 0x00u, 0x00u, 0x00u,

    /* Characteristic User Description */
    (uint8)'R', (uint8)'E', (uint8)'A', (uint8)'D', (uint8)'I', (uint8)'N', (uint8)'G', (uint8)'_', (uint8)'S',
    (uint8)'C', (uint8)'A', (uint8)'L', (uint8)'E', (uint8)'D',

    /* ALERT_STATUS */
    0x00u,

    /* Characteristic User Description */
    (uint8)'A', (uint8)'L', (uint8)'E', (uint8)'R', (uint8)'T', (uint8)'_', (uint8)'S', (uint8)'T', (uint8)'A',
    (uint8)'T', (uint8)'U', (uint8)'S',

};
#if(CYBLE_GATT_DB_CCCD_COUNT != 0u)
uint8 cyBle_attValuesCCCD[CYBLE_GATT_DB_CCCD_COUNT];
#endif /* CYBLE_GATT_DB_CCCD_COUNT != 0u */

CYBLE_GATTS_ATT_GEN_VAL_LEN_T cyBle_attValuesLen[CYBLE_GATT_DB_ATT_VAL_COUNT] = {
    { 0x0016u, (void *)&cyBle_attValues[0] }, /* Device Name */
    { 0x0002u, (void *)&cyBle_attValues[22] }, /* Appearance */
    { 0x0008u, (void *)&cyBle_attValues[24] }, /* Peripheral Preferred Connection Parameters */
    { 0x0001u, (void *)&cyBle_attValues[32] }, /* Central Address Resolution */
    { 0x0001u, (void *)&cyBle_attValues[33] }, /* Resolvable Private Address Only */
    { 0x0004u, (void *)&cyBle_attValues[34] }, /* Service Changed */
    { 0x0002u, (void *)&cyBle_attValuesCCCD[0] }, /* Client Characteristic Configuration */
    { 0x0001u, (void *)&cyBle_attValues[38] }, /* SYS_REMOTE */
    { 0x000Au, (void *)&cyBle_attValues[39] }, /* Characteristic User Description */
    { 0x0002u, (void *)&cyBle_attValuesCCCD[2] }, /* Client Characteristic Configuration */
    { 0x0002u, (void *)&cyBle_attValues[49] }, /* SYS_READING */
    { 0x000Bu, (void *)&cyBle_attValues[51] }, /* Characteristic User Description */
    { 0x0002u, (void *)&cyBle_attValuesCCCD[4] }, /* SYS_READING_CCCD */
    { 0x0002u, (void *)&cyBle_attValues[62] }, /* SYS_STATUS */
    { 0x000Au, (void *)&cyBle_attValues[64] }, /* Characteristic User Description */
    { 0x0002u, (void *)&cyBle_attValuesCCCD[6] }, /* Client Characteristic Configuration */
    { 0x0001u, (void *)&cyBle_attValues[74] }, /* SYS_CAN_CHAN */
    { 0x000Cu, (void *)&cyBle_attValues[75] }, /* Characteristic User Description */
    { 0x0001u, (void *)&cyBle_attValues[87] }, /* SYS_CAN_ID */
    { 0x000Au, (void *)&cyBle_attValues[88] }, /* Characteristic User Description */
    { 0x0002u, (void *)&cyBle_attValues[98] }, /* SYS_CAL_LOOP */
    { 0x000Cu, (void *)&cyBle_attValues[100] }, /* Characteristic User Description */
    { 0x0002u, (void *)&cyBle_attValues[112] }, /* SYS_ADC */
    { 0x0007u, (void *)&cyBle_attValues[114] }, /* Characteristic User Description */
    { 0x0002u, (void *)&cyBle_attValuesCCCD[8] }, /* Client Characteristic Configuration */
    { 0x0002u, (void *)&cyBle_attValues[121] }, /* SYS_ALERT_1 */
    { 0x000Bu, (void *)&cyBle_attValues[123] }, /* Characteristic User Description */
    { 0x0002u, (void *)&cyBle_attValues[134] }, /* SYS_ALERT_2 */
    { 0x000Bu, (void *)&cyBle_attValues[136] }, /* Characteristic User Description */
    { 0x0002u, (void *)&cyBle_attValues[147] }, /* SYS_ALERT_1_ASC */
    { 0x000Fu, (void *)&cyBle_attValues[149] }, /* Characteristic User Description */
    { 0x0002u, (void *)&cyBle_attValues[164] }, /* SYS_ALERT_2_ASC */
    { 0x000Fu, (void *)&cyBle_attValues[166] }, /* Characteristic User Description */
    { 0x0002u, (void *)&cyBle_attValues[181] }, /* SYS_FRACTIONAL */
    { 0x000Eu, (void *)&cyBle_attValues[183] }, /* Characteristic User Description */
    { 0x0001u, (void *)&cyBle_attValues[197] }, /* RESOLUTION */
    { 0x000Au, (void *)&cyBle_attValues[198] }, /* Characteristic User Description */
    { 0x0002u, (void *)&cyBle_attValuesCCCD[10] }, /* Client Characteristic Configuration */
    { 0x0001u, (void *)&cyBle_attValues[208] }, /* RESOLUTION2 */
    { 0x000Bu, (void *)&cyBle_attValues[209] }, /* Characteristic User Description */
    { 0x0004u, (void *)&cyBle_attValues[220] }, /* RELAYS */
    { 0x0006u, (void *)&cyBle_attValues[224] }, /* Characteristic User Description */
    { 0x0002u, (void *)&cyBle_attValuesCCCD[12] }, /* Client Characteristic Configuration */
    { 0x0028u, (void *)&cyBle_attValues[230] }, /* LOG1 */
    { 0x0002u, (void *)&cyBle_attValuesCCCD[14] }, /* Client Characteristic Configuration */
    { 0x0004u, (void *)&cyBle_attValues[270] }, /* Characteristic User Description */
    { 0x0028u, (void *)&cyBle_attValues[274] }, /* LOG2 */
    { 0x0004u, (void *)&cyBle_attValues[314] }, /* Characteristic User Description */
    { 0x0002u, (void *)&cyBle_attValuesCCCD[16] }, /* Client Characteristic Configuration */
    { 0x0028u, (void *)&cyBle_attValues[318] }, /* LOG3 */
    { 0x0004u, (void *)&cyBle_attValues[358] }, /* Characteristic User Description */
    { 0x0002u, (void *)&cyBle_attValuesCCCD[18] }, /* Client Characteristic Configuration */
    { 0x0028u, (void *)&cyBle_attValues[362] }, /* LOG4 */
    { 0x0002u, (void *)&cyBle_attValuesCCCD[20] }, /* Client Characteristic Configuration */
    { 0x0004u, (void *)&cyBle_attValues[402] }, /* Characteristic User Description */
    { 0x0028u, (void *)&cyBle_attValues[406] }, /* LOG5 */
    { 0x0004u, (void *)&cyBle_attValues[446] }, /* Characteristic User Description */
    { 0x0002u, (void *)&cyBle_attValuesCCCD[22] }, /* Client Characteristic Configuration */
    { 0x0028u, (void *)&cyBle_attValues[450] }, /* LOG6 */
    { 0x0004u, (void *)&cyBle_attValues[490] }, /* Characteristic User Description */
    { 0x0002u, (void *)&cyBle_attValuesCCCD[24] }, /* Client Characteristic Configuration */
    { 0x0028u, (void *)&cyBle_attValues[494] }, /* LOG7 */
    { 0x0004u, (void *)&cyBle_attValues[534] }, /* Characteristic User Description */
    { 0x0002u, (void *)&cyBle_attValuesCCCD[26] }, /* Client Characteristic Configuration */
    { 0x0028u, (void *)&cyBle_attValues[538] }, /* LOG8 */
    { 0x0004u, (void *)&cyBle_attValues[578] }, /* Characteristic User Description */
    { 0x0002u, (void *)&cyBle_attValuesCCCD[28] }, /* Client Characteristic Configuration */
    { 0x0028u, (void *)&cyBle_attValues[582] }, /* LOG9 */
    { 0x0004u, (void *)&cyBle_attValues[622] }, /* Characteristic User Description */
    { 0x0002u, (void *)&cyBle_attValuesCCCD[30] }, /* Client Characteristic Configuration */
    { 0x0028u, (void *)&cyBle_attValues[626] }, /* LOG10 */
    { 0x0005u, (void *)&cyBle_attValues[666] }, /* Characteristic User Description */
    { 0x0002u, (void *)&cyBle_attValuesCCCD[32] }, /* Client Characteristic Configuration */
    { 0x0002u, (void *)&cyBle_attValues[671] }, /* SLEEP_TIME */
    { 0x000Au, (void *)&cyBle_attValues[673] }, /* Characteristic User Description */
    { 0x0002u, (void *)&cyBle_attValuesCCCD[34] }, /* Client Characteristic Configuration */
    { 0x0004u, (void *)&cyBle_attValues[683] }, /* GPS_LAT */
    { 0x0007u, (void *)&cyBle_attValues[687] }, /* Characteristic User Description */
    { 0x0002u, (void *)&cyBle_attValuesCCCD[36] }, /* Client Characteristic Configuration */
    { 0x0004u, (void *)&cyBle_attValues[694] }, /* GPS_LONG */
    { 0x0008u, (void *)&cyBle_attValues[698] }, /* Characteristic User Description */
    { 0x0002u, (void *)&cyBle_attValuesCCCD[38] }, /* Client Characteristic Configuration */
    { 0x0005u, (void *)&cyBle_attValues[706] }, /* DATE_TIME */
    { 0x0009u, (void *)&cyBle_attValues[711] }, /* Characteristic User Description */
    { 0x0002u, (void *)&cyBle_attValuesCCCD[40] }, /* Client Characteristic Configuration */
    { 0x0001u, (void *)&cyBle_attValues[720] }, /* SELECTED_CHANNEL */
    { 0x0010u, (void *)&cyBle_attValues[721] }, /* Characteristic User Description */
    { 0x0013u, (void *)&cyBle_attValues[737] }, /* Manufacturer Name String */
    { 0x0006u, (void *)&cyBle_attValues[756] }, /* Model Number String */
    { 0x000Cu, (void *)&cyBle_attValues[762] }, /* Serial Number String */
    { 0x0004u, (void *)&cyBle_attValues[774] }, /* Hardware Revision String */
    { 0x0004u, (void *)&cyBle_attValues[778] }, /* Firmware Revision String */
    { 0x0002u, (void *)&cyBle_attValues[782] }, /* Software Revision String */
    { 0x0008u, (void *)&cyBle_attValues[784] }, /* System ID */
    { 0x0001u, (void *)&cyBle_attValues[792] }, /* IEEE 11073-20601 Regulatory Certification Data List */
    { 0x0007u, (void *)&cyBle_attValues[793] }, /* PnP ID */
    { 0x0004u, (void *)&cyBle_attValues[800] }, /* PIN */
    { 0x0003u, (void *)&cyBle_attValues[804] }, /* Characteristic User Description */
    { 0x0001u, (void *)&cyBle_attValues[807] }, /* SENSOR_TYPE */
    { 0x000Bu, (void *)&cyBle_attValues[808] }, /* Characteristic User Description */
    { 0x0001u, (void *)&cyBle_attValues[819] }, /* SENSOR_TYPE2 */
    { 0x000Cu, (void *)&cyBle_attValues[820] }, /* Characteristic User Description */
    { 0x0002u, (void *)&cyBle_attValues[832] }, /* SENSOR_RANGE */
    { 0x000Cu, (void *)&cyBle_attValues[834] }, /* Characteristic User Description */
    { 0x0002u, (void *)&cyBle_attValues[846] }, /* SENSOR_RANGE2 */
    { 0x000Du, (void *)&cyBle_attValues[848] }, /* Characteristic User Description */
    { 0x000Au, (void *)&cyBle_attValues[861] }, /* SENSOR_GAS_NOTE */
    { 0x000Fu, (void *)&cyBle_attValues[871] }, /* Characteristic User Description */
    { 0x000Au, (void *)&cyBle_attValues[886] }, /* SENSOR_GAS_NOTE2 */
    { 0x0010u, (void *)&cyBle_attValues[896] }, /* Characteristic User Description */
    { 0x0014u, (void *)&cyBle_attValues[912] }, /* SENSOR_TAG */
    { 0x000Au, (void *)&cyBle_attValues[932] }, /* Characteristic User Description */
    { 0x0001u, (void *)&cyBle_attValues[942] }, /* SENSOR_TEMPERATURE */
    { 0x0012u, (void *)&cyBle_attValues[943] }, /* Characteristic User Description */
    { 0x0002u, (void *)&cyBle_attValuesCCCD[42] }, /* Client Characteristic Configuration */
    { 0x0002u, (void *)&cyBle_attValues[961] }, /* SENSOR_CAL_LEVEL */
    { 0x0010u, (void *)&cyBle_attValues[963] }, /* Characteristic User Description */
    { 0x0002u, (void *)&cyBle_attValuesCCCD[44] }, /* Client Characteristic Configuration */
    { 0x0002u, (void *)&cyBle_attValues[979] }, /* CAL_Z_COUNTS */
    { 0x000Cu, (void *)&cyBle_attValues[981] }, /* Characteristic User Description */
    { 0x0002u, (void *)&cyBle_attValues[993] }, /* ZERO_READING */
    { 0x000Cu, (void *)&cyBle_attValues[995] }, /* Characteristic User Description */
    { 0x0002u, (void *)&cyBle_attValuesCCCD[46] }, /* Client Characteristic Configuration */
    { 0x0001u, (void *)&cyBle_attValues[1007] }, /* CAL_ZERO_ERROR */
    { 0x000Eu, (void *)&cyBle_attValues[1008] }, /* Characteristic User Description */
    { 0x0002u, (void *)&cyBle_attValuesCCCD[48] }, /* Client Characteristic Configuration */
    { 0x0001u, (void *)&cyBle_attValues[1022] }, /* CAL_STEP */
    { 0x0008u, (void *)&cyBle_attValues[1023] }, /* Characteristic User Description */
    { 0x0002u, (void *)&cyBle_attValuesCCCD[50] }, /* Client Characteristic Configuration */
    { 0x0002u, (void *)&cyBle_attValues[1031] }, /* CAL_SPEED */
    { 0x0009u, (void *)&cyBle_attValues[1033] }, /* Characteristic User Description */
    { 0x0002u, (void *)&cyBle_attValuesCCCD[52] }, /* Client Characteristic Configuration */
    { 0x0004u, (void *)&cyBle_attValues[1042] }, /* CAL_AS_FOUND */
    { 0x000Cu, (void *)&cyBle_attValues[1046] }, /* Characteristic User Description */
    { 0x0002u, (void *)&cyBle_attValuesCCCD[54] }, /* Client Characteristic Configuration */
    { 0x0001u, (void *)&cyBle_attValues[1058] }, /* CAL_ADJUSTED */
    { 0x000Cu, (void *)&cyBle_attValues[1059] }, /* Characteristic User Description */
    { 0x0002u, (void *)&cyBle_attValuesCCCD[56] }, /* Client Characteristic Configuration */
    { 0x0002u, (void *)&cyBle_attValues[1071] }, /* CAL_COUNTS */
    { 0x000Au, (void *)&cyBle_attValues[1073] }, /* Characteristic User Description */
    { 0x0001u, (void *)&cyBle_attValues[1083] }, /* SENSOR_LIFE */
    { 0x000Bu, (void *)&cyBle_attValues[1084] }, /* Characteristic User Description */
    { 0x0002u, (void *)&cyBle_attValuesCCCD[58] }, /* Client Characteristic Configuration */
    { 0x0002u, (void *)&cyBle_attValues[1095] }, /* CAL_CLEARING */
    { 0x000Cu, (void *)&cyBle_attValues[1097] }, /* Characteristic User Description */
    { 0x0002u, (void *)&cyBle_attValuesCCCD[60] }, /* Client Characteristic Configuration */
    { 0x0001u, (void *)&cyBle_attValues[1109] }, /* CAL_SPAN_ERROR */
    { 0x000Eu, (void *)&cyBle_attValues[1110] }, /* Characteristic User Description */
    { 0x0002u, (void *)&cyBle_attValuesCCCD[62] }, /* Client Characteristic Configuration */
    { 0x0001u, (void *)&cyBle_attValues[1124] }, /* CALIBRATION_STATUS */
    { 0x0012u, (void *)&cyBle_attValues[1125] }, /* Characteristic User Description */
    { 0x0002u, (void *)&cyBle_attValuesCCCD[64] }, /* Client Characteristic Configuration */
    { 0x0004u, (void *)&cyBle_attValues[1143] }, /* READING_SCALED */
    { 0x000Eu, (void *)&cyBle_attValues[1147] }, /* Characteristic User Description */
    { 0x0002u, (void *)&cyBle_attValuesCCCD[66] }, /* Client Characteristic Configuration */
    { 0x0001u, (void *)&cyBle_attValues[1161] }, /* ALERT_STATUS */
    { 0x000Cu, (void *)&cyBle_attValues[1162] }, /* Characteristic User Description */
    { 0x0002u, (void *)&cyBle_attValuesCCCD[68] }, /* Client Characteristic Configuration */
};

const CYBLE_GATTS_DB_T cyBle_gattDB[0xE8u] = {
    { 0x0001u, 0x2800u /* Primary service                     */, 0x00000001u /*           */, 0x000Bu, {{0x1800u, NULL}}                           },
    { 0x0002u, 0x2803u /* Characteristic                      */, 0x00020001u /* rd        */, 0x0003u, {{0x2A00u, NULL}}                           },
    { 0x0003u, 0x2A00u /* Device Name                         */, 0x01020001u /* rd        */, 0x0003u, {{0x0016u, (void *)&cyBle_attValuesLen[0]}} },
    { 0x0004u, 0x2803u /* Characteristic                      */, 0x00020001u /* rd        */, 0x0005u, {{0x2A01u, NULL}}                           },
    { 0x0005u, 0x2A01u /* Appearance                          */, 0x01020001u /* rd        */, 0x0005u, {{0x0002u, (void *)&cyBle_attValuesLen[1]}} },
    { 0x0006u, 0x2803u /* Characteristic                      */, 0x00020001u /* rd        */, 0x0007u, {{0x2A04u, NULL}}                           },
    { 0x0007u, 0x2A04u /* Peripheral Preferred Connection Par */, 0x01020001u /* rd        */, 0x0007u, {{0x0008u, (void *)&cyBle_attValuesLen[2]}} },
    { 0x0008u, 0x2803u /* Characteristic                      */, 0x00020001u /* rd        */, 0x0009u, {{0x2AA6u, NULL}}                           },
    { 0x0009u, 0x2AA6u /* Central Address Resolution          */, 0x01020001u /* rd        */, 0x0009u, {{0x0001u, (void *)&cyBle_attValuesLen[3]}} },
    { 0x000Au, 0x2803u /* Characteristic                      */, 0x00020001u /* rd        */, 0x000Bu, {{0x2AC9u, NULL}}                           },
    { 0x000Bu, 0x2AC9u /* Resolvable Private Address Only     */, 0x01020001u /* rd        */, 0x000Bu, {{0x0001u, (void *)&cyBle_attValuesLen[4]}} },
    { 0x000Cu, 0x2800u /* Primary service                     */, 0x00000001u /*           */, 0x000Fu, {{0x1801u, NULL}}                           },
    { 0x000Du, 0x2803u /* Characteristic                      */, 0x00200001u /* ind       */, 0x000Fu, {{0x2A05u, NULL}}                           },
    { 0x000Eu, 0x2A05u /* Service Changed                     */, 0x01200000u /* ind       */, 0x000Fu, {{0x0004u, (void *)&cyBle_attValuesLen[5]}} },
    { 0x000Fu, 0x2902u /* Client Characteristic Configuration */, 0x010A0101u /* rd,wr     */, 0x000Fu, {{0x0002u, (void *)&cyBle_attValuesLen[6]}} },
    { 0x0010u, 0x2800u /* Primary service                     */, 0x00000001u /*           */, 0x007Eu, {{0xB82Cu, NULL}}                           },
    { 0x0011u, 0x2803u /* Characteristic                      */, 0x001A0001u /* rd,wr,ntf */, 0x0014u, {{0x14A7u, NULL}}                           },
    { 0x0012u, 0x14A7u /* SYS_REMOTE                          */, 0x011A0101u /* rd,wr,ntf */, 0x0014u, {{0x0001u, (void *)&cyBle_attValuesLen[7]}} },
    { 0x0013u, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x0013u, {{0x000Au, (void *)&cyBle_attValuesLen[8]}} },
    { 0x0014u, 0x2902u /* Client Characteristic Configuration */, 0x010A0101u /* rd,wr     */, 0x0014u, {{0x0002u, (void *)&cyBle_attValuesLen[9]}} },
    { 0x0015u, 0x2803u /* Characteristic                      */, 0x00120001u /* rd,ntf    */, 0x0018u, {{0x3EEBu, NULL}}                           },
    { 0x0016u, 0x3EEBu /* SYS_READING                         */, 0x01120001u /* rd,ntf    */, 0x0018u, {{0x0002u, (void *)&cyBle_attValuesLen[10]}} },
    { 0x0017u, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x0017u, {{0x000Bu, (void *)&cyBle_attValuesLen[11]}} },
    { 0x0018u, 0x2902u /* SYS_READING_CCCD                    */, 0x010A0101u /* rd,wr     */, 0x0018u, {{0x0002u, (void *)&cyBle_attValuesLen[12]}} },
    { 0x0019u, 0x2803u /* Characteristic                      */, 0x00120001u /* rd,ntf    */, 0x001Cu, {{0x38C9u, NULL}}                           },
    { 0x001Au, 0x38C9u /* SYS_STATUS                          */, 0x01120001u /* rd,ntf    */, 0x001Cu, {{0x0002u, (void *)&cyBle_attValuesLen[13]}} },
    { 0x001Bu, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x001Bu, {{0x000Au, (void *)&cyBle_attValuesLen[14]}} },
    { 0x001Cu, 0x2902u /* Client Characteristic Configuration */, 0x010A0101u /* rd,wr     */, 0x001Cu, {{0x0002u, (void *)&cyBle_attValuesLen[15]}} },
    { 0x001Du, 0x2803u /* Characteristic                      */, 0x000A0001u /* rd,wr     */, 0x001Fu, {{0xD515u, NULL}}                           },
    { 0x001Eu, 0xD515u /* SYS_CAN_CHAN                        */, 0x010A0101u /* rd,wr     */, 0x001Fu, {{0x0001u, (void *)&cyBle_attValuesLen[16]}} },
    { 0x001Fu, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x001Fu, {{0x000Cu, (void *)&cyBle_attValuesLen[17]}} },
    { 0x0020u, 0x2803u /* Characteristic                      */, 0x000A0001u /* rd,wr     */, 0x0022u, {{0xF0E5u, NULL}}                           },
    { 0x0021u, 0xF0E5u /* SYS_CAN_ID                          */, 0x010A0101u /* rd,wr     */, 0x0022u, {{0x0001u, (void *)&cyBle_attValuesLen[18]}} },
    { 0x0022u, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x0022u, {{0x000Au, (void *)&cyBle_attValuesLen[19]}} },
    { 0x0023u, 0x2803u /* Characteristic                      */, 0x000A0001u /* rd,wr     */, 0x0025u, {{0x243Au, NULL}}                           },
    { 0x0024u, 0x243Au /* SYS_CAL_LOOP                        */, 0x010A0101u /* rd,wr     */, 0x0025u, {{0x0002u, (void *)&cyBle_attValuesLen[20]}} },
    { 0x0025u, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x0025u, {{0x000Cu, (void *)&cyBle_attValuesLen[21]}} },
    { 0x0026u, 0x2803u /* Characteristic                      */, 0x00120001u /* rd,ntf    */, 0x0029u, {{0xD959u, NULL}}                           },
    { 0x0027u, 0xD959u /* SYS_ADC                             */, 0x01120001u /* rd,ntf    */, 0x0029u, {{0x0002u, (void *)&cyBle_attValuesLen[22]}} },
    { 0x0028u, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x0028u, {{0x0007u, (void *)&cyBle_attValuesLen[23]}} },
    { 0x0029u, 0x2902u /* Client Characteristic Configuration */, 0x010A0101u /* rd,wr     */, 0x0029u, {{0x0002u, (void *)&cyBle_attValuesLen[24]}} },
    { 0x002Au, 0x2803u /* Characteristic                      */, 0x000A0001u /* rd,wr     */, 0x002Cu, {{0x1C98u, NULL}}                           },
    { 0x002Bu, 0x1C98u /* SYS_ALERT_1                         */, 0x010A0101u /* rd,wr     */, 0x002Cu, {{0x0002u, (void *)&cyBle_attValuesLen[25]}} },
    { 0x002Cu, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x002Cu, {{0x000Bu, (void *)&cyBle_attValuesLen[26]}} },
    { 0x002Du, 0x2803u /* Characteristic                      */, 0x000A0001u /* rd,wr     */, 0x002Fu, {{0x409Bu, NULL}}                           },
    { 0x002Eu, 0x409Bu /* SYS_ALERT_2                         */, 0x010A0101u /* rd,wr     */, 0x002Fu, {{0x0002u, (void *)&cyBle_attValuesLen[27]}} },
    { 0x002Fu, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x002Fu, {{0x000Bu, (void *)&cyBle_attValuesLen[28]}} },
    { 0x0030u, 0x2803u /* Characteristic                      */, 0x000A0001u /* rd,wr     */, 0x0032u, {{0x7830u, NULL}}                           },
    { 0x0031u, 0x7830u /* SYS_ALERT_1_ASC                     */, 0x010A0101u /* rd,wr     */, 0x0032u, {{0x0002u, (void *)&cyBle_attValuesLen[29]}} },
    { 0x0032u, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x0032u, {{0x000Fu, (void *)&cyBle_attValuesLen[30]}} },
    { 0x0033u, 0x2803u /* Characteristic                      */, 0x000A0001u /* rd,wr     */, 0x0035u, {{0x85DDu, NULL}}                           },
    { 0x0034u, 0x85DDu /* SYS_ALERT_2_ASC                     */, 0x010A0101u /* rd,wr     */, 0x0035u, {{0x0002u, (void *)&cyBle_attValuesLen[31]}} },
    { 0x0035u, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x0035u, {{0x000Fu, (void *)&cyBle_attValuesLen[32]}} },
    { 0x0036u, 0x2803u /* Characteristic                      */, 0x00020001u /* rd        */, 0x0038u, {{0x805Fu, NULL}}                           },
    { 0x0037u, 0x805Fu /* SYS_FRACTIONAL                      */, 0x01020001u /* rd        */, 0x0038u, {{0x0002u, (void *)&cyBle_attValuesLen[33]}} },
    { 0x0038u, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x0038u, {{0x000Eu, (void *)&cyBle_attValuesLen[34]}} },
    { 0x0039u, 0x2803u /* Characteristic                      */, 0x00120001u /* rd,ntf    */, 0x003Cu, {{0x48A5u, NULL}}                           },
    { 0x003Au, 0x48A5u /* RESOLUTION                          */, 0x01120001u /* rd,ntf    */, 0x003Cu, {{0x0001u, (void *)&cyBle_attValuesLen[35]}} },
    { 0x003Bu, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x003Bu, {{0x000Au, (void *)&cyBle_attValuesLen[36]}} },
    { 0x003Cu, 0x2902u /* Client Characteristic Configuration */, 0x010A0101u /* rd,wr     */, 0x003Cu, {{0x0002u, (void *)&cyBle_attValuesLen[37]}} },
    { 0x003Du, 0x2803u /* Characteristic                      */, 0x00020001u /* rd        */, 0x003Fu, {{0x97B4u, NULL}}                           },
    { 0x003Eu, 0x97B4u /* RESOLUTION2                         */, 0x01020001u /* rd        */, 0x003Fu, {{0x0001u, (void *)&cyBle_attValuesLen[38]}} },
    { 0x003Fu, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x003Fu, {{0x000Bu, (void *)&cyBle_attValuesLen[39]}} },
    { 0x0040u, 0x2803u /* Characteristic                      */, 0x001A0001u /* rd,wr,ntf */, 0x0043u, {{0x44BAu, NULL}}                           },
    { 0x0041u, 0x44BAu /* RELAYS                              */, 0x011A0101u /* rd,wr,ntf */, 0x0043u, {{0x0004u, (void *)&cyBle_attValuesLen[40]}} },
    { 0x0042u, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x0042u, {{0x0006u, (void *)&cyBle_attValuesLen[41]}} },
    { 0x0043u, 0x2902u /* Client Characteristic Configuration */, 0x010A0101u /* rd,wr     */, 0x0043u, {{0x0002u, (void *)&cyBle_attValuesLen[42]}} },
    { 0x0044u, 0x2803u /* Characteristic                      */, 0x00020001u /* rd        */, 0x0047u, {{0xDC2Cu, NULL}}                           },
    { 0x0045u, 0xDC2Cu /* LOG1                                */, 0x01020001u /* rd        */, 0x0047u, {{0x0028u, (void *)&cyBle_attValuesLen[43]}} },
    { 0x0046u, 0x2902u /* Client Characteristic Configuration */, 0x010A0101u /* rd,wr     */, 0x0046u, {{0x0002u, (void *)&cyBle_attValuesLen[44]}} },
    { 0x0047u, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x0047u, {{0x0004u, (void *)&cyBle_attValuesLen[45]}} },
    { 0x0048u, 0x2803u /* Characteristic                      */, 0x00020001u /* rd        */, 0x004Bu, {{0x7BE0u, NULL}}                           },
    { 0x0049u, 0x7BE0u /* LOG2                                */, 0x01020001u /* rd        */, 0x004Bu, {{0x0028u, (void *)&cyBle_attValuesLen[46]}} },
    { 0x004Au, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x004Au, {{0x0004u, (void *)&cyBle_attValuesLen[47]}} },
    { 0x004Bu, 0x2902u /* Client Characteristic Configuration */, 0x010A0101u /* rd,wr     */, 0x004Bu, {{0x0002u, (void *)&cyBle_attValuesLen[48]}} },
    { 0x004Cu, 0x2803u /* Characteristic                      */, 0x00020001u /* rd        */, 0x004Fu, {{0xECD4u, NULL}}                           },
    { 0x004Du, 0xECD4u /* LOG3                                */, 0x01020001u /* rd        */, 0x004Fu, {{0x0028u, (void *)&cyBle_attValuesLen[49]}} },
    { 0x004Eu, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x004Eu, {{0x0004u, (void *)&cyBle_attValuesLen[50]}} },
    { 0x004Fu, 0x2902u /* Client Characteristic Configuration */, 0x010A0101u /* rd,wr     */, 0x004Fu, {{0x0002u, (void *)&cyBle_attValuesLen[51]}} },
    { 0x0050u, 0x2803u /* Characteristic                      */, 0x00020001u /* rd        */, 0x0053u, {{0x59DEu, NULL}}                           },
    { 0x0051u, 0x59DEu /* LOG4                                */, 0x01020001u /* rd        */, 0x0053u, {{0x0028u, (void *)&cyBle_attValuesLen[52]}} },
    { 0x0052u, 0x2902u /* Client Characteristic Configuration */, 0x010A0101u /* rd,wr     */, 0x0052u, {{0x0002u, (void *)&cyBle_attValuesLen[53]}} },
    { 0x0053u, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x0053u, {{0x0004u, (void *)&cyBle_attValuesLen[54]}} },
    { 0x0054u, 0x2803u /* Characteristic                      */, 0x00020001u /* rd        */, 0x0057u, {{0x9119u, NULL}}                           },
    { 0x0055u, 0x9119u /* LOG5                                */, 0x01020001u /* rd        */, 0x0057u, {{0x0028u, (void *)&cyBle_attValuesLen[55]}} },
    { 0x0056u, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x0056u, {{0x0004u, (void *)&cyBle_attValuesLen[56]}} },
    { 0x0057u, 0x2902u /* Client Characteristic Configuration */, 0x010A0101u /* rd,wr     */, 0x0057u, {{0x0002u, (void *)&cyBle_attValuesLen[57]}} },
    { 0x0058u, 0x2803u /* Characteristic                      */, 0x00020001u /* rd        */, 0x005Bu, {{0xB6DDu, NULL}}                           },
    { 0x0059u, 0xB6DDu /* LOG6                                */, 0x01020001u /* rd        */, 0x005Bu, {{0x0028u, (void *)&cyBle_attValuesLen[58]}} },
    { 0x005Au, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x005Au, {{0x0004u, (void *)&cyBle_attValuesLen[59]}} },
    { 0x005Bu, 0x2902u /* Client Characteristic Configuration */, 0x010A0101u /* rd,wr     */, 0x005Bu, {{0x0002u, (void *)&cyBle_attValuesLen[60]}} },
    { 0x005Cu, 0x2803u /* Characteristic                      */, 0x00020001u /* rd        */, 0x005Fu, {{0x0E9Du, NULL}}                           },
    { 0x005Du, 0x0E9Du /* LOG7                                */, 0x01020001u /* rd        */, 0x005Fu, {{0x0028u, (void *)&cyBle_attValuesLen[61]}} },
    { 0x005Eu, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x005Eu, {{0x0004u, (void *)&cyBle_attValuesLen[62]}} },
    { 0x005Fu, 0x2902u /* Client Characteristic Configuration */, 0x010A0101u /* rd,wr     */, 0x005Fu, {{0x0002u, (void *)&cyBle_attValuesLen[63]}} },
    { 0x0060u, 0x2803u /* Characteristic                      */, 0x00020001u /* rd        */, 0x0063u, {{0xE70Eu, NULL}}                           },
    { 0x0061u, 0xE70Eu /* LOG8                                */, 0x01020001u /* rd        */, 0x0063u, {{0x0028u, (void *)&cyBle_attValuesLen[64]}} },
    { 0x0062u, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x0062u, {{0x0004u, (void *)&cyBle_attValuesLen[65]}} },
    { 0x0063u, 0x2902u /* Client Characteristic Configuration */, 0x010A0101u /* rd,wr     */, 0x0063u, {{0x0002u, (void *)&cyBle_attValuesLen[66]}} },
    { 0x0064u, 0x2803u /* Characteristic                      */, 0x00020001u /* rd        */, 0x0067u, {{0x8D3Au, NULL}}                           },
    { 0x0065u, 0x8D3Au /* LOG9                                */, 0x01020001u /* rd        */, 0x0067u, {{0x0028u, (void *)&cyBle_attValuesLen[67]}} },
    { 0x0066u, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x0066u, {{0x0004u, (void *)&cyBle_attValuesLen[68]}} },
    { 0x0067u, 0x2902u /* Client Characteristic Configuration */, 0x010A0101u /* rd,wr     */, 0x0067u, {{0x0002u, (void *)&cyBle_attValuesLen[69]}} },
    { 0x0068u, 0x2803u /* Characteristic                      */, 0x00020001u /* rd        */, 0x006Bu, {{0x3AE5u, NULL}}                           },
    { 0x0069u, 0x3AE5u /* LOG10                               */, 0x01020001u /* rd        */, 0x006Bu, {{0x0028u, (void *)&cyBle_attValuesLen[70]}} },
    { 0x006Au, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x006Au, {{0x0005u, (void *)&cyBle_attValuesLen[71]}} },
    { 0x006Bu, 0x2902u /* Client Characteristic Configuration */, 0x010A0101u /* rd,wr     */, 0x006Bu, {{0x0002u, (void *)&cyBle_attValuesLen[72]}} },
    { 0x006Cu, 0x2803u /* Characteristic                      */, 0x001A0001u /* rd,wr,ntf */, 0x006Fu, {{0x9017u, NULL}}                           },
    { 0x006Du, 0x9017u /* SLEEP_TIME                          */, 0x011A0101u /* rd,wr,ntf */, 0x006Fu, {{0x0002u, (void *)&cyBle_attValuesLen[73]}} },
    { 0x006Eu, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x006Eu, {{0x000Au, (void *)&cyBle_attValuesLen[74]}} },
    { 0x006Fu, 0x2902u /* Client Characteristic Configuration */, 0x010A0101u /* rd,wr     */, 0x006Fu, {{0x0002u, (void *)&cyBle_attValuesLen[75]}} },
    { 0x0070u, 0x2803u /* Characteristic                      */, 0x001A0001u /* rd,wr,ntf */, 0x0073u, {{0x6156u, NULL}}                           },
    { 0x0071u, 0x6156u /* GPS_LAT                             */, 0x011A0101u /* rd,wr,ntf */, 0x0073u, {{0x0004u, (void *)&cyBle_attValuesLen[76]}} },
    { 0x0072u, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x0072u, {{0x0007u, (void *)&cyBle_attValuesLen[77]}} },
    { 0x0073u, 0x2902u /* Client Characteristic Configuration */, 0x010A0101u /* rd,wr     */, 0x0073u, {{0x0002u, (void *)&cyBle_attValuesLen[78]}} },
    { 0x0074u, 0x2803u /* Characteristic                      */, 0x001A0001u /* rd,wr,ntf */, 0x0077u, {{0x9B53u, NULL}}                           },
    { 0x0075u, 0x9B53u /* GPS_LONG                            */, 0x011A0101u /* rd,wr,ntf */, 0x0077u, {{0x0004u, (void *)&cyBle_attValuesLen[79]}} },
    { 0x0076u, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x0076u, {{0x0008u, (void *)&cyBle_attValuesLen[80]}} },
    { 0x0077u, 0x2902u /* Client Characteristic Configuration */, 0x010A0101u /* rd,wr     */, 0x0077u, {{0x0002u, (void *)&cyBle_attValuesLen[81]}} },
    { 0x0078u, 0x2803u /* Characteristic                      */, 0x000A0001u /* rd,wr     */, 0x007Bu, {{0x40D2u, NULL}}                           },
    { 0x0079u, 0x40D2u /* DATE_TIME                           */, 0x010A0101u /* rd,wr     */, 0x007Bu, {{0x0005u, (void *)&cyBle_attValuesLen[82]}} },
    { 0x007Au, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x007Au, {{0x0009u, (void *)&cyBle_attValuesLen[83]}} },
    { 0x007Bu, 0x2902u /* Client Characteristic Configuration */, 0x010A0101u /* rd,wr     */, 0x007Bu, {{0x0002u, (void *)&cyBle_attValuesLen[84]}} },
    { 0x007Cu, 0x2803u /* Characteristic                      */, 0x000A0001u /* rd,wr     */, 0x007Eu, {{0x925Au, NULL}}                           },
    { 0x007Du, 0x925Au /* SELECTED_CHANNEL                    */, 0x010A0101u /* rd,wr     */, 0x007Eu, {{0x0001u, (void *)&cyBle_attValuesLen[85]}} },
    { 0x007Eu, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x007Eu, {{0x0010u, (void *)&cyBle_attValuesLen[86]}} },
    { 0x007Fu, 0x2800u /* Primary service                     */, 0x00000001u /*           */, 0x0091u, {{0x180Au, NULL}}                           },
    { 0x0080u, 0x2803u /* Characteristic                      */, 0x00020001u /* rd        */, 0x0081u, {{0x2A29u, NULL}}                           },
    { 0x0081u, 0x2A29u /* Manufacturer Name String            */, 0x01020001u /* rd        */, 0x0081u, {{0x0013u, (void *)&cyBle_attValuesLen[87]}} },
    { 0x0082u, 0x2803u /* Characteristic                      */, 0x00020001u /* rd        */, 0x0083u, {{0x2A24u, NULL}}                           },
    { 0x0083u, 0x2A24u /* Model Number String                 */, 0x01020001u /* rd        */, 0x0083u, {{0x0006u, (void *)&cyBle_attValuesLen[88]}} },
    { 0x0084u, 0x2803u /* Characteristic                      */, 0x00020001u /* rd        */, 0x0085u, {{0x2A25u, NULL}}                           },
    { 0x0085u, 0x2A25u /* Serial Number String                */, 0x01020001u /* rd        */, 0x0085u, {{0x000Cu, (void *)&cyBle_attValuesLen[89]}} },
    { 0x0086u, 0x2803u /* Characteristic                      */, 0x00020001u /* rd        */, 0x0087u, {{0x2A27u, NULL}}                           },
    { 0x0087u, 0x2A27u /* Hardware Revision String            */, 0x01020001u /* rd        */, 0x0087u, {{0x0004u, (void *)&cyBle_attValuesLen[90]}} },
    { 0x0088u, 0x2803u /* Characteristic                      */, 0x00020001u /* rd        */, 0x0089u, {{0x2A26u, NULL}}                           },
    { 0x0089u, 0x2A26u /* Firmware Revision String            */, 0x01020001u /* rd        */, 0x0089u, {{0x0004u, (void *)&cyBle_attValuesLen[91]}} },
    { 0x008Au, 0x2803u /* Characteristic                      */, 0x00020001u /* rd        */, 0x008Bu, {{0x2A28u, NULL}}                           },
    { 0x008Bu, 0x2A28u /* Software Revision String            */, 0x01020001u /* rd        */, 0x008Bu, {{0x0002u, (void *)&cyBle_attValuesLen[92]}} },
    { 0x008Cu, 0x2803u /* Characteristic                      */, 0x00020001u /* rd        */, 0x008Du, {{0x2A23u, NULL}}                           },
    { 0x008Du, 0x2A23u /* System ID                           */, 0x01020001u /* rd        */, 0x008Du, {{0x0008u, (void *)&cyBle_attValuesLen[93]}} },
    { 0x008Eu, 0x2803u /* Characteristic                      */, 0x00020001u /* rd        */, 0x008Fu, {{0x2A2Au, NULL}}                           },
    { 0x008Fu, 0x2A2Au /* IEEE 11073-20601 Regulatory Certifi */, 0x01020001u /* rd        */, 0x008Fu, {{0x0001u, (void *)&cyBle_attValuesLen[94]}} },
    { 0x0090u, 0x2803u /* Characteristic                      */, 0x00020001u /* rd        */, 0x0091u, {{0x2A50u, NULL}}                           },
    { 0x0091u, 0x2A50u /* PnP ID                              */, 0x01020001u /* rd        */, 0x0091u, {{0x0007u, (void *)&cyBle_attValuesLen[95]}} },
    { 0x0092u, 0x2800u /* Primary service                     */, 0x00000001u /*           */, 0x00E8u, {{0xE551u, NULL}}                           },
    { 0x0093u, 0x2803u /* Characteristic                      */, 0x000A0001u /* rd,wr     */, 0x0095u, {{0x6051u, NULL}}                           },
    { 0x0094u, 0x6051u /* PIN                                 */, 0x010A0101u /* rd,wr     */, 0x0095u, {{0x0004u, (void *)&cyBle_attValuesLen[96]}} },
    { 0x0095u, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x0095u, {{0x0003u, (void *)&cyBle_attValuesLen[97]}} },
    { 0x0096u, 0x2803u /* Characteristic                      */, 0x00020001u /* rd        */, 0x0098u, {{0x670Bu, NULL}}                           },
    { 0x0097u, 0x670Bu /* SENSOR_TYPE                         */, 0x01020001u /* rd        */, 0x0098u, {{0x0001u, (void *)&cyBle_attValuesLen[98]}} },
    { 0x0098u, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x0098u, {{0x000Bu, (void *)&cyBle_attValuesLen[99]}} },
    { 0x0099u, 0x2803u /* Characteristic                      */, 0x00020001u /* rd        */, 0x009Bu, {{0xF5A9u, NULL}}                           },
    { 0x009Au, 0xF5A9u /* SENSOR_TYPE2                        */, 0x01020001u /* rd        */, 0x009Bu, {{0x0001u, (void *)&cyBle_attValuesLen[100]}} },
    { 0x009Bu, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x009Bu, {{0x000Cu, (void *)&cyBle_attValuesLen[101]}} },
    { 0x009Cu, 0x2803u /* Characteristic                      */, 0x00020001u /* rd        */, 0x009Eu, {{0xFDF7u, NULL}}                           },
    { 0x009Du, 0xFDF7u /* SENSOR_RANGE                        */, 0x01020001u /* rd        */, 0x009Eu, {{0x0002u, (void *)&cyBle_attValuesLen[102]}} },
    { 0x009Eu, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x009Eu, {{0x000Cu, (void *)&cyBle_attValuesLen[103]}} },
    { 0x009Fu, 0x2803u /* Characteristic                      */, 0x00020001u /* rd        */, 0x00A1u, {{0x529Du, NULL}}                           },
    { 0x00A0u, 0x529Du /* SENSOR_RANGE2                       */, 0x01020001u /* rd        */, 0x00A1u, {{0x0002u, (void *)&cyBle_attValuesLen[104]}} },
    { 0x00A1u, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x00A1u, {{0x000Du, (void *)&cyBle_attValuesLen[105]}} },
    { 0x00A2u, 0x2803u /* Characteristic                      */, 0x00020001u /* rd        */, 0x00A4u, {{0x8547u, NULL}}                           },
    { 0x00A3u, 0x8547u /* SENSOR_GAS_NOTE                     */, 0x01020001u /* rd        */, 0x00A4u, {{0x000Au, (void *)&cyBle_attValuesLen[106]}} },
    { 0x00A4u, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x00A4u, {{0x000Fu, (void *)&cyBle_attValuesLen[107]}} },
    { 0x00A5u, 0x2803u /* Characteristic                      */, 0x00020001u /* rd        */, 0x00A7u, {{0x2E2Fu, NULL}}                           },
    { 0x00A6u, 0x2E2Fu /* SENSOR_GAS_NOTE2                    */, 0x01020001u /* rd        */, 0x00A7u, {{0x000Au, (void *)&cyBle_attValuesLen[108]}} },
    { 0x00A7u, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x00A7u, {{0x0010u, (void *)&cyBle_attValuesLen[109]}} },
    { 0x00A8u, 0x2803u /* Characteristic                      */, 0x000A0001u /* rd,wr     */, 0x00AAu, {{0x32BDu, NULL}}                           },
    { 0x00A9u, 0x32BDu /* SENSOR_TAG                          */, 0x010A0101u /* rd,wr     */, 0x00AAu, {{0x0014u, (void *)&cyBle_attValuesLen[110]}} },
    { 0x00AAu, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x00AAu, {{0x000Au, (void *)&cyBle_attValuesLen[111]}} },
    { 0x00ABu, 0x2803u /* Characteristic                      */, 0x00120001u /* rd,ntf    */, 0x00AEu, {{0x4A4Cu, NULL}}                           },
    { 0x00ACu, 0x4A4Cu /* SENSOR_TEMPERATURE                  */, 0x01120001u /* rd,ntf    */, 0x00AEu, {{0x0001u, (void *)&cyBle_attValuesLen[112]}} },
    { 0x00ADu, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x00ADu, {{0x0012u, (void *)&cyBle_attValuesLen[113]}} },
    { 0x00AEu, 0x2902u /* Client Characteristic Configuration */, 0x010A0101u /* rd,wr     */, 0x00AEu, {{0x0002u, (void *)&cyBle_attValuesLen[114]}} },
    { 0x00AFu, 0x2803u /* Characteristic                      */, 0x001A0001u /* rd,wr,ntf */, 0x00B2u, {{0x645Du, NULL}}                           },
    { 0x00B0u, 0x645Du /* SENSOR_CAL_LEVEL                    */, 0x011A0101u /* rd,wr,ntf */, 0x00B2u, {{0x0002u, (void *)&cyBle_attValuesLen[115]}} },
    { 0x00B1u, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x00B1u, {{0x0010u, (void *)&cyBle_attValuesLen[116]}} },
    { 0x00B2u, 0x2902u /* Client Characteristic Configuration */, 0x010A0101u /* rd,wr     */, 0x00B2u, {{0x0002u, (void *)&cyBle_attValuesLen[117]}} },
    { 0x00B3u, 0x2803u /* Characteristic                      */, 0x00020001u /* rd        */, 0x00B5u, {{0xDFD6u, NULL}}                           },
    { 0x00B4u, 0xDFD6u /* CAL_Z_COUNTS                        */, 0x01020001u /* rd        */, 0x00B5u, {{0x0002u, (void *)&cyBle_attValuesLen[118]}} },
    { 0x00B5u, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x00B5u, {{0x000Cu, (void *)&cyBle_attValuesLen[119]}} },
    { 0x00B6u, 0x2803u /* Characteristic                      */, 0x00120001u /* rd,ntf    */, 0x00B9u, {{0x7D4Fu, NULL}}                           },
    { 0x00B7u, 0x7D4Fu /* ZERO_READING                        */, 0x01120001u /* rd,ntf    */, 0x00B9u, {{0x0002u, (void *)&cyBle_attValuesLen[120]}} },
    { 0x00B8u, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x00B8u, {{0x000Cu, (void *)&cyBle_attValuesLen[121]}} },
    { 0x00B9u, 0x2902u /* Client Characteristic Configuration */, 0x010A0101u /* rd,wr     */, 0x00B9u, {{0x0002u, (void *)&cyBle_attValuesLen[122]}} },
    { 0x00BAu, 0x2803u /* Characteristic                      */, 0x00120001u /* rd,ntf    */, 0x00BDu, {{0x71ADu, NULL}}                           },
    { 0x00BBu, 0x71ADu /* CAL_ZERO_ERROR                      */, 0x01120001u /* rd,ntf    */, 0x00BDu, {{0x0001u, (void *)&cyBle_attValuesLen[123]}} },
    { 0x00BCu, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x00BCu, {{0x000Eu, (void *)&cyBle_attValuesLen[124]}} },
    { 0x00BDu, 0x2902u /* Client Characteristic Configuration */, 0x010A0101u /* rd,wr     */, 0x00BDu, {{0x0002u, (void *)&cyBle_attValuesLen[125]}} },
    { 0x00BEu, 0x2803u /* Characteristic                      */, 0x00120001u /* rd,ntf    */, 0x00C1u, {{0xB354u, NULL}}                           },
    { 0x00BFu, 0xB354u /* CAL_STEP                            */, 0x01120001u /* rd,ntf    */, 0x00C1u, {{0x0001u, (void *)&cyBle_attValuesLen[126]}} },
    { 0x00C0u, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x00C0u, {{0x0008u, (void *)&cyBle_attValuesLen[127]}} },
    { 0x00C1u, 0x2902u /* Client Characteristic Configuration */, 0x010A0101u /* rd,wr     */, 0x00C1u, {{0x0002u, (void *)&cyBle_attValuesLen[128]}} },
    { 0x00C2u, 0x2803u /* Characteristic                      */, 0x00120001u /* rd,ntf    */, 0x00C5u, {{0xEC95u, NULL}}                           },
    { 0x00C3u, 0xEC95u /* CAL_SPEED                           */, 0x01120001u /* rd,ntf    */, 0x00C5u, {{0x0002u, (void *)&cyBle_attValuesLen[129]}} },
    { 0x00C4u, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x00C4u, {{0x0009u, (void *)&cyBle_attValuesLen[130]}} },
    { 0x00C5u, 0x2902u /* Client Characteristic Configuration */, 0x010A0101u /* rd,wr     */, 0x00C5u, {{0x0002u, (void *)&cyBle_attValuesLen[131]}} },
    { 0x00C6u, 0x2803u /* Characteristic                      */, 0x00120001u /* rd,ntf    */, 0x00C9u, {{0xB157u, NULL}}                           },
    { 0x00C7u, 0xB157u /* CAL_AS_FOUND                        */, 0x01120001u /* rd,ntf    */, 0x00C9u, {{0x0004u, (void *)&cyBle_attValuesLen[132]}} },
    { 0x00C8u, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x00C8u, {{0x000Cu, (void *)&cyBle_attValuesLen[133]}} },
    { 0x00C9u, 0x2902u /* Client Characteristic Configuration */, 0x010A0101u /* rd,wr     */, 0x00C9u, {{0x0002u, (void *)&cyBle_attValuesLen[134]}} },
    { 0x00CAu, 0x2803u /* Characteristic                      */, 0x00120001u /* rd,ntf    */, 0x00CDu, {{0x86F7u, NULL}}                           },
    { 0x00CBu, 0x86F7u /* CAL_ADJUSTED                        */, 0x01120001u /* rd,ntf    */, 0x00CDu, {{0x0001u, (void *)&cyBle_attValuesLen[135]}} },
    { 0x00CCu, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x00CCu, {{0x000Cu, (void *)&cyBle_attValuesLen[136]}} },
    { 0x00CDu, 0x2902u /* Client Characteristic Configuration */, 0x010A0101u /* rd,wr     */, 0x00CDu, {{0x0002u, (void *)&cyBle_attValuesLen[137]}} },
    { 0x00CEu, 0x2803u /* Characteristic                      */, 0x00020001u /* rd        */, 0x00D0u, {{0xE8EBu, NULL}}                           },
    { 0x00CFu, 0xE8EBu /* CAL_COUNTS                          */, 0x01020001u /* rd        */, 0x00D0u, {{0x0002u, (void *)&cyBle_attValuesLen[138]}} },
    { 0x00D0u, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x00D0u, {{0x000Au, (void *)&cyBle_attValuesLen[139]}} },
    { 0x00D1u, 0x2803u /* Characteristic                      */, 0x00120001u /* rd,ntf    */, 0x00D4u, {{0x2EEFu, NULL}}                           },
    { 0x00D2u, 0x2EEFu /* SENSOR_LIFE                         */, 0x01120001u /* rd,ntf    */, 0x00D4u, {{0x0001u, (void *)&cyBle_attValuesLen[140]}} },
    { 0x00D3u, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x00D3u, {{0x000Bu, (void *)&cyBle_attValuesLen[141]}} },
    { 0x00D4u, 0x2902u /* Client Characteristic Configuration */, 0x010A0101u /* rd,wr     */, 0x00D4u, {{0x0002u, (void *)&cyBle_attValuesLen[142]}} },
    { 0x00D5u, 0x2803u /* Characteristic                      */, 0x00120001u /* rd,ntf    */, 0x00D8u, {{0x1ECAu, NULL}}                           },
    { 0x00D6u, 0x1ECAu /* CAL_CLEARING                        */, 0x01120001u /* rd,ntf    */, 0x00D8u, {{0x0002u, (void *)&cyBle_attValuesLen[143]}} },
    { 0x00D7u, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x00D7u, {{0x000Cu, (void *)&cyBle_attValuesLen[144]}} },
    { 0x00D8u, 0x2902u /* Client Characteristic Configuration */, 0x010A0101u /* rd,wr     */, 0x00D8u, {{0x0002u, (void *)&cyBle_attValuesLen[145]}} },
    { 0x00D9u, 0x2803u /* Characteristic                      */, 0x00120001u /* rd,ntf    */, 0x00DCu, {{0x04B7u, NULL}}                           },
    { 0x00DAu, 0x04B7u /* CAL_SPAN_ERROR                      */, 0x01120001u /* rd,ntf    */, 0x00DCu, {{0x0001u, (void *)&cyBle_attValuesLen[146]}} },
    { 0x00DBu, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x00DBu, {{0x000Eu, (void *)&cyBle_attValuesLen[147]}} },
    { 0x00DCu, 0x2902u /* Client Characteristic Configuration */, 0x010A0101u /* rd,wr     */, 0x00DCu, {{0x0002u, (void *)&cyBle_attValuesLen[148]}} },
    { 0x00DDu, 0x2803u /* Characteristic                      */, 0x001A0001u /* rd,wr,ntf */, 0x00E0u, {{0x276Du, NULL}}                           },
    { 0x00DEu, 0x276Du /* CALIBRATION_STATUS                  */, 0x011A0101u /* rd,wr,ntf */, 0x00E0u, {{0x0001u, (void *)&cyBle_attValuesLen[149]}} },
    { 0x00DFu, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x00DFu, {{0x0012u, (void *)&cyBle_attValuesLen[150]}} },
    { 0x00E0u, 0x2902u /* Client Characteristic Configuration */, 0x010A0101u /* rd,wr     */, 0x00E0u, {{0x0002u, (void *)&cyBle_attValuesLen[151]}} },
    { 0x00E1u, 0x2803u /* Characteristic                      */, 0x00120001u /* rd,ntf    */, 0x00E4u, {{0x1825u, NULL}}                           },
    { 0x00E2u, 0x1825u /* READING_SCALED                      */, 0x01120001u /* rd,ntf    */, 0x00E4u, {{0x0004u, (void *)&cyBle_attValuesLen[152]}} },
    { 0x00E3u, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x00E3u, {{0x000Eu, (void *)&cyBle_attValuesLen[153]}} },
    { 0x00E4u, 0x2902u /* Client Characteristic Configuration */, 0x010A0101u /* rd,wr     */, 0x00E4u, {{0x0002u, (void *)&cyBle_attValuesLen[154]}} },
    { 0x00E5u, 0x2803u /* Characteristic                      */, 0x00120001u /* rd,ntf    */, 0x00E8u, {{0x9150u, NULL}}                           },
    { 0x00E6u, 0x9150u /* ALERT_STATUS                        */, 0x01120001u /* rd,ntf    */, 0x00E8u, {{0x0001u, (void *)&cyBle_attValuesLen[155]}} },
    { 0x00E7u, 0x2901u /* Characteristic User Description     */, 0x01020001u /* rd        */, 0x00E7u, {{0x000Cu, (void *)&cyBle_attValuesLen[156]}} },
    { 0x00E8u, 0x2902u /* Client Characteristic Configuration */, 0x010A0101u /* rd,wr     */, 0x00E8u, {{0x0002u, (void *)&cyBle_attValuesLen[157]}} },
};


#endif /* (CYBLE_GATT_ROLE_SERVER) */

#if(CYBLE_GATT_ROLE_CLIENT)
    
CYBLE_CLIENT_STATE_T cyBle_clientState;
CYBLE_GATTC_T cyBle_gattc;
CYBLE_GATT_ATTR_HANDLE_RANGE_T cyBle_gattcDiscoveryRange;
    
#endif /* (CYBLE_GATT_ROLE_CLIENT) */


#if(CYBLE_GATT_ROLE_SERVER)

/****************************************************************************** 
* Function Name: CyBle_GattsReInitGattDb
***************************************************************************//**
* 
*  Reinitializes the GATT database.
* 
*  \return
*  CYBLE_API_RESULT_T: A function result states if it succeeded or failed with
*  error codes:

*  Errors codes                          | Description
*  ------------                          | -----------
*  CYBLE_ERROR_OK						 | GATT database was reinitialized successfully.
*  CYBLE_ERROR_INVALID_STATE             | If the function is called in any state except CYBLE_STATE_DISCONNECTED.
*  CYBLE_ERROR_INVALID_PARAMETER         | If the Database has zero entries or is a NULL pointer.
* 
******************************************************************************/
CYBLE_API_RESULT_T CyBle_GattsReInitGattDb(void)
{
    CYBLE_API_RESULT_T apiResult;
    
    if(CyBle_GetState() == CYBLE_STATE_DISCONNECTED)
    {
        apiResult = CyBle_GattsDbRegister(cyBle_gattDB, CYBLE_GATT_DB_INDEX_COUNT, CYBLE_GATT_DB_MAX_VALUE_LEN);
    }
    else
    {
        apiResult = CYBLE_ERROR_INVALID_STATE;
    }
    
    return(apiResult);
}


/****************************************************************************** 
* Function Name: CyBle_GattsWriteEventHandler
***************************************************************************//**
* 
*  Handles the Write Request Event for GATT service.
* 
*  \param eventParam: The pointer to the data structure specified by the event.
* 
*  \return
*  CYBLE_GATT_ERR_CODE_T: A function returns one of the following status 
*  values.

*  Errors codes                          | Description
*  --------------------                  | -----------
*  CYBLE_GATT_ERR_NONE                   | Write is successful.
* 
******************************************************************************/
CYBLE_GATT_ERR_CODE_T CyBle_GattsWriteEventHandler(CYBLE_GATTS_WRITE_REQ_PARAM_T *eventParam)
{
    CYBLE_GATT_ERR_CODE_T gattErr = CYBLE_GATT_ERR_NONE;
    
    /* Client Characteristic Configuration descriptor write request */
    if(eventParam->handleValPair.attrHandle == cyBle_gatts.cccdHandle)
    {
        /* Store value to database */
        gattErr = CyBle_GattsWriteAttributeValue(&eventParam->handleValPair, 0u, 
                        &eventParam->connHandle, CYBLE_GATT_DB_PEER_INITIATED);
        
        if(gattErr == CYBLE_GATT_ERR_NONE)
        {
            if(CYBLE_IS_INDICATION_ENABLED_IN_PTR(eventParam->handleValPair.value.val))
            {
                CyBle_ApplCallback((uint32)CYBLE_EVT_GATTS_INDICATION_ENABLED, eventParam);
            }
            else
            {
                CyBle_ApplCallback((uint32)CYBLE_EVT_GATTS_INDICATION_DISABLED, eventParam);
            }
        }
        cyBle_eventHandlerFlag &= (uint8)~CYBLE_CALLBACK;
    }
    return (gattErr);
}


#endif /* (CYBLE_GATT_ROLE_SERVER) */

#if(CYBLE_GATT_ROLE_CLIENT)

/****************************************************************************** 
* Function Name: CyBle_GattcStartDiscovery
***************************************************************************//**
* 
*  Starts the automatic server discovery process. 
*   
*   Discovery procedure is based on the user configuration. It discovers only services, characteristics, 
*   descriptors which were declared in the GATT database.
*   Discovery procedure has the following flow:
*   - discovering primary services by BLE Stack function CyBle_GattcDiscoverAllPrimaryServices();
*   - discovering included services by BLE Stack function CyBle_GattcFindIncludedServices();
*   - discovering characteristics for available services by BLE Stack function CyBle_GattcDiscoverAllCharacteristics();
*   - discovering characteristic descriptors by BLE Stack function CyBle_GattcDiscoverAllCharacteristicDescriptors();
*
*   During the discovery procedure the discovery-specific stack events are handled by the component 
*   and thus aren’t passed to the application callback: CYBLE_EVT_GATTC_READ_BY_GROUP_TYPE_RSP,
*   CYBLE_EVT_GATTC_READ_BY_TYPE_RSP, CYBLE_EVT_GATTC_FIND_INFO_RSP, CYBLE_EVT_GATTC_ERROR_RSP.              
*   
*   After the discovery procedure all information about available services is stored in CYBLE_DISC_SRVC_INFO_T structures,
*   and discovered attributes handles are stored in service-specific client structures, such as CYBLE_BASC_T for 
*   Battery Service or CYBLE_HRSC_T for Heart Rate Service.
*
*  \param connHandle: The handle which consists of the device ID and ATT connection ID.
* 
*  \return
*  CYBLE_API_RESULT_T : Return value indicates if the function succeeded or
*                        failed. Following are the possible error codes.
*
*   <table>	
*   <tr>
*	  <th>Errors codes</th>
*	  <th>Description</th>
*	</tr>
*	<tr>
*	  <td>CYBLE_ERROR_OK</td>
*	  <td>On successful operation</td>
*	</tr>
*	<tr>
*	  <td>CYBLE_ERROR_INVALID_PARAMETER</td>
*	  <td>'connHandle' value does not represent any existing entry.</td>
*	</tr>
*	<tr>
*	  <td>CYBLE_ERROR_INVALID_OPERATION</td>
*	  <td>The operation is not permitted</td>
*	</tr>
*   <tr>
*	  <td>CYBLE_ERROR_MEMORY_ALLOCATION_FAILED</td>
*	  <td>Memory allocation failed</td>
*	</tr>
*   <tr>
*	  <td>CYBLE_ERROR_INVALID_STATE</td>
*	  <td>If the function is called in any state except connected or discovered</td>
*	</tr>
*   </table>
*
*  \events
*   The following events may be generated after calling this function:
*   * CYBLE_EVT_GATTC_DISCOVERY_COMPLETE  - event is generated when the remote device was successfully discovered. 
*   * CYBLE_EVT_GATTC_ERROR_RSP  		- is generated if the device discovery has failed.
*   * CYBLE_EVT_GATTC_SRVC_DUPLICATION 	- is generated if duplicate service record was found during the server device
*        								  discovery.
*   * CYBLE_EVT_GATTC_CHAR_DUPLICATION 	- is generated if duplicate service's characteristic descriptor record was found
*        								  during the server device discovery.
*   * CYBLE_EVT_GATTC_DESCR_DUPLICATION - is generated if duplicate service's characteristic descriptor record was found
*        								  during the server device discovery.
*  
******************************************************************************/
CYBLE_API_RESULT_T CyBle_GattcStartDiscovery(CYBLE_CONN_HANDLE_T connHandle)
{
    uint8 j;
    CYBLE_API_RESULT_T apiResult;
    
    if((CyBle_GetState() != CYBLE_STATE_CONNECTED) || 
       ((CyBle_GetClientState() != CYBLE_CLIENT_STATE_CONNECTED) && 
        (CyBle_GetClientState() != CYBLE_CLIENT_STATE_DISCOVERED))) 
    {
        apiResult = CYBLE_ERROR_INVALID_STATE;
    }
    else
    {
        /* Clean old discovery information */
        for(j = 0u; j < (uint8) CYBLE_SRVI_COUNT; j++)
        {
            (void)memset(&cyBle_serverInfo[j].range, 0, sizeof(cyBle_serverInfo[0].range));
        }

        cyBle_connHandle = connHandle;
        cyBle_gattcDiscoveryRange.startHandle = CYBLE_GATT_ATTR_HANDLE_START_RANGE;
        cyBle_gattcDiscoveryRange.endHandle = CYBLE_GATT_ATTR_HANDLE_END_RANGE;
        
        CyBle_ServiceInit();
        
        apiResult = CyBle_GattcDiscoverAllPrimaryServices(connHandle);

        if(apiResult == CYBLE_ERROR_OK)
        {
            CyBle_SetClientState(CYBLE_CLIENT_STATE_SRVC_DISCOVERING);
            cyBle_eventHandlerFlag |= CYBLE_AUTO_DISCOVERY;
        }
    }
    
    return (apiResult);
}


/****************************************************************************** 
* Function Name: CyBle_GattcStartPartialDiscovery
***************************************************************************//**
* 
*  Starts the automatic server discovery process as per the range provided
*  on a GATT Server to which it is connected. This function could be used for 
*  partial server discovery after indication received to the Service Changed
*  Characteristic Value. 
* 
*  \param connHandle: The handle which consists of the device ID and ATT connection ID.
*  \param startHandle: Start of affected attribute handle range.
*  \param endHandle: End of affected attribute handle range.
* 
*  \return
*	CYBLE_API_RESULT_T : Return value indicates if the function succeeded or
*                        failed. Following are the possible error codes.
*
*   <table>	
*   <tr>
*	  <th>Errors codes</th>
*	  <th>Description</th>
*	</tr>
*	<tr>
*	  <td>CYBLE_ERROR_OK</td>
*	  <td>On successful operation</td>
*	</tr>
*	<tr>
*	  <td>CYBLE_ERROR_INVALID_PARAMETER</td>
*	  <td>'connHandle' value does not represent any existing entry.</td>
*	</tr>
*	<tr>
*	  <td>CYBLE_ERROR_INVALID_OPERATION</td>
*	  <td>The operation is not permitted</td>
*	</tr>
*   <tr>
*	  <td>CYBLE_ERROR_MEMORY_ALLOCATION_FAILED</td>
*	  <td>Memory allocation failed</td>
*	</tr>
*   <tr>
*	  <td>CYBLE_ERROR_INVALID_STATE</td>
*	  <td>If the function is called in any state except connected or discovered</td>
*	</tr>
*   </table>
* 
*  \events
*  Two events may be generated after calling this function:
*  CYBLE_EVT_GATTC_DISCOVERY_COMPLETE or CYBLE_EVT_GATTC_ERROR_RSP.
*  The CYBLE_EVT_GATTC_DISCOVERY_COMPLETE event is 
*  generated when the remote device was successfully discovered. The
*  CYBLE_EVT_GATTC_ERROR_RSP is generated if the device discovery is failed.
*
******************************************************************************/
CYBLE_API_RESULT_T CyBle_GattcStartPartialDiscovery(CYBLE_CONN_HANDLE_T connHandle,
                        CYBLE_GATT_DB_ATTR_HANDLE_T startHandle, CYBLE_GATT_DB_ATTR_HANDLE_T endHandle)
{
    uint8 j;
    CYBLE_API_RESULT_T apiResult;
    
    if((CyBle_GetState() != CYBLE_STATE_CONNECTED) || 
       ((CyBle_GetClientState() != CYBLE_CLIENT_STATE_CONNECTED) && 
        (CyBle_GetClientState() != CYBLE_CLIENT_STATE_DISCOVERED))) 
    {
        apiResult = CYBLE_ERROR_INVALID_STATE;
    }
    else
    {
        /* Clean old discovery information of affected attribute range */
        for(j = 0u; j < (uint8) CYBLE_SRVI_COUNT; j++)
        {
            if((cyBle_serverInfo[j].range.startHandle >= startHandle) &&
               (cyBle_serverInfo[j].range.startHandle <= endHandle))
            {
                (void)memset(&cyBle_serverInfo[j].range, 0, sizeof(cyBle_serverInfo[0].range));
            }
        }

        cyBle_connHandle = connHandle;
        cyBle_gattcDiscoveryRange.startHandle = startHandle;
        cyBle_gattcDiscoveryRange.endHandle = endHandle;

        CyBle_ServiceInit();

        apiResult = CyBle_GattcDiscoverPrimaryServices(connHandle, &cyBle_gattcDiscoveryRange);

        if(apiResult == CYBLE_ERROR_OK)
        {
            CyBle_SetClientState(CYBLE_CLIENT_STATE_SRVC_DISCOVERING);
            cyBle_eventHandlerFlag |= CYBLE_AUTO_DISCOVERY;
        }
    }
    
    return (apiResult);
}


/******************************************************************************
* Function Name: CyBle_GattcDiscoverCharacteristicsEventHandler
***************************************************************************//**
* 
*  This function is called on receiving a CYBLE_EVT_GATTC_READ_BY_TYPE_RSP
*  event. Based on the service UUID, an appropriate data structure is populated
*  using the data received as part of the callback.
* 
*  \param discCharInfo: The pointer to a characteristic information structure.
* 
* \return
*  None
* 
******************************************************************************/
void CyBle_GattcDiscoverCharacteristicsEventHandler(CYBLE_DISC_CHAR_INFO_T *discCharInfo)
{
    if(discCharInfo->uuid.uuid16 == CYBLE_UUID_CHAR_SERVICE_CHANGED)
    {
        CyBle_CheckStoreCharHandle(cyBle_gattc.serviceChanged);
    }
}


/******************************************************************************
* Function Name: CyBle_GattcDiscoverCharDescriptorsEventHandler
***************************************************************************//**
* 
*  This function is called on receiving a "CYBLE_EVT_GATTC_FIND_INFO_RSP" event.
*  Based on the descriptor UUID, an appropriate data structure is populated 
*  using the data received as part of the callback.
* 
*  \param discDescrInfo: The pointer to a descriptor information structure.
*  \param discoveryService: The index of the service instance
* 
* \return
*  None
* 
******************************************************************************/
void CyBle_GattcDiscoverCharDescriptorsEventHandler(CYBLE_DISC_DESCR_INFO_T *discDescrInfo)
{
    if(discDescrInfo->uuid.uuid16 == CYBLE_UUID_CHAR_CLIENT_CONFIG)
    {
        CyBle_CheckStoreCharDescrHandle(cyBle_gattc.cccdHandle);
    }
}


/******************************************************************************
* Function Name: CyBle_GattcIndicationEventHandler
***************************************************************************//**
* 
*  Handles the Indication Event.
* 
*  \param eventParam: The pointer to the data structure specified by the event.
* 
* \return
*  None.
* 
******************************************************************************/
void CyBle_GattcIndicationEventHandler(CYBLE_GATTC_HANDLE_VALUE_IND_PARAM_T *eventParam)
{
    if(cyBle_gattc.serviceChanged.valueHandle == eventParam->handleValPair.attrHandle)
    {
        CyBle_ApplCallback((uint32)CYBLE_EVT_GATTC_INDICATION, eventParam);
        cyBle_eventHandlerFlag &= (uint8)~CYBLE_CALLBACK;
    }
}


#endif /* (CYBLE_GATT_ROLE_CLIENT) */


/* [] END OF FILE */
