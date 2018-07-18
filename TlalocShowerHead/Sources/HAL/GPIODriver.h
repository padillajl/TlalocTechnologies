/*
 * GPIODriver.h
 *
 *  Created on: Oct 15, 2016
 *      Author: JoséLuis
 */

#ifndef GPIODRIVER_H_
#define GPIODRIVER_H_


/*************************************************************************************************/
/*********************						Includes						**********************/
/*************************************************************************************************/

#include "derivative.h"
#include "MyTypes.h"


/*************************************************************************************************/
/*********************						Defines							**********************/
/*************************************************************************************************/

/* Ports bases */
#define A  	PTA_BASE_PTR
#define B  	PTB_BASE_PTR
#define C  	PTC_BASE_PTR
#define D  	PTD_BASE_PTR
#define E  	PTE_BASE_PTR

#define _A  PORTA_BASE_PTR
#define _B  PORTB_BASE_PTR
#define _C  PORTC_BASE_PTR
#define _D  PORTD_BASE_PTR
#define _E  PORTE_BASE_PTR


/* USE PORT BASES FOR THIS MACROS
GPIO_CONFIG_PIN_AS_INPUT(port,pin)				
GPIO_CONFIG_PIN_AS_OUTPUT(port,pin)				
GPIO_TOGGLE_PIN(port,pin)                      		
GPIO_READ_PIN(port,pin)							
GPIO_WRITE_PIN(port,pin,set)

EXAMPLE : GPIO_WRITE_PIN(C,13,1)
*/

/* USE PORT BASES _# FOR THIS MACROS 
GPIO_CONFIG_PIN_FUNCTION(port,pin,alternative)
GPIO_CONFIG_PULL_UP(port,pin,set)
GPIO_CONFIG_PULL_DOWN(port,pin,set)

EXAMPLE : GPIO_CONFIG_PULL_DOWN(_C,13,1)
 */

#define GPIO_ENABLE_MODULE_CLOCK(Module)\
		if(Module == _I2C0)\
			SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK;\
		else if(Module == _I2C1)\
			SIM_SCGC4 |= SIM_SCGC4_I2C1_MASK;\
		else if(Module == _UART0)\
			SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;\
		else if(Module == _UART1)\
			SIM_SCGC4 |= SIM_SCGC4_UART1_MASK;\
		else if(Module == _UART2)\
			SIM_SCGC4 |= SIM_SCGC4_UART2_MASK;\
		else if(Module == _USBOTG)\
			SIM_SCGC4 |= SIM_SCGC4_USBOTG_MASK;\
		else if(Module == _CMP)\
			SIM_SCGC4 |= SIM_SCGC4_CMP_MASK;\
		else if(Module == _SPI0)\
			SIM_SCGC4 |= SIM_SCGC4_SPI0_MASK;\
		else if(Module == _SPI1)\
			SIM_SCGC4 |= SIM_SCGC4_SPI1_MASK;\
		else if(Module == _LPTMR)\
			SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;\
		else if(Module == _TSI)\
			SIM_SCGC5 |= SIM_SCGC5_TSI_MASK;\
		else if(Module == _PORTA)\
			SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;\
		else if(Module == _PORTB)\
			SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;\
		else if(Module == _PORTC)\
			SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;\
		else if(Module == _PORTD)\
			SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;\
		else if(Module == _PORTE)\
			SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;\
		else if(Module == _FTF)\
			SIM_SCGC6 |= SIM_SCGC6_FTF_MASK;\
		else if(Module == _DMAMUX)\
			SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;\
		else if(Module == _PIT)\
			SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;\
		else if(Module == _TPM0)\
			SIM_SCGC6 |= SIM_SCGC6_TPM0_MASK;\
		else if(Module == _TPM1)\
			SIM_SCGC6 |= SIM_SCGC6_TPM1_MASK;\
		else if(Module == _TPM2)\
			SIM_SCGC6 |= SIM_SCGC6_TPM2_MASK;\
		else if(Module == _ADC0)\
			SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;\
		else if(Module == _RTC)\
			SIM_SCGC6 |= SIM_SCGC6_RTC_MASK;\
		else if(Module == _DAC0)\
			SIM_SCGC6 |= SIM_SCGC6_DAC0_MASK;\
		else if(Module == _DMA)\
			SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;\
        
#define GPIO_CONFIG_PIN_FUNCTION(port,pin,alternative)\
	    PORT_PCR_REG(port,pin) = PORT_PCR_MUX(alternative);

/*
		if(port == _PORTA)\
			PORTA_BASE_PTR->PCR[pin] = PORT_PCR_MUX(alternative);\
		else if(port == _PORTB)\
			PORTB_BASE_PTR->PCR[pin] = PORT_PCR_MUX(alternative);\
		else if(port == _PORTC)\
			PORTC_BASE_PTR->PCR[pin] = PORT_PCR_MUX(alternative);\
		else if(port == _PORTD)\
			PORTD_BASE_PTR->PCR[pin] = PORT_PCR_MUX(alternative);\
		else if(port == _PORTE)\
			PORTE_BASE_PTR->PCR[pin] = PORT_PCR_MUX(alternative);
			*/

#define GPIO_CONFIG_PULL_UP(port,pin,set) \
		if(set == 1) \
		{ \
			PORT_PCR_REG(port,pin) |= PORT_PCR_PE_MASK; \
			PORT_PCR_REG(port,pin) |= PORT_PCR_PS_MASK; \
		} \
		else  \
			PORT_PCR_REG(port,pin) &= ~PORT_PCR_PE_MASK;\
		
				
#define GPIO_CONFIG_PULL_DOWN(port,pin,set) \
		if(set == 1) \
		{ \
			PORT_PCR_REG(port,pin) |= PORT_PCR_PE_MASK; \
			PORT_PCR_REG(port,pin) &= ~PORT_PCR_PS_MASK; \
		} \
		else \
		    PORT_PCR_REG(port,pin) &= ~PORT_PCR_PE_MASK;\
		

#define GPIO_CONFIG_PIN_INTERRUPT(port,pin,interruptconfig)\
		PORT_PCR_REG(port,pin) |= PORT_PCR_IRQC(interruptconfig);\

#define GPIO_CHECK_PIN_INTERRUPT_FLAG(port,pin)  (PORT_PCR_REG(port,pin) & PORT_PCR_ISF_MASK)

#define GPIO_CLEAR_PIN_INTERRUPT_FLAG(port,pin)  (PORT_PCR_REG(port,pin) |= PORT_PCR_ISF_MASK)



#define GPIO_CONFIG_PIN_AS_INPUT(port,pin)				(GPIO_PDDR_REG(port) &= ~(1<<pin))
#define GPIO_CONFIG_PIN_AS_OUTPUT(port,pin)				(GPIO_PDDR_REG(port) |= (1<<pin))
#define GPIO_TOGGLE_PIN(port,pin)                       (GPIO_PTOR_REG(port) |= (1<<pin))		
#define GPIO_READ_PIN(port,pin)							(GPIO_PDIR_REG(port) & (1<<pin))

#define GPIO_WRITE_PIN(port,pin,set)\
		if(set == 1)\
			GPIO_PDOR_REG(port) |= (1<<pin);\
		else\
			GPIO_PDOR_REG(port) &= ~(1<<pin);\
		

/* LEDS CONFIGURATION */

#define GPIO_RED_LED_CONFIG								GPIO_ENABLE_MODULE_CLOCK(_PORTB);\
														GPIO_CONFIG_PIN_FUNCTION(_B,18,1);\
														GPIO_CONFIG_PIN_AS_OUTPUT(B,18)

#define GPIO_GREEN_LED_CONFIG							GPIO_ENABLE_MODULE_CLOCK(_PORTB);\
														GPIO_CONFIG_PIN_FUNCTION(_B,19,1);\
														GPIO_CONFIG_PIN_AS_OUTPUT(B,19)

#define GPIO_BLUE_LED_CONFIG							GPIO_ENABLE_MODULE_CLOCK(_PORTD);\
														GPIO_CONFIG_PIN_FUNCTION(_D,1,1);\
														GPIO_CONFIG_PIN_AS_OUTPUT(D,1)

/* LEDS CONTROL */

#define GPIO_TURN_ON_RED_LED							GPIO_WRITE_PIN(B,18,0);\
														GPIO_WRITE_PIN(B,19,1);\
														GPIO_WRITE_PIN(D,1,1)

#define GPIO_TURN_ON_GREEN_LED							GPIO_WRITE_PIN(B,18,1);\
														GPIO_WRITE_PIN(B,19,0);\
														GPIO_WRITE_PIN(D,1,1)

#define GPIO_TURN_ON_BLUE_LED							GPIO_WRITE_PIN(B,18,1);\
														GPIO_WRITE_PIN(B,19,1);\
														GPIO_WRITE_PIN(D,1,0)
			

				
/*************************************************************************************************/
/*********************						Typedefs						**********************/
/*************************************************************************************************/
typedef enum
{
	_I2C0=0,
	_I2C1,
	_UART0,
	_UART1,
	_UART2,
	_USBOTG,
	_CMP,
	_SPI0,
	_SPI1,
	_LPTMR,
	_TSI,
	_PORTA,
	_PORTB,
	_PORTC,
	_PORTD,
	_PORTE,
	_FTF,
	_DMAMUX,
	_PIT,
	_TPM0,
	_TPM1,
	_TPM2,
	_ADC0,
	_RTC,
	_DAC0,
	_DMA,
	
}GPIO_ModuleClock;
/*************************************************************************************************/
/*********************			 	 Functions Like Macros					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Extern Variables					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Extern Constants					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Function Prototypes					**********************/
/*************************************************************************************************/

/*************************************************************************************************/

#endif /* GPIODRIVER_H_ */
