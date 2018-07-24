/*
 * SensorsControl.c
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
#include "SensorsControl.h"
#include "SwTimers.h"
#include "GPIODriver.h"
#include "ADC.h"
/*************************************************************************************************/
/*********************						Defines							**********************/
/*************************************************************************************************/

#define SENSORSCONTROL_ADC_PERIOD								(1000) //1000 ms = 1 sec
#define	SENSORSCONTROL_FLOW_SENSOR_PERIOD						(1000) //1000 ms = 1 sec
#define	SENSORSCONTROL_IR_SENSOR_PERIOD							(20) /* Miliseconds */


//--------------------------------------------------------------------------------------
#define SENSORSCONTROL_FLOW_SENSOR_PIN_CONFIG 			GPIO_ENABLE_MODULE_CLOCK(_PORTD);\
														NVIC_ISER |= (1<<31);\
														GPIO_CONFIG_PIN_FUNCTION(_D,4,1);\
														GPIO_CONFIG_PIN_AS_INPUT(D,4);\
														GPIO_CONFIG_PIN_INTERRUPT(_D,4,0x9)
//--------------------------------------------------------------------------------------
#define SENSORSCONTROL_IR_SENSOR_ON_PIN_CONFIG			GPIO_ENABLE_MODULE_CLOCK(_PORTC);\
														GPIO_CONFIG_PIN_FUNCTION(_C,12,1);\
														GPIO_CONFIG_PULL_UP(_C,12,1);\
														GPIO_CONFIG_PIN_AS_INPUT(C,12)
												

#define SENSORSCONTROL_IR_SENSOR_OFF_PIN_CONFIG			GPIO_CONFIG_PIN_FUNCTION(_C,13,1);\
														GPIO_CONFIG_PULL_UP(_C,13,1);\
														GPIO_CONFIG_PIN_AS_INPUT(C,13)
//--------------------------------------------------------------------------------------
#define SENSORSCONTROL_ELECTROVALVE_PIN_CONFIG			GPIO_CONFIG_PIN_FUNCTION(_C,17,1);\
														GPIO_CONFIG_PULL_UP(_C,17,1);\
														GPIO_CONFIG_PIN_AS_OUTPUT(C,17)
											
											
/*************************************************************************************************/
/*********************						Typedefs						**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Function Prototypes					**********************/
/*************************************************************************************************/
// State machine declaration which is used to detect two stages of edges for each Infrared sensor
 
void(*apFnISUpSM[SENSORSCONTROL_SM_NUM_FUNCTIONS])(void)=
{
	vfnSensorsControlDriverOnFirstEdge,
	vfnSensorsControlDriverOnSecondEdge
};

void(*apFnISDownSM[SENSORSCONTROL_SM_NUM_FUNCTIONS])(void)=
{
	SENSORSCONTROL_vfnDownFirstEdge,
	SENSORSCONTROL_vfnDownSecondEdge
};
/*************************************************************************************************/
/*********************					Static Variables					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Global Variables					**********************/
/*************************************************************************************************/
u16							 		dwSensorsControlPulseCounter; 
u08									bSensorsControlTimerStatus;
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


void SensorsControl_vfnInit(void)
{	
	GPIO_RED_LED_CONFIG;
	GPIO_BLUE_LED_CONFIG;
	GPIO_GREEN_LED_CONFIG;	
	
	//Pin configuration for flow sensor
	SENSORSCONTROL_FLOW_SENSOR_PIN_CONFIG;		
	//Pin configuration for infrared sensors
	SENSORSCONTROL_IR_SENSOR_ON_PIN_CONFIG_;
	SENSORSCONTROL_IR_SENSOR_OFF_PIN_CONFIG;	
	//Pin configuration for electrovalve pin
	SENSORSCONTROL_ELECTROVALVE_PIN_CONFIG;
	
	
	ADC_vfnDriverInit ();
	
	//Request timer for ADC
	baSwTimersCurrentTimers[SwTimersDriverADCTimer]= SwTimers_bfnRequestTimer;
	//Request a timer for FlowSensor
	baSwTimersCurrentTimers[SwTimersDriverFlowSensorTimer]= SwTimers_bfnRequestTimer;
	                        
	/* RED_LED timer init */
	SwTimers_vfnStartTimer(baSwTimersCurrentTimers[SwTimersDriverADCTimer],SENSORSCONTROL_ADC_PERIOD);
	//Flow Sensor Timer Init
	SwTimers_vfnStartTimer(baSwTimersCurrentTimers[SwTimersDriverFlowSensorTimer],SENSORSCONTROL_FLOW_SENSOR_PERIOD);
	
	
}
void SensorsControl_vfnTask(void)
{		
	// Check the timer for ADC conversion 
	if(SwTimers_bfnGetStatus(baSwTimersCurrentTimers[SwTimersDriverADCTimer]))
	{
		gbSCADCTimerStatus = 1;		
		gbSCADCData = ADC_bfnStartConversion(ADC_CHANNEL_0);
		SwTimers_vfnStartTimer(baSwTimersCurrentTimers[SwTimersDriverADCTimer],SENSORSCONTROL_ADC_PERIOD);
	}	
	if(SwTimers_bfnGetStatus(baSwTimersCurrentTimers[SwTimersDriverFlowSensorTimer]))
	{
		gbSCFlowSensorStatus = 1;				
		SwTimers_vfnStartTimer(baSwTimersCurrentTimers[SwTimersDriverFlowSensorTimer],SENSORSCONTROL_FLOW_SENSOR_PERIOD);
	}	
}

void PORTD_IRQHandler(void)
{	
	//Check for a edge
	if(GPIO_CHECK_PIN_INTERRUPT_FLAG(_D,4))
	{
		dwSensorsControlPulseCounter++;
		GPIO_CLEAR_PIN_INTERRUPT_FLAG(_D,4);
	}	
}
