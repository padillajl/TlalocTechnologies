/*
 * MainDriver.c
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
#include "MainDriver.h"


/*************************************************************************************************/
/*********************						Defines							**********************/
/*************************************************************************************************/
#define MAINDRIVER_MAX_MESSAGES				(6)
#define MAINDRIVER_MESSAGES_SIZE			(16)

/*************************************************************************************************/
/*********************						Typedefs						**********************/
/*************************************************************************************************/
typedef enum
{
	MainDriverShowInitScreenState,
	MainDriverShowMainScreenState,
	
}__MainDriverStates_;

/*************************************************************************************************/
/*********************					Function Prototypes					**********************/
/*************************************************************************************************/
void vfnMainDriverShowInitScreenState(void);
void vfnMainDriverShowMainScreenState(void);

void (* const vfnptrMainDriverStates[])(void)=
{
	vfnMainDriverShowInitScreenState,
	vfnMainDriverShowMainScreenState,
};

void vfnMainDriverDisplaySetMessages(u08 *lbRow0, u08 *lbRow1);
/*************************************************************************************************/
/*********************					Static Variables					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Global Variables					**********************/
/*************************************************************************************************/
SSM sSMMainDriverStateMachine;
/*************************************************************************************************/
/*********************					Static Constants					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Global Constants					**********************/
/*************************************************************************************************/
const u08 baAMainDriverGeneralMessages[MAINDRIVER_MAX_MESSAGES][1][MAINDRIVER_MESSAGES_SIZE]=
{
	{{"     Tlaloc     "}},		//00
	{{"  Technologies  "}},		//01
	{{"LTS:            "}},		//02
	{{"TEMP:           "}},		//03
	{{"    Valve ON    "}},		//04
	{{"    Valve OFF   "}}		//05
};

typedef enum
{
	Message_Tlaloc, 			//00
	Message_Technologies,		//01
	Message_Liters,				//02
	Message_Temperature,		//03
	Message_ValveOn,			//04
	Message_ValveOff,			//04
}__MainDriverGeneralMessages_;

/*************************************************************************************************/
/*********************						Functions						**********************/
/*************************************************************************************************/
/*************************************************************************************************/
void vfnMainDriverInit(void)
{
	sSMMainDriverStateMachine.bActualState = MainDriverShowInitScreenState;
}
void vfnMainDriverManager(void)
{
	
	void (* lvfnptrMainDriverState)(void);	
	
	if( sSMMainDriverStateMachine.bActualState>=(sizeof(vfnptrMainDriverStates)/sizeof(vfnptrMainDriverStates[0])) )
		sSMMainDriverStateMachine.bActualState = MainDriverShowMainScreenState;
		
	lvfnptrMainDriverState = vfnptrMainDriverStates[sSMMainDriverStateMachine.bActualState];	
	lvfnptrMainDriverState();
	
	
	//Check if a ON event has occurred			
	if(bSensorsDriverEventStatus & (1<<SensorsDriverPinOnEventFlag))
	{
		GPIO_TURN_ON_GREEN_LED;						
		GPIO_WRITE_PIN(C,17,0);
		bSensorsDriverEventStatus &= ~(1<<SensorsDriverPinOnEventFlag);
	}
	
	if(bSensorsDriverEventStatus & (1<<SensorsDriverPinOffEventFlag))
	{
		GPIO_TURN_ON_BLUE_LED;
		GPIO_WRITE_PIN(C,17,1);
		bSensorsDriverEventStatus &= ~(1<<SensorsDriverPinOffEventFlag);			
	}		
	
	if(bSensorsDriverTimerStatus & (1<<SensorsDriverADCTimerStatusFlag))
		bSensorsDriverTimerStatus &= ~(1<<SensorsDriverADCTimerStatusFlag);		
	else if(bSensorsDriverTimerStatus & (1<<SensorsDriverFlowSensorTimerStatusFlag))
	{
		u32 ldwTempVar;			
		ldwTempVar = (dwSensorsDriverPulseCounter * 1000)/SENSORSDRIVER_PULSESPERLITER;
		vfnBin32ToASCIIBCD(dwSensorsDriverPulseCounter,&baLCDBuffer[0],10,0);
		bSensorsDriverTimerStatus &= ~(1<<SensorsDriverFlowSensorTimerStatusFlag);
	}
}
void vfnMainDriverShowInitScreenState (void)
{
	vfnMainDriverDisplaySetMessages((u08*)&baAMainDriverGeneralMessages[Message_Tlaloc],
									(u08*)&baAMainDriverGeneralMessages[Message_Technologies]);
}
void vfnMainDriverShowMainScreenState (void)
{
	vfnMainDriverDisplaySetMessages((u08*)&baAMainDriverGeneralMessages[Message_Liters],
									(u08*)&baAMainDriverGeneralMessages[Message_Temperature]);

}
void vfnMainDriverDisplaySetMessages(u08 *lbRow0, u08 *lbRow1)
{
	(void)memcpy(&baLCDBuffer[0],lbRow0,16);
	(void)memcpy(&baLCDBuffer[16],lbRow1,16);
}