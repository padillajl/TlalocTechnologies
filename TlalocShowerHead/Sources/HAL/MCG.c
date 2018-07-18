/*
 * MCG.c
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
#include "MCG.h"
/*************************************************************************************************/
/*********************						Defines							**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************						Typedefs						**********************/
/*************************************************************************************************/
#define										BUS_CLOCK 					(FREQ_48MHZ)
#define										FREQ_40MHZ					(1)
#define										FREQ_48MHZ					(2)

/*************************************************************************************************/
/*********************					Function Prototypes					**********************/
/*************************************************************************************************/


/*************************************************************************************************/
/*********************					Static Variables					**********************/
/*************************************************************************************************/


/*************************************************************************************************/
/*********************					Global Variables					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Static Constants					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Global Constants					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************						Functions						**********************/
/*************************************************************************************************/

void MCG_vfnFLLInit(void)
{
#if BUS_CLOCK == FREQ_40MHZ
	/* This configuration is for 40 Mhz bus clock */
	/* Enables external reference clock */
	OSC0_CR 	|=	OSC_CR_ERCLKEN_MASK;
	
	/* FLL external reference divider */
	/* Divider factor to obtain 31.25khz is 256 with oscillator of 8 MHz */
	MCG_C1		|= 	MCG_C1_FRDIV(DIVIDE_FACTOR_256); 
	
	/* External reference clock for the FLL */	
	MCG_C1		&=	~MCG_C1_IREFS_MASK;
	
	/* High frequency range select for the crystal oscillator */
	MCG_C2		|=	MCG_C2_RANGE0(01);
	
	/* DCO frequency range 31.25Khz */
	MCG_C4		&= ~MCG_C4_DMX32_MASK;
	
	/* DCO mid range select */
	MCG_C4		|=	MCG_C4_DRST_DRS(01);
	
	/* Clock source select */
	/* Output of FLL or PLL is selected (depends on PLLS control bit).*/	
	MCG_C1		|=	MCG_C1_CLKS(00); 
#endif
	
#if BUS_CLOCK == FREQ_48MHZ
	
	/* FEE Mode */	
	/* Enables external reference clock */
	OSC0_CR 	|=	OSC_CR_ERCLKEN_MASK;
	
	/* Encoding 0 � Output of FLL or PLL is selected (depends on PLLS control bit).*/	
	MCG_C1		|=	MCG_C1_CLKS(00); 
	
	/* External reference clock for the FLL */	
	MCG_C1		&=	~MCG_C1_IREFS_MASK;	
	
	/* If Range0 is 0 , Divider is 1 with a 0 */
	MCG_C1		|= 	MCG_C1_FRDIV(0); 
	
	/* High frequency range select for the crystal oscillator */
	MCG_C2		|=	MCG_C2_RANGE0(00);
	
	/* DCO mid range select 32.768Khz * 1464 = 48Mhz*/
	MCG_C4		|=	MCG_C4_DRST_DRS(01);
	MCG_C4		|=  MCG_C4_DMX32_MASK;

	
	
#endif
	
}

