/*
 * TPMA.c
 *
 *  Created on: Mar 28, 2017
 *      Author: Toxic
 */
/*************************************************************************************************/
/*********************						Includes						**********************/
/*************************************************************************************************/
/* System includes */
#include "derivative.h"

/* Includes used in this file */

/* Own includes */

#include "TPMA.h"

/*************************************************************************************************/
/*********************						Defines							**********************/
/*************************************************************************************************/
#define									NUMBER_OF_TIMERS			(2)
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
u16								gwFreqTPM0 = FREQ_TPM0CH0;
u16								gwFreqTPM1 = FREQ_TPM1CH0;
volatile u08						gbTPM0CH0Status;
volatile u08						gbTPM1CH0Status;

/*************************************************************************************************/
/*********************					Static Constants					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Global Constants					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************						Functions						**********************/
/*************************************************************************************************/


/* Search chapter 31 for information */
void TPMA_vfnInit(void)
{
#if NUMBER_OF_TIMERS == 1
	/* TPM0 clock gating */
	SIM_SCGC6	|=	SIM_SCGC6_TPM0_MASK; 
	/* Select clock source */
	SIM_SOPT2 	|= SIM_SOPT2_TPMSRC(1);
	/* Enable interrupt vector */
	NVIC_ISER 	|= 	TPM0_INTERRUPT_VECTOR;
	/* Prescaler of bus frequency */
	/* TPM counter increments on every TPM counter clock */
	TPM0_SC 	|= 	TPM_SC_PS(PRESCALER_FACTOR) | TPM_SC_CMOD(1);
	/* Set as output compare */
	TPM0_C0SC	|= 	TPM_CnSC_MSA_MASK;
	/* Channel interrupt enable */
	TPM0_C0SC	|= 	TPM_CnSC_CHIE_MASK;
	/* TPM value */
	TPM0_C0V	|= FREQ_TPM0CH0;
#endif
	
#if NUMBER_OF_TIMERS == 2
	
	/* TPM0 clock gating */
	SIM_SCGC6	|=	SIM_SCGC6_TPM0_MASK; 
	/* Select clock source */
	SIM_SOPT2 	|= SIM_SOPT2_TPMSRC(1);
	/* Enable interrupt vector */
	NVIC_ISER 	|= 	TPM0_INTERRUPT_VECTOR;
	/* Prescaler of bus frequency */
	/* TPM counter increments on every TPM counter clock */
	TPM0_SC 	|= 	TPM_SC_PS(PRESCALER_FACTOR)| TPM_SC_CMOD(1);
	/* Set as output compare */
	TPM0_C0SC	|= 	TPM_CnSC_MSA_MASK;
	/* Channel interrupt enable */
	TPM0_C0SC	|= 	TPM_CnSC_CHIE_MASK;
	/* TPM value */
	TPM0_C0V	|= FREQ_TPM0CH0;
	
	/* TPM0 clock gating */
	SIM_SCGC6	|=	SIM_SCGC6_TPM1_MASK; 
	/* Select clock source */
	SIM_SOPT2 	|= SIM_SOPT2_TPMSRC(1);
	/* Enable interrupt vector */
	NVIC_ISER 	|= 	TPM1_INTERRUPT_VECTOR;
	/* Prescaler of bus frequency */
	/* TPM counter increments on every TPM counter clock */
	TPM1_SC 	|= 	TPM_SC_PS(PRESCALER_FACTOR) | TPM_SC_CMOD(1);
	/* Set as output compare */
	TPM1_C0SC	|= 	TPM_CnSC_MSA_MASK;
	/* Channel interrupt enable */
	TPM1_C0SC	|= 	TPM_CnSC_CHIE_MASK;
	/* TPM value */
	TPM1_C0V	|= FREQ_TPM1CH0;
	
#endif 
}

void FTM0_IRQHandler(void)
{
	/* Check if an interrupt in channel 0 has ocurred */
	if(TPM0_C0SC & TPM_CnSC_CHF_MASK)
	{		
		gbTPM0CH0Status = 1;
		/* Writing 1 clear the interrupt occurred flag */
		TPM0_C0SC |= TPM_CnSC_CHF_MASK;
		
		TPM0_C0V	= TPM0_C0V + gwFreqTPM0;
	}	
}

void FTM1_IRQHandler(void)
{
	/* Check if an interrupt in channel 0 has ocurred */
	if(TPM1_C0SC & TPM_CnSC_CHF_MASK)
	{		
		gbTPM1CH0Status = 1;
		/* Writing 1 clear the interrupt occurred flag */
		TPM1_C0SC |= TPM_CnSC_CHF_MASK;
		
		TPM1_C0V	= TPM1_C0V + gwFreqTPM1;
	}	
}

u08 TPMA_vfnTPM0GetStatus(void)
{
	u08 bStatus = 0;
	if(gbTPM0CH0Status)
	{
		bStatus = 1;
		TPMA_CLEAR_TPM0CH0_STATUS;
	}
	return bStatus;
}

