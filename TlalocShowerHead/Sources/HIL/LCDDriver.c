/*
 * LCDDriver.c
 *
 *  Created on: Oct 14, 2016
 *      Author: Jos�Luis
 */

/*************************************************************************************************/
/*********************						Includes						**********************/
/*************************************************************************************************/
/* System includes */
#include "derivative.h"
/* Includes used in this file */

/* Own includes */
#include "LCDDriver.h"
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


#define LCD_PERIOD								(1) //Timer in miliseconds of de LCD

#define LCD_REFRESH_DISPLAY_CHN					(4) 	//Chanel used for refresh the display
#define LCD_REFRESH_DISPLAY_PERIOD				(3000) 	//3 seconds timer for refresh


/* LCD INSTRUCTION DEFINES */
#define LCD_CLEAR_DISPLAY						(0x01)	//00000001
#define LCD_DISPLAY_ONOFF						(0x0E) 	//00001110 Display On, Cursor underline on
#define LCD_CONFIG_MODE							(0x38)	//00111000 8 bit interface, 2 line mode, 5x7 dot format
#define LCD_ENTRY_MODE							(0x06)	//00000110 Incremental Mod

/* Address to print in second line or first line */ 
#define LCD_FIRST_LINE_BASE_ADDRESS				(0x80)
#define LCD_SECOND_LINE_BASE_ADDRESS			(0xC0)

#define LCD_NUMBER_OF_FUNCTIONS					(9)		

/* LCD COMMAND DEFINES */

/* RS LOW RW LOW */
#define LCD_RS_L_RW_L							(0x0)
/* RS LOW RW HIGH */
#define LCD_RS_L_RW_H							(0x1)
/* RS HIGH RW LOW */
#define LCD_RS_H_RW_L							(0x2)
/* RS HIGH RW HIGH */
#define LCD_RS_H_RW_H							(0x3)

#define LCD_ASCII_NUM_OFFSET					(0x30)

/*************************************************************************************************/
/*********************						Typedefs						**********************/
/*************************************************************************************************/
typedef enum
{
	LCDDriverIdleState,							//00
	
	LCDDriverConfig0State,						//01
	LCDDriverConfig1State,						//02
	LCDDriverConfig2State,						//03
	LCDDriverConfig3State,						//04	
	
	LCDDriverSetRow1AddressState,				//05
	LCDDriverPrintRow1State,					//06
	LCDDriverSetRow2AddressState,				//07
	LCDDriverPrintRow2State						//08
	
}__LCDDriverStateMachineStates__;

typedef enum
{
	LCDDriverStateMachineBusy,					//00
}__LCDDriverStateMachineStatus;
/*************************************************************************************************/
/*********************					Function Prototypes					**********************/
/*************************************************************************************************/
void vfnLCDDriverByteAssign(u08 lbCommand, u08 lbData);
void vfnLCDDriverSetTriggerEnableAndChangeState(u08 lbActualState);


void vfnLCDDriverIdleState(void);				//00

void vfnLCDDriverConfig0State(void); 			//01
void vfnLCDDriverConfig1State(void);			//02
void vfnLCDDriverConfig2State(void);			//03
void vfnLCDDriverConfig3State(void);			//04

void vfnLCDDriverSetRow1AddressState (void);	//05
void vfnLCDDriverPrintRow1State(void);			//06
void vfnLCDDriverSetRow2AddressState (void);	//07
void vfnLCDDriverPrintRow2State(void);			//08

/* States of LCD */
void(* const vfnaLCDDriverStateMachineStates[LCD_NUMBER_OF_FUNCTIONS])(void)={
		
	vfnLCDDriverIdleState,						//00
	//Config LCD
	vfnLCDDriverConfig0State,					//01
	vfnLCDDriverConfig1State,					//02
	vfnLCDDriverConfig2State,					//03
	vfnLCDDriverConfig3State,					//04	
	//Print LCD States
	vfnLCDDriverSetRow1AddressState,			//05
	vfnLCDDriverPrintRow1State,					//06
	vfnLCDDriverSetRow2AddressState,			//07
	vfnLCDDriverPrintRow2State					//08
};

/*************************************************************************************************/
/*********************					Static Variables					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Global Variables					**********************/
/*************************************************************************************************/
SSM sSMLCDDriverStateMachine;
u08 bLCDDriverStateMachineStatus;
u08 bLCDDriverCmd;
u08 baLCDBuffer[LCD_BUFFER_SIZE	];
u08 bLCDCharCounter;

u08 bLCDRefreshDisplayFlag;
/*************************************************************************************************/
/*********************					Static Constants					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Global Constants					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************						Functions						**********************/
/*************************************************************************************************/
/*************************************************************************************************/
/*!
	\fn		vfnLCDDriverInit
	\brief	Function that initialize the pin out configuration, and start the timers for LCD operations	 	 	
	\param	none
	\return	none
*/
/*************************************************************************************************/
void vfnLCDDriverInit(void)
{		
	bLCDCharCounter = 0;
	bLCDRefreshDisplayFlag = 0;
	
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
	
	//Start LCD configuration
	bLCDDriverCmd |= (1 << LCDDriverCmdConfig);
	
	//Request LCD Timer
	baSwTimersCurrentTimers[SwTimersDriverLCDTimer]= SwTimers_bfnRequestTimer();
	// LCD operations timer init
	SwTimers_vfnStartTimer(baSwTimersCurrentTimers[SwTimersDriverLCDTimer],LCD_PERIOD);
}

/*************************************************************************************************/
/*!
	\fn		vfnLCDDriver
	\brief	Function that drives the operations of the LCD Module	 	 	
	\param	none
	\return	none
*/
/*************************************************************************************************/
void vfnLCDDriver(void)
{
	//Pointer to the actual state of the state machine
	void (*lvptrRunStateMachine)(void);	
	
	//Check if state machine is free
	if( !(bLCDDriverStateMachineStatus) & (1 << LCDDriverStateMachineBusy))
	{
		//Display configuration
		if(bLCDDriverCmd & (1 << LCDDriverCmdConfig))
		{
			sSMLCDDriverStateMachine.bActualState = LCDDriverConfig0State;				
			bLCDDriverStateMachineStatus |= (1 << LCDDriverStateMachineBusy);
		}		
		//Display print rows
		else if(bLCDDriverCmd & (1 << LCDDriverCmdPrintRows))
		{
			sSMLCDDriverStateMachine.bActualState = LCDDriverSetRow1AddressState;
			bLCDDriverStateMachineStatus |= (1 << LCDDriverStateMachineBusy);
		}
	}
	
	//Validate that the actual state is in range of the state machine states
	if(sSMLCDDriverStateMachine.bActualState >= (sizeof(vfnaLCDDriverStateMachineStates)/sizeof(vfnaLCDDriverStateMachineStates[0])))
		sSMLCDDriverStateMachine.bActualState = LCDDriverIdleState;
	
	//Pointer to the current state
	lvptrRunStateMachine = vfnaLCDDriverStateMachineStates[sSMLCDDriverStateMachine.bActualState];
	// Run LCD state machine every 1 ms*/
	if(SwTimers_bfnGetStatus(baSwTimersCurrentTimers[SwTimersDriverLCDTimer]))
	{	
		lvptrRunStateMachine();		
		SwTimers_vfnStartTimer(baSwTimersCurrentTimers[SwTimersDriverLCDTimer],LCD_PERIOD);
	}		
		
}

/*************************************************************************************************/
/*!
	\fn		vfnLCDDriverIdleState
	\brief	Idle state of the state machine where the flags are released	 	 	
	\param	none
	\return	none
*/
/*************************************************************************************************/
void vfnLCDDriverIdleState(void)
{	
	//Restart char counter
	bLCDCharCounter = 0;
	
	bLCDDriverStateMachineStatus &= ~( 1 << LCDDriverStateMachineBusy);
}

/*************************************************************************************************/
/*!
	\fn		vfnLCDDriverConfig0State
	\brief	State that clears the display	 	 	
	\param	none
	\return	none
*/
/*************************************************************************************************/
void vfnLCDDriverConfig0State(void)
{
	//Clear display configuration
	vfnLCDDriverByteAssign(LCD_RS_L_RW_L,LCD_CLEAR_DISPLAY);

	vfnLCDDriverSetTriggerEnableAndChangeState(LCDDriverConfig1State);
}

/*************************************************************************************************/
/*!
	\fn		vfnLCDDriverConfig1State
	\brief	State that turns the display on and set the cursor underlined	 	 	
	\param	none
	\return	none
*/
/*************************************************************************************************/
void vfnLCDDriverConfig1State(void)
{
	//Display On, Cursor underline on
	vfnLCDDriverByteAssign(LCD_RS_L_RW_L,LCD_DISPLAY_ONOFF);
	
	vfnLCDDriverSetTriggerEnableAndChangeState(LCDDriverConfig2State);
}

/*************************************************************************************************/
/*!
	\fn		vfnLCDDriverConfig2State
	\brief	State that configures de LCD as 8 bit interface, 2 line mode, and 5x7 dot format	 	 	
	\param	none
	\return	none
*/
/*************************************************************************************************/
void vfnLCDDriverConfig2State(void)
{
	//8 bit interface, 2 line mode, 5x7 dot format
	vfnLCDDriverByteAssign(LCD_RS_L_RW_L,LCD_CONFIG_MODE);
	
	vfnLCDDriverSetTriggerEnableAndChangeState(LCDDriverConfig3State);	
}

/*************************************************************************************************/
/*!
	\fn		vfnLCDDriverConfig3State
	\brief	State that sets the entry mode as incremental mode	 	 	
	\param	none
	\return	none
*/
/*************************************************************************************************/
void vfnLCDDriverConfig3State(void)
{
	//The entry mode is incremental
	vfnLCDDriverByteAssign(LCD_RS_L_RW_L,LCD_ENTRY_MODE);	

	vfnLCDDriverSetTriggerEnableAndChangeState(LCDDriverIdleState);
	bLCDDriverCmd |= (1 << LCDDriverCmdPrintRows);
	bLCDDriverCmd &= ~(1 << LCDDriverCmdConfig);
}

/*************************************************************************************************/
/*!
	\fn		vfnLCDDriverSetRow1AddressState
	\brief	State that sets the initial address to print in Row 1	 	 	
	\param	none
	\return	none
*/
/*************************************************************************************************/
void vfnLCDDriverSetRow1AddressState(void)
{
	//Set the initial address of Row 1
	vfnLCDDriverByteAssign(LCD_RS_L_RW_L,LCD_FIRST_LINE_BASE_ADDRESS);
	
	vfnLCDDriverSetTriggerEnableAndChangeState(LCDDriverPrintRow1State);
}

/*************************************************************************************************/
/*!
	\fn		vfnLCDDriverPrintRow1State
	\brief	State that prints in Row 1	 	 	
	\param	none
	\return	none
*/
/*************************************************************************************************/
void vfnLCDDriverPrintRow1State(void)
{	
	//Print each character in display
	vfnLCDDriverByteAssign(LCD_RS_H_RW_L,baLCDBuffer[bLCDCharCounter]);		
	bLCDCharCounter++;
	
	/* Enable's trigger */
	LCD_ENABLE_ON;
	LCD_ENABLE_OFF;
	
	if(bLCDCharCounter >= 16)
	{
		bLCDCharCounter = 16;		
		sSMLCDDriverStateMachine.bActualState = LCDDriverSetRow2AddressState;
	}
}

/*************************************************************************************************/
/*!
	\fn		vfnLCDDriverSetRow2AddressState
	\brief	State that sets the initial address to print in Row 2	 	 	
	\param	none
	\return	none
*/
/*************************************************************************************************/
void vfnLCDDriverSetRow2AddressState(void)
{
	//Set the initial address of Row 1
	vfnLCDDriverByteAssign(LCD_RS_L_RW_L,LCD_SECOND_LINE_BASE_ADDRESS);
	
	vfnLCDDriverSetTriggerEnableAndChangeState(LCDDriverPrintRow2State);
}
/*************************************************************************************************/
/*!
	\fn		vfnLCDDriverPrintRow2State
	\brief	State that prints in Row 2
	\param	none
	\return	none
*/
/*************************************************************************************************/
void vfnLCDDriverPrintRow2State(void)
{
	//Print each character in display
	vfnLCDDriverByteAssign(LCD_RS_H_RW_L,baLCDBuffer[bLCDCharCounter]);		
	bLCDCharCounter++;
	
	/* Enable's trigger */
	LCD_ENABLE_ON;
	LCD_ENABLE_OFF;
	
	if(bLCDCharCounter >= 32)
	{		
		sSMLCDDriverStateMachine.bActualState = LCDDriverIdleState;
		bLCDDriverCmd &= ~(1 << LCDDriverCmdPrintRows);
	}

}

/*************************************************************************************************/
/*!
	\fn		vfnLCDDriverSetTriggerEnableAndChangeState
	\brief	Function that set the control trigger of the LCD and then change the state machine state
	\param	lbActualState
	\return	none
*/
/*************************************************************************************************/
void vfnLCDDriverSetTriggerEnableAndChangeState(u08 lbActualState)
{
	/* Enable's trigger */	
	LCD_ENABLE_ON;
	LCD_ENABLE_OFF;
	
	sSMLCDDriverStateMachine.bActualState = lbActualState;
}

/*************************************************************************************************/
/*!
	\fn		vfnLCDDriverByteAssign
	\brief	Function that set the control trigger of the LCD and then change the state machine state
	\param	lbCommand: Is the Instruction command of the LCD
	\param	lbData: Is the character that is going to be printed
	\return	none
*/
/*************************************************************************************************/

void vfnLCDDriverByteAssign(u08 lbCommand, u08 lbData)
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
/*************************************************************************************************/
/*!
	\fn		vfnLCDDriverClearScreen
	\brief	Function that set with spaces (' ') the LCD buffer to simulate its clear	
	\return	none
*/
/*************************************************************************************************/
void vfnLCDDriverClearScreen(void)
{
	memset(&baLCDBuffer[0],' ',sizeof(baLCDBuffer));
}
