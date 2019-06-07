/*
 * MyTypes.h
 *
 *  Created on: Sep 22, 2017
 *      Author: Toxic
 */

#ifndef _MY_TYPES_H_
#define _MY_TYPES_H_

typedef unsigned long u32;
typedef unsigned short u16;
typedef unsigned char u08;
typedef signed long s32;
typedef signed short s16;
typedef signed char s08;

typedef union
{
	u32 dwData;
	u16 waData[2];
	u08 baData[4];
}t32Bits;

#endif /* MY_TYPES_H_ */
