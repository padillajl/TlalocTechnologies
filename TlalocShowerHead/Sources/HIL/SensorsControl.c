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
#define ADC_CHN							(0)
#define ADC_PERIOD						(1000)	

#define SC_FLOW_SENSOR_CHN				(3)
#define	SC_FLOW_SENSOR_PERIOD			(1000)

#define SC_FLOW_SENSOR_CONFIG 			GPIO_ENABLE_MODULE_CLOCK(_PORTD);\
										NVIC_ISER |= (1<<31);\
										GPIO_CONFIG_PIN_FUNCTION(_D,4,1);\
										GPIO_CONFIG_PIN_AS_INPUT(D,4);\
										GPIO_CONFIG_PIN_INTERRUPT(_D,4,0x9)

#define SC_TRIGGER_PIN_CONFIG 	    	GPIO_CONFIG_PIN_FUNCTION(_C,17,1);\
										GPIO_CONFIG_PIN_AS_OUTPUT(C,17);\
										GPIO_WRITE_PIN(C,17,1)
											
											
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
volatile u08 					 gbSCADCTimerStatus = 0;
u08						         gbSCADCData;
volatile u08					 gbSCFlowSensorStatus=0;
u16							 dwPulseCounter;
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
	
	SC_FLOW_SENSOR_CONFIG;	
	SC_TRIGGER_PIN_CONFIG;
	
	ADC_vfnDriverInit ();
	/* RED_LED timer init */
	SwTimers_vfnStartTimer(ADC_CHN,ADC_PERIOD);
	
	SwTimers_vfnStartTimer(SC_FLOW_SENSOR_CHN,SC_FLOW_SENSOR_PERIOD);
	
	
}
void SensorsControl_vfnTask(void)
{		
	/* Check the timer for the Red Led Channel */
	if(SwTimers_bfnGetStatus(ADC_CHN))
	{
		gbSCADCTimerStatus = 1;		
		gbSCADCData = ADC_bfnStartConversion(ADC_CHANNEL_0);
		SwTimers_vfnStartTimer(ADC_CHN,ADC_PERIOD);
	}
	
	if(SwTimers_bfnGetStatus(SC_FLOW_SENSOR_CHN))
	{
		gbSCFlowSensorStatus = 1;				
		SwTimers_vfnStartTimer(SC_FLOW_SENSOR_CHN,SC_FLOW_SENSOR_PERIOD);
	}
	
}

void PORTD_IRQHandler(void)
{	
	
	asm("nop");
	
	if(GPIO_CHECK_PIN_INTERRUPT_FLAG(_D,4))
	{
		dwPulseCounter++;
		GPIO_CLEAR_PIN_INTERRUPT_FLAG(_D,4);
	}
	
}
