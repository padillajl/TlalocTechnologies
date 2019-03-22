/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : SMasterLdd1.c
**     Project     : Test_TranceptorRF
**     Processor   : MKL25Z128VLK4
**     Component   : SPIMaster_LDD
**     Version     : Component 01.111, Driver 01.02, CPU db: 3.00.000
**     Compiler    : GNU C Compiler
**     Date/Time   : 2019-03-20, 14:19, # CodeGen: 8
**     Abstract    :
**         This component "SPIMaster_LDD" implements MASTER part of synchronous
**         serial master-slave communication.
**     Settings    :
**          Component name                                 : SMasterLdd1
**          Device                                         : SPI1
**          Interrupt service/event                        : Enabled
**            Input interrupt                              : INT_SPI1
**            Input interrupt priority                     : medium priority
**            Output interrupt                             : INT_SPI1
**            Output interrupt priority                    : medium priority
**          Settings                                       : 
**            Input pin                                    : Enabled
**              Pin                                        : PTE1/SPI1_MOSI/UART1_RX/SPI1_MISO/I2C1_SCL
**              Pin signal                                 : 
**            Output pin                                   : Enabled
**              Pin                                        : PTE3/SPI1_MISO/SPI1_MOSI
**              Pin signal                                 : 
**            Clock pin                                    : 
**              Pin                                        : PTE2/SPI1_SCK
**              Pin signal                                 : 
**            Chip select list                             : 0
**            Attribute set list                           : 1
**              Attribute set 0                            : 
**                Width                                    : 8 bits
**                MSB first                                : yes
**                Clock polarity                           : Low
**                Clock phase                              : Capture on leading edge
**                Parity                                   : None
**                Chip select toggling                     : no
**                Clock rate index                         : 0
**            Clock rate                                   : 1.907349 �s
**            HW input buffer size                         : 1
**            HW input watermark                           : 1
**            Receiver DMA                                 : Disabled
**            HW output buffer size                        : 1
**            HW output watermark                          : 1
**            Transmitter DMA                              : Disabled
**          Initialization                                 : 
**            Initial chip select                          : 0
**            Initial attribute set                        : 0
**            Enabled in init. code                        : yes
**            Auto initialization                          : no
**            Event mask                                   : 
**              OnBlockSent                                : Enabled
**              OnBlockReceived                            : Enabled
**              OnError                                    : Disabled
**          CPU clock/configuration selection              : 
**            Clock configuration 0                        : This component enabled
**            Clock configuration 1                        : This component disabled
**            Clock configuration 2                        : This component disabled
**            Clock configuration 3                        : This component disabled
**            Clock configuration 4                        : This component disabled
**            Clock configuration 5                        : This component disabled
**            Clock configuration 6                        : This component disabled
**            Clock configuration 7                        : This component disabled
**     Contents    :
**         Init         - LDD_TDeviceData* SMasterLdd1_Init(LDD_TUserData *UserDataPtr);
**         SendBlock    - LDD_TError SMasterLdd1_SendBlock(LDD_TDeviceData *DeviceDataPtr, LDD_TData...
**         ReceiveBlock - LDD_TError SMasterLdd1_ReceiveBlock(LDD_TDeviceData *DeviceDataPtr, LDD_TData...
**
**     Copyright : 1997 - 2014 Freescale Semiconductor, Inc. 
**     All Rights Reserved.
**     
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**     
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**     
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**     
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**     
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**     
**     http: www.freescale.com
**     mail: support@freescale.com
** ###################################################################*/
/*!
** @file SMasterLdd1.c
** @version 01.02
** @brief
**         This component "SPIMaster_LDD" implements MASTER part of synchronous
**         serial master-slave communication.
*/         
/*!
**  @addtogroup SMasterLdd1_module SMasterLdd1 module documentation
**  @{
*/         

/* MODULE SMasterLdd1. */
/*lint -save  -e926 -e927 -e928 -e929 Disable MISRA rule (11.4) checking. */

#include "SM1.h"
#include "SMasterLdd1.h"
/* {Default RTOS Adapter} No RTOS includes */

#ifdef __cplusplus
extern "C" {
#endif 

#define AVAILABLE_EVENTS_MASK (LDD_SPIMASTER_ON_BLOCK_RECEIVED | LDD_SPIMASTER_ON_BLOCK_SENT)

/* These constants contain pins masks */
#define SMasterLdd1_AVAILABLE_PIN_MASK (LDD_SPIMASTER_INPUT_PIN | LDD_SPIMASTER_OUTPUT_PIN | LDD_SPIMASTER_CLK_PIN)

typedef struct {
  LDD_SPIMASTER_TError ErrFlag;        /* Error flags */
  uint16_t InpRecvDataNum;             /* The counter of received characters */
  uint8_t *InpDataPtr;                 /* The buffer pointer for received characters */
  uint16_t InpDataNumReq;              /* The counter of characters to receive by ReceiveBlock() */
  uint16_t OutSentDataNum;             /* The counter of sent characters */
  uint8_t *OutDataPtr;                 /* The buffer pointer for data to be transmitted */
  uint16_t OutDataNumReq;              /* The counter of characters to be send by SendBlock() */
  LDD_TUserData *UserData;             /* User device data structure */
} SMasterLdd1_TDeviceData;             /* Device data structure type */

typedef SMasterLdd1_TDeviceData* SMasterLdd1_TDeviceDataPtr; /* Pointer to the device data structure */

/* {Default RTOS Adapter} Static object used for simulation of dynamic driver memory allocation */
static SMasterLdd1_TDeviceData DeviceDataPrv__DEFAULT_RTOS_ALLOC;
/* {Default RTOS Adapter} Global variable used for passing a parameter into ISR */
static SMasterLdd1_TDeviceDataPtr INT_SPI1__DEFAULT_RTOS_ISRPARAM;
/* Internal method prototypes */

/*
** ===================================================================
**     Method      :  SMasterLdd1_Init (component SPIMaster_LDD)
*/
/*!
**     @brief
**         Initializes the device. Allocates memory for the device data
**         structure, allocates interrupt vectors and sets interrupt
**         priority, sets pin routing, sets timing, etc.
**         If the "Enable in init. code" is set to "yes" value then the
**         device is also enabled(see the description of the Enable()
**         method). In this case the Enable() method is not necessary
**         and needn't to be generated. 
**         This method can be called only once. Before the second call
**         of Init() the Deinit() must be called first.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. This pointer will be
**                           passed as an event or callback parameter.
**     @return
**                         - Device data structure pointer.
*/
/* ===================================================================*/
LDD_TDeviceData* SMasterLdd1_Init(LDD_TUserData *UserDataPtr)
{
  /* Allocate LDD device structure */
  SMasterLdd1_TDeviceDataPtr DeviceDataPrv;

  /* {Default RTOS Adapter} Driver memory allocation: Dynamic allocation is simulated by a pointer to the static object */
  DeviceDataPrv = &DeviceDataPrv__DEFAULT_RTOS_ALLOC;
  DeviceDataPrv->UserData = UserDataPtr; /* Store the RTOS device structure */
  /* Interrupt vector(s) allocation */
  /* {Default RTOS Adapter} Set interrupt vector: IVT is static, ISR parameter is passed by the global variable */
  INT_SPI1__DEFAULT_RTOS_ISRPARAM = DeviceDataPrv;
  DeviceDataPrv->ErrFlag = 0x00U;      /* Clear error flags */
  /* Clear the receive counters and pointer */
  DeviceDataPrv->InpRecvDataNum = 0x00U; /* Clear the counter of received characters */
  DeviceDataPrv->InpDataNumReq = 0x00U; /* Clear the counter of characters to receive by ReceiveBlock() */
  DeviceDataPrv->InpDataPtr = NULL;    /* Clear the buffer pointer for received characters */
  /* Clear the transmit counters and pointer */
  DeviceDataPrv->OutSentDataNum = 0x00U; /* Clear the counter of sent characters */
  DeviceDataPrv->OutDataNumReq = 0x00U; /* Clear the counter of characters to be send by SendBlock() */
  DeviceDataPrv->OutDataPtr = NULL;    /* Clear the buffer pointer for data to be transmitted */
  /* SIM_SCGC4: SPI1=1 */
  SIM_SCGC4 |= SIM_SCGC4_SPI1_MASK;
  /* Interrupt vector(s) priority setting */
  /* NVIC_IPR2: PRI_11=0x80 */
  NVIC_IPR2 = (uint32_t)((NVIC_IPR2 & (uint32_t)~(uint32_t)(
               NVIC_IP_PRI_11(0x7F)
              )) | (uint32_t)(
               NVIC_IP_PRI_11(0x80)
              ));
  /* NVIC_ISER: SETENA|=0x0800 */
  NVIC_ISER |= NVIC_ISER_SETENA(0x0800);
  /* PORTE_PCR1: ISF=0,MUX=5 */
  PORTE_PCR1 = (uint32_t)((PORTE_PCR1 & (uint32_t)~(uint32_t)(
                PORT_PCR_ISF_MASK |
                PORT_PCR_MUX(0x02)
               )) | (uint32_t)(
                PORT_PCR_MUX(0x05)
               ));
  /* PORTE_PCR3: ISF=0,MUX=5 */
  PORTE_PCR3 = (uint32_t)((PORTE_PCR3 & (uint32_t)~(uint32_t)(
                PORT_PCR_ISF_MASK |
                PORT_PCR_MUX(0x02)
               )) | (uint32_t)(
                PORT_PCR_MUX(0x05)
               ));
  /* PORTE_PCR2: ISF=0,MUX=2 */
  PORTE_PCR2 = (uint32_t)((PORTE_PCR2 & (uint32_t)~(uint32_t)(
                PORT_PCR_ISF_MASK |
                PORT_PCR_MUX(0x05)
               )) | (uint32_t)(
                PORT_PCR_MUX(0x02)
               ));
  /* SPI1_C1: SPIE=0,SPE=0,SPTIE=0,MSTR=1,CPOL=0,CPHA=0,SSOE=1,LSBFE=0 */
  SPI1_C1 = (SPI_C1_MSTR_MASK | SPI_C1_SSOE_MASK); /* Set configuration register */
  /* SPI1_C2: SPMIE=0,??=0,TXDMAE=0,MODFEN=1,BIDIROE=0,RXDMAE=0,SPISWAI=0,SPC0=0 */
  SPI1_C2 = SPI_C2_MODFEN_MASK;        /* Set configuration register */
  /* SPI1_BR: ??=0,SPPR=4,SPR=2 */
  SPI1_BR = (SPI_BR_SPPR(0x04) | SPI_BR_SPR(0x02)); /* Set baud rate register */
  /* SPI1_C1: SPE=1 */
  SPI1_C1 |= SPI_C1_SPE_MASK;          /* Enable SPI module */
  /* Registration of the device structure */
  PE_LDD_RegisterDeviceStructure(PE_LDD_COMPONENT_SMasterLdd1_ID,DeviceDataPrv);
  return ((LDD_TDeviceData *)DeviceDataPrv); /* Return pointer to the data data structure */
}

/*
** ===================================================================
**     Method      :  SMasterLdd1_ReceiveBlock (component SPIMaster_LDD)
*/
/*!
**     @brief
**         This method specifies the number of data to receive. The
**         method returns ERR_BUSY until the specified number of
**         characters is received. The method <CancelBlockReception>
**         can be used to cancel a running receive operation.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by <Init> method.
**     @param
**         BufferPtr       - Pointer to A buffer where
**                           received characters will be stored.
**     @param
**         Size            - Size of the block
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active clock configuration
**                           ERR_DISABLED - Component is disabled
**                           ERR_BUSY - The previous receive request is
**                           pending
*/
/* ===================================================================*/
LDD_TError SMasterLdd1_ReceiveBlock(LDD_TDeviceData *DeviceDataPtr, LDD_TData *BufferPtr, uint16_t Size)
{
  if (((SMasterLdd1_TDeviceDataPtr)DeviceDataPtr)->InpDataNumReq != 0x00U) { /* Is the previous receive operation pending? */
    return ERR_BUSY;                   /* If yes then error */
  }
  /* {Default RTOS Adapter} Critical section begin, general PE function is used */
  EnterCritical();
  ((SMasterLdd1_TDeviceDataPtr)DeviceDataPtr)->InpDataPtr = (uint8_t*)BufferPtr; /* Store a pointer to the input data. */
  ((SMasterLdd1_TDeviceDataPtr)DeviceDataPtr)->InpDataNumReq = Size; /* Store a number of characters to be received. */
  ((SMasterLdd1_TDeviceDataPtr)DeviceDataPtr)->InpRecvDataNum = 0x00U; /* Set number of received characters to zero. */
  if ((SPI_PDD_ReadStatusReg(SPI1_BASE_PTR) & SPI_PDD_RX_BUFFER_FULL) != 0U) {
    (void)SPI_PDD_ReadData8bit(SPI1_BASE_PTR); /* Dummy read of the data register */
  }
  SPI_PDD_EnableInterruptMask(SPI1_BASE_PTR, SPI_PDD_RX_BUFFER_FULL_OR_FAULT); /* Enable Rx buffer full interrupt */
  /* {Default RTOS Adapter} Critical section end, general PE function is used */
  ExitCritical();
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  SMasterLdd1_SendBlock (component SPIMaster_LDD)
*/
/*!
**     @brief
**         This method sends a block of characters. The method returns
**         ERR_BUSY when the previous block transmission is not
**         completed. The method <CancelBlockTransmission> can be used
**         to cancel a transmit operation.
**     @param
**         DeviceDataPtr   - Device data structure
**                           pointer returned by <Init> method.
**     @param
**         BufferPtr       - Pointer to the block of data
**                           to send.
**     @param
**         Size            - Number of characters in the buffer.
**     @return
**                         - Error code, possible codes:
**                           ERR_OK - OK
**                           ERR_SPEED - This device does not work in
**                           the active clock configuration
**                           ERR_DISABLED - Component is disabled
**                           ERR_BUSY - The previous transmit request is
**                           pending
*/
/* ===================================================================*/
LDD_TError SMasterLdd1_SendBlock(LDD_TDeviceData *DeviceDataPtr, LDD_TData *BufferPtr, uint16_t Size)
{
  if (((SMasterLdd1_TDeviceDataPtr)DeviceDataPtr)->OutDataNumReq != 0x00U) { /* Is the previous transmit operation pending? */
    return ERR_BUSY;                   /* If yes then error */
  }
  /* {Default RTOS Adapter} Critical section begin, general PE function is used */
  EnterCritical();
  ((SMasterLdd1_TDeviceDataPtr)DeviceDataPtr)->OutDataPtr = (uint8_t*)BufferPtr; /* Set a pointer to the output data. */
  ((SMasterLdd1_TDeviceDataPtr)DeviceDataPtr)->OutDataNumReq = Size; /* Set the counter of characters to be sent. */
  ((SMasterLdd1_TDeviceDataPtr)DeviceDataPtr)->OutSentDataNum = 0x00U; /* Clear the counter of sent characters. */
  SPI_PDD_EnableInterruptMask(SPI1_BASE_PTR, SPI_PDD_TX_BUFFER_EMPTY); /* Enable Tx buffer empty interrupt */
  /* {Default RTOS Adapter} Critical section end, general PE function is used */
  ExitCritical();
  return ERR_OK;                       /* OK */
}

/*
** ===================================================================
**     Method      :  SMasterLdd1_Interrupt (component SPIMaster_LDD)
**
**     Description :
**         The ISR function handling the device receive/transmit 
**         interrupt.
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
PE_ISR(SMasterLdd1_Interrupt)
{
  /* {Default RTOS Adapter} ISR parameter is passed through the global variable */
  SMasterLdd1_TDeviceDataPtr DeviceDataPrv = INT_SPI1__DEFAULT_RTOS_ISRPARAM;
  uint8_t StatReg = SPI_PDD_ReadStatusReg(SPI1_BASE_PTR); /* Read status register */

  (void)DeviceDataPrv;                 /* Supress unused variable warning if needed */
  if ((StatReg & SPI_PDD_RX_BUFFER_FULL) != 0U) { /* Is any char in HW Rx buffer? */
    if (DeviceDataPrv->InpDataNumReq != 0x00U) { /* Is the receive block operation pending? */
      *(DeviceDataPrv->InpDataPtr++) = SPI_PDD_ReadData8bit(SPI1_BASE_PTR); /* Put a character to the receive buffer and increment pointer to receive buffer */
      DeviceDataPrv->InpRecvDataNum++; /* Increment received char. counter */
      if (DeviceDataPrv->InpRecvDataNum == DeviceDataPrv->InpDataNumReq) { /* Is the requested number of characters received? */
        SPI_PDD_DisableInterruptMask(SPI1_BASE_PTR, SPI_PDD_RX_BUFFER_FULL_OR_FAULT); /* Disable Rx buffer full interrupt */
        DeviceDataPrv->InpDataNumReq = 0x00U; /* If yes then clear number of requested characters to be received. */
        SMasterLdd1_OnBlockReceived(DeviceDataPrv->UserData);
      }
    }
  }
  if ((StatReg & SPI_PDD_TX_BUFFER_EMPTYG) != 0U) { /* Is HW Tx buffer empty? */
    if (DeviceDataPrv->OutSentDataNum < DeviceDataPrv->OutDataNumReq) { /* Is number of sent characters less than the number of requested incoming characters? */
      SPI_PDD_WriteData8Bit(SPI1_BASE_PTR, (*((uint8_t *)DeviceDataPrv->OutDataPtr++))); /* Put a character with command to the transmit register and increment pointer to the transmitt buffer */
      DeviceDataPrv->OutSentDataNum++; /* Increment the counter of sent characters. */
      if (DeviceDataPrv->OutSentDataNum == DeviceDataPrv->OutDataNumReq) {
        DeviceDataPrv->OutDataNumReq = 0x00U; /* Clear the counter of characters to be send by SendBlock() */
        SMasterLdd1_OnBlockSent(DeviceDataPrv->UserData);
      }
    } else {
      SPI_PDD_DisableInterruptMask(SPI1_BASE_PTR, SPI_PDD_TX_BUFFER_EMPTY); /* Disable TX interrupt */
    }
  }
}

/*lint -restore Enable MISRA rule (11.4) checking. */
/* END SMasterLdd1. */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.3 [05.09]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
