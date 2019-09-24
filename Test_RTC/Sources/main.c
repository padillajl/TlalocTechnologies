/* ###################################################################
**     Filename    : main.c
**     Project     : Test_RTC
**     Processor   : MKL25Z128VLK4
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2019-09-23, 10:48, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.01
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "RTC1.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

/* User includes (#include below this line is not maintained by Processor Expert) */
#include "MyTypes.h"
void vfnRTCGetTime(void);

typedef struct
{
	u08 bSeconds;
	u08 bMinutes;
	u08 bHour;
	u08 bDay;
	u08 bMonth;
	u08 bYear;
}sTime;

LDD_TUserData *ptrLDD_RTC;
LDD_RTC_TTime LDD_RTC_TIME;
u08 bRTCOneSecondFlag;

sTime RTCTime;
/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
  /* Write your local variable definition here */

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/
  ptrLDD_RTC = RTC1_Init((LDD_TUserData*)NULL,TRUE);
  
  RTC_CR |= RTC_CR_OSCE_MASK;
  
  /* Obtain time for the first time */
  (void)RTC1_GetTime(ptrLDD_RTC,&LDD_RTC_TIME);
  vfnRTCGetTime();
  /* Write your code here */
  /* For example: for(;;) { } */
  for(;;)
  {
	  /* Obtain time every second */
	  if( 1 )
	  {
		  (void)RTC1_GetTime(ptrLDD_RTC,&LDD_RTC_TIME);
		  vfnRTCGetTime();
		  
		  bRTCOneSecondFlag = 0;
	  }
  }
  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.3 [05.09]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
void vfnRTCGetTime(void)
{
	RTCTime.bHour = LDD_RTC_TIME.Hour;
	RTCTime.bMinutes = LDD_RTC_TIME.Minute;
	RTCTime.bSeconds = LDD_RTC_TIME.Second;
	
	RTCTime.bYear = LDD_RTC_TIME.Year;
	RTCTime.bMonth = LDD_RTC_TIME.Month;
	RTCTime.bDay = LDD_RTC_TIME.Day;
}
