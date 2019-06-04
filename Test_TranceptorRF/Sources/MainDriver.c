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
#include "MainDriver.h"

/*************************************************************************************************/
/*********************						Defines							**********************/
/*************************************************************************************************/
#define MAINDRIVER_OPERATION_MODE_TRANSMITTER		(1)
#define MAINDRIVER_OPERATION_MODE_RECEIVER 			(0)	
#define MAINDRIVER_OPERATION_MODE 					(MAINDRIVER_OPERATION_MODE_RECEIVER)

#define MAINDRIVER_PAYLOAD_SIZE			 			(16)
#define MAINDRIVER_CHANNEL_NO 					 	(2)

#define MAINDRIVER_RXTX_CONFIG_1					(NRF24L01DRIVER_RF_SETUP_RF_PWR_0|NRF24L01DRIVER_RF_SETUP_RF_DR_250) /* Output power in Tx mode of 0dBm and 250 kbps data rate */
#define MAINDRIVER_RXTX_CONFIG_2					MAINDRIVER_PAYLOAD_SIZE /* Number of payload bytes to send and receive */
#define MAINDRIVER_RXTX_CONFIG_3					MAINDRIVER_CHANNEL_NO /* Sets the frequency channel nRF24L01+ operates on */
#define MAINDRIVER_RXTX_CONFIG_4					NRF24L01DRIVER_EN_RXADDR_ERX_P0 /* Enable RX_ADDR_P0 address matching */

#define MAINDRIVER_TX_CONFIG_1						NRF24L01DRIVER_EN_AA_ENAA_P0 /* Enable auto acknowledgement data pipe 0 */
#define MAINDRIVER_TX_CONFIG_2						NRF24L01DRIVER_SETUP_RETR_ARD_750_US|NRF24L01DRIVER_SETUP_RETR_ARC_15 /* Set up of automatic retransmission with 750 us delay and up to 15 retransmit count */
#define MAINDRIVER_TX_CONFIG_3						(NRF24L01DRIVER_EN_CRC|NRF24L01DRIVER_CRCO|NRF24L01DRIVER_PWR_UP|NRF24L01DRIVER_PRIM_TX) /* 2 bytes encoding CRC, Power UP, Enable CRC, Tx control*/

#define MAINDRIVER_RX_CONFIG_1						(NRF24L01DRIVER_EN_CRC|NRF24L01DRIVER_CRCO|NRF24L01DRIVER_PWR_UP|NRF24L01DRIVER_PRIM_RX)
/* 2 bytes encoding CRC, Power UP, Enable CRC, Rx control*/

/*************************************************************************************************/
/*********************						Typedefs						**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Function Prototypes					**********************/
/*************************************************************************************************/
u08 bfnMainDriverValidateConfiguration(void);
/*************************************************************************************************/
/*********************					Static Variables					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Global Variables					**********************/
/*************************************************************************************************/
u08 baMainDriverConfigBuffer[7];
static volatile u08 bMainDriverISRFlag;
#if MAINDRIVER_OPERATION_MODE == MAINDRIVER_OPERATION_MODE_TRANSMITTER
static u08 baMainDriverPayloadBuffer[MAINDRIVER_PAYLOAD_SIZE]= "PACKET NUMBER:  ";
u08 bPacketCounter = 0;
#endif
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
	
	/* Init LED's in off state */
	GreenLED_SetVal();
	RedLED_SetVal();
	BlueLED_SetVal();
	
	baMainDriverConfigBuffer[0] = MAINDRIVER_RXTX_CONFIG_1;
	vfnNRF24L01DriverWriteRegister(NRF24L01DRIVER_REG_RF_SETUP,MAINDRIVER_RXTX_CONFIG_1); /* RXTX Config 1 */
	
	baMainDriverConfigBuffer[1] = MAINDRIVER_RXTX_CONFIG_2;
	vfnNRF24L01DriverWriteRegister(NRF24L01DRIVER_RX_PW_P0,MAINDRIVER_RXTX_CONFIG_2); /* RXTX Config 2 */
	
	baMainDriverConfigBuffer[2] = MAINDRIVER_RXTX_CONFIG_3;
	vfnNRF24L01DriverWriteRegister(NRF24L01DRIVER_REG_RF_CH,MAINDRIVER_RXTX_CONFIG_3); /* RXTX Config 3 */
	
	/* Handle automatic acknowledge */
	/* Receive address data pipe 5 bytes maximum length */  
	vfnNRF24L01DriverWriteRegisterData(NRF24L01DRIVER_RX_ADDR_P0,&baMainDriverDeviceAddress[0],sizeof(baMainDriverDeviceAddress));
	/* Transmit address. Set RX Address equal to this address to handle automatic acknowledge */
	vfnNRF24L01DriverWriteRegisterData(NRF24L01DRIVER_TX_ADDR,&baMainDriverDeviceAddress[0],sizeof(baMainDriverDeviceAddress));
	
	baMainDriverConfigBuffer[3] = MAINDRIVER_RXTX_CONFIG_4;
	vfnNRF24L01DriverWriteRegister(NRF24L01DRIVER_REG_EN_RXADDR,MAINDRIVER_RXTX_CONFIG_4); /* RXTX Config 4 */
	
	/* Clear interrupt flags */
	vfnNRF24L01DriverResetStatusIRQ(NRF24L01DRIVER_STATUS_RX_DR|NRF24L01DRIVER_STATUS_TX_DS|NRF24L01DRIVER_STATUS_MAX_RT);
	
	#if MAINDRIVER_OPERATION_MODE == MAINDRIVER_OPERATION_MODE_TRANSMITTER
	
	baMainDriverConfigBuffer[4] = MAINDRIVER_TX_CONFIG_1;
	vfnNRF24L01DriverWriteRegister(NRF24L01DRIVER_REG_EN_AA,MAINDRIVER_TX_CONFIG_1); /* TX Config 1 */
	
	baMainDriverConfigBuffer[5] = MAINDRIVER_TX_CONFIG_2;
	vfnNRF24L01DriverWriteRegister(NRF24L01DRIVER_REG_SETUP_RETR,MAINDRIVER_TX_CONFIG_2); /* TX Config 2 */
	
	baMainDriverConfigBuffer[6] = MAINDRIVER_TX_CONFIG_3;
	vfnNRF24L01DriverWriteRegister(NRF24L01DRIVER_REG_CONFIG,MAINDRIVER_TX_CONFIG_3); /* TX Config 3 */ /* Power up in transmitting mode */		
	/* Will pulse this later to send data */
	NRF24L01DRIVER_CE_LOW();
	
	#elif MAINDRIVER_OPERATION_MODE == MAINDRIVER_OPERATION_MODE_RECEIVER
	baMainDriverConfigBuffer[4] = MAINDRIVER_RX_CONFIG_1;
	vfnNRF24L01DriverWriteRegister(NRF24L01DRIVER_REG_CONFIG,MAINDRIVER_RX_CONFIG_1); /* RX Config 1 */ /* Power up in receiving mode */
	/* High level to listen for packets */
	NRF24L01DRIVER_CE_HIGH();
	#endif
	
	/* Driver Validation */
	if( bfnMainDriverValidateConfiguration() )
		GreenLED_ClrVal();
	else
		RedLED_ClrVal();
	
	
}

void vfnMainDriverManager(void)
{
	u08 lbMainDriverStatus;
#if MAINDRIVER_OPERATION_MODE == MAINDRIVER_OPERATION_MODE_TRANSMITTER
	/*Add packet number */
	vfnBin8ToASCIIBCD(bPacketCounter,&baMainDriverPayloadBuffer[14],2,0);
	bPacketCounter++;
	vfnNRF24L01DriverTransmitPayload(&baMainDriverPayloadBuffer[0],sizeof(baMainDriverPayloadBuffer));
	/*Validate packet number limit */
	if(bPacketCounter > 99)
		bPacketCounter = 0;
	
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

	}
	
#endif	
}

u08 bfnMainDriverValidateConfiguration(void)
{
	u08 lbaMainDriverReadConfigBuffer[7];
	u08 lbConfigIndex = 0;
	u08 lbConfigTotal;
	
	//Clean local buffer
	memset(&lbaMainDriverReadConfigBuffer[0],0,sizeof(lbaMainDriverReadConfigBuffer));
	
	lbaMainDriverReadConfigBuffer[0] = bfnNRF24L01DriverReadRegister(NRF24L01DRIVER_REG_RF_SETUP);
	lbaMainDriverReadConfigBuffer[1] = bfnNRF24L01DriverReadRegister(NRF24L01DRIVER_RX_PW_P0);
	lbaMainDriverReadConfigBuffer[2] = bfnNRF24L01DriverReadRegister(NRF24L01DRIVER_REG_RF_CH);
	lbaMainDriverReadConfigBuffer[3] = bfnNRF24L01DriverReadRegister(NRF24L01DRIVER_REG_EN_RXADDR); /* Enable data pipe 0 */
	
	#if MAINDRIVER_OPERATION_MODE == MAINDRIVER_OPERATION_MODE_TRANSMITTER
	/* TX */
	lbaMainDriverReadConfigBuffer[4] = bfnNRF24L01DriverReadRegister(NRF24L01DRIVER_REG_EN_AA);
	lbaMainDriverReadConfigBuffer[5] = bfnNRF24L01DriverReadRegister(NRF24L01DRIVER_REG_SETUP_RETR);
	lbaMainDriverReadConfigBuffer[6] = bfnNRF24L01DriverReadRegister(NRF24L01DRIVER_REG_CONFIG);
	
	/* 7 configuration for TX */
	lbConfigTotal = 7;
	
	#elif MAINDRIVER_OPERATION_MODE == MAINDRIVER_OPERATION_MODE_RECEIVER
	/* RX */
	lbaMainDriverReadConfigBuffer[4] = bfnNRF24L01DriverReadRegister(NRF24L01DRIVER_REG_CONFIG);
	lbConfigTotal = 5;	
	#endif
	
	/* Validate read configuration */
	while( lbConfigIndex < lbConfigTotal)
	{
		/* If a value is different, the write configuration is wrong */
		if(lbaMainDriverReadConfigBuffer[lbConfigIndex] != baMainDriverConfigBuffer [lbConfigIndex])
			return 0;
		
		lbConfigIndex++;
	}
	
	return 1;
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
