/*
 * MiscFunctions.h
 *
 *  Created on: Jul 12, 2018
 *      Author: Toxic
 */

#ifndef MISCFUNCTIONS_H_
#define MISCFUNCTIONS_H_
/*************************************************************************************************/
/*********************						Includes						**********************/
/*************************************************************************************************/
#include "MyTypes.h"
#include <string.h>
/*************************************************************************************************/
/*********************						Defines							**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************						Typedefs						**********************/
/*************************************************************************************************/
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

u08 bfnBCDToASCII(u08 lbNibble);
void vfnBin8ToBCD(u08 lbBin8);
void vfnBin16ToBCD(u16 lwBin16);
void vfnBin32ToBCD(u32 ldwBin32);
void vfnBin16ToASCIIBCD(u16 lwValue, u08* lbptrBuffer, u08 lbCopySize, u08 lbBCDInit);
void vfnBin32ToASCIIBCD(u32 ldwValue, u08* lbptrBuffer, u08 lbCopySize, u08 lbBCDInit);
void vfnBin32ToASCIIBCDWithDecimalDot(u32 ldwValue, u08* lbptrBuffer, u08 lbCopySize, u08 lbBCDInit,u08 lbDecimalSize);
void vfnInvertString(u08 *lbpSource,u08 lbSize);	
u08 bfnAsciiZeroSupressAndShiftToLeft(u08* lbptrBuffer, u08 lbSize, u08 lbZerosToDelete);

/*************************************************************************************************/


#endif /* MISCFUNCTIONS_H_ */
