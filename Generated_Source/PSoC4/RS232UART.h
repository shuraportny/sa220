/*******************************************************************************
* File Name: RS232UART.h
* Version 2.50
*
* Description:
*  Contains the function prototypes and constants available to the UART
*  user module.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#if !defined(CY_UART_RS232UART_H)
#define CY_UART_RS232UART_H

#include "cyfitter.h"
#include "cytypes.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */


/***************************************
* Conditional Compilation Parameters
***************************************/

#define RS232UART_RX_ENABLED                     (1u)
#define RS232UART_TX_ENABLED                     (1u)
#define RS232UART_HD_ENABLED                     (0u)
#define RS232UART_RX_INTERRUPT_ENABLED           (1u)
#define RS232UART_TX_INTERRUPT_ENABLED           (0u)
#define RS232UART_INTERNAL_CLOCK_USED            (1u)
#define RS232UART_RXHW_ADDRESS_ENABLED           (0u)
#define RS232UART_OVER_SAMPLE_COUNT              (8u)
#define RS232UART_PARITY_TYPE                    (0u)
#define RS232UART_PARITY_TYPE_SW                 (0u)
#define RS232UART_BREAK_DETECT                   (0u)
#define RS232UART_BREAK_BITS_TX                  (13u)
#define RS232UART_BREAK_BITS_RX                  (13u)
#define RS232UART_TXCLKGEN_DP                    (1u)
#define RS232UART_USE23POLLING                   (1u)
#define RS232UART_FLOW_CONTROL                   (0u)
#define RS232UART_CLK_FREQ                       (0u)
#define RS232UART_TX_BUFFER_SIZE                 (4u)
#define RS232UART_RX_BUFFER_SIZE                 (8u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component UART_v2_50 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */

#if defined(RS232UART_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG)
    #define RS232UART_CONTROL_REG_REMOVED            (0u)
#else
    #define RS232UART_CONTROL_REG_REMOVED            (1u)
#endif /* End RS232UART_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */


/***************************************
*      Data Structure Definition
***************************************/

/* Sleep Mode API Support */
typedef struct RS232UART_backupStruct_
{
    uint8 enableState;

    #if(RS232UART_CONTROL_REG_REMOVED == 0u)
        uint8 cr;
    #endif /* End RS232UART_CONTROL_REG_REMOVED */

} RS232UART_BACKUP_STRUCT;


/***************************************
*       Function Prototypes
***************************************/

void RS232UART_Start(void) ;
void RS232UART_Stop(void) ;
uint8 RS232UART_ReadControlRegister(void) ;
void RS232UART_WriteControlRegister(uint8 control) ;

void RS232UART_Init(void) ;
void RS232UART_Enable(void) ;
void RS232UART_SaveConfig(void) ;
void RS232UART_RestoreConfig(void) ;
void RS232UART_Sleep(void) ;
void RS232UART_Wakeup(void) ;

/* Only if RX is enabled */
#if( (RS232UART_RX_ENABLED) || (RS232UART_HD_ENABLED) )

    #if (RS232UART_RX_INTERRUPT_ENABLED)
        #define RS232UART_EnableRxInt()  CyIntEnable (RS232UART_RX_VECT_NUM)
        #define RS232UART_DisableRxInt() CyIntDisable(RS232UART_RX_VECT_NUM)
        CY_ISR_PROTO(RS232UART_RXISR);
    #endif /* RS232UART_RX_INTERRUPT_ENABLED */

    void RS232UART_SetRxAddressMode(uint8 addressMode)
                                                           ;
    void RS232UART_SetRxAddress1(uint8 address) ;
    void RS232UART_SetRxAddress2(uint8 address) ;

    void  RS232UART_SetRxInterruptMode(uint8 intSrc) ;
    uint8 RS232UART_ReadRxData(void) ;
    uint8 RS232UART_ReadRxStatus(void) ;
    uint8 RS232UART_GetChar(void) ;
    uint16 RS232UART_GetByte(void) ;
    uint8 RS232UART_GetRxBufferSize(void)
                                                            ;
    void RS232UART_ClearRxBuffer(void) ;

    /* Obsolete functions, defines for backward compatible */
    #define RS232UART_GetRxInterruptSource   RS232UART_ReadRxStatus

#endif /* End (RS232UART_RX_ENABLED) || (RS232UART_HD_ENABLED) */

/* Only if TX is enabled */
#if(RS232UART_TX_ENABLED || RS232UART_HD_ENABLED)

    #if(RS232UART_TX_INTERRUPT_ENABLED)
        #define RS232UART_EnableTxInt()  CyIntEnable (RS232UART_TX_VECT_NUM)
        #define RS232UART_DisableTxInt() CyIntDisable(RS232UART_TX_VECT_NUM)
        #define RS232UART_SetPendingTxInt() CyIntSetPending(RS232UART_TX_VECT_NUM)
        #define RS232UART_ClearPendingTxInt() CyIntClearPending(RS232UART_TX_VECT_NUM)
        CY_ISR_PROTO(RS232UART_TXISR);
    #endif /* RS232UART_TX_INTERRUPT_ENABLED */

    void RS232UART_SetTxInterruptMode(uint8 intSrc) ;
    void RS232UART_WriteTxData(uint8 txDataByte) ;
    uint8 RS232UART_ReadTxStatus(void) ;
    void RS232UART_PutChar(uint8 txDataByte) ;
    void RS232UART_PutString(const char8 string[]) ;
    void RS232UART_PutArray(const uint8 string[], uint8 byteCount)
                                                            ;
    void RS232UART_PutCRLF(uint8 txDataByte) ;
    void RS232UART_ClearTxBuffer(void) ;
    void RS232UART_SetTxAddressMode(uint8 addressMode) ;
    void RS232UART_SendBreak(uint8 retMode) ;
    uint8 RS232UART_GetTxBufferSize(void)
                                                            ;
    /* Obsolete functions, defines for backward compatible */
    #define RS232UART_PutStringConst         RS232UART_PutString
    #define RS232UART_PutArrayConst          RS232UART_PutArray
    #define RS232UART_GetTxInterruptSource   RS232UART_ReadTxStatus

#endif /* End RS232UART_TX_ENABLED || RS232UART_HD_ENABLED */

#if(RS232UART_HD_ENABLED)
    void RS232UART_LoadRxConfig(void) ;
    void RS232UART_LoadTxConfig(void) ;
#endif /* End RS232UART_HD_ENABLED */


/* Communication bootloader APIs */
#if defined(CYDEV_BOOTLOADER_IO_COMP) && ((CYDEV_BOOTLOADER_IO_COMP == CyBtldr_RS232UART) || \
                                          (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_Custom_Interface))
    /* Physical layer functions */
    void    RS232UART_CyBtldrCommStart(void) CYSMALL ;
    void    RS232UART_CyBtldrCommStop(void) CYSMALL ;
    void    RS232UART_CyBtldrCommReset(void) CYSMALL ;
    cystatus RS232UART_CyBtldrCommWrite(const uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;
    cystatus RS232UART_CyBtldrCommRead(uint8 pData[], uint16 size, uint16 * count, uint8 timeOut) CYSMALL
             ;

    #if (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_RS232UART)
        #define CyBtldrCommStart    RS232UART_CyBtldrCommStart
        #define CyBtldrCommStop     RS232UART_CyBtldrCommStop
        #define CyBtldrCommReset    RS232UART_CyBtldrCommReset
        #define CyBtldrCommWrite    RS232UART_CyBtldrCommWrite
        #define CyBtldrCommRead     RS232UART_CyBtldrCommRead
    #endif  /* (CYDEV_BOOTLOADER_IO_COMP == CyBtldr_RS232UART) */

    /* Byte to Byte time out for detecting end of block data from host */
    #define RS232UART_BYTE2BYTE_TIME_OUT (25u)
    #define RS232UART_PACKET_EOP         (0x17u) /* End of packet defined by bootloader */
    #define RS232UART_WAIT_EOP_DELAY     (5u)    /* Additional 5ms to wait for End of packet */
    #define RS232UART_BL_CHK_DELAY_MS    (1u)    /* Time Out quantity equal 1mS */

#endif /* CYDEV_BOOTLOADER_IO_COMP */


/***************************************
*          API Constants
***************************************/
/* Parameters for SetTxAddressMode API*/
#define RS232UART_SET_SPACE      (0x00u)
#define RS232UART_SET_MARK       (0x01u)

/* Status Register definitions */
#if( (RS232UART_TX_ENABLED) || (RS232UART_HD_ENABLED) )
    #if(RS232UART_TX_INTERRUPT_ENABLED)
        #define RS232UART_TX_VECT_NUM            (uint8)RS232UART_TXInternalInterrupt__INTC_NUMBER
        #define RS232UART_TX_PRIOR_NUM           (uint8)RS232UART_TXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* RS232UART_TX_INTERRUPT_ENABLED */

    #define RS232UART_TX_STS_COMPLETE_SHIFT          (0x00u)
    #define RS232UART_TX_STS_FIFO_EMPTY_SHIFT        (0x01u)
    #define RS232UART_TX_STS_FIFO_NOT_FULL_SHIFT     (0x03u)
    #if(RS232UART_TX_ENABLED)
        #define RS232UART_TX_STS_FIFO_FULL_SHIFT     (0x02u)
    #else /* (RS232UART_HD_ENABLED) */
        #define RS232UART_TX_STS_FIFO_FULL_SHIFT     (0x05u)  /* Needs MD=0 */
    #endif /* (RS232UART_TX_ENABLED) */

    #define RS232UART_TX_STS_COMPLETE            (uint8)(0x01u << RS232UART_TX_STS_COMPLETE_SHIFT)
    #define RS232UART_TX_STS_FIFO_EMPTY          (uint8)(0x01u << RS232UART_TX_STS_FIFO_EMPTY_SHIFT)
    #define RS232UART_TX_STS_FIFO_FULL           (uint8)(0x01u << RS232UART_TX_STS_FIFO_FULL_SHIFT)
    #define RS232UART_TX_STS_FIFO_NOT_FULL       (uint8)(0x01u << RS232UART_TX_STS_FIFO_NOT_FULL_SHIFT)
#endif /* End (RS232UART_TX_ENABLED) || (RS232UART_HD_ENABLED)*/

#if( (RS232UART_RX_ENABLED) || (RS232UART_HD_ENABLED) )
    #if(RS232UART_RX_INTERRUPT_ENABLED)
        #define RS232UART_RX_VECT_NUM            (uint8)RS232UART_RXInternalInterrupt__INTC_NUMBER
        #define RS232UART_RX_PRIOR_NUM           (uint8)RS232UART_RXInternalInterrupt__INTC_PRIOR_NUM
    #endif /* RS232UART_RX_INTERRUPT_ENABLED */
    #define RS232UART_RX_STS_MRKSPC_SHIFT            (0x00u)
    #define RS232UART_RX_STS_BREAK_SHIFT             (0x01u)
    #define RS232UART_RX_STS_PAR_ERROR_SHIFT         (0x02u)
    #define RS232UART_RX_STS_STOP_ERROR_SHIFT        (0x03u)
    #define RS232UART_RX_STS_OVERRUN_SHIFT           (0x04u)
    #define RS232UART_RX_STS_FIFO_NOTEMPTY_SHIFT     (0x05u)
    #define RS232UART_RX_STS_ADDR_MATCH_SHIFT        (0x06u)
    #define RS232UART_RX_STS_SOFT_BUFF_OVER_SHIFT    (0x07u)

    #define RS232UART_RX_STS_MRKSPC           (uint8)(0x01u << RS232UART_RX_STS_MRKSPC_SHIFT)
    #define RS232UART_RX_STS_BREAK            (uint8)(0x01u << RS232UART_RX_STS_BREAK_SHIFT)
    #define RS232UART_RX_STS_PAR_ERROR        (uint8)(0x01u << RS232UART_RX_STS_PAR_ERROR_SHIFT)
    #define RS232UART_RX_STS_STOP_ERROR       (uint8)(0x01u << RS232UART_RX_STS_STOP_ERROR_SHIFT)
    #define RS232UART_RX_STS_OVERRUN          (uint8)(0x01u << RS232UART_RX_STS_OVERRUN_SHIFT)
    #define RS232UART_RX_STS_FIFO_NOTEMPTY    (uint8)(0x01u << RS232UART_RX_STS_FIFO_NOTEMPTY_SHIFT)
    #define RS232UART_RX_STS_ADDR_MATCH       (uint8)(0x01u << RS232UART_RX_STS_ADDR_MATCH_SHIFT)
    #define RS232UART_RX_STS_SOFT_BUFF_OVER   (uint8)(0x01u << RS232UART_RX_STS_SOFT_BUFF_OVER_SHIFT)
    #define RS232UART_RX_HW_MASK                     (0x7Fu)
#endif /* End (RS232UART_RX_ENABLED) || (RS232UART_HD_ENABLED) */

/* Control Register definitions */
#define RS232UART_CTRL_HD_SEND_SHIFT                 (0x00u) /* 1 enable TX part in Half Duplex mode */
#define RS232UART_CTRL_HD_SEND_BREAK_SHIFT           (0x01u) /* 1 send BREAK signal in Half Duplez mode */
#define RS232UART_CTRL_MARK_SHIFT                    (0x02u) /* 1 sets mark, 0 sets space */
#define RS232UART_CTRL_PARITY_TYPE0_SHIFT            (0x03u) /* Defines the type of parity implemented */
#define RS232UART_CTRL_PARITY_TYPE1_SHIFT            (0x04u) /* Defines the type of parity implemented */
#define RS232UART_CTRL_RXADDR_MODE0_SHIFT            (0x05u)
#define RS232UART_CTRL_RXADDR_MODE1_SHIFT            (0x06u)
#define RS232UART_CTRL_RXADDR_MODE2_SHIFT            (0x07u)

#define RS232UART_CTRL_HD_SEND               (uint8)(0x01u << RS232UART_CTRL_HD_SEND_SHIFT)
#define RS232UART_CTRL_HD_SEND_BREAK         (uint8)(0x01u << RS232UART_CTRL_HD_SEND_BREAK_SHIFT)
#define RS232UART_CTRL_MARK                  (uint8)(0x01u << RS232UART_CTRL_MARK_SHIFT)
#define RS232UART_CTRL_PARITY_TYPE_MASK      (uint8)(0x03u << RS232UART_CTRL_PARITY_TYPE0_SHIFT)
#define RS232UART_CTRL_RXADDR_MODE_MASK      (uint8)(0x07u << RS232UART_CTRL_RXADDR_MODE0_SHIFT)

/* StatusI Register Interrupt Enable Control Bits. As defined by the Register map for the AUX Control Register */
#define RS232UART_INT_ENABLE                         (0x10u)

/* Bit Counter (7-bit) Control Register Bit Definitions. As defined by the Register map for the AUX Control Register */
#define RS232UART_CNTR_ENABLE                        (0x20u)

/*   Constants for SendBreak() "retMode" parameter  */
#define RS232UART_SEND_BREAK                         (0x00u)
#define RS232UART_WAIT_FOR_COMPLETE_REINIT           (0x01u)
#define RS232UART_REINIT                             (0x02u)
#define RS232UART_SEND_WAIT_REINIT                   (0x03u)

#define RS232UART_OVER_SAMPLE_8                      (8u)
#define RS232UART_OVER_SAMPLE_16                     (16u)

#define RS232UART_BIT_CENTER                         (RS232UART_OVER_SAMPLE_COUNT - 2u)

#define RS232UART_FIFO_LENGTH                        (4u)
#define RS232UART_NUMBER_OF_START_BIT                (1u)
#define RS232UART_MAX_BYTE_VALUE                     (0xFFu)

/* 8X always for count7 implementation */
#define RS232UART_TXBITCTR_BREAKBITS8X   ((RS232UART_BREAK_BITS_TX * RS232UART_OVER_SAMPLE_8) - 1u)
/* 8X or 16X for DP implementation */
#define RS232UART_TXBITCTR_BREAKBITS ((RS232UART_BREAK_BITS_TX * RS232UART_OVER_SAMPLE_COUNT) - 1u)

#define RS232UART_HALF_BIT_COUNT   \
                            (((RS232UART_OVER_SAMPLE_COUNT / 2u) + (RS232UART_USE23POLLING * 1u)) - 2u)
#if (RS232UART_OVER_SAMPLE_COUNT == RS232UART_OVER_SAMPLE_8)
    #define RS232UART_HD_TXBITCTR_INIT   (((RS232UART_BREAK_BITS_TX + \
                            RS232UART_NUMBER_OF_START_BIT) * RS232UART_OVER_SAMPLE_COUNT) - 1u)

    /* This parameter is increased on the 2 in 2 out of 3 mode to sample voting in the middle */
    #define RS232UART_RXBITCTR_INIT  ((((RS232UART_BREAK_BITS_RX + RS232UART_NUMBER_OF_START_BIT) \
                            * RS232UART_OVER_SAMPLE_COUNT) + RS232UART_HALF_BIT_COUNT) - 1u)

#else /* RS232UART_OVER_SAMPLE_COUNT == RS232UART_OVER_SAMPLE_16 */
    #define RS232UART_HD_TXBITCTR_INIT   ((8u * RS232UART_OVER_SAMPLE_COUNT) - 1u)
    /* 7bit counter need one more bit for OverSampleCount = 16 */
    #define RS232UART_RXBITCTR_INIT      (((7u * RS232UART_OVER_SAMPLE_COUNT) - 1u) + \
                                                      RS232UART_HALF_BIT_COUNT)
#endif /* End RS232UART_OVER_SAMPLE_COUNT */

#define RS232UART_HD_RXBITCTR_INIT                   RS232UART_RXBITCTR_INIT


/***************************************
* Global variables external identifier
***************************************/

extern uint8 RS232UART_initVar;
#if (RS232UART_TX_INTERRUPT_ENABLED && RS232UART_TX_ENABLED)
    extern volatile uint8 RS232UART_txBuffer[RS232UART_TX_BUFFER_SIZE];
    extern volatile uint8 RS232UART_txBufferRead;
    extern uint8 RS232UART_txBufferWrite;
#endif /* (RS232UART_TX_INTERRUPT_ENABLED && RS232UART_TX_ENABLED) */
#if (RS232UART_RX_INTERRUPT_ENABLED && (RS232UART_RX_ENABLED || RS232UART_HD_ENABLED))
    extern uint8 RS232UART_errorStatus;
    extern volatile uint8 RS232UART_rxBuffer[RS232UART_RX_BUFFER_SIZE];
    extern volatile uint8 RS232UART_rxBufferRead;
    extern volatile uint8 RS232UART_rxBufferWrite;
    extern volatile uint8 RS232UART_rxBufferLoopDetect;
    extern volatile uint8 RS232UART_rxBufferOverflow;
    #if (RS232UART_RXHW_ADDRESS_ENABLED)
        extern volatile uint8 RS232UART_rxAddressMode;
        extern volatile uint8 RS232UART_rxAddressDetected;
    #endif /* (RS232UART_RXHW_ADDRESS_ENABLED) */
#endif /* (RS232UART_RX_INTERRUPT_ENABLED && (RS232UART_RX_ENABLED || RS232UART_HD_ENABLED)) */


/***************************************
* Enumerated Types and Parameters
***************************************/

#define RS232UART__B_UART__AM_SW_BYTE_BYTE 1
#define RS232UART__B_UART__AM_SW_DETECT_TO_BUFFER 2
#define RS232UART__B_UART__AM_HW_BYTE_BY_BYTE 3
#define RS232UART__B_UART__AM_HW_DETECT_TO_BUFFER 4
#define RS232UART__B_UART__AM_NONE 0

#define RS232UART__B_UART__NONE_REVB 0
#define RS232UART__B_UART__EVEN_REVB 1
#define RS232UART__B_UART__ODD_REVB 2
#define RS232UART__B_UART__MARK_SPACE_REVB 3



/***************************************
*    Initial Parameter Constants
***************************************/

/* UART shifts max 8 bits, Mark/Space functionality working if 9 selected */
#define RS232UART_NUMBER_OF_DATA_BITS    ((8u > 8u) ? 8u : 8u)
#define RS232UART_NUMBER_OF_STOP_BITS    (1u)

#if (RS232UART_RXHW_ADDRESS_ENABLED)
    #define RS232UART_RX_ADDRESS_MODE    (0u)
    #define RS232UART_RX_HW_ADDRESS1     (0u)
    #define RS232UART_RX_HW_ADDRESS2     (0u)
#endif /* (RS232UART_RXHW_ADDRESS_ENABLED) */

#define RS232UART_INIT_RX_INTERRUPTS_MASK \
                                  (uint8)((1 << RS232UART_RX_STS_FIFO_NOTEMPTY_SHIFT) \
                                        | (0 << RS232UART_RX_STS_MRKSPC_SHIFT) \
                                        | (0 << RS232UART_RX_STS_ADDR_MATCH_SHIFT) \
                                        | (0 << RS232UART_RX_STS_PAR_ERROR_SHIFT) \
                                        | (0 << RS232UART_RX_STS_STOP_ERROR_SHIFT) \
                                        | (0 << RS232UART_RX_STS_BREAK_SHIFT) \
                                        | (0 << RS232UART_RX_STS_OVERRUN_SHIFT))

#define RS232UART_INIT_TX_INTERRUPTS_MASK \
                                  (uint8)((0 << RS232UART_TX_STS_COMPLETE_SHIFT) \
                                        | (0 << RS232UART_TX_STS_FIFO_EMPTY_SHIFT) \
                                        | (0 << RS232UART_TX_STS_FIFO_FULL_SHIFT) \
                                        | (0 << RS232UART_TX_STS_FIFO_NOT_FULL_SHIFT))


/***************************************
*              Registers
***************************************/

#ifdef RS232UART_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define RS232UART_CONTROL_REG \
                            (* (reg8 *) RS232UART_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
    #define RS232UART_CONTROL_PTR \
                            (  (reg8 *) RS232UART_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG )
#endif /* End RS232UART_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(RS232UART_TX_ENABLED)
    #define RS232UART_TXDATA_REG          (* (reg8 *) RS232UART_BUART_sTX_TxShifter_u0__F0_REG)
    #define RS232UART_TXDATA_PTR          (  (reg8 *) RS232UART_BUART_sTX_TxShifter_u0__F0_REG)
    #define RS232UART_TXDATA_AUX_CTL_REG  (* (reg8 *) RS232UART_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define RS232UART_TXDATA_AUX_CTL_PTR  (  (reg8 *) RS232UART_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG)
    #define RS232UART_TXSTATUS_REG        (* (reg8 *) RS232UART_BUART_sTX_TxSts__STATUS_REG)
    #define RS232UART_TXSTATUS_PTR        (  (reg8 *) RS232UART_BUART_sTX_TxSts__STATUS_REG)
    #define RS232UART_TXSTATUS_MASK_REG   (* (reg8 *) RS232UART_BUART_sTX_TxSts__MASK_REG)
    #define RS232UART_TXSTATUS_MASK_PTR   (  (reg8 *) RS232UART_BUART_sTX_TxSts__MASK_REG)
    #define RS232UART_TXSTATUS_ACTL_REG   (* (reg8 *) RS232UART_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)
    #define RS232UART_TXSTATUS_ACTL_PTR   (  (reg8 *) RS232UART_BUART_sTX_TxSts__STATUS_AUX_CTL_REG)

    /* DP clock */
    #if(RS232UART_TXCLKGEN_DP)
        #define RS232UART_TXBITCLKGEN_CTR_REG        \
                                        (* (reg8 *) RS232UART_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define RS232UART_TXBITCLKGEN_CTR_PTR        \
                                        (  (reg8 *) RS232UART_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG)
        #define RS232UART_TXBITCLKTX_COMPLETE_REG    \
                                        (* (reg8 *) RS232UART_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
        #define RS232UART_TXBITCLKTX_COMPLETE_PTR    \
                                        (  (reg8 *) RS232UART_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG)
    #else     /* Count7 clock*/
        #define RS232UART_TXBITCTR_PERIOD_REG    \
                                        (* (reg8 *) RS232UART_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define RS232UART_TXBITCTR_PERIOD_PTR    \
                                        (  (reg8 *) RS232UART_BUART_sTX_sCLOCK_TxBitCounter__PERIOD_REG)
        #define RS232UART_TXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) RS232UART_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define RS232UART_TXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) RS232UART_BUART_sTX_sCLOCK_TxBitCounter__CONTROL_AUX_CTL_REG)
        #define RS232UART_TXBITCTR_COUNTER_REG   \
                                        (* (reg8 *) RS232UART_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
        #define RS232UART_TXBITCTR_COUNTER_PTR   \
                                        (  (reg8 *) RS232UART_BUART_sTX_sCLOCK_TxBitCounter__COUNT_REG)
    #endif /* RS232UART_TXCLKGEN_DP */

#endif /* End RS232UART_TX_ENABLED */

#if(RS232UART_HD_ENABLED)

    #define RS232UART_TXDATA_REG             (* (reg8 *) RS232UART_BUART_sRX_RxShifter_u0__F1_REG )
    #define RS232UART_TXDATA_PTR             (  (reg8 *) RS232UART_BUART_sRX_RxShifter_u0__F1_REG )
    #define RS232UART_TXDATA_AUX_CTL_REG     (* (reg8 *) RS232UART_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)
    #define RS232UART_TXDATA_AUX_CTL_PTR     (  (reg8 *) RS232UART_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define RS232UART_TXSTATUS_REG           (* (reg8 *) RS232UART_BUART_sRX_RxSts__STATUS_REG )
    #define RS232UART_TXSTATUS_PTR           (  (reg8 *) RS232UART_BUART_sRX_RxSts__STATUS_REG )
    #define RS232UART_TXSTATUS_MASK_REG      (* (reg8 *) RS232UART_BUART_sRX_RxSts__MASK_REG )
    #define RS232UART_TXSTATUS_MASK_PTR      (  (reg8 *) RS232UART_BUART_sRX_RxSts__MASK_REG )
    #define RS232UART_TXSTATUS_ACTL_REG      (* (reg8 *) RS232UART_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define RS232UART_TXSTATUS_ACTL_PTR      (  (reg8 *) RS232UART_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End RS232UART_HD_ENABLED */

#if( (RS232UART_RX_ENABLED) || (RS232UART_HD_ENABLED) )
    #define RS232UART_RXDATA_REG             (* (reg8 *) RS232UART_BUART_sRX_RxShifter_u0__F0_REG )
    #define RS232UART_RXDATA_PTR             (  (reg8 *) RS232UART_BUART_sRX_RxShifter_u0__F0_REG )
    #define RS232UART_RXADDRESS1_REG         (* (reg8 *) RS232UART_BUART_sRX_RxShifter_u0__D0_REG )
    #define RS232UART_RXADDRESS1_PTR         (  (reg8 *) RS232UART_BUART_sRX_RxShifter_u0__D0_REG )
    #define RS232UART_RXADDRESS2_REG         (* (reg8 *) RS232UART_BUART_sRX_RxShifter_u0__D1_REG )
    #define RS232UART_RXADDRESS2_PTR         (  (reg8 *) RS232UART_BUART_sRX_RxShifter_u0__D1_REG )
    #define RS232UART_RXDATA_AUX_CTL_REG     (* (reg8 *) RS232UART_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG)

    #define RS232UART_RXBITCTR_PERIOD_REG    (* (reg8 *) RS232UART_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define RS232UART_RXBITCTR_PERIOD_PTR    (  (reg8 *) RS232UART_BUART_sRX_RxBitCounter__PERIOD_REG )
    #define RS232UART_RXBITCTR_CONTROL_REG   \
                                        (* (reg8 *) RS232UART_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define RS232UART_RXBITCTR_CONTROL_PTR   \
                                        (  (reg8 *) RS232UART_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG )
    #define RS232UART_RXBITCTR_COUNTER_REG   (* (reg8 *) RS232UART_BUART_sRX_RxBitCounter__COUNT_REG )
    #define RS232UART_RXBITCTR_COUNTER_PTR   (  (reg8 *) RS232UART_BUART_sRX_RxBitCounter__COUNT_REG )

    #define RS232UART_RXSTATUS_REG           (* (reg8 *) RS232UART_BUART_sRX_RxSts__STATUS_REG )
    #define RS232UART_RXSTATUS_PTR           (  (reg8 *) RS232UART_BUART_sRX_RxSts__STATUS_REG )
    #define RS232UART_RXSTATUS_MASK_REG      (* (reg8 *) RS232UART_BUART_sRX_RxSts__MASK_REG )
    #define RS232UART_RXSTATUS_MASK_PTR      (  (reg8 *) RS232UART_BUART_sRX_RxSts__MASK_REG )
    #define RS232UART_RXSTATUS_ACTL_REG      (* (reg8 *) RS232UART_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
    #define RS232UART_RXSTATUS_ACTL_PTR      (  (reg8 *) RS232UART_BUART_sRX_RxSts__STATUS_AUX_CTL_REG )
#endif /* End  (RS232UART_RX_ENABLED) || (RS232UART_HD_ENABLED) */

#if(RS232UART_INTERNAL_CLOCK_USED)
    /* Register to enable or disable the digital clocks */
    #define RS232UART_INTCLOCK_CLKEN_REG     (* (reg8 *) RS232UART_IntClock__PM_ACT_CFG)
    #define RS232UART_INTCLOCK_CLKEN_PTR     (  (reg8 *) RS232UART_IntClock__PM_ACT_CFG)

    /* Clock mask for this clock. */
    #define RS232UART_INTCLOCK_CLKEN_MASK    RS232UART_IntClock__PM_ACT_MSK
#endif /* End RS232UART_INTERNAL_CLOCK_USED */


/***************************************
*       Register Constants
***************************************/

#if(RS232UART_TX_ENABLED)
    #define RS232UART_TX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End RS232UART_TX_ENABLED */

#if(RS232UART_HD_ENABLED)
    #define RS232UART_TX_FIFO_CLR            (0x02u) /* FIFO1 CLR */
#endif /* End RS232UART_HD_ENABLED */

#if( (RS232UART_RX_ENABLED) || (RS232UART_HD_ENABLED) )
    #define RS232UART_RX_FIFO_CLR            (0x01u) /* FIFO0 CLR */
#endif /* End  (RS232UART_RX_ENABLED) || (RS232UART_HD_ENABLED) */


/***************************************
* The following code is DEPRECATED and
* should not be used in new projects.
***************************************/

/* UART v2_40 obsolete definitions */
#define RS232UART_WAIT_1_MS      RS232UART_BL_CHK_DELAY_MS   

#define RS232UART_TXBUFFERSIZE   RS232UART_TX_BUFFER_SIZE
#define RS232UART_RXBUFFERSIZE   RS232UART_RX_BUFFER_SIZE

#if (RS232UART_RXHW_ADDRESS_ENABLED)
    #define RS232UART_RXADDRESSMODE  RS232UART_RX_ADDRESS_MODE
    #define RS232UART_RXHWADDRESS1   RS232UART_RX_HW_ADDRESS1
    #define RS232UART_RXHWADDRESS2   RS232UART_RX_HW_ADDRESS2
    /* Backward compatible define */
    #define RS232UART_RXAddressMode  RS232UART_RXADDRESSMODE
#endif /* (RS232UART_RXHW_ADDRESS_ENABLED) */

/* UART v2_30 obsolete definitions */
#define RS232UART_initvar                    RS232UART_initVar

#define RS232UART_RX_Enabled                 RS232UART_RX_ENABLED
#define RS232UART_TX_Enabled                 RS232UART_TX_ENABLED
#define RS232UART_HD_Enabled                 RS232UART_HD_ENABLED
#define RS232UART_RX_IntInterruptEnabled     RS232UART_RX_INTERRUPT_ENABLED
#define RS232UART_TX_IntInterruptEnabled     RS232UART_TX_INTERRUPT_ENABLED
#define RS232UART_InternalClockUsed          RS232UART_INTERNAL_CLOCK_USED
#define RS232UART_RXHW_Address_Enabled       RS232UART_RXHW_ADDRESS_ENABLED
#define RS232UART_OverSampleCount            RS232UART_OVER_SAMPLE_COUNT
#define RS232UART_ParityType                 RS232UART_PARITY_TYPE

#if( RS232UART_TX_ENABLED && (RS232UART_TXBUFFERSIZE > RS232UART_FIFO_LENGTH))
    #define RS232UART_TXBUFFER               RS232UART_txBuffer
    #define RS232UART_TXBUFFERREAD           RS232UART_txBufferRead
    #define RS232UART_TXBUFFERWRITE          RS232UART_txBufferWrite
#endif /* End RS232UART_TX_ENABLED */
#if( ( RS232UART_RX_ENABLED || RS232UART_HD_ENABLED ) && \
     (RS232UART_RXBUFFERSIZE > RS232UART_FIFO_LENGTH) )
    #define RS232UART_RXBUFFER               RS232UART_rxBuffer
    #define RS232UART_RXBUFFERREAD           RS232UART_rxBufferRead
    #define RS232UART_RXBUFFERWRITE          RS232UART_rxBufferWrite
    #define RS232UART_RXBUFFERLOOPDETECT     RS232UART_rxBufferLoopDetect
    #define RS232UART_RXBUFFER_OVERFLOW      RS232UART_rxBufferOverflow
#endif /* End RS232UART_RX_ENABLED */

#ifdef RS232UART_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG
    #define RS232UART_CONTROL                RS232UART_CONTROL_REG
#endif /* End RS232UART_BUART_sCR_SyncCtl_CtrlReg__CONTROL_REG */

#if(RS232UART_TX_ENABLED)
    #define RS232UART_TXDATA                 RS232UART_TXDATA_REG
    #define RS232UART_TXSTATUS               RS232UART_TXSTATUS_REG
    #define RS232UART_TXSTATUS_MASK          RS232UART_TXSTATUS_MASK_REG
    #define RS232UART_TXSTATUS_ACTL          RS232UART_TXSTATUS_ACTL_REG
    /* DP clock */
    #if(RS232UART_TXCLKGEN_DP)
        #define RS232UART_TXBITCLKGEN_CTR        RS232UART_TXBITCLKGEN_CTR_REG
        #define RS232UART_TXBITCLKTX_COMPLETE    RS232UART_TXBITCLKTX_COMPLETE_REG
    #else     /* Count7 clock*/
        #define RS232UART_TXBITCTR_PERIOD        RS232UART_TXBITCTR_PERIOD_REG
        #define RS232UART_TXBITCTR_CONTROL       RS232UART_TXBITCTR_CONTROL_REG
        #define RS232UART_TXBITCTR_COUNTER       RS232UART_TXBITCTR_COUNTER_REG
    #endif /* RS232UART_TXCLKGEN_DP */
#endif /* End RS232UART_TX_ENABLED */

#if(RS232UART_HD_ENABLED)
    #define RS232UART_TXDATA                 RS232UART_TXDATA_REG
    #define RS232UART_TXSTATUS               RS232UART_TXSTATUS_REG
    #define RS232UART_TXSTATUS_MASK          RS232UART_TXSTATUS_MASK_REG
    #define RS232UART_TXSTATUS_ACTL          RS232UART_TXSTATUS_ACTL_REG
#endif /* End RS232UART_HD_ENABLED */

#if( (RS232UART_RX_ENABLED) || (RS232UART_HD_ENABLED) )
    #define RS232UART_RXDATA                 RS232UART_RXDATA_REG
    #define RS232UART_RXADDRESS1             RS232UART_RXADDRESS1_REG
    #define RS232UART_RXADDRESS2             RS232UART_RXADDRESS2_REG
    #define RS232UART_RXBITCTR_PERIOD        RS232UART_RXBITCTR_PERIOD_REG
    #define RS232UART_RXBITCTR_CONTROL       RS232UART_RXBITCTR_CONTROL_REG
    #define RS232UART_RXBITCTR_COUNTER       RS232UART_RXBITCTR_COUNTER_REG
    #define RS232UART_RXSTATUS               RS232UART_RXSTATUS_REG
    #define RS232UART_RXSTATUS_MASK          RS232UART_RXSTATUS_MASK_REG
    #define RS232UART_RXSTATUS_ACTL          RS232UART_RXSTATUS_ACTL_REG
#endif /* End  (RS232UART_RX_ENABLED) || (RS232UART_HD_ENABLED) */

#if(RS232UART_INTERNAL_CLOCK_USED)
    #define RS232UART_INTCLOCK_CLKEN         RS232UART_INTCLOCK_CLKEN_REG
#endif /* End RS232UART_INTERNAL_CLOCK_USED */

#define RS232UART_WAIT_FOR_COMLETE_REINIT    RS232UART_WAIT_FOR_COMPLETE_REINIT

#endif  /* CY_UART_RS232UART_H */


/* [] END OF FILE */
