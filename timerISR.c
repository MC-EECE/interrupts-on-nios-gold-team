/*
 * timerISR.c
 *
 *  Created on: 
 *      Author: 
 */
#include "system.h"
#include "alt_types.h"
#include "io.h"
#include "HexDisplay.h"

/*****************************************************************************
 * Interval timer interrupt service routine
 *
 *
******************************************************************************/
#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
void timerISR(void* context)
#else
void timerISR(void * context, alt_u32 id)
#endif
{
	/* Cast context. It is important that this be declared
	 * volatile to avoid unwanted compiler optimization.
	 */
	alt_u32* count_ptr = (alt_u32*)context;
	/* clear the interrupt */
	IOWR(INTERVAL_TIMER_BASE, 0, 0);
	/* Act upon the interrupt */
	*count_ptr = *count_ptr + 1;
	/* Display count value 
	 * (Is this the best place to do this? 
	 * Why or why not?) 
	 */
	HexDisplay((alt_u32*)HEX3_HEX0_BASE, *count_ptr);

	return;
}



