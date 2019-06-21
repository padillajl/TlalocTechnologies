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
/* predefined commands */

/*************************************************************************************************/

/*********************						Typedefs						**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Function Prototypes					**********************/
/*************************************************************************************************/
static u08 bfnParseCommand(const u08 *lbCmd, bool *lhandled, const CLS1_StdIOType *lInputOutput);

/* Shell Commands */
static void vfnPrintHelp(const CLS1_StdIOType *lInputOutput);
static void vfnPrintStatus(const CLS1_StdIOType *lInputOutput);
/* User Commands */
/*************************************************************************************************/
/*********************					Static Variables					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Global Variables					**********************/
/*************************************************************************************************/
u08 bEchoEnabled;
/*************************************************************************************************/
/*********************					Static Constants					**********************/
/*************************************************************************************************/
static const CLS1_ParseCommandCallback CmdParserTable[] = 
{
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
      \fn       void vfnShellInit(void)
      \brief	Function that init shell parameters
      \param    
      \return   None
*/
/*******************************************************************************/
void vfnShellInit(void)
{
#if CLS1_ECHO_ENABLED                   
	bEchoEnabled = 1;
#else
	bEchoEnabled = 0;
#endif	
}

/*******************************************************************************/
/*!
      \fn       static void pvShellTask(void *pvParameters)
      \brief	Task that runs the shell activity
      \param    *pvParameters
      \return   None
*/
/*******************************************************************************/
void vfnShellTask(void)
{
	u08 baCmdBuffer[32];
	/* Init buffer */
	baCmdBuffer[0] = '\0';
	/* Print command list and prints as well the prompt */
	/* the command list is attached to CLS1_CMD_HELP */
	(void)CLS1_ParseWithCommandTable((u08*)CLS1_CMD_HELP,CLS1_GetStdio(),CmdParserTable);
	
	for(;;)
	{
		/* Wait for input and parse it */
		(void)CLS1_ReadAndParseWithCommandTable(baCmdBuffer,sizeof(baCmdBuffer),CLS1_GetStdio(),CmdParserTable);
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
	/* Handling echo on command */
	else if ((UTIL1_strcmp((char*)lbCmd, "echo on")==0)) 
	{
		/* Turn echo on */
		CLS1_ParseCommand((const u08*)"CLS1 echo on",lhandled,lInputOutput);
		bEchoEnabled = 1;
	}
	/* Handling echo off command */
	else if ((UTIL1_strcmp((char*)lbCmd, "echo off")==0)) 
	{
		/* Turn echo off */
		CLS1_ParseCommand((const u08*)"CLS1 echo off",lhandled,lInputOutput);
		bEchoEnabled = 0;
	}
	  
	return ERR_OK;
}
/*******************************************************************************/
/*!
      \fn       static void vfnPrintHelp(const CLS1_StdIOType *lInputOutput)
      \brief	Help function command that prints the command list
      \param    *lInputOutput: I/O channel to be used
      \return   None
*/
/*******************************************************************************/
static void vfnPrintHelp(const CLS1_StdIOType *lInputOutput)
{
	/* Send tittle string */
	CLS1_SendStr((unsigned char*)"\rGroup of shell commands\r\n", lInputOutput->stdOut);
	/* Print command list */
	CLS1_SendHelpStr((unsigned char*)"help", (const unsigned char*)"Print help information\r\n", lInputOutput->stdOut);
	CLS1_SendHelpStr((unsigned char*)"status", (const unsigned char*)"Print status information\r\n", lInputOutput->stdOut);
#if CLS1_ECHO_ENABLED
	CLS1_SendHelpStr((unsigned char*)"echo (on|off)", (const unsigned char*)"Turn echo on or off\r\n", lInputOutput->stdOut);
#endif
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
	/* Send tittle description */
	CLS1_SendStr((unsigned char*)"\rShell Status\r\n", lInputOutput->stdOut);
	CLS1_SendStatusStr((const unsigned char*)"Build", (const unsigned char*)__DATE__, lInputOutput->stdOut);
	CLS1_SendStr((unsigned char*)" ", lInputOutput->stdOut);
	CLS1_SendStr((unsigned char*)__TIME__, lInputOutput->stdOut);
	CLS1_SendStr((unsigned char*)"\r\n", lInputOutput->stdOut);
	#if CLS1_ECHO_ENABLED
		if(bEchoEnabled)
			CLS1_SendStatusStr((const unsigned char*)"echo", (const unsigned char*)"On\r\n", lInputOutput->stdOut);
		else
			CLS1_SendStatusStr((const unsigned char*)"echo", (const unsigned char*)"Off\r\n", lInputOutput->stdOut);
	#endif
}
