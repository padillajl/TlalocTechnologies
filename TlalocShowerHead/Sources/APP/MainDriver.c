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
#define MAINDRIVER_MAX_MESSAGES				(7)
#define MAINDRIVER_MESSAGES_SIZE			(16)

/*************************************************************************************************/
/*********************						Typedefs						**********************/
/*************************************************************************************************/
typedef enum
{
	MainDriverShowInitScreenState,
	MainDriverShowMainScreenState,
	MainDriverElectrovalveOnState,
	MainDriverElectrovalveOffState,
	
}__MainDriverStates_;

/*************************************************************************************************/
/*********************					Function Prototypes					**********************/
/*************************************************************************************************/
void vfnMainDriverShowInitScreenState(void);
void vfnMainDriverShowMainScreenState(void);
void vfnMainDriverElectrovalveOnState(void);
void vfnMainDriverElectrovalveOffState(void);

void (* const vfnptrMainDriverStates[])(void)=
{
	vfnMainDriverShowInitScreenState,
	vfnMainDriverShowMainScreenState,
	vfnMainDriverElectrovalveOnState,
	vfnMainDriverElectrovalveOffState
};

void vfnMainDriverDisplaySetMessages(u08 *lbRow0, u08 *lbRow1);
void vfnMainDriverRefreshMenuTimer(u08 lbTimerValue);
void vfnMainDriverValidateMenuTimer(void);
/*************************************************************************************************/
/*********************					Static Variables					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Global Variables					**********************/
/*************************************************************************************************/
SSM sSMMainDriverStateMachine;
u08 bMainDriverMenuTimeOutTimer;
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
	{{"    Valve OFF   "}},		//05
	{{"                "}}		//06
};

typedef enum
{
	Message_Tlaloc, 			//00
	Message_Technologies,		//01
	Message_Liters,				//02
	Message_Temperature,		//03
	Message_ValveOn,			//04
	Message_ValveOff,			//05
	Message_Blank				//06
}__MainDriverGeneralMessages_;

/*************************************************************************************************/
/*********************						Functions						**********************/
/*************************************************************************************************/
/*************************************************************************************************/
void vfnMainDriverInit(void)
{
	sSMMainDriverStateMachine.bActualState = MainDriverShowInitScreenState;
	vfnMainDriverRefreshMenuTimer(3);
}
void vfnMainDriverManager(void)
{
	
	void (* lvfnptrMainDriverState)(void);	
	
	if( sSMMainDriverStateMachine.bActualState>=(sizeof(vfnptrMainDriverStates)/sizeof(vfnptrMainDriverStates[0])) )
		sSMMainDriverStateMachine.bActualState = MainDriverShowMainScreenState;
		
	lvfnptrMainDriverState = vfnptrMainDriverStates[sSMMainDriverStateMachine.bActualState];	
	lvfnptrMainDriverState();
	
	//Check if LCD has been configured and finished of printing
	if(!(bLCDDriverCmd & (1 << LCDDriverCmdPrintRows)) && !(bLCDDriverCmd & (1 << LCDDriverCmdPrintRows)))
		bLCDDriverCmd |= (1 << LCDDriverCmdPrintRows);
	
	//Check if a IR event has occurred			
	if(bSensorsDriverEventStatus & (1<<SensorsDriverIRSensorEventStatusFlag))
	{
		//if electrovalve is off, turn on
		if(!(bSensorsDriverEventStatus & (1<<SensorsDriverPinOnOffEventStatusFlag)))
		{
			//Turn on the electrovalve
			GPIO_WRITE_PIN(C,17,1);
			GPIO_TURN_ON_GREEN_LED;			
			bSensorsDriverEventStatus |= (1<<SensorsDriverPinOnOffEventStatusFlag);
			sSMMainDriverStateMachine.bActualState = MainDriverElectrovalveOnState;
					
		}
		else
		{
			//Turn off the electrovalve
			GPIO_WRITE_PIN(C,17,0);			
			GPIO_TURN_ON_BLUE_LED;						
			bSensorsDriverEventStatus &= ~(1<<SensorsDriverPinOnOffEventStatusFlag);
			sSMMainDriverStateMachine.bActualState = MainDriverElectrovalveOffState;
		}
		vfnMainDriverRefreshMenuTimer(2);
		bSensorsDriverEventStatus &= ~(1<<SensorsDriverIRSensorEventStatusFlag);
	}		
	
	if(bSensorsDriverTimerStatus & (1<<SensorsDriverADCTimerStatusFlag))
		bSensorsDriverTimerStatus &= ~(1<<SensorsDriverADCTimerStatusFlag);		
	if(bSensorsDriverTimerStatus & (1<<SensorsDriverFlowSensorTimerStatusFlag))
	{
		u32 ldwTempVar;			
		ldwTempVar = (dwSensorsDriverPulseCounter * 1000)/SENSORSDRIVER_PULSESPERLITER;		
		bSensorsDriverTimerStatus &= ~(1<<SensorsDriverFlowSensorTimerStatusFlag);
	}
}
void vfnMainDriverShowInitScreenState (void)
{
	vfnMainDriverDisplaySetMessages((u08*)&baAMainDriverGeneralMessages[Message_Tlaloc],
									(u08*)&baAMainDriverGeneralMessages[Message_Technologies]);
	vfnMainDriverValidateMenuTimer();
}
void vfnMainDriverShowMainScreenState (void)
{
	u08 lbaPulsesCounterBuffer[5];
	u08 lbaBufferSizeWithOutZeros;
	
	vfnLCDDriverClearScreen();
	
	//Print the pulses on screen
	vfnBin32ToASCIIBCD(dwSensorsDriverPulseCounter,&lbaPulsesCounterBuffer[0],5,0);
	//Delete zeros and reorder string
	lbaBufferSizeWithOutZeros = bfnAsciiZeroSupressAndShiftToLeft(&lbaPulsesCounterBuffer[0],5,3);
	//Copy new buffer information to the LCD buffer
	memcpy(&baLCDBuffer[5],&lbaPulsesCounterBuffer[0],lbaBufferSizeWithOutZeros);
	
	memcpy(&baLCDBuffer[0],&baAMainDriverGeneralMessages[Message_Liters],4);
	memcpy(&baLCDBuffer[16],&baAMainDriverGeneralMessages[Message_Temperature],5);	
}
void vfnMainDriverElectrovalveOnState(void)
{
	vfnMainDriverDisplaySetMessages((u08*)&baAMainDriverGeneralMessages[Message_ValveOn],
									(u08*)&baAMainDriverGeneralMessages[Message_Blank]);
	vfnMainDriverValidateMenuTimer();
}
void vfnMainDriverElectrovalveOffState(void)
{
	vfnMainDriverDisplaySetMessages((u08*)&baAMainDriverGeneralMessages[Message_ValveOff],
									(u08*)&baAMainDriverGeneralMessages[Message_Blank]);
	vfnMainDriverValidateMenuTimer();
	
}

void vfnMainDriverDisplaySetMessages(u08 *lbRow0, u08 *lbRow1)
{
	(void)memcpy(&baLCDBuffer[0],lbRow0,16);
	(void)memcpy(&baLCDBuffer[16],lbRow1,16);
}
void vfnMainDriverRefreshMenuTimer(u08 lbTimerValue)
{
	bMainDriverMenuTimeOutTimer = lbTimerValue;
}

void vfnMainDriverValidateMenuTimer(void)
{
	if(!bMainDriverMenuTimeOutTimer)
		sSMMainDriverStateMachine.bActualState = MainDriverShowMainScreenState;	
		
}

