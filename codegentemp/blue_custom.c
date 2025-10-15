/***************************************************************************//**
* \file CYBLE_custom.c
* \version 3.40
* 
* \brief
*  Contains the source code for the Custom Service.
* 
********************************************************************************
* \copyright
* Copyright 2014-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#include "blue_eventHandler.h"

#ifdef CYBLE_CUSTOM_SERVER

/* If any custom service with custom characteristics is defined in the
* customizer's GUI their handles will be present in this array.
*/
/* This array contains attribute handles for the defined Custom Services and their characteristics and descriptors.
   The array index definitions are located in the CYBLE_custom.h file. */
const CYBLE_CUSTOMS_T cyBle_customs[0x02u] = {

    /* SGA1 service */
    {
        0x0010u, /* Handle of the SGA1 service */ 
        {

            /* SYS_REMOTE characteristic */
            {
                0x0012u, /* Handle of the SYS_REMOTE characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x0013u, /* Handle of the Characteristic User Description descriptor */ 
                    0x0014u, /* Handle of the Client Characteristic Configuration descriptor */ 
                }, 
            },

            /* SYS_READING characteristic */
            {
                0x0016u, /* Handle of the SYS_READING characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x0017u, /* Handle of the Characteristic User Description descriptor */ 
                    0x0018u, /* Handle of the SYS_READING_CCCD descriptor */ 
                }, 
            },

            /* SYS_STATUS characteristic */
            {
                0x001Au, /* Handle of the SYS_STATUS characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x001Bu, /* Handle of the Characteristic User Description descriptor */ 
                    0x001Cu, /* Handle of the Client Characteristic Configuration descriptor */ 
                }, 
            },

            /* SYS_CAN_CHAN characteristic */
            {
                0x001Eu, /* Handle of the SYS_CAN_CHAN characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x001Fu, /* Handle of the Characteristic User Description descriptor */ 
                    CYBLE_GATT_INVALID_ATTR_HANDLE_VALUE, 
                }, 
            },

            /* SYS_CAN_ID characteristic */
            {
                0x0021u, /* Handle of the SYS_CAN_ID characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x0022u, /* Handle of the Characteristic User Description descriptor */ 
                    CYBLE_GATT_INVALID_ATTR_HANDLE_VALUE, 
                }, 
            },

            /* SYS_CAL_LOOP characteristic */
            {
                0x0024u, /* Handle of the SYS_CAL_LOOP characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x0025u, /* Handle of the Characteristic User Description descriptor */ 
                    CYBLE_GATT_INVALID_ATTR_HANDLE_VALUE, 
                }, 
            },

            /* SYS_ADC characteristic */
            {
                0x0027u, /* Handle of the SYS_ADC characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x0028u, /* Handle of the Characteristic User Description descriptor */ 
                    0x0029u, /* Handle of the Client Characteristic Configuration descriptor */ 
                }, 
            },

            /* SYS_ALERT_1 characteristic */
            {
                0x002Bu, /* Handle of the SYS_ALERT_1 characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x002Cu, /* Handle of the Characteristic User Description descriptor */ 
                    CYBLE_GATT_INVALID_ATTR_HANDLE_VALUE, 
                }, 
            },

            /* SYS_ALERT_2 characteristic */
            {
                0x002Eu, /* Handle of the SYS_ALERT_2 characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x002Fu, /* Handle of the Characteristic User Description descriptor */ 
                    CYBLE_GATT_INVALID_ATTR_HANDLE_VALUE, 
                }, 
            },

            /* SYS_ALERT_1_ASC characteristic */
            {
                0x0031u, /* Handle of the SYS_ALERT_1_ASC characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x0032u, /* Handle of the Characteristic User Description descriptor */ 
                    CYBLE_GATT_INVALID_ATTR_HANDLE_VALUE, 
                }, 
            },

            /* SYS_ALERT_2_ASC characteristic */
            {
                0x0034u, /* Handle of the SYS_ALERT_2_ASC characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x0035u, /* Handle of the Characteristic User Description descriptor */ 
                    CYBLE_GATT_INVALID_ATTR_HANDLE_VALUE, 
                }, 
            },

            /* SYS_FRACTIONAL characteristic */
            {
                0x0037u, /* Handle of the SYS_FRACTIONAL characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x0038u, /* Handle of the Characteristic User Description descriptor */ 
                    CYBLE_GATT_INVALID_ATTR_HANDLE_VALUE, 
                }, 
            },

            /* RESOLUTION characteristic */
            {
                0x003Au, /* Handle of the RESOLUTION characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x003Bu, /* Handle of the Characteristic User Description descriptor */ 
                    0x003Cu, /* Handle of the Client Characteristic Configuration descriptor */ 
                }, 
            },

            /* RESOLUTION2 characteristic */
            {
                0x003Eu, /* Handle of the RESOLUTION2 characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x003Fu, /* Handle of the Characteristic User Description descriptor */ 
                    CYBLE_GATT_INVALID_ATTR_HANDLE_VALUE, 
                }, 
            },

            /* RELAYS characteristic */
            {
                0x0041u, /* Handle of the RELAYS characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x0042u, /* Handle of the Characteristic User Description descriptor */ 
                    0x0043u, /* Handle of the Client Characteristic Configuration descriptor */ 
                }, 
            },

            /* LOG1 characteristic */
            {
                0x0045u, /* Handle of the LOG1 characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x0046u, /* Handle of the Client Characteristic Configuration descriptor */ 
                    0x0047u, /* Handle of the Characteristic User Description descriptor */ 
                }, 
            },

            /* LOG2 characteristic */
            {
                0x0049u, /* Handle of the LOG2 characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x004Au, /* Handle of the Characteristic User Description descriptor */ 
                    0x004Bu, /* Handle of the Client Characteristic Configuration descriptor */ 
                }, 
            },

            /* LOG3 characteristic */
            {
                0x004Du, /* Handle of the LOG3 characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x004Eu, /* Handle of the Characteristic User Description descriptor */ 
                    0x004Fu, /* Handle of the Client Characteristic Configuration descriptor */ 
                }, 
            },

            /* LOG4 characteristic */
            {
                0x0051u, /* Handle of the LOG4 characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x0052u, /* Handle of the Client Characteristic Configuration descriptor */ 
                    0x0053u, /* Handle of the Characteristic User Description descriptor */ 
                }, 
            },

            /* LOG5 characteristic */
            {
                0x0055u, /* Handle of the LOG5 characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x0056u, /* Handle of the Characteristic User Description descriptor */ 
                    0x0057u, /* Handle of the Client Characteristic Configuration descriptor */ 
                }, 
            },

            /* LOG6 characteristic */
            {
                0x0059u, /* Handle of the LOG6 characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x005Au, /* Handle of the Characteristic User Description descriptor */ 
                    0x005Bu, /* Handle of the Client Characteristic Configuration descriptor */ 
                }, 
            },

            /* LOG7 characteristic */
            {
                0x005Du, /* Handle of the LOG7 characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x005Eu, /* Handle of the Characteristic User Description descriptor */ 
                    0x005Fu, /* Handle of the Client Characteristic Configuration descriptor */ 
                }, 
            },

            /* LOG8 characteristic */
            {
                0x0061u, /* Handle of the LOG8 characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x0062u, /* Handle of the Characteristic User Description descriptor */ 
                    0x0063u, /* Handle of the Client Characteristic Configuration descriptor */ 
                }, 
            },

            /* LOG9 characteristic */
            {
                0x0065u, /* Handle of the LOG9 characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x0066u, /* Handle of the Characteristic User Description descriptor */ 
                    0x0067u, /* Handle of the Client Characteristic Configuration descriptor */ 
                }, 
            },

            /* LOG10 characteristic */
            {
                0x0069u, /* Handle of the LOG10 characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x006Au, /* Handle of the Characteristic User Description descriptor */ 
                    0x006Bu, /* Handle of the Client Characteristic Configuration descriptor */ 
                }, 
            },

            /* SLEEP_TIME characteristic */
            {
                0x006Du, /* Handle of the SLEEP_TIME characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x006Eu, /* Handle of the Characteristic User Description descriptor */ 
                    0x006Fu, /* Handle of the Client Characteristic Configuration descriptor */ 
                }, 
            },

            /* GPS_LAT characteristic */
            {
                0x0071u, /* Handle of the GPS_LAT characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x0072u, /* Handle of the Characteristic User Description descriptor */ 
                    0x0073u, /* Handle of the Client Characteristic Configuration descriptor */ 
                }, 
            },

            /* GPS_LONG characteristic */
            {
                0x0075u, /* Handle of the GPS_LONG characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x0076u, /* Handle of the Characteristic User Description descriptor */ 
                    0x0077u, /* Handle of the Client Characteristic Configuration descriptor */ 
                }, 
            },

            /* DATE_TIME characteristic */
            {
                0x0079u, /* Handle of the DATE_TIME characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x007Au, /* Handle of the Characteristic User Description descriptor */ 
                    0x007Bu, /* Handle of the Client Characteristic Configuration descriptor */ 
                }, 
            },
        }, 
    },

    /* SGA2 service */
    {
        0x008Fu, /* Handle of the SGA2 service */ 
        {

            /* PIN characteristic */
            {
                0x0091u, /* Handle of the PIN characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x0092u, /* Handle of the Characteristic User Description descriptor */ 
                    CYBLE_GATT_INVALID_ATTR_HANDLE_VALUE, 
                }, 
            },

            /* SENSOR_TYPE characteristic */
            {
                0x0094u, /* Handle of the SENSOR_TYPE characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x0095u, /* Handle of the Characteristic User Description descriptor */ 
                    CYBLE_GATT_INVALID_ATTR_HANDLE_VALUE, 
                }, 
            },

            /* SENSOR_TYPE2 characteristic */
            {
                0x0097u, /* Handle of the SENSOR_TYPE2 characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x0098u, /* Handle of the Characteristic User Description descriptor */ 
                    CYBLE_GATT_INVALID_ATTR_HANDLE_VALUE, 
                }, 
            },

            /* SENSOR_RANGE characteristic */
            {
                0x009Au, /* Handle of the SENSOR_RANGE characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x009Bu, /* Handle of the Characteristic User Description descriptor */ 
                    CYBLE_GATT_INVALID_ATTR_HANDLE_VALUE, 
                }, 
            },

            /* SENSOR_RANGE2 characteristic */
            {
                0x009Du, /* Handle of the SENSOR_RANGE2 characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x009Eu, /* Handle of the Characteristic User Description descriptor */ 
                    CYBLE_GATT_INVALID_ATTR_HANDLE_VALUE, 
                }, 
            },

            /* SENSOR_GAS_NOTE characteristic */
            {
                0x00A0u, /* Handle of the SENSOR_GAS_NOTE characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x00A1u, /* Handle of the Characteristic User Description descriptor */ 
                    CYBLE_GATT_INVALID_ATTR_HANDLE_VALUE, 
                }, 
            },

            /* SENSOR_GAS_NOTE2 characteristic */
            {
                0x00A3u, /* Handle of the SENSOR_GAS_NOTE2 characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x00A4u, /* Handle of the Characteristic User Description descriptor */ 
                    CYBLE_GATT_INVALID_ATTR_HANDLE_VALUE, 
                }, 
            },

            /* SENSOR_TAG characteristic */
            {
                0x00A6u, /* Handle of the SENSOR_TAG characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x00A7u, /* Handle of the Characteristic User Description descriptor */ 
                    CYBLE_GATT_INVALID_ATTR_HANDLE_VALUE, 
                }, 
            },

            /* SENSOR_TEMPERATURE characteristic */
            {
                0x00A9u, /* Handle of the SENSOR_TEMPERATURE characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x00AAu, /* Handle of the Characteristic User Description descriptor */ 
                    0x00ABu, /* Handle of the Client Characteristic Configuration descriptor */ 
                }, 
            },

            /* SENSOR_CAL_LEVEL characteristic */
            {
                0x00ADu, /* Handle of the SENSOR_CAL_LEVEL characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x00AEu, /* Handle of the Characteristic User Description descriptor */ 
                    0x00AFu, /* Handle of the Client Characteristic Configuration descriptor */ 
                }, 
            },

            /* CAL_Z_COUNTS characteristic */
            {
                0x00B1u, /* Handle of the CAL_Z_COUNTS characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x00B2u, /* Handle of the Characteristic User Description descriptor */ 
                    CYBLE_GATT_INVALID_ATTR_HANDLE_VALUE, 
                }, 
            },

            /* ZERO_READING characteristic */
            {
                0x00B4u, /* Handle of the ZERO_READING characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x00B5u, /* Handle of the Characteristic User Description descriptor */ 
                    0x00B6u, /* Handle of the Client Characteristic Configuration descriptor */ 
                }, 
            },

            /* CAL_ZERO_ERROR characteristic */
            {
                0x00B8u, /* Handle of the CAL_ZERO_ERROR characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x00B9u, /* Handle of the Characteristic User Description descriptor */ 
                    0x00BAu, /* Handle of the Client Characteristic Configuration descriptor */ 
                }, 
            },

            /* CAL_STEP characteristic */
            {
                0x00BCu, /* Handle of the CAL_STEP characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x00BDu, /* Handle of the Characteristic User Description descriptor */ 
                    0x00BEu, /* Handle of the Client Characteristic Configuration descriptor */ 
                }, 
            },

            /* CAL_SPEED characteristic */
            {
                0x00C0u, /* Handle of the CAL_SPEED characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x00C1u, /* Handle of the Characteristic User Description descriptor */ 
                    0x00C2u, /* Handle of the Client Characteristic Configuration descriptor */ 
                }, 
            },

            /* CAL_AS_FOUND characteristic */
            {
                0x00C4u, /* Handle of the CAL_AS_FOUND characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x00C5u, /* Handle of the Characteristic User Description descriptor */ 
                    0x00C6u, /* Handle of the Client Characteristic Configuration descriptor */ 
                }, 
            },

            /* CAL_ADJUSTED characteristic */
            {
                0x00C8u, /* Handle of the CAL_ADJUSTED characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x00C9u, /* Handle of the Characteristic User Description descriptor */ 
                    0x00CAu, /* Handle of the Client Characteristic Configuration descriptor */ 
                }, 
            },

            /* CAL_COUNTS characteristic */
            {
                0x00CCu, /* Handle of the CAL_COUNTS characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x00CDu, /* Handle of the Characteristic User Description descriptor */ 
                    CYBLE_GATT_INVALID_ATTR_HANDLE_VALUE, 
                }, 
            },

            /* SENSOR_LIFE characteristic */
            {
                0x00CFu, /* Handle of the SENSOR_LIFE characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x00D0u, /* Handle of the Characteristic User Description descriptor */ 
                    0x00D1u, /* Handle of the Client Characteristic Configuration descriptor */ 
                }, 
            },

            /* CAL_CLEARING characteristic */
            {
                0x00D3u, /* Handle of the CAL_CLEARING characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x00D4u, /* Handle of the Characteristic User Description descriptor */ 
                    0x00D5u, /* Handle of the Client Characteristic Configuration descriptor */ 
                }, 
            },

            /* CAL_SPAN_ERROR characteristic */
            {
                0x00D7u, /* Handle of the CAL_SPAN_ERROR characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x00D8u, /* Handle of the Characteristic User Description descriptor */ 
                    0x00D9u, /* Handle of the Client Characteristic Configuration descriptor */ 
                }, 
            },

            /* CALIBRATION_STATUS characteristic */
            {
                0x00DBu, /* Handle of the CALIBRATION_STATUS characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x00DCu, /* Handle of the Characteristic User Description descriptor */ 
                    0x00DDu, /* Handle of the Client Characteristic Configuration descriptor */ 
                }, 
            },

            /* READING_SCALED characteristic */
            {
                0x00DFu, /* Handle of the READING_SCALED characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x00E0u, /* Handle of the Characteristic User Description descriptor */ 
                    0x00E1u, /* Handle of the Client Characteristic Configuration descriptor */ 
                }, 
            },

            /* ALERT_STATUS characteristic */
            {
                0x00E3u, /* Handle of the ALERT_STATUS characteristic */ 
                
                /* Array of Descriptors handles */
                {
                    0x00E4u, /* Handle of the Characteristic User Description descriptor */ 
                    0x00E5u, /* Handle of the Client Characteristic Configuration descriptor */ 
                }, 
            },
            {
                CYBLE_GATT_INVALID_ATTR_HANDLE_VALUE, 
                
                /* Array of Descriptors handles */
                {
                    CYBLE_GATT_INVALID_ATTR_HANDLE_VALUE, 
                    CYBLE_GATT_INVALID_ATTR_HANDLE_VALUE, 
                }, 
            },
            {
                CYBLE_GATT_INVALID_ATTR_HANDLE_VALUE, 
                
                /* Array of Descriptors handles */
                {
                    CYBLE_GATT_INVALID_ATTR_HANDLE_VALUE, 
                    CYBLE_GATT_INVALID_ATTR_HANDLE_VALUE, 
                }, 
            },
            {
                CYBLE_GATT_INVALID_ATTR_HANDLE_VALUE, 
                
                /* Array of Descriptors handles */
                {
                    CYBLE_GATT_INVALID_ATTR_HANDLE_VALUE, 
                    CYBLE_GATT_INVALID_ATTR_HANDLE_VALUE, 
                }, 
            },
            {
                CYBLE_GATT_INVALID_ATTR_HANDLE_VALUE, 
                
                /* Array of Descriptors handles */
                {
                    CYBLE_GATT_INVALID_ATTR_HANDLE_VALUE, 
                    CYBLE_GATT_INVALID_ATTR_HANDLE_VALUE, 
                }, 
            },
            {
                CYBLE_GATT_INVALID_ATTR_HANDLE_VALUE, 
                
                /* Array of Descriptors handles */
                {
                    CYBLE_GATT_INVALID_ATTR_HANDLE_VALUE, 
                    CYBLE_GATT_INVALID_ATTR_HANDLE_VALUE, 
                }, 
            },
        }, 
    },
};


#endif /* (CYBLE_CUSTOM_SERVER) */

#ifdef CYBLE_CUSTOM_CLIENT
    

static uint16 cyBle_customDisServIndex;
static uint16 cyBle_customDisCharIndex;

#endif /* (CYBLE_CUSTOM_CLIENT) */


/****************************************************************************** 
* Function Name: CyBle_CustomInit
***************************************************************************//**
* 
*  This function initializes Custom Service.
* 
* \return
*  None
* 
******************************************************************************/
void CyBle_CustomInit(void)
{
    
#ifdef CYBLE_CUSTOM_CLIENT

    uint16 locServIndex;
    uint16 locCharIndex;
    uint16 locDescIndex;
    
    for(locServIndex = 0u; locServIndex < CYBLE_CUSTOMC_SERVICE_COUNT; locServIndex++)
    {
        if(cyBle_serverInfo[CYBLE_SRVI_CUSTOMS + locServIndex].range.startHandle == CYBLE_GATT_INVALID_ATTR_HANDLE_VALUE)
        {
            for(locCharIndex = 0u; locCharIndex < cyBle_customCServ[locServIndex].charCount; locCharIndex++)
            {
                cyBle_customCServ[locServIndex].customServChar[locCharIndex].
                    customServCharHandle = 0u;
                
                for(locDescIndex = 0u; locDescIndex < 
                    cyBle_customCServ[locServIndex].customServChar[locCharIndex].descCount; 
                        locDescIndex++)
                {
                    cyBle_customCServ[locServIndex].customServChar[locCharIndex].
                        customServCharDesc[locDescIndex].descHandle = 0u;
                }
            }
        }
    }

#endif /* (CYBLE_CUSTOM_CLIENT) */
}


#ifdef CYBLE_CUSTOM_CLIENT

    
/******************************************************************************
* Function Name: CyBle_CustomcDiscoverServiceEventHandler
***************************************************************************//**
* 
*  This function is called on receiving a Read By Group Response event or 
*  Read response with 128-bit service uuid. 
* 
*  \param *discServInfo: The pointer to a service information structure.
* 
* \return
*  None
* 
******************************************************************************/
void CyBle_CustomcDiscoverServiceEventHandler(const CYBLE_DISC_SRVC128_INFO_T *discServInfo)
{
    uint16 j;
    uint8 flag = 0u;
    
    /* Services with 128 bit UUID have discServInfo->uuid equal to 0 and address to 
       128 uuid is stored in cyBle_customCServ.uuid128
    */
	for(j = 0u; (j < CYBLE_CUSTOMC_SERVICE_COUNT) && (flag == 0u); j++)
    {
        if(cyBle_customCServ[j].uuidFormat == CYBLE_GATT_128_BIT_UUID_FORMAT)
        {
            if(memcmp(cyBle_customCServ[j].uuid, &discServInfo->uuid.uuid128, CYBLE_GATT_128_BIT_UUID_SIZE) == 0u)
            {
                if(cyBle_serverInfo[j + (uint16)CYBLE_SRVI_CUSTOMS].range.startHandle == 
                   CYBLE_GATT_INVALID_ATTR_HANDLE_VALUE)
                {
                    cyBle_serverInfo[j + (uint16)CYBLE_SRVI_CUSTOMS].range = discServInfo->range;
                    cyBle_disCount++;
                    flag = 1u;
                }
            }
        }
    }
}


/******************************************************************************
* Function Name: CyBle_CustomcDiscoverCharacteristicsEventHandler
***************************************************************************//**
* 
*  This function is called on receiving a CYBLE_EVT_GATTC_READ_BY_TYPE_RSP
*  event. Based on the service index, an appropriate data structure is populated
*  using the data received as part of the callback.
* 
*  \param *discCharInfo: The pointer to a characteristic information structure.
*  \param discoveryService: The index of the service instance.
* 
* \return
*  None
* 
******************************************************************************/
void CyBle_CustomcDiscoverCharacteristicsEventHandler(uint16 discoveryService, const CYBLE_DISC_CHAR_INFO_T *discCharInfo)
{
    uint16 locCharIndex;
    static CYBLE_GATT_DB_ATTR_HANDLE_T *customsLastEndHandle = NULL;
    static uint16 discoveryLastServ = 0u;    
    uint8 locReqHandle = 0u;

    /* Update last characteristic endHandle to declaration handle of this characteristic */
    if(customsLastEndHandle != NULL)
    {
        if(discoveryLastServ == discoveryService)
        {
            *customsLastEndHandle = discCharInfo->charDeclHandle - 1u;
        }
        customsLastEndHandle = NULL;
    }
    
    for(locCharIndex = 0u; (locCharIndex < cyBle_customCServ[discoveryService].charCount) && (locReqHandle == 0u); 
        locCharIndex++)
    {
        uint8 flag = 0u;
        
        /* Support 128 bit uuid */
        if((discCharInfo->uuidFormat == CYBLE_GATT_128_BIT_UUID_FORMAT) &&
           (cyBle_customCServ[discoveryService].customServChar[locCharIndex].uuidFormat == 
                CYBLE_GATT_128_BIT_UUID_FORMAT))
        {
            if(memcmp(cyBle_customCServ[discoveryService].customServChar[locCharIndex].uuid, 
                &discCharInfo->uuid.uuid128, CYBLE_GATT_128_BIT_UUID_SIZE) == 0u)
            {
                flag = 1u;
            }
        }
        
        /* And support 16 bit uuid */
        if((discCharInfo->uuidFormat == CYBLE_GATT_16_BIT_UUID_FORMAT) &&
           (cyBle_customCServ[discoveryService].customServChar[locCharIndex].uuidFormat == 
                CYBLE_GATT_16_BIT_UUID_FORMAT))
        {
            if(memcmp(cyBle_customCServ[discoveryService].customServChar[locCharIndex].uuid, 
                &discCharInfo->uuid.uuid16, CYBLE_GATT_16_BIT_UUID_SIZE) == 0u)
            {
                flag = 1u;
            }
        }
        
        if((flag == 1u) && 
           (cyBle_customCServ[discoveryService].customServChar[locCharIndex].customServCharHandle
                == CYBLE_GATT_INVALID_ATTR_HANDLE_VALUE))
        {
            cyBle_customCServ[discoveryService].customServChar[locCharIndex].customServCharHandle = 
                discCharInfo->valueHandle;
            cyBle_customCServ[discoveryService].customServChar[locCharIndex].properties = 
                discCharInfo->properties;
            /* Init pointer to characteristic endHandle */
            customsLastEndHandle = &cyBle_customCServ[discoveryService].customServChar[locCharIndex].
                                    customServCharEndHandle;
            /* Init service index of discovered characteristic */
            discoveryLastServ = discoveryService;
            locReqHandle = 1u;
        }
    }
    
    /* Init characteristic endHandle to Service endHandle.
       Characteristic endHandle will be updated to the declaration
       Handler of the following characteristic,
       in the following characteristic discovery procedure. */
    if(customsLastEndHandle != NULL)
    {
        *customsLastEndHandle = cyBle_serverInfo[cyBle_disCount].range.endHandle;
    }
}


/****************************************************************************** 
* Function Name: CyBle_GetCustomCharRange
***************************************************************************//**
* 
*  Returns a possible range of the current characteristic descriptor
*  which is pointed by custom service and char index.
* 
*  \param incrementIndex: Not zero value indicates that service and characteristic index
*                  should be incremented.
* \return
*  CYBLE_GATT_ATTR_HANDLE_RANGE_T range: the block of start and end handles.
* 
******************************************************************************/
CYBLE_GATT_ATTR_HANDLE_RANGE_T CyBle_CustomcGetCharRange(uint8 incrementIndex)
{
    CYBLE_GATT_ATTR_HANDLE_RANGE_T charRange = {CYBLE_GATT_INVALID_ATTR_HANDLE_VALUE, CYBLE_GATT_INVALID_ATTR_HANDLE_VALUE};

    do{
        if(incrementIndex != CYBLE_DISCOVERY_INIT)
        {
            if((cyBle_customDisCharIndex + 1u) < cyBle_customCServ[cyBle_customDisServIndex].charCount)
            {
                cyBle_customDisCharIndex++;
            }
            else 
            {
                if((cyBle_customDisServIndex + 1u) < CYBLE_CUSTOMC_SERVICE_COUNT)
                {
                    cyBle_customDisServIndex++;      /* Discover descriptors for next custom service */
                    /* Set characteristic index to first characteristic of custom service */
                    cyBle_customDisCharIndex = 0u;
                }
                else /* Increment general discovery index when custom characteristic discovery is done. */
                {
                    cyBle_disCount++;  
                    cyBle_customDisCharIndex++;
                }
            }
        }
        else    /* Increment indexes in the following loop */
        {
            cyBle_customDisServIndex = 0u;
            cyBle_customDisCharIndex = 0u;
            incrementIndex = 1u;
        }            
        /* Read characteristic range */
        if(cyBle_customDisCharIndex < (cyBle_customCServ[cyBle_customDisServIndex].charCount))
        {
            charRange.startHandle = cyBle_customCServ[cyBle_customDisServIndex].
                                customServChar[cyBle_customDisCharIndex].customServCharHandle + 1u;
            charRange.endHandle = cyBle_customCServ[cyBle_customDisServIndex].
                                customServChar[cyBle_customDisCharIndex].customServCharEndHandle;
        }
    }while(((charRange.startHandle <= cyBle_gattcDiscoveryRange.startHandle) || 
            (charRange.startHandle > cyBle_gattcDiscoveryRange.endHandle) ||
            (charRange.endHandle < cyBle_gattcDiscoveryRange.startHandle) ||
            (charRange.startHandle > charRange.endHandle)) && 
            (cyBle_customDisCharIndex < cyBle_customCServ[cyBle_customDisServIndex].charCount));
    
    return(charRange);
}


/******************************************************************************
* Function Name: CyBle_CustomcDiscoverCharDescriptorsEventHandler
***************************************************************************//**
* 
*  This function is called on receiving a CYBLE_EVT_GATTC_FIND_INFO_RSP event.
*  Based on the descriptor UUID, an appropriate data structure is populated using
*  the data received as part of the callback.
* 
*  \param *discDescrInfo: The pointer to a descriptor information structure.
* 
* \return
*  None
* 
******************************************************************************/
void CyBle_CustomcDiscoverCharDescriptorsEventHandler(const CYBLE_DISC_DESCR_INFO_T *discDescrInfo)
{
    uint8 locDescIndex;
    uint8 locReqHandle = 0u;

    for(locDescIndex = 0u; (locDescIndex < cyBle_customCServ[cyBle_customDisServIndex].
          customServChar[cyBle_customDisCharIndex].descCount) && (locReqHandle == 0u); locDescIndex++)
    {
        uint8 flag = 0u;
        
        if((discDescrInfo->uuidFormat == CYBLE_GATT_128_BIT_UUID_FORMAT) &&
           (cyBle_customCServ[cyBle_customDisServIndex].customServChar[cyBle_customDisCharIndex].
             customServCharDesc[locDescIndex].uuidFormat == CYBLE_GATT_128_BIT_UUID_FORMAT))
        {
            if(memcmp(cyBle_customCServ[cyBle_customDisServIndex].
                customServChar[cyBle_customDisCharIndex].customServCharDesc[locDescIndex].uuid, 
                &discDescrInfo->uuid.uuid128, CYBLE_GATT_128_BIT_UUID_SIZE) == 0u)
            {
                flag = 1u;
            }
        }
        
        if((discDescrInfo->uuidFormat == CYBLE_GATT_16_BIT_UUID_FORMAT) &&
           (cyBle_customCServ[cyBle_customDisServIndex].customServChar[cyBle_customDisCharIndex].
             customServCharDesc[locDescIndex].uuidFormat == CYBLE_GATT_16_BIT_UUID_FORMAT))
        {
            if(memcmp(cyBle_customCServ[cyBle_customDisServIndex].
                customServChar[cyBle_customDisCharIndex].customServCharDesc[locDescIndex].uuid, 
                &discDescrInfo->uuid.uuid16, CYBLE_GATT_16_BIT_UUID_SIZE) == 0u)
            {
                flag = 1u;
            }
        }
        
        if((flag == 1u) && 
           (cyBle_customCServ[cyBle_customDisServIndex].customServChar[cyBle_customDisCharIndex].
               customServCharDesc[locDescIndex].descHandle == CYBLE_GATT_INVALID_ATTR_HANDLE_VALUE))
        {
            cyBle_customCServ[cyBle_customDisServIndex].customServChar[cyBle_customDisCharIndex].
                customServCharDesc[locDescIndex].descHandle = discDescrInfo->descrHandle;
            locReqHandle = 1u;
        }
    }
}

#endif /* (CYBLE_CUSTOM_CLIENT) */


/* [] END OF FILE */
