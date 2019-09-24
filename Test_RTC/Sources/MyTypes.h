/*
 * MyTypes.h
 *
 *  Created on: Jul 12, 2018
 *      Author: Toxic
 */

#ifndef MYTYPES_H_
#define MYTYPES_H_

#ifndef __u08__
	#define	__u08__
		typedef unsigned char u08;
#endif

#ifndef __s08__
	#define	__s08__
		typedef signed char s08;
#endif
		
#ifndef __u16__
	#define	__u16__
		typedef unsigned short u16;
#endif

#ifndef __s16__
	#define	__s16__
		typedef signed short s16;
#endif
		
#ifndef __u32__
	#define	__u32__
		typedef unsigned long u32;
#endif
		
#ifndef __s32__
	#define	__s32__
		typedef signed long s32;
#endif
		
#ifndef __u64__
	#define	__u64__
		typedef unsigned long  long u64;
#endif
		
#ifndef __SSM__
	#define __SSM__
		typedef struct
		{
			u08 bActualState;
			u08 bPrevState;
			u08 bNextState;
			u08 bErrorState;
			u08 bLastState;
		}SSM;
#endif

#endif /* MYTYPES_H_ */
