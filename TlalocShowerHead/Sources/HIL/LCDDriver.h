/*
 * LCD.h
 *
 *  Created on: Oct 14, 2016
 *      Author: Jos�Luis
 */

#ifndef LCDDRIVER_H_
#define LCDDRIVER_H_
/*************************************************************************************************/
/*********************						Includes						**********************/
/*************************************************************************************************/
#include "MyTypes.h"
#include "MiscFunctions.h"
/*************************************************************************************************/
/*********************						Defines							**********************/
/*************************************************************************************************/
#define LCD_BUFFER_SIZE							(32)
/*************************************************************************************************/
/*********************						Typedefs						**********************/
/*************************************************************************************************/
typedef enum
{
	LCDDriverCmdConfig,							//00
	LCDDriverCmdPrintRows						//01				
	
}__LCDDriverCommandFlags__;
/*************************************************************************************************/
/*********************			 	 Functions Like Macros					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Extern Variables					**********************/
/*************************************************************************************************/
extern u08 baLCDBuffer[LCD_BUFFER_SIZE];
extern u08 bLCDDriverCmd;
/*************************************************************************************************/
/*********************					Extern Constants					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Function Prototypes					**********************/
/*************************************************************************************************/
void vfnLCDDriverInit(void);
void vfnLCDDriver(void);
void vfnLCDDriverClearScreen(void);


/*************************************************************************************************/
#endif /* LCD_H_ */
