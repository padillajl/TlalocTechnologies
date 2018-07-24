/*
 * SensorsControl.c
 *
 *  Created on: Jul 29, 2017
 *      Author: Toxic
 */


/*************************************************************************************************/
/*********************						Includes						**********************/
/*************************************************************************************************/
/* System includes */
#include "derivative.h"
/* Includes used in this file */

/* Own includes */
#include "SensorsControl.h"
#include "SwTimers.h"
/*************************************************************************************************/
/*********************						Defines							**********************/
/*************************************************************************************************/


#define SENSORSCONTROL_SM_NUM_FUNCTIONS					(2)
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
u08 SENSORSCONTROL_gbStatus;
tSensorsControlSM sSensorsControlSM;

/*************************************************************************************************/
/*********************					Static Constants					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Global Constants					**********************/
/*************************************************************************************************/

void SENSORSCONTROL_vfnInit(void)
{
	/* Infrared sensors Up and Down timer init */
	SwTimers_vfnStartTimer(SENSORSCONTROL_CHN,SENSORSCONTROL_PERIOD);	
}

void SENSORSCONTROL_vfnTask(void)
{
	
	if(SwTimers_bfnGetStatus(SENSORSCONTROL_CHN))
	{
		/* Run every infrared period in ms to each state machine  */
		(*apFnISUpSM[sSensorsControlSM.bUpCurrentState])();		
		(*apFnISDownSM[sSensorsControlSM.bDownCurrentState])();	
		
		if(!SENSORSCONTROL_READ_PRESENCE_PIN)	
		{			
			GPIO_WRITE_PIN(C,17,1);		
		}
		else
		{
			//GPIO_WRITE_PIN(C,17,0);
		}
		/* Restart timer */
		SwTimers_vfnStartTimer(SENSORSCONTROL_CHN,SENSORSCONTROL_PERIOD);
	}
	
	
}


void SENSORSCONTROL_vfnUpFirstEdge(void)
{
	/* Check for the first time */
	if(!SENSORSCONTROL_READ_UP_PIN)
	{		
		/* Continue checking to confirm the edge */
		sSensorsControlSM.bUpCurrentState = eSENSORSCONTROL_SECONDEDGE;
	}
	
}
void SENSORSCONTROL_vfnUpSecondEdge(void)
{
	/* Check for the second time */
	if(!SENSORSCONTROL_READ_UP_PIN)	
	{
		/* Set event */
		SENSORSCONTROL_gbStatus |= (1<<eSENSORSCONTROL_UP_EDGE);		
	}
	else
	{
		/* Return to check for the first time */
		sSensorsControlSM.bUpCurrentState = eSENSORSCONTROL_FIRSTEDGE;
	}
		
	
}

void SENSORSCONTROL_vfnDownFirstEdge(void)
{
	/* Check for the first time */
	if(!SENSORSCONTROL_READ_DOWN_PIN)
	{		
		/* Continue checking to confirm the edge */
		sSensorsControlSM.bDownCurrentState = eSENSORSCONTROL_SECONDEDGE;
	}
		
}
void SENSORSCONTROL_vfnDownSecondEdge(void)
{	
	/* Check for the second time */
	if(!SENSORSCONTROL_READ_DOWN_PIN)	
	{
		/* Set event */
		SENSORSCONTROL_gbStatus |= (1<<eSENSORSCONTROL_DOWN_EDGE);		
	}
	else
	{
		/* Return to check for the first time */
		sSensorsControlSM.bDownCurrentState = eSENSORSCONTROL_FIRSTEDGE;
	}
}


