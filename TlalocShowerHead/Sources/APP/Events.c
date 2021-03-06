/*
 * Events.c
 *
 *  Created on: Jul 25, 2018
 *      Author: Omar Ayala
 */

/*************************************************************************************************/
/*********************						Includes						**********************/
/*************************************************************************************************/
/* System includes */

/* Includes used in this file */

/* Own includes */
#include "Events.h"


/*************************************************************************************************/
/*********************						Defines							**********************/
/*************************************************************************************************/


/*************************************************************************************************/
/*********************						Typedefs						**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Function Prototypes					**********************/
/*************************************************************************************************/
void vfnEventsInit(void);
void vfnEventsDriver (void);
/*************************************************************************************************/
/*********************					Static Variables					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Global Variables					**********************/
/*************************************************************************************************/
u08 bEventsOneSecondFlag;

/*************************************************************************************************/
/*********************					Static Constants					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Global Constants					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************						Functions						**********************/
/*************************************************************************************************/
/*************************************************************************************************/
void vfnEventsInit(void)
{
	//Get a one sec timer
	baSwTimersCurrentTimers[SwTimersDriverEventsTimer]= SwTimers_bfnRequestTimer();
	//Init one sec Timer
	SwTimers_vfnStartTimer(baSwTimersCurrentTimers[SwTimersDriverEventsTimer],1000);
}
void vfnEventsDriver (void)
{
	if(SwTimers_bfnGetStatus(baSwTimersCurrentTimers[SwTimersDriverEventsTimer]))
	{
		bEventsOneSecondFlag = 1;
		SwTimers_vfnStartTimer(baSwTimersCurrentTimers[SwTimersDriverEventsTimer],1000);
	}
}
