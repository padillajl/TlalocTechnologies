/*
 * InfraredSensors.h
 *
 *  Created on: Jul 29, 2017
 *      Author: Toxic
 */

#ifndef INFRAREDSENSORS_H_
#define INFRAREDSENSORS_H_

/*************************************************************************************************/
/*********************						Includes						**********************/
/*************************************************************************************************/
#include "GPIODriver.h"
#include "MyTypes.h"
/*************************************************************************************************/
/*********************						Defines							**********************/
/*************************************************************************************************/
/* Sensors are active in low */
#define INFRAREDSENSORS_READ_UP_PIN			GPIO_READ_PIN(C,12)	
#define INFRAREDSENSORS_READ_DOWN_PIN		GPIO_READ_PIN(C,13)	
#define INFRAREDSENSORS_READ_PRESENCE_PIN	GPIO_READ_PIN(C,16)	

#define INFRAREDSENSORS_CHECK_UP_EDGE		(InfraredSensors_gbStatus & (1<<eINFRAREDSENSORS_UP_EDGE))
#define INFRAREDSENSORS_CLEAR_UP_EDGE		(InfraredSensors_gbStatus &= ~(1<<eINFRAREDSENSORS_UP_EDGE))

#define INFRAREDSENSORS_CHECK_DOWN_EDGE		(InfraredSensors_gbStatus & (1<<eINFRAREDSENSORS_DOWN_EDGE))
#define INFRAREDSENSORS_CLEAR_DOWN_EDGE		(InfraredSensors_gbStatus &= ~(1<<eINFRAREDSENSORS_DOWN_EDGE))



/*************************************************************************************************/
/*********************						Typedefs						**********************/
/*************************************************************************************************/
typedef struct
{
	u08 bUpCurrentState;
	u08 bDownCurrentState;
	u08 bPresenceCurrentState;
	
}tInfraredSensorsSM;


typedef enum
{
	eINFRAREDSENSORS_FIRSTEDGE=0,
	eINFRAREDSENSORS_SECONDEDGE
}InfraredSensors_status;

typedef enum
{
	eINFRAREDSENSORS_UP_EDGE = 1,
	eINFRAREDSENSORS_DOWN_EDGE,
	eINFRAREDSENSORS_PRESENCE_EDGE
	
}_InfraredSensors_Debounce;
/*************************************************************************************************/
/*********************			 	 Functions Like Macros					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Extern Variables					**********************/
/*************************************************************************************************/
extern u08 InfraredSensors_gbStatus;
/*************************************************************************************************/
/*********************					Extern Constants					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Function Prototypes					**********************/
/*************************************************************************************************/
void InfraredSensors_vfnInit(void);
void InfraredSensors_vfnTask(void);

void InfraredSensors_vfnUpFirstEdge(void);
void InfraredSensors_vfnUpSecondEdge(void);

void InfraredSensors_vfnDownFirstEdge(void);
void InfraredSensors_vfnDownSecondEdge(void);

void InfraredSensors_vfnPresenceFirstEdge(void);
void InfraredSensors_vfnPresenceSecondEdge(void);

/*************************************************************************************************/

#endif /* INFRAREDSENSORS_H_ */
