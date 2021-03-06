/*
 * main implementation: use this 'C' sample to create your own application
 *
 */



/*************************************************************************************************/
/*********************						Includes						**********************/
/*************************************************************************************************/
/* System includes */
#include "derivative.h"
#include "MainDriver.h"
#include "Events.h"
/* Includes used in this file */

/* Own includes */

/*************************************************************************************************/
/*********************						Defines							**********************/
/*************************************************************************************************/

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

/*************************************************************************************************/
/*********************					Static Constants					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************					Global Constants					**********************/
/*************************************************************************************************/

/*************************************************************************************************/
/*********************						Functions						**********************/
/*************************************************************************************************/


int main(void)
{
	SwTimers_vfnInit();	
	vfnSensorsDriverInit();
	vfnLCDDriverInit();	
	vfnEventsInit();
	vfnMainDriverInit();
	
	for(;;) 
	{			
		
		SwTimers_vfnTask();		
		vfnSensorsDriver();		
		vfnLCDDriver();
		vfnMainDriverManager();
		vfnEventsDriver();
		
		//One seconds tasks
		if(bEventsOneSecondFlag)
		{
			//Timer to go to main screen
			if(bMainDriverMenuTimeOutTimer)
				bMainDriverMenuTimeOutTimer--;
			
			bEventsOneSecondFlag = 0;
		}
		
		
		
		
					
	}
	
	return 0;
}
