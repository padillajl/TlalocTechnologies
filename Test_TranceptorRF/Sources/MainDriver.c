/*
 * MainDriver.c
 *
 *  Created on: Mar 21, 2019
 *      Author: johernandezpc
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
#define MAINDRIVER_OPERATION_MODE_TRANSMITTER		(1)
#define MAINDRIVER_OPERATION_MODE_RECEIVER 			(0)	
#define MAINDRIVER_OPERATION_MODE 					(MAINDRIVER_OPERATION_MODE_TRANSMITTER)

#define MAINDRIVER_PAYLOAD_SIZE			 			(16)
#define MAINDRIVER_CHANNEL_NO 					 	(2)

/* 2 bytes encoding CRC, Power UP, Enable CRC, Tx control*/
#define MAINDRIVER_CONFIG_TX_POWERUP() 		vfnNRF24L01DriverWriteRegister(NRF24L01DRIVER_REG_CONFIG,(NRF24L01DRIVER_EN_CRC|NRF24L01DRIVER_CRCO|NRF24L01DRIVER_PWR_UP|NRF24L01DRIVER_PRIM_TX))
/* 2 bytes encoding CRC, Power UP, Enable CRC, Rx control*/
#define MAINDRIVER_CONFIG_RX_POWERUP() 		vfnNRF24L01DriverWriteRegister(NRF24L01DRIVER_REG_CONFIG,(NRF24L01DRIVER_EN_CRC|NRF24L01DRIVER_CRCO|NRF24L01DRIVER_PWR_UP|NRF24L01DRIVER_PRIM_RX))
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
static u08 baMainDriverPayloadBuffer[MAINDRIVER_PAYLOAD_SIZE]= "PACKET NUMBER:  ";
static volatile u08 bMainDriverISRFlag;
/*************************************************************************************************/
/*********************					Static Constants					**********************/
/*************************************************************************************************/
static const u08 baMainDriverDeviceAddress[5] = {0x11,0x22,0x33,0x44,0x55};
/*************************************************************************************************/
/*********************					Global Constants					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************						Functions						**********************/
/*************************************************************************************************/
void vfnMainDriverInit(void)
{
	vfnNRF24L01DriverInit();
	
	/* Output power in Tx mode of 0dBm and 250 kbps data rate */
	vfnNRF24L01DriverWriteRegister(NRF24L01DRIVER_REG_RF_SETUP,NRF24L01DRIVER_RF_SETUP_RF_PWR_0|NRF24L01DRIVER_RF_SETUP_RF_DR_250);
	/* Number of payload bytes to send and receive */
	vfnNRF24L01DriverWriteRegister(NRF24L01DRIVER_RX_PW_P0,MAINDRIVER_PAYLOAD_SIZE);
	/* Sets the frequency channel nRF24L01+ operates on */
	vfnNRF24L01DriverWriteRegister(NRF24L01DRIVER_REG_RF_CH,MAINDRIVER_CHANNEL_NO);
	/* Handle automatic acknowledge */
	/* Receive address data pipe 5 bytes maximum length */  
	vfnNRF24L01DriverWriteRegisterData(NRF24L01DRIVER_RX_ADDR_P0,&baMainDriverDeviceAddress[0],sizeof(baMainDriverDeviceAddress));
	/* Transmit address. Set RX Address equal to this address to handle automatic acknowledge */
	vfnNRF24L01DriverWriteRegisterData(NRF24L01DRIVER_TX_ADDR,&MainDriverDeviceAddress[0],sizeof(MainDriverDeviceAddress));
	
	/* Enable RX_ADDR_P0 address matching */
	vfnNRF24L01DriverWriteRegister(NRF24L01DRIVER_REG_EN_RXADDR,NRF24L01DRIVER_EN_RXADDR_ERX_P0); /* Enable data pipe 0 */
	/* Clear interrupt flags */
	vfnNRF24L01DriverResetStatusIRQ(NRF24L01DRIVER_STATUS_RX_DR|NRF24L01DRIVER_STATUS_TX_DS|NRF24L01DRIVER_STATUS_MAX_RT);
	
#if MAINDRIVER_OPERATION_MODE == MAINDRIVER_OPERATION_MODE_TRANSMITTER
	/* Enable auto acknowledgement data pipe 0 */
	vfnNRF24L01DriverWriteRegister(NRF24L01DRIVER_REG_EN_AA,NRF24L01DRIVER_EN_AA_ENAA_P0);
	/* Set up of automatic retransmission with 750 us delay and up to 15 retransmit count */
	vfnNRF24L01DriverWriteRegister(NRF24L01DRIVER_REG_SETUP_RETR,NRF24L01DRIVER_SETUP_RETR_ARD_750_US|NRF24L01DRIVER_SETUP_RETR_ARC_15);
	/* Power up in transmitting mode */
	MAINDRIVER_CONFIG_TX_POWERUP();
	/* Will pulse this later to send data */
	NRF24L01DRIVER_CE_LOW();
#elif MAINDRIVER_OPERATION_MODE == MAINDRIVER_OPERATION_MODE_RECEIVER
	/* Power up in receiving mode */
	MAINDRIVER_CONFIG_RX_POWERUP();
	/* High level to listen for packets */
	NRF24L01DRIVER_CE_HIGH();
#endif
}

void vfnMainDriverManager(void)
{
	u08 lbMainDriverStatus;
#if MAINDRIVER_OPERATION_MODE == MAINDRIVER_OPERATION_MODE_TRANSMITTER
	vfnNRF24L01DriverTransmitPayload(&baMainDriverPayloadBuffer[0],sizeof(baMainDriverPayloadBuffer));
	
	/* Check for a received interrupt */
	if(bMainDriverISRFlag)
	{
		bMainDriverISRFlag = FALSE; /* Reset interrupt flag */
		lbMainDriverStatus = bfnNRF24L01DriverGetStatus(); /* Obtain status*/
		/* Check if data is ready in RX FIFO */
		if( lbMainDriverStatus & NRF24L01DRIVER_STATUS_RX_DR )
			vfnNRF24L01DriverResetStatusIRQ(NRF24L01DRIVER_STATUS_RX_DR);
		/* Check if data has been sent */
		else if( lbMainDriverStatus & NRF24L01DRIVER_STATUS_TX_DS )
			vfnNRF24L01DriverResetStatusIRQ(NRF24L01DRIVER_STATUS_TX_DS);
		/* Check if has tried the maximum number of TX retransmits */
		else if( lbMainDriverStatus & NRF24L01DRIVER_STATUS_MAX_RT )
			vfnNRF24L01DriverResetStatusIRQ(NRF24L01DRIVER_STATUS_MAX_RT);
		
		/* Esperar 1 milisecond */		
	}
#elif MAINDRIVER_OPERATION_MODE == MAINDRIVER_OPERATION_MODE_RECEIVER
	/* Check for a received interrupt */
	if(bMainDriverISRFlag)
	{
		bMainDriverISRFlag = FALSE; /* Reset interrupt flag */
		lbMainDriverStatus = bfnNRF24L01DriverGetStatus(); /* Obtain status*/0
		/* Check if data is ready in RX FIFO */
		if( lbMainDriverStatus & NRF24L01DRIVER_STATUS_RX_DR )
			vfnNRF24L01DriverResetStatusIRQ(NRF24L01DRIVER_STATUS_RX_DR);
		/* Check if data has been sent */
		else if( lbMainDriverStatus & NRF24L01DRIVER_STATUS_TX_DS )
			vfnNRF24L01DriverResetStatusIRQ(NRF24L01DRIVER_STATUS_TX_DS);
		/* Check if has tried the maximum number of TX retransmits */
		else if( lbMainDriverStatus & NRF24L01DRIVER_STATUS_MAX_RT )
			vfnNRF24L01DriverResetStatusIRQ(NRF24L01DRIVER_STATUS_MAX_RT);

	}
	
#endif	
}
/************************************************************************************************************************/
/*!
	\fn			vfnMainDriver_ISR
	\brief		Interrupt service routine: is a software process by an interrupt request from a hardware device
	\param		none
	\return		none
*/
/************************************************************************************************************************/
void vfnMainDriver_ISR(void)
{
	/* Stop sending / listening */
	NRF24L01DRIVER_CE_LOW();
	bMainDriverISRFlag = TRUE;	
}
