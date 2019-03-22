/*
 * NRF24L01Driver.c
 *
 *  Created on: Jan 14, 2019
 *      Author: José Padilla
 */
/*************************************************************************************************/
/*********************						Includes						**********************/
/*************************************************************************************************/
/* System includes */

/* Includes used in this file */

/* Own includes */
#include "NRF24L01Driver.h"

/*************************************************************************************************/
/*********************						Defines							**********************/
/*************************************************************************************************/
#define NRF24L01DriverBusSTXCmd								(0x02)
#define NRF24L01DriverBusETXCmd								(0x03)

#define NRF24L01DriverBufferSize							(5)
/*************************************************************************************************/
/*********************						Typedefs						**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Function Prototypes					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Static Variables					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Global Variables					**********************/
/*************************************************************************************************/
u08 bNRF24L01DriverRxCharCounter;
u08 bNRF24L01DriverTxCharCounter;

u08 baNRF24L01DriverRxBuffer[NRF24L01DriverBufferSize];
u08 baNRF24L01DriverTxBuffer[NRF24L01DriverBufferSize] = "0123";

u08 *bptrNRF24L01DriverRxBuffer;
u08 *bptrNRF24L01DriverTxBuffer;

u08 bNRF24L01DriverStatus;

LDD_TDeviceData *SPISlaveLDD;
/*************************************************************************************************/
/*********************					Static Constants					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Global Constants					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************						Functions						**********************/
/*************************************************************************************************/
static u08 SPI_WriteRead(u08 lbData)
{
	u08 lbChar;
	
	while(SM1_GetCharsInTxBuf()!=0){} /* Wait until transmission buffer is empty */
	while(SM1_SendChar(lbData)!= ERR_OK){} /* Send character */
	while(SM1_GetCharsInTxBuf()!=0){} /* Wait until transmission buffer is empty */
	while(SM1_GetCharsInRxBuf()!=0){} /* Wait until we receive data */
	while(SM1_RecvChar(&lbChar)!= ERR_OK){} /* Obtain data */
	
	return lbChar;	
}

static void SPI_WriteBuffer(const u08 *lbBufferData,u08 lbBufferSize)
{
	while(lbBufferSize)
	{
		SPI_WriteRead(*lbBufferData++); /* Send byte per byte */
		lbBufferSize--;
	}
}

static void SPI_WriteReadBuffer(const u08 *lbBufferData,u08 lbBufferSize)
{
	while(lbBufferSize)
	{
		SPI_WriteRead(*lbBufferData++); /* Send byte per byte */
		lbBufferSize--;
	}
}

void vfnNRF24L01DriverWriteByte(u08 lbData)
{
	NRF24L01DRIVER_CSN_LOW(); /* CSN Low: intiate command sequence*/
	(void)SPI_WriteRead(lbData);
	NRF24L01DRIVER_CSN_HIGH(); /* CSN High: end command sequence*/
}

void vfnNRF24L01DriverInit(void)
{
	NRF24L01DRIVER_CE_LOW(); /* CE Low: not sending or listening */
	NRF24L01DRIVER_CSN_HIGH(); /* CSN High: not sending commands*/	
}
 
void vfnNRF24L01DriverTransmitPayload(u08 *lbBufferPayload,u08 lbPayloadSize)
{
	/* Flush transmission FIFO */
	vfnNRF24L01DriverWriteByte(NRF24L01DRIVER_FLUSH_TX);
	/* Write payload */
	vfnNRF24L01DriverWriteRegisterData(NRF24L01DRIVER_W_TX_PAYLOAD,lbBufferPayload,lbPayloadSize);
	/* Start transmission */
	NRF24L01DRIVER_CE_HIGH(); /* Is nedeed a high pulse on the CE for more than 10us */
	/* Keep signal high for 15 micro-seconds */
	NRF24L01DRIVER_CE_LOW(); /* End high pulse */	
}

void vfnNRF24L01DriverReceivePayload(u08 *lbBufferPayload,u08 lbPayloadSize)
{
	NRF24L01DRIVER_CSN_LOW(); /* CSN Low: intiate command sequence*/
	(void)SPI_WriteRead(lbData);
	NRF24L01DRIVER_CSN_HIGH(); /* CSN High: end command sequence*/
}

u08 bfnNRF24L01DriverGetStatus(void)
{
	/* Command name "NOP" is used to read the STATUS register */
	return SPI_WriteRead(NRF24L01DRIVER_NOP);	
}
void vfnNRF24L01DriverWriteRegister(u08 lbRegister,u08 lbValue)
{
	NRF24L01DRIVER_CSN_LOW(); /* CSN Low: intiate command sequence*/
	(void)SPI_WriteRead(NRF24L01DRIVER_W_REGISTER|lbRegister); /* Write register command */
	(void)SPI_WriteRead(lbValue); /* Write value */
	NRF24L01DRIVER_CSN_HIGH(); /* CSN High: end command sequence*/
}

void vfnNRF24L01DriverWriteRegisterData(u08 lbRegister,const u08 *lbBufferData,u08 lbBufferSize)
{
	NRF24L01DRIVER_CSN_LOW(); /* CSN Low: intiate command sequence*/
	(void)SPI_WriteRead(NRF24L01DRIVER_W_REGISTER|lbRegister); /* Write register command */	
	SPI_WriteBuffer(lbBufferData,lbBufferSize);	/* Send complete buffer */
	NRF24L01DRIVER_CSN_HIGH(); /* CSN High: end command sequence*/
}
void vfnNRF24L01DriverReadRegisterData(u08 lbRegister,u08 *lbBufferData,u08 lbBufferSize)
{
	NRF24L01DRIVER_CSN_LOW(); /* CSN Low: intiate command sequence*/
	(void)SPI_WriteRead(NRF24L01DRIVER_R_RX_PAYLOAD|lbRegister); /* Write register command */	
	SPI_WriteBuffer(lbBufferData,lbBufferSize);	/* Send complete buffer */
	NRF24L01DRIVER_CSN_HIGH(); /* CSN High: end command sequence*/
}
void vfnNRF24L01DriverResetStatusIRQ(u08 lbFlags)
{
	NRF24L01DRIVER_CSN_LOW(); /* CSN Low: intiate command sequence*/
	vfnNRF24L01DriverWriteRegister(NRF24L01DRIVER_REG_STATUS,lbFlags)
	NRF24L01DRIVER_CSN_HIGH(); /* CSN High: end command sequence*/
}
