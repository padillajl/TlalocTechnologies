///////////////////////////////////////////////////////////////////////////////////////////////////
//                                      Includes Section                        
///////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdint.h>
#include "derivative.h"
#include "SwTimers.h"
#include "TPMA.h"




///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Defines & Macros Section                   
///////////////////////////////////////////////////////////////////////////////////////////////////

#define SWTIMERS_MAX_TIMERS		(8)

#define ENABLE_TIMER(Channel)			(gbTimersEnabled |= (1<<Channel))

#define DISABLE_TIMER(Channel)			(gbTimersEnabled &= ~(1<<Channel))

#define CHECK_TIMER(Channel)			(gbTimersEnabled & (1<<Channel))
///////////////////////////////////////////////////////////////////////////////////////////////////
//                                       Typedef Section                        
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//                                  Function Prototypes Section                 
///////////////////////////////////////////////////////////////////////////////////////////////////

static void SwTimer_vfnInitHwTimer (void);

static u08 SwTimer_bfnGetHwTimerStatus (void);

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Global Constants Section                   
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Static Constants Section                   
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Global Variables Section                   
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Static Variables Section                   
///////////////////////////////////////////////////////////////////////////////////////////////////


static u16 gwCounters[SWTIMERS_MAX_TIMERS];

static u08 gbTimersEnabled = 0;

static u08 gbTimersAllocated = 0;

u08 baSwTimersCurrentTimers[SWTIMERS_CURRENT_TIMERS];

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                      Functions Section                       
///////////////////////////////////////////////////////////////////////////////////////////////////

void SwTimers_vfnInit(void)
{
	gbTimersEnabled = 0;

	gbTimersAllocated = 0;
	/* Init HW timer */
	SwTimer_vfnInitHwTimer();	
	
}

void SwTimers_vfnTask(void)
{
	u08 TimerOffset = 0;
	u08 HwTimerStatus = 0;
	
	/* check if a timeout has passed */
	HwTimerStatus = SwTimer_bfnGetHwTimerStatus();
	
	if(HwTimerStatus)
	{
		while(TimerOffset < SWTIMERS_MAX_TIMERS)
		{
			if(CHECK_TIMER(TimerOffset))
			{
				/* decrement timer only when not zero */
				if(gwCounters[TimerOffset])
				{
					gwCounters[TimerOffset]--;
				}
			}
			
			TimerOffset++;
		}
	}
}

u08 SwTimers_bfnRequestTimer(void)
{
	u08 TimerOffset = 0;
	
	while(TimerOffset < SWTIMERS_MAX_TIMERS)
	{
		/* If the bit is 0, means the timer is free */
		/* if is 1, means the timer is allocated and must move to the next*/
		if(!(gbTimersAllocated & 1<<TimerOffset))
		{
			gbTimersAllocated |= (1<<TimerOffset);
			/* exit the cycle*/
			break;
		}
		else
		{
			TimerOffset++;
		}
	}
	
	/* send error in case there wasn't any timer available*/
	if(TimerOffset > SWTIMERS_MAX_TIMERS)
	{
		TimerOffset = 0xFF;
	}
	
	return(TimerOffset);
}

void SwTimers_vfnStartTimer(u08 Channel, u32 Timeout)
{
	if(Channel < SWTIMERS_MAX_TIMERS)
	{
		/* get the timeout in time base counts */
		Timeout = Timeout/SWTIMERS_TIME_BASE;
		gwCounters[Channel] = Timeout;
		ENABLE_TIMER(Channel);		
	}
} 


void SwTimers_vfnStopTimer(u08 Channel)
{
	if(Channel < SWTIMERS_MAX_TIMERS)
	{
		/* get the timeout in time base counts */
		gwCounters[Channel] = 0;
		DISABLE_TIMER(Channel);
		
	}
}
u08 SwTimers_bfnGetStatus(u08 Channel)
{
	u08 TimerStatus = 0;
	
	/* first confirm the timer is active*/
	if(CHECK_TIMER(Channel))
	{
		/* when the timer is 0 means is done*/
		if(!gwCounters[Channel])
		{
			TimerStatus = 1;
		}		
	}
	
	return(TimerStatus);
}

static void SwTimer_vfnInitHwTimer (void)
{
	/* place here your HW Timer init functions*/
	TPMA_vfnInit();
}

static u08 SwTimer_bfnGetHwTimerStatus (void)
{
	u08 bStatus;
	/* place here your getstatus function from HW timer */
	bStatus = TPMA_vfnTPM0GetStatus();	
	
	return (bStatus);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// EOF
///////////////////////////////////////////////////////////////////////////////////////////////////
