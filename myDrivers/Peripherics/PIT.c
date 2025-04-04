/*
 * PIT.c
 *
 *  Created on: 11 mar. 2025
 *      Author: qcezs
 */
#include <Peripherics/PIT.h>

void PIT_Config(void) {
	/*
	 * Enable PIT clock
	 */
	*SIM_SCGC6 = (1 << 23);

	/*
	 * MDIS (1)	= 0 (Clock for standard PIT timers is enabled)
	 * FRZ  (0)	= 1 (Timers are stopped in Debug mode)
	 */
	*PIT_MCR = (1 << 0);

	/*
	 *	Set time value to generate an interrput.
	 *	In this case, is = (BusClockFreq / 22,050) - 1
	 */
	*PIT_LDVAL0 = 0x1770;

	/*
	 * CHN (2)	= 0 (Timer is NOT chained)
	 * TIE (1)	= 1 (Timer Interruptions Enabled)
	 * TEN (0)	= 1 (Timer Enabled)
	 */
	*PIT_TCTRL0 = (1 << 1) | (1 << 0);

	/* Enable interruptions for PIT */
    *NVIC_ISER = (1 << (PIT0_IRQn & 0x1F));
}

