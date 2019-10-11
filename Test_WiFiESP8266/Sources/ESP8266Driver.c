/*
 * ESP8266Driver.c
 *
 *  Created on: Jun 4, 2019
 *      Author: johernandezpc
 */
/*************************************************************************************************/
/*********************						Includes						**********************/
/*************************************************************************************************/
/* System includes */
#include "WiFiSerial.h"

/* Includes used in this file */

/* Own includes */
#include "ESP8266Driver.h"
#include "Shell.h"

/*************************************************************************************************/
/*********************						Defines							**********************/
/*************************************************************************************************/
/*************************************************************************************************/

/*********************						Typedefs						**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Function Prototypes					**********************/
/*************************************************************************************************/
static u08 ESP8266Driver_GetResponse(u08 *lbRxBuffer, size_t lRxBufferLength, u08 *lbExpectedTail, u16 lwMsTimeOut);
/*************************************************************************************************/
/*********************					Static Variables					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Global Variables					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Static Constants					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Global Constants					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************						Functions						**********************/
/*************************************************************************************************/
void vfnESP8266DriverInit(void)
{
	/* Clean Rx Buffer */
	WiFiSerial_ClearRxBuf();
}

/*******************************************************************************/
/*!
      \fn       u08 ESP8266Driver_ParseCommand (const u08 *lbCmd, bool *lhandled, const CLS1_StdIOType, *lInputOutput)
      \brief	Parses a command and executes the instruction
      \param    lbCmd: Command string to be parsed
      \param    lhandled: Sets this variable to TRUE if command was handled
      \param    *lInputOutput: I/O stream to be used for input/output
      \return   Error code, ERR_OK if everything was fine
*/
/*******************************************************************************/
u08 ESP8266Driver_ParseCommand(const u08 *lbCmd, bool *lhandled, const CLS1_StdIOType *lInputOutput)
{
	const u08 *lbCmdStrPtr;
	u08 lbResponse;
	/* ESP send instruction */
	if( UTIL1_strncmp((char*)lbCmd, "ESP send ", sizeof("ESP send ")-1) == 0 )
	{
		*lhandled = TRUE;
		/* Point to the end of the string of the command */
		lbCmdStrPtr = lbCmd + sizeof("ESP send ")-1;
		/* Send string to the ESP8266 */
		(void)ESP8266Driver_SendString(lbCmdStrPtr,lInputOutput);
		
		lbResponse = ERR_OK;
	}	
	return lbResponse;
}

/*******************************************************************************/
/*!
      \fn       u08 ESP8266Driver_SendString(const u08 *lString, CLS1_ConstStdIOType *lInputOutput)
      \brief	Sends the string data to the ESP8266 device
      \param    lbString: Points to the string that will be sent      
      \param    *lInputOutput: I/O stream to be used for input/output
      \return   Error code, ERR_OK if everything was fine
*/
/*******************************************************************************/
u08 ESP8266Driver_SendString(const u08 *lbString, CLS1_ConstStdIOType *lInputOutput)
{
	u16 lwTimeOutMs;
	u08 lbaStringBuffer[64];
	u08 lbaRxBuffer[48];	
	
	#define RX_TIMEOUT_MS 3000
	
	WiFiSerial_TComData lbChar;
	
	/* Copy the string to the local buffer */
	UTIL1_strcpy(&lbaStringBuffer[0],sizeof(lbaStringBuffer),lbString);
	/* Append the "\r\n" to the local buffer*/
	UTIL1_strcat(&lbaStringBuffer[0],sizeof(lbaStringBuffer),(u08*)"\r\n");
	/* Send AT Command */
	(void)ESP8266Driver_SendATCommand(lbaStringBuffer,lbaRxBuffer,sizeof(lbaRxBuffer),NULL,RX_TIMEOUT_MS,lInputOutput);
	
	lwTimeOutMs = 0;
	while( lwTimeOutMs < RX_TIMEOUT_MS )
	{
		WAIT1_WaitOSms(100);
		lwTimeOutMs += 100;
		
		while( WiFiSerial_GetCharsInRxBuf() > 0 )
		{
			(void)WiFiSerial_RecvChar(&lbChar);
			CLS1_SendChar(lbChar);
		}			
	}
	
	return ERR_OK;
}

/*******************************************************************************/
/*!
      \fn       u08 ESP8266Driver_SendATCommand(u08 *lbCmd, u08 *lbRxBuffer, size_t lRxBufferSize, u08 *lbExpectedTailString, u16 lMsTimeOut, const CLS1_StdIOType *lInputOutput)
      \brief	Sends the string data to the ESP8266 device
      \param    lbCmd: Command string to send
      \param    lbRxBuffer: Buffer for the response, can be NULL
      \param    lRxBufferSize: Size of response Buffer   
      \param	lbExpectedTailString: Expected response from the module, can be NULL
      \param    lwMsTimeOut: Timeout time in milliseconds      
      \param    *lInputOutput: I/O stream to be used for input/output
      \return   Error code, ERR_OK if everything was fine
*/
/*******************************************************************************/
u08 ESP8266Driver_SendATCommand(u08 *lbCmd, u08 *lbRxBuffer, size_t lRxBufferSize, u08 *lbExpectedTailString, u16 lwMsTimeOut, const CLS1_StdIOType *lInputOutput)
{
	u16 lwSizeSentPointer;
	u08 lbResponse; 
	
	/* Check if it points to the buffer location */
	if( lbRxBuffer != NULL )
		lbRxBuffer[0] = '\0';	
	/* Check if it points to the IO Channel */
	if( lInputOutput != NULL )
	{
		CLS1_SendStr((u08*)"Sent>>:\r\n",lInputOutput->stdOut);
		/* Write in shell the AT Command sent */
		CLS1_SendStr(lbCmd,lInputOutput->stdOut);
	}
	
	/* Send block via Serial */
	if( WiFiSerial_SendBlock(lbCmd,(u16)UTIL1_strlen((char*)lbCmd),&lwSizeSentPointer) != ERR_OK )
		return ERR_FAILED;	
	
	if( lbRxBuffer != NULL )
		lbResponse = ESP8266Driver_GetResponse(lbRxBuffer,lRxBufferSize,lbExpectedTailString,lwMsTimeOut);
	
	/* Print on shell the data receive */
	if( lInputOutput!= NULL )
	{
		CLS1_SendStr((u08*)"Received>>:\r\n",lInputOutput->stdOut);
		/* Write in shell the AT Command received */
		CLS1_SendStr(lbRxBuffer,lInputOutput->stdOut);
	}
	
	return lbResponse;
	return 0;
}


/*******************************************************************************/
/*!
      \fn       static u08 ESP8266Driver_GetResponse(u08 *lbRxBuffer, size_t lRxBufferLength, u08 *lbExpectedTail, u16 lwMsTimeOut)
      \brief	Handle the response from the ESP8266 Module      
      \param    lbRxBuffer: Buffer for the response
      \param    lRxBufferLength: Size of response Buffer   
      \param	lbExpectedTail: Expected response from the module
      \param    lwMsTimeOut: Timeout time in milliseconds      
      \return   Error code, ERR_OK if everything was fine
*/
/*******************************************************************************/
static u08 ESP8266Driver_GetResponse(u08 *lbRxBuffer, size_t lRxBufferLength, u08 *lbExpectedTail, u16 lwMsTimeOut)
{
	u08 *lbRxBufferPtr;
	u08 lbChar;
	u08 lbResponse = ERR_OK;	
	
	if( lRxBufferLength < sizeof("x\r\n") )
		return ERR_OVERFLOW;
	/* Points to the RxBuffer */
	lbRxBufferPtr = lbRxBuffer;
	lbRxBufferPtr[0] = '\0';
	
	for(;;)
	{
		/* If timer is over */
		if( !lwMsTimeOut )
			break;
		/* If buffer length is over */
		else if( !lRxBufferLength )
		{
			lbResponse = ERR_OVERFLOW;
			break;
		}
		/* If there are still chars in buffer */
		else if( WiFiSerial_GetCharsInRxBuf() > 0 )
		{
			(void)WiFiSerial_RecvChar(&lbChar);
			/* Increase the pointer and get the char */
			*lbRxBufferPtr++ = lbChar;
			/* Always terminate */
			*lbRxBufferPtr = '\0';
			/* Decrease the buffer length */
			lRxBufferLength--;
		}
		else if( lbExpectedTail != NULL && lbExpectedTail[0] != '\0' && UTIL1_strtailcmp(lbRxBuffer,lbExpectedTail) == 0)
			break; /* Finished */
		else
		{
			/* Wait 1 ms */
			WAIT1_WaitOSms(1);
			/* Decrease Ms timeout */
			lwMsTimeOut--;			
		}		
	}	
	if( !lwMsTimeOut )
	{
		/* If expected something and its empty is an error */		
		if( lbExpectedTail[0] != '\0' || lbRxBuffer[0] == '\0' )
			lbResponse = ERR_FAULT;	
	}
	return lbResponse;	
}

