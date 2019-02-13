/*
 * MyTypes.h
 *
 *  Created on: Sep 10, 2014
 *      Author: RVillafuerte
 */

#include "String.h"
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

	#ifndef __TimeStamp__
		#define __TimeStamp__
			typedef struct
			{
				u08 bHour;
				u08 bMinutes;
				u08 bSeconds;
				u08 bDay;
				u08 bMonth;
				u08 bYear;
			}sTime;
	#endif
			
	#ifndef __GPSStruct__
		#define __GPSStruct__
			typedef struct
			{				
				u32 dwGPSLatitud;
				u08 bGPSIndicatorNS;
				u32 dwGPSLongitude;
				u08 bGPSIndicatorEW;
				sTime sGPSTime;
				u32 dwGPSSpeed;
				u16	wGPSCourse;
				u08 bGPSSatellites;
			}sGPS;
	#endif
			
	#ifndef __ServicePositionStruct__
		#define __ServicePositionStruct__
			typedef struct
			{				
				u32 dwGPSLatitud;
				u08 bGPSIndicatorNS;
				u32 dwGPSLongitude;
				u08 bGPSIndicatorEW;				
			}sServPos;
	#endif
			
	#ifndef __MainMeterStruct__
		#define __MainMeterStruct__
			typedef struct
			{				
				u32 dwMainMeterVolumen;
				u32 dwMainMeterMass;				
				u16 wMainMeterDensity;
				u16 wMainMeterTemperature;
				u16 wMainMeterFlow;
				u08 bMainMeterFlowDirection;				
				u08 baMainMeterVersion[4];				
			}Meter;
	#endif
			
	#ifndef __ModbusCommStruct__
		#define __ModbusCommStruct__
			typedef struct __sMB__
			{
				u08 bMBSlaveAddress;
				u08 bMBFunction;
				u16 wMBStartAddress;
				u16 wMBNoRegisters;
				u08 bMBDataSize;  
			}sMB;
	#endif
				
	#define ERROR  1	    
	#define NoERROR 0			
		
#endif /* MYTYPES_H_ */
