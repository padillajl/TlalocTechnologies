/*
 * MainDriver.c
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
#include "MainDriver.h"

/*************************************************************************************************/
/*********************						Defines							**********************/
/*************************************************************************************************/
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

/*************************************************************************************************/
/*********************					Static Constants					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Global Constants					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************						Functions						**********************/
/*************************************************************************************************/
void vfnMainDriverInit(void)
{	
	vfnShellInit();
}
void vfnMainDriverManager(void)
{
	/* Shell task */
	vfnShellTask();
}
