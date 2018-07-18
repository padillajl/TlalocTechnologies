/*
 * LCD.c
 *
 *  Created on: Oct 14, 2016
 *      Author: JoséLuis
 */

/*************************************************************************************************/
/*********************						Includes						**********************/
/*************************************************************************************************/
/* System includes */
#include "derivative.h"
/* Includes used in this file */

/* Own includes */
#include "LCD.h"
#include "GPIODriver.h"
#include "SwTimers.h"
/*************************************************************************************************/
/*********************						Defines							**********************/
/*************************************************************************************************/


#define LCD_CONFIG_D0							GPIO_ENABLE_MODULE_CLOCK(_PORTC);\
												GPIO_CONFIG_PIN_FUNCTION(_C,0,1);\
												GPIO_CONFIG_PIN_AS_OUTPUT(C,0)
												
#define LCD_CONFIG_D1							GPIO_CONFIG_PIN_FUNCTION(_C,1,1);\
												GPIO_CONFIG_PIN_AS_OUTPUT(C,1)

#define LCD_CONFIG_D2							GPIO_CONFIG_PIN_FUNCTION(_C,2,1);\
												GPIO_CONFIG_PIN_AS_OUTPUT(C,2)

#define LCD_CONFIG_D3							GPIO_CONFIG_PIN_FUNCTION(_C,3,1);\
												GPIO_CONFIG_PIN_AS_OUTPUT(C,3)

#define LCD_CONFIG_D4							GPIO_CONFIG_PIN_FUNCTION(_C,4,1);\
												GPIO_CONFIG_PIN_AS_OUTPUT(C,4)
												
#define LCD_CONFIG_D5							GPIO_CONFIG_PIN_FUNCTION(_C,5,1);\
												GPIO_CONFIG_PIN_AS_OUTPUT(C,5)
												
#define LCD_CONFIG_D6							GPIO_CONFIG_PIN_FUNCTION(_C,6,1);\
												GPIO_CONFIG_PIN_AS_OUTPUT(C,6)
												
#define LCD_CONFIG_D7							GPIO_CONFIG_PIN_FUNCTION(_C,7,1);\
												GPIO_CONFIG_PIN_AS_OUTPUT(C,7)
												
#define LCD_CONFIG_RW							GPIO_CONFIG_PIN_FUNCTION(_C,8,1);\
												GPIO_CONFIG_PIN_AS_OUTPUT(C,8)
												
#define LCD_CONFIG_RS							GPIO_CONFIG_PIN_FUNCTION(_C,9,1);\
												GPIO_CONFIG_PIN_AS_OUTPUT(C,9)
												
#define LCD_CONFIG_ENABLE						GPIO_CONFIG_PIN_FUNCTION(_C,10,1);\
												GPIO_CONFIG_PIN_AS_OUTPUT(C,10)

#define LCD_ENABLE_ON							GPIO_WRITE_PIN(C,10,1)

#define LCD_ENABLE_OFF							GPIO_WRITE_PIN(C,10,0)


#define LCD_CHN									(2) //Chanel used for the timer of LCD operations 
#define LCD_PERIOD								(1) //Timer in miliseconds of de LCD										


/* LCD INSTRUCTION DEFINES */
#define LCD_CLEAR_DISPLAY						(0x1)
#define LCD_DISPLAY_ONOFF						(0xE)
/* 8 bit mode ON 2 lines 5*8 font */
#define LCD_CONFIG_MODE							(0x38)
#define LCD_ENTRY_MODE							(0x6)

/* Macros to choose line to write */
#define LCD_FIRST_LINE_ADDRESS					(0)
#define LCD_SECOND_LINE_ADDRESS					(1)

/* Address to print in second line or first line */ 
#define LCD_FIRST_LINE_BASE_ADDRESS				(0x80)
#define LCD_SECOND_LINE_BASE_ADDRESS			(0xC0)

#define LCD_NUMBER_OF_FUNCTIONS					(5)											

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
u08 baLCDBuffer[LCD_BUFFER_SIZE	];
tLCDStateMachine sLCDSMStruct;
u08	gbResult = 0;

/* States of LCD */
void(*apFnLCDSM[LCD_NUMBER_OF_FUNCTIONS])(void)={
		LCD_vfnClearDisplay,
		LCD_vfnDisplayOnOff,
		LCD_vfnConfigMode,
		LCD_vfnEntryMode,
		LCD_vfnPrintString
};

/*************************************************************************************************/
/*********************					Static Constants					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Global Constants					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************						Functions						**********************/
/*************************************************************************************************/

void LCD_vfnInit(void)
{

	// Pin out config for 8 bits mode 	
	LCD_CONFIG_D0;
	LCD_CONFIG_D1;
	LCD_CONFIG_D2;
	LCD_CONFIG_D3;
	LCD_CONFIG_D4;
	LCD_CONFIG_D5;
	LCD_CONFIG_D6;
	LCD_CONFIG_D7;
	LCD_CONFIG_RW;
	LCD_CONFIG_RS;
	LCD_CONFIG_ENABLE;	
		
	sLCDSMStruct.bCurrentState = eLCD_CLEAR_DISPLAY;
	
	// LCD perations timer init
	SwTimers_vfnStartTimer(LCD_CHN,LCD_PERIOD);	
}

void LCD_vfnTask(void)
{
	/* Run LCD state machine every LCD Period seconds */
	if(SwTimers_bfnGetStatus(LCD_CHN))
	{
		(*apFnLCDSM[sLCDSMStruct.bCurrentState])();
		SwTimers_vfnStartTimer(LCD_CHN,LCD_PERIOD);
	}
}

void LCD_vfnClearDisplay(void)
{
	LCD_vfnByteAssign(LCD_RS_L_RW_L,LCD_CLEAR_DISPLAY);	
	
	sLCDSMStruct.bCurrentState = eLCD_DISPLAY_ONOFF;	
	
	/* Enable's trigger */
	LCD_ENABLE_ON;
	LCD_ENABLE_OFF;
}

void LCD_vfnDisplayOnOff(void)
{
	LCD_vfnByteAssign(LCD_RS_L_RW_L,LCD_DISPLAY_ONOFF);
	
	sLCDSMStruct.bCurrentState = eLCD_CONFIG_MODE;
	
	/* Enable's trigger */
	LCD_ENABLE_ON;
	LCD_ENABLE_OFF;
}

void LCD_vfnConfigMode(void)
{
	LCD_vfnByteAssign(LCD_RS_L_RW_L,LCD_CONFIG_MODE);
	
	sLCDSMStruct.bCurrentState = eLCD_ENTRY_MODE;
	
	/* Enable's trigger */
	LCD_ENABLE_ON;
	LCD_ENABLE_OFF;
}

void LCD_vfnEntryMode(void)
{
	LCD_vfnByteAssign(LCD_RS_L_RW_L,LCD_ENTRY_MODE);
	
	sLCDSMStruct.bCurrentState = eLCD_WORDS_INIT;

	/* Enable's trigger */
	LCD_ENABLE_ON;
	LCD_ENABLE_OFF;
}




void LCD_vfnSetAddress(u08 lbCommand, u08 bAddress)
{
	LCD_vfnByteAssign(lbCommand,bAddress);
	
	/* Enable's trigger */
	asm("nop");	
	LCD_ENABLE_ON;
	LCD_ENABLE_OFF;
	
	sLCDSMStruct.bCurrentState = eLCD_PRINT_STRING;
}

void LCD_vfnPrintString(void)
{
	LCD_vfnByteAssign(LCD_RS_H_RW_L,01/*pendiente*/);	
		
	/* Enable's trigger */
	LCD_ENABLE_ON;
	LCD_ENABLE_OFF;		
}

void LCD_vfnConvertDataToAscii(u08 lbData, u08 bSensor)
{ 
	u08 bTemp  = 0;
	u08 bCentenas = 0;
	u08 bDecenas = 0;
	u08 bUnidades = 0;
	
	bTemp = lbData / 100;
	bCentenas = bTemp;
	bTemp = lbData - (bTemp*100);
	bDecenas = bTemp / 10;
	bUnidades = bTemp - bDecenas * 10;	
		
}

void LCD_vfnByteAssign(u08 lbCommand, u08 lbData)
{	
	/* BIT ZERO */
	if((1<<0) & lbData)
	{
		GPIO_WRITE_PIN(C,0,1);
	}
	else
	{
		GPIO_WRITE_PIN(C,0,0);
	}
	
	/* BIT ONE */
	if((1<<1) & lbData)
	{
		GPIO_WRITE_PIN(C,1,1);
	}
	else
	{
		GPIO_WRITE_PIN(C,1,0);
	}
	
	
	/* BIT TWO */
	if((1<<2) & lbData){
		GPIO_WRITE_PIN(C,2,1);
	}
	else
	{
		GPIO_WRITE_PIN(C,2,0);
	}
	/* BIT THREE */
	if((1<<3) & lbData)
	{
		GPIO_WRITE_PIN(C,3,1);
	}
	else
	{
		GPIO_WRITE_PIN(C,3,0);
	}
	/* BIT FOUR */
	if((1<<4) & lbData)
	{
		GPIO_WRITE_PIN(C,4,1);
	}
	else
	{
		GPIO_WRITE_PIN(C,4,0);
	}
	/* BIT FIVE */
	if((1<<5) & lbData){
		GPIO_WRITE_PIN(C,5,1);
	}
	else{
		GPIO_WRITE_PIN(C,5,0);
	}
	/* BIT SIX */
	if((1<<6) & lbData)
	{
		GPIO_WRITE_PIN(C,6,1);
	}
	else{
		GPIO_WRITE_PIN(C,6,0);
	}
	
	/* BIT SEVEN */
	if((1<<7) & lbData){
		GPIO_WRITE_PIN(C,7,1);
	}
	else
	{
		GPIO_WRITE_PIN(C,7,0);
	}
	
	if((1<<0) & lbCommand)
	{
		GPIO_WRITE_PIN(C,8,1);
	}
	else
	{
		GPIO_WRITE_PIN(C,8,0);
	}
	if((1<<1) & lbCommand)
	{
		GPIO_WRITE_PIN(C,9,1);
	}
	else
	{
		GPIO_WRITE_PIN(C,9,0);
	}
}

void delay(void)
{
	u32 Counter = 4000000;
	
	while(Counter!=0)
	{
		Counter--;
	}
	
}

											
										
												
												
												
