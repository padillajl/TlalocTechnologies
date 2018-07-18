/*
 * ADC.h
 *
 *  Created on: Aug 16, 2016
 *      Author: Dell
 */

#ifndef ADC_H_
#define ADC_H_

#include "MyTypes.h"

#define         ADC_NUMBER_OF_FUNCTIONS     		 2
#define         ADC_CHANNEL_0              			0x0
#define 		ADC_CHANNEL_3 						0x3
#define         CONVERSION_MODE          			0   
#define			ADC_INPUT_PIN			 			(1<<20)	



typedef enum{		
	
	eSTATE_STARTCONVERSION,		/*0*/	
	eSTATE_CHECKSTATUS,		/*1*/
	eSTATE_POTSTATUS,	/*2*/	
	
}teStates;



void ADC_vfnDriverInit(void);
u08 ADC_vfnGetStatus(void);
void ADC_vfnStartConversion(u08 bADCChannel);
u16  ADC_bfnGetData(void);
u16 ADC_bfnStartConversion(u08 bADCChannel);

#endif /* ADC_H_ */
