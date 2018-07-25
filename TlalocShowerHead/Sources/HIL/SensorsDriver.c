/*
 * SensorsDriver.c
 *
 *  Created on: Jul 24, 2017
 *      Author: Toxic
 */

/*************************************************************************************************/
/*********************						Includes						**********************/
/*************************************************************************************************/
/* System includes */
#include "derivative.h"
/* Includes used in this file */

/* Own includes */
#include "SensorsDriver.h"
#include "SwTimers.h"
#include "GPIODriver.h"
#include "ADC.h"
/*************************************************************************************************/
/*********************						Defines							**********************/
/*************************************************************************************************/

#define SENSORSDRIVER_ADC_PERIOD								(1000) //1000 ms = 1 sec
#define	SENSORSDRIVER_FLOW_SENSOR_PERIOD						(1000) //1000 ms = 1 sec
#define	SENSORSDRIVER_IR_SENSOR_PERIOD							(20) /* Miliseconds */


//--------------------------------------------------------------------------------------
#define SENSORSDRIVER_FLOW_SENSOR_PIN_CONFIG 			GPIO_ENABLE_MODULE_CLOCK(_PORTD);\
														NVIC_ISER |= (1<<31);\
														GPIO_CONFIG_PIN_FUNCTION(_D,4,1);\
														GPIO_CONFIG_PIN_AS_INPUT(D,4);\
														GPIO_CONFIG_PIN_INTERRUPT(_D,4,0x9)
//--------------------------------------------------------------------------------------
#define SENSORSDRIVER_IR_SENSOR_ON_PIN_CONFIG			GPIO_ENABLE_MODULE_CLOCK(_PORTC);\
														GPIO_CONFIG_PIN_FUNCTION(_C,12,1);\
														GPIO_CONFIG_PULL_UP(_C,12,1);\
														GPIO_CONFIG_PIN_AS_INPUT(C,12)
												

#define SENSORSDRIVER_IR_SENSOR_OFF_PIN_CONFIG			GPIO_CONFIG_PIN_FUNCTION(_C,13,1);\
														GPIO_CONFIG_PULL_UP(_C,13,1);\
														GPIO_CONFIG_PIN_AS_INPUT(C,13)
//--------------------------------------------------------------------------------------
#define SENSORSDRIVER_ELECTROVALVE_PIN_CONFIG			GPIO_CONFIG_PIN_FUNCTION(_C,17,1);\
														GPIO_CONFIG_PULL_UP(_C,17,1);\
														GPIO_CONFIG_PIN_AS_OUTPUT(C,17)

#define SENSORSDRIVER_READ_ON_PIN						GPIO_READ_PIN(C,12)	
#define SENSORSDRIVER_READ_OFF_PIN						GPIO_READ_PIN(C,13)	

											
											
/*************************************************************************************************/
/*********************						Typedefs						**********************/
/*************************************************************************************************/
typedef enum
{	
	SensorsDriverDebounceStatusFirstEdge,
	SensorsDriverDebounceStatusSecondEdge	
}__SensorsDriverDebounceStatus__;
/*************************************************************************************************/
/*********************					Function Prototypes					**********************/
/*************************************************************************************************/
void vfnSensorsDriverOnFirstEdge(void);
void vfnSensorsDriverOnSecondEdge(void);

void vfnSensorsDriverOffFirstEdge(void);
void vfnSensorsDriverOffSecondEdge(void);

// State machine declaration which is used to detect two stages of edges for each Infrared sensor
void(* const vfnaSensorsDriverOnStateMachineStates[2])(void)=
{
	vfnSensorsDriverOnFirstEdge,			//00
	vfnSensorsDriverOnSecondEdge			//01
};

void(*const vfnaSensorsDriverOffStateMachineStates[2])(void)=
{
	vfnSensorsDriverOffFirstEdge,		//00
	vfnSensorsDriverOffSecondEdge		//01
};
/*************************************************************************************************/
/*********************					Static Variables					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Global Variables					**********************/
/*************************************************************************************************/
SSM sSMSensorsDriverOnPinStateMachine;
SSM sSMSensorsDriverOffPinStateMachine;
u16							 		dwSensorsDriverPulseCounter; 
u08									bSensorsDriverTimerStatus;
u08									bSensorsDriverEventStatus;
u08						         	gbSCADCData;
volatile u08					 	gbSCFlowSensorStatus=0;

/*************************************************************************************************/
/*********************					Static Constants					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Global Constants					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************						Functions						**********************/
/*************************************************************************************************/


void vfnSensorsDriverInit(void)
{	
	GPIO_RED_LED_CONFIG;
	GPIO_BLUE_LED_CONFIG;
	GPIO_GREEN_LED_CONFIG;	
	
	//Pin configuration for flow sensor
	SENSORSDRIVER_FLOW_SENSOR_PIN_CONFIG;		
	//Pin configuration for infrared sensors
	SENSORSDRIVER_IR_SENSOR_ON_PIN_CONFIG;
	SENSORSDRIVER_IR_SENSOR_OFF_PIN_CONFIG;	
	//Pin configuration for electrovalve pin
	SENSORSDRIVER_ELECTROVALVE_PIN_CONFIG;
	
	
	ADC_vfnDriverInit ();
	
	//Request timer for ADC
	baSwTimersCurrentTimers[SwTimersDriverADCTimer]= SwTimers_bfnRequestTimer();
	//Request a timer for FlowSensor
	baSwTimersCurrentTimers[SwTimersDriverFlowSensorTimer]= SwTimers_bfnRequestTimer();
	//Request a timer for the infrared sensor debouncer
	baSwTimersCurrentTimers[SwTimersDriverInfraredSensorTimer]= SwTimers_bfnRequestTimer();

	                        
	//ADC timer init
	SwTimers_vfnStartTimer(baSwTimersCurrentTimers[SwTimersDriverADCTimer],SENSORSDRIVER_ADC_PERIOD);
	//Flow Sensor Timer Init
	SwTimers_vfnStartTimer(baSwTimersCurrentTimers[SwTimersDriverFlowSensorTimer],SENSORSDRIVER_FLOW_SENSOR_PERIOD);	
	// Infrared sensors On and Off timer init 
	SwTimers_vfnStartTimer(baSwTimersCurrentTimers[SwTimersDriverInfraredSensorTimer],SENSORSDRIVER_IR_SENSOR_PERIOD);	
	
	
}
void vfnSensorsDriver(void)
{		
	// Check the timer for ADC conversion 
	if(SwTimers_bfnGetStatus(baSwTimersCurrentTimers[SwTimersDriverADCTimer]))
	{
		bSensorsDriverTimerStatus |= SensorsDriverADCTimerStatusFlag;
		gbSCADCData = ADC_bfnStartConversion(ADC_CHANNEL_0);
		SwTimers_vfnStartTimer(baSwTimersCurrentTimers[SwTimersDriverADCTimer],SENSORSDRIVER_ADC_PERIOD);
	}	
	if(SwTimers_bfnGetStatus(baSwTimersCurrentTimers[SwTimersDriverFlowSensorTimer]))
	{
		bSensorsDriverTimerStatus |= SensorsDriverFlowSensorTimerStatusFlag;		
		SwTimers_vfnStartTimer(baSwTimersCurrentTimers[SwTimersDriverFlowSensorTimer],SENSORSDRIVER_FLOW_SENSOR_PERIOD);
	}
	if(SwTimers_bfnGetStatus(baSwTimersCurrentTimers[SwTimersDriverInfraredSensorTimer]))
	{
		// Run every infrared sensor period in ms to each state machine 
		(*vfnaSensorsDriverOnStateMachineStates[sSMSensorsDriverOnPinStateMachine.bActualState])();		
		(*vfnaSensorsDriverOffStateMachineStates[sSMSensorsDriverOnPinStateMachine.bActualState])();

		/* Restart timer */
		SwTimers_vfnStartTimer(baSwTimersCurrentTimers[SwTimersDriverInfraredSensorTimer],SENSORSDRIVER_IR_SENSOR_PERIOD);
	}
}

void PORTD_IRQHandler(void)
{	
	//Check for a edge
	if(GPIO_CHECK_PIN_INTERRUPT_FLAG(_D,4))
	{
		dwSensorsDriverPulseCounter++;
		GPIO_CLEAR_PIN_INTERRUPT_FLAG(_D,4);
	}	
}

void vfnSensorsDriverOnFirstEdge(void)
{
	/* Check for the first time */
	if(!SENSORSDRIVER_READ_ON_PIN)
	{		
		// Continue checking to confirm the edge
		sSMSensorsDriverOnPinStateMachine.bActualState = SensorsDriverDebounceStatusSecondEdge;
	}	
}
void vfnSensorsDriverOnSecondEdge(void)
{
	// Check for the second time
	if(!SENSORSDRIVER_READ_ON_PIN)	
	{
		/* Set event */
		bSensorsDriverEventStatus |= (1<<SensorsDriverPinOnEventFlag);		
	}
	else
	{
		// Return to check for the first time 
		sSMSensorsDriverOnPinStateMachine.bActualState = SensorsDriverDebounceStatusFirstEdge;
	}	
}

void vfnSensorsDriverOffFirstEdge(void)
{
	// Check for the first time 
	if(!SENSORSDRIVER_READ_OFF_PIN)
	{		
		// Continue checking to confirm the edge
		sSMSensorsDriverOffPinStateMachine.bActualState = SensorsDriverDebounceStatusSecondEdge;
	}
		
}
void vfnSensorsDriverOffSecondEdge(void)
{	
	// Check for the second time 
	if(!SENSORSDRIVER_READ_OFF_PIN)	
	{
		// Set event
		bSensorsDriverEventStatus |= (1<<SensorsDriverPinOffEventFlag);		
	}
	else
	{
		// Return to check for the first time
		sSMSensorsDriverOffPinStateMachine.bActualState = SensorsDriverDebounceStatusFirstEdge;
	}
}