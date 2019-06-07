/*
 * Shell.c
 *
 *  Created on: Jun 4, 2019
 *      Author: johernandezpc
 */
/*************************************************************************************************/
/*********************						Includes						**********************/
/*************************************************************************************************/
/* System includes */

/* Includes used in this file */

/* Own includes */
#include "Shell.h"
#include "UTIL1.h"
#include "CLS1.h"


/*************************************************************************************************/
/*********************						Defines							**********************/
/*************************************************************************************************/
/*************************************************************************************************/

/*********************						Typedefs						**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Function Prototypes					**********************/
/*************************************************************************************************/
static void pvShellTask(void *pvParameters);
static u08 bfnParseCommand(const u08 *lbCmd, bool *lhandled, const CLS1_StdIOType *lInputOutput);

/* Shell Commands */
static void vfnPrintHelp(const CLS1_StdIOType *lInputOutput);
static void vfnPrintStatus(const CLS1_StdIOType *lInputOutput);
/*************************************************************************************************/
/*********************					Static Variables					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Global Variables					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Static Constants					**********************/
/*************************************************************************************************/
static const CLS1_ParseCommandCallback CmdParserTable[] = 
{
	CLS1_ParseCommand, /* Processor Expert Shell component, is first in list */
	bfnParseCommand, /* My own parser */
	NULL
};
/*************************************************************************************************/
/*********************					Global Constants					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************						Functions						**********************/
/*************************************************************************************************/

/*******************************************************************************/
/*!
      \fn       void SHELL_vfnInit(void)
      \brief	Function that creates the Shell Task
      \param    None
      \return   None
*/
/*******************************************************************************/
void SHELL_vfnInit(void)
{
	/* Create Free RTOS Shell Task */
	if(FRTOS1_xTaskCreate(pvShellTask,"Shell",configMINIMAL_STACK_SIZE+200,NULL,tskIDLE_PRIORITY,NULL) != pdPASS)
	{
		for(;;)
		{
			asm("nop"); /*Error*/
		} 
	}
}

/*******************************************************************************/
/*!
      \fn       static void pvShellTask(void *pvParameters)
      \brief	Task that runs the shell activity
      \param    *pvParameters
      \return   None
*/
/*******************************************************************************/
static void pvShellTask(void *pvParameters)
{
	u08 baCmdBuffer[32];
	/* Unused */
	(void)pvParameters;
	/* Init buffer */
	baCmdBuffer[0] = '\0';
	/* Print help and prints as well the prompt */
	(void)CLS1_ParseWithCommandTable((u08*)CLS1_CMD_HELP,CLS1_GetStdio(),CmdParserTable);
	
	for(;;)
	{
		/* Wait for input and parse it */
		(void)CLS1_ReadAndParseWithCommandTable(baCmdBuffer,sizeof(baCmdBuffer),CLS1_GetStdio(),CmdParserTable);
		FRTOS1_vTaskDelay(50/portTICK_RATE_MS);
	}	
}

/*******************************************************************************/
/*!
      \fn       u08 bfnParseCommand(const u08 *lbCmd, bool *lhandled, const CLS1_StdIOType, *lInputOutput)
      \brief	Parses a command
      \param    lbCmd: Command string to be parsed
      \param    lhandled: Sets this variable to TRUE if command was handled
      \param    *lInputOutput: I/O stream to be used for input/output
      \return   Error code, ERR_OK if everything was fine
*/
/*******************************************************************************/
static u08 bfnParseCommand(const u08 *lbCmd, bool *lhandled, const CLS1_StdIOType *lInputOutput)
{
	/* Handling help command */
	if( UTIL1_strcmp((char*)lbCmd, (char*)CLS1_CMD_HELP) == 0)
	{
		vfnPrintHelp(lInputOutput);
		*lhandled = TRUE;
	}
	/* Print status command */
	else if(UTIL1_strcmp((char*)lbCmd, (char*)CLS1_CMD_STATUS) == 0)
	{
		vfnPrintStatus(lInputOutput);
		*lhandled = TRUE;
	}	
	return ERR_OK;
}
/*******************************************************************************/
/*!
      \fn       static void vfnPrintHelp(const CLS1_StdIOType *lInputOutput)
      \brief	Help function command that prints the help text to the console
      \param    *lInputOutput: I/O channel to be used
      \return   None
*/
/*******************************************************************************/
static void vfnPrintHelp(const CLS1_StdIOType *lInputOutput)
{
	/* Local help */
	CLS1_SendHelpStr((const u08*)"My help", (const u08*)"My help text \r\n", lInputOutput->stdOut);
}
/*******************************************************************************/
/*!
      \fn       static void vfnPrintStatus(const CLS1_StdIOType *lInputOutput)
      \brief	Help function command that prints the status text to the console
      \param    *lInputOutput: I/O channel to be used
      \return   None
*/
/*******************************************************************************/
static void vfnPrintStatus(const CLS1_StdIOType *lInputOutput)
{
	/* Local help */
	CLS1_SendStatusStr((const u08*)"My status", (const u08*)"My status text \r\n", lInputOutput->stdOut);
}
