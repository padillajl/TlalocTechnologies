/*
 * ADC.c
 *
 *  Created on: Aug 16, 2016
 *      Author: Dell
 */
#include <stdint.h>
#include "derivative.h"
#include "ADC.h"

volatile u08 		gbADCStatus;

void ADC_vfnDriverInit (void)
{
	/* ADC CLOCK GATING PAGE 207 RM*/
	SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK; 
	/*  CLOCK GATING FOR THE PORT TO USE AS INPUT */
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;
	
	/* PUT PTE20 AS INPUT */
	GPIOE_PDDR 		&=  ~ADC_INPUT_PIN;
	/* PUT PTE 22 AS INPUT */
	GPIOE_PDDR 		&=  ~(1<<22);
	
	/* PTE 20 BY DEFAULT IS THE INPUT WHEN WE HAVE THE CHANNEL WITH 0 AND DIFF TOO 
	 * WITH DIFF = 0 WE GOT SINGLE ENDED MODE
	 * PAGE 162, 79, 184 RM*/	
	PORTE_PCR20 	|= 		PORT_PCR_MUX(0);	
	PORTE_PCR22		|=		PORT_PCR_MUX(0);
	
	/* CONVERSION MODE SELECTION */
	/*When DIFF=0:It is single-ended 12-bit conversion; when DIFF=1, it is differential 9-bit conversion with
	2's complement output..*/
	ADC0_CFG1 	|= ADC_CFG1_MODE(CONVERSION_MODE);	
	
	/* High speed conversion */
	ADC0_CFG2	|=	ADC_CFG2_ADHSC_MASK;
	
	/* Long sample time select */
	ADC0_CFG2	|=	ADC_CFG2_ADLSTS(0x2);
		
	
	/*Con CAL_MASK se inicia la calibracion al ponerlo en set, y con esto
	 * se aborta cualquier calibracion que se este haciendo en ese momento
	 * Pag 473*/
	ADC0_SC3  |= ADC_SC3_CAL_MASK;
	
	/* SINGLE ENDED MODE */	
	ADC0_SC1A &= ~ADC_SC1_DIFF_MASK;


	/* INPUT CLOCK SELECT
	 * 00 BUS CLOCK */	
	ADC0_CFG1 &= ~ADC_CFG1_ADICLK_MASK;		
	

	#if ADC_CHANNEL != 0x0
	#error ADC Channel is not the required
	#endif	

}



void ADC_vfnStartConversion(u08 bADCChannel)
{
	ADC0_SC1A = ADC_SC1_ADCH(bADCChannel);		
}

u16  ADC_bfnGetData(void){
	/* Clear flag before read the register */
	gbADCStatus = 0;
	return ADC0_RA;
}


u08 ADC_vfnGetStatus(void)
{
	if(ADC0_SC1A & ADC_SC1_COCO_MASK)
		{
			gbADCStatus = 1;
		}	
	return gbADCStatus;	
}


u16 ADC_bfnStartConversion(u08 bADCChannel)
{
	
	ADC0_SC1A = ADC_SC1_ADCH(bADCChannel);
	
	
	while(!(ADC0_SC1A & ADC_SC1_COCO_MASK))
	{
		asm("nop");
	}
	
	return ADC0_RA;
}

