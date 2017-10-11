/*
 * buttonISR.c
 *
 *  Created on: 
 *      Author: 
 */
#include "system.h"
#include "io.h"
#include "alt_types.h"
#include "HexDisplay.h"
#include "buttonISR.h"
#include "altera_avalon_pio_regs.h"

#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
void buttonISR(void* context)
#else
void buttonISR(void* context, alt_u32 id)
#endif
{
    /* Cast context It is important that this be declared 
     * volatile to avoid unwanted compiler optimization.
     */
	alt_u32* pCount = (alt_u32*)context;
    /* Store the value in the PUSHBUTTONS's edge capture register in *context. */
	alt_u8 keys = IORD(PUSHBUTTONS_BASE, 3);
    /* Reset the PUSHBUTTONS's edge capture register. */
    IOWR_ALTERA_AVALON_PIO_EDGE_CAP(PUSHBUTTONS_BASE, 0); // Equivalent to IOWR(PUSHBUTTONS_BASE, 3, 123);

    /* Act upon the interrupt */
    switch(keys)
    {
    case 2: { // KEY1
    	alt_u8 status = IORD(INTERVAL_TIMER_BASE, 0);
    	if ((0x2&status) != 0) // we are running, let's pause
    	{
    		status = 0xB;
    		// send msg to LCD
			LCD_cursor(0,0);
    		LCD_text ("STOPPED...       ");
    	} else
    	{
    		status = 0x7;
    		// send msg to LCD
			LCD_cursor(0,0);
    		LCD_text("Operating.....  ");
    	}
    	IOWR(INTERVAL_TIMER_BASE, 1, status);
    }
    	break;
    case 4:  // KEY2
    	*pCount = 0;
    	HexDisplay((alt_u32*)HEX3_HEX0_BASE, *pCount);
    	// send msg to LCD
		LCD_cursor(0,0);
    	LCD_text("Reset...       ");
    	break;
    case 8:  // KEY3
    	toggleBCD();
    	HexDisplay((alt_u32*)HEX3_HEX0_BASE, *pCount);
    	// send msg to LCD
		LCD_cursor(0,0);
    	LCD_text("Toggled...      ");
    	break;
    default: // we should never get here
    	break;
    }
    /*
     * Read the PIO to delay ISR exit. This is done to prevent a spurious
     * interrupt in systems with high processor -> pio latency and fast
     * interrupts.
     */
    IORD_ALTERA_AVALON_PIO_EDGE_CAP(PUSHBUTTONS_BASE);
}
