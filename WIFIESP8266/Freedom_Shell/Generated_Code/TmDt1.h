/** ###################################################################
**     THIS COMPONENT MODULE IS GENERATED BY THE TOOL. DO NOT MODIFY IT.
**     Filename    : TmDt1.h
**     Project     : ProcessorExpert
**     Processor   : MKL25Z128VLK4
**     Component   : GenericTimeDate
**     Version     : Component 01.001, Driver 01.00, CPU db: 3.00.000
**     Compiler    : GNU C Compiler
**     Date/Time   : 2012-09-19, 07:00, # CodeGen: 76
**     Abstract    :
**
**     Settings    :
**
**     Contents    :
**         AddTick - void TmDt1_AddTick(void);
**         SetTime - byte TmDt1_SetTime(byte Hour, byte Min, byte Sec, byte Sec100);
**         GetTime - byte TmDt1_GetTime(TIMEREC *Time);
**         SetDate - byte TmDt1_SetDate(word Year, byte Month, byte Day);
**         GetDate - byte TmDt1_GetDate(DATEREC *Date);
**         Init    - void TmDt1_Init(void);
**
**     (c) Copyright Freescale Semiconductor, 2012
**     http: www.freescale.com
**     Source code is based on the original TimeDate Processor Expert component.
** ###################################################################*/

#ifndef __TmDt1_H
#define __TmDt1_H

/* MODULE TmDt1. */

/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* Include inherited beans */
#ifndef __BWUserType_TIMEREC
#define __BWUserType_TIMEREC
  typedef struct {                     /* It contains actual number of hours, minutes, seconds and hundreth of seconds. */
    byte Hour;                         /* hours (0 - 23) */
    byte Min;                          /* minutes (0 - 59) */
    byte Sec;                          /* seconds (0 - 59) */
    byte Sec100;                       /* hundredth of seconds (0 - 99) */
  } TIMEREC;
#endif
#ifndef __BWUserType_DATEREC
#define __BWUserType_DATEREC
  typedef struct {                     /* It contains actual year, month, and day description. */
    word Year;                         /* years (1998 - 2099) */
    byte Month;                        /* months (1 - 12) */
    byte Day;                          /* days (1 - 31) */
  } DATEREC;
#endif

#include "Cpu.h"




byte TmDt1_SetTime(byte Hour, byte Min, byte Sec, byte Sec100);
/*
** ===================================================================
**     Method      :  TmDt1_SetTime (component GenericTimeDate)
**
**     Description :
**         This method sets a new actual time.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Hour            - Hours (0 - 23)
**         Min             - Minutes (0 - 59)
**         Sec             - Seconds (0 - 59)
**         Sec100          - Hundredth of seconds (0 - 99)
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

void TmDt1_AddTick(void);
/*
** ===================================================================
**     Method      :  TmDt1_AddTick (component GenericTimeDate)
**
**     Description :
**         Needs to be called periodically by the application to
**         increase the time tick count.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

byte TmDt1_GetTime(TIMEREC *Time);
/*
** ===================================================================
**     Method      :  TmDt1_GetTime (component GenericTimeDate)
**
**     Description :
**         This method returns current time.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Time            - Pointer to the structure TIMEREC. It
**                           contains actual number of hours, minutes,
**                           seconds and hundredth of seconds.
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

byte TmDt1_SetDate(word Year, byte Month, byte Day);
/*
** ===================================================================
**     Method      :  TmDt1_SetDate (component GenericTimeDate)
**
**     Description :
**         This method sets a new actual date.
**     Parameters  :
**         NAME            - DESCRIPTION
**         Year            - Years (16-bit unsigned integer)
**         Month           - Months (8-bit unsigned integer)
**         Day             - Days (8-bit unsigned integer)
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

byte TmDt1_GetDate(DATEREC *Date);
/*
** ===================================================================
**     Method      :  TmDt1_GetDate (component GenericTimeDate)
**
**     Description :
**         This method returns current date.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * Date            - Pointer to DATEREC
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

void TmDt1_Init(void);
/*
** ===================================================================
**     Method      :  TmDt1_Init (component GenericTimeDate)
**
**     Description :
**         Initialization method
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

/* END TmDt1. */

#endif
/* ifndef __TmDt1_H */
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.0 [05.03]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
