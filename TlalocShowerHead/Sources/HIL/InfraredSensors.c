/*
 * InfraredSensors.c
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
#include "InfraredSensors.h"
#include "SwTimers.h"
/*************************************************************************************************/
/*********************						Defines							**********************/
/*************************************************************************************************/
#define INFRAREDSENSORS_CONFIG_UP_PIN			GPIO_ENABLE_MODULE_CLOCK(_PORTC);\
												GPIO_CONFIG_PIN_FUNCTION(_C,12,1);\
												GPIO_CONFIG_PULL_UP(_C,12,1);\
												GPIO_CONFIG_PIN_AS_INPUT(C,12)
												

#define INFRAREDSENSORS_CONFIG_DOWN_PIN 		GPIO_CONFIG_PIN_FUNCTION(_C,13,1);\
												GPIO_CONFIG_PULL_UP(_C,13,1);\
												GPIO_CONFIG_PIN_AS_INPUT(C,13)

#define INFRAREDSENSORS_CONFIG_PRESENCE_PIN		GPIO_CONFIG_PIN_FUNCTION(_C,16,1);\
												GPIO_CONFIG_PULL_UP(_C,16,1);\
												GPIO_CONFIG_PIN_AS_INPUT(C,16)

#define INFRAREDSENSORS_CONFIG_PRESENCE_OUT		GPIO_CONFIG_PIN_FUNCTION(_C,17,1);\
												GPIO_CONFIG_PULL_UP(_C,17,1);\
												GPIO_CONFIG_PIN_AS_OUTPUT(C,17)

#define INFRAREDSENSORS_SM_NUM_FUNCTIONS		(2)

#define INFRAREDSENSORS_CHN						(1)
#define	INFRAREDSENSORS_PERIOD					(20) /* Miliseconds */





/*************************************************************************************************/
/*********************						Typedefs						**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Function Prototypes					**********************/
/*************************************************************************************************/

/* State machine declaration which is used to detect two stages of edges for each
 * Infrared sensor
 * */
void(*apFnISUpSM[INFRAREDSENSORS_SM_NUM_FUNCTIONS])(void)=
{
	InfraredSensors_vfnUpFirstEdge,
	InfraredSensors_vfnUpSecondEdge
};

void(*apFnISDownSM[INFRAREDSENSORS_SM_NUM_FUNCTIONS])(void)=
{
	InfraredSensors_vfnDownFirstEdge,
	InfraredSensors_vfnDownSecondEdge
};

/*************************************************************************************************/
/*********************					Static Variables					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Global Variables					**********************/
/*************************************************************************************************/
u08 InfraredSensors_gbStatus;
tInfraredSensorsSM sInfraredSensorsSM;

/*************************************************************************************************/
/*********************					Static Constants					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Global Constants					**********************/
/*************************************************************************************************/

void InfraredSensors_vfnInit(void)
{	/* Configuration for each pin that corresponds to each Infrared Sensor */
	INFRAREDSENSORS_CONFIG_UP_PIN;
	INFRAREDSENSORS_CONFIG_DOWN_PIN;
	INFRAREDSENSORS_CONFIG_PRESENCE_PIN;
	INFRAREDSENSORS_CONFIG_PRESENCE_OUT;
	
	/* Infrared sensors Up and Down timer init */
	SwTimers_vfnStartTimer(INFRAREDSENSORS_CHN,INFRAREDSENSORS_PERIOD);
	
}

void InfraredSensors_vfnTask(void)
{
	
	if(SwTimers_bfnGetStatus(INFRAREDSENSORS_CHN))
	{
		/* Run every infrared period in ms to each state machine  */
		(*apFnISUpSM[sInfraredSensorsSM.bUpCurrentState])();		
		(*apFnISDownSM[sInfraredSensorsSM.bDownCurrentState])();	
		
		if(!INFRAREDSENSORS_READ_PRESENCE_PIN)	
		{			
			GPIO_WRITE_PIN(C,17,1);		
		}
		else
		{
			//GPIO_WRITE_PIN(C,17,0);
		}
		/* Restart timer */
		SwTimers_vfnStartTimer(INFRAREDSENSORS_CHN,INFRAREDSENSORS_PERIOD);
	}
	
	
}


void InfraredSensors_vfnUpFirstEdge(void)
{
	/* Check for the first time */
	if(!INFRAREDSENSORS_READ_UP_PIN)
	{		
		/* Continue checking to confirm the edge */
		sInfraredSensorsSM.bUpCurrentState = eINFRAREDSENSORS_SECONDEDGE;
	}
	
}
void InfraredSensors_vfnUpSecondEdge(void)
{
	/* Check for the second time */
	if(!INFRAREDSENSORS_READ_UP_PIN)	
	{
		/* Set event */
		InfraredSensors_gbStatus |= (1<<eINFRAREDSENSORS_UP_EDGE);		
	}
	else
	{
		/* Return to check for the first time */
		sInfraredSensorsSM.bUpCurrentState = eINFRAREDSENSORS_FIRSTEDGE;
	}
		
	
}

void InfraredSensors_vfnDownFirstEdge(void)
{
	/* Check for the first time */
	if(!INFRAREDSENSORS_READ_DOWN_PIN)
	{		
		/* Continue checking to confirm the edge */
		sInfraredSensorsSM.bDownCurrentState = eINFRAREDSENSORS_SECONDEDGE;
	}
		
}
void InfraredSensors_vfnDownSecondEdge(void)
{	
	/* Check for the second time */
	if(!INFRAREDSENSORS_READ_DOWN_PIN)	
	{
		/* Set event */
		InfraredSensors_gbStatus |= (1<<eINFRAREDSENSORS_DOWN_EDGE);		
	}
	else
	{
		/* Return to check for the first time */
		sInfraredSensorsSM.bDownCurrentState = eINFRAREDSENSORS_FIRSTEDGE;
	}
}


