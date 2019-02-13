/*
 * MiscFunctions.h
 *
 *  Created on: Oct 20, 2014
 *      Author: Administrador
 */

#ifndef MISCFUNCTIONS_H_
	#define MISCFUNCTIONS_H_
	
	#include "MyTypes.h"
	#include <string.h>
	
	extern u08 baBCD[20];
	
	u08 bfnBCDToASCII(u08 lbNibble);
	u08 bfnASCIIToBCD(u08 lbAscii);
	void vfnBin8ToBCD (u08 lbBin8);
	void vfnBin8ToASCIIBCD(u08 lbValue, u08* lbptrBuffer, u08 lbCopySize, u08 lbBCDInit);
	void vfnBin8ToASCIIBCDWithDecimalDot(u08 lbValue, u08* lbptrBuffer, u08 lbCopySize, u08 lbBCDInit,u08 lbDecimalSize);
	void vfnBin16ToBCD(u16 lwBin16);
	void vfnBin16ToASCIIBCD(u16 lwValue, u08* lbptrBuffer, u08 lbCopySize, u08 lbBCDInit);
	void vfnBin16ToHEXASCII(u16 lwValue, u08* lbptrBuffer);
	void vfnBin32ToBCD (u32 ldwBin32);
	void vfnBin32ToASCIIBCD(u32 ldwValue, u08* lbptrBuffer, u08 lbCopySize, u08 lbBCDInit);
	void vfnBin32ToASCIIBCDWithDecimalDot(u32 ldwValue, u08* lbptrBuffer, u08 lbCopySize, u08 lbBCDInit,u08 lbDecimalSize);	
	void vfnBin64ToBCD (u64 lqwBin64);
	void vfnBin64ToASCIIBCD(u64 lqwValue, u08* lbptrBuffer, u08 lbCopySize, u08 lbBCDInit);
	void vfnBin64ToASCIIBCDWithDecimalDot(u64 lqwValue, u08* lbptrBuffer, u08 lbCopySize, u08 lbBCDInit,u08 lbDecimalSize);				
	u08 bfnASCIIBCDToBin8(u08* lbpASCII, u08* lbpDecResult, u08 lbASCIIFrameSize);
	u08 bfnASCIIBCDToBin16(u08* lbpASCII, u16* lwpDecResult, u08 lbASCIIFrameSize);
	u08 bfnASCIIBCDToBin32(u08* lbpASCII, u32* ldwpDecResult, u08 lbASCIIFrameSize);
	u08 bfnHEXASCIIBCDToBin32(u08* lbpASCII, u32* ldwpDecResult, u08 lbASCIIFrameSize);
	u08 bfnCheckParity(u08 lbData, u08 lbParity);
	void vfnInvertString(u08 *lbpSource,u08 lbSize);
	void vfnAsciiZeroSupress(u08* lbptrBuffer, u08 lbSize);
	u08 bfnAsciiZeroSupressAndShiftToLeft(u08* lbptrBuffer, u08 lbSize, u08 lbZerosToDelete);
	void vfnAsciiZeroSupressWithPrefix(u08* lbptrBuffer, u08 lbSize, u08 lbChar);
	u08* bpfnFindByte(u08 * lbpBufferStartAddress, u08 lbByteToFind, u16 lwBufferSize);
	u08 bfnLRC(u08* lbpLRCPtr,u16 lwLRCCounter);
	u08 bfnCRC(u08* lbpCRCPtr, u08 lbCRCFrameSize);
	u16 wRi505CRC16CCITT(u08 *lbpPointer,u16 lwCounter,u16 lwTruncatedPolynomial);
	u08 bfnConvertToText(u08 *lbptrPrinterBuffer);
	u08 bfnIsASCII(u08 lbAscii);
	u08 bfnIsDigitASCII(u08 lbAscii);
	u08 bfnBufferDatasAreDigitsASCII(u08 *lbptrBuffer,u08 lbSize);
	sTime strAddDaystoDate(sTime lsDate, u16 lwAddDays);
	u16 bfnModBusLRC(u08* lbpLRCMFMPtr, u08 lbLRCMFMCounter);
	void vfnDateToASCII(sTime lTime, u08* lbptrBuffer);
	u08 vfnGetPositionData(u08 lbSeed, u08 lbElement);
#endif /* MISCFUNCTIONS_H_ */
