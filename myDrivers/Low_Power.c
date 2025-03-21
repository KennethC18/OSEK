/*
 * Low_Power.c
 *
 *  Created on: 7 mar. 2025
 *      Author: qcezs
 */
#include "Low_Power.h"

/*!
 * @brief  enter into sleep mode using CMSIS (WAIT)
 */
void sleep(void)
{
	SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
/*!
 * Disable all interrupts except GPIO to only wake up when there's an interrupt from PORTC (1Hz square wave input)
 */
	DisableIRQ(PIT0_IRQn);
	DisableIRQ(PIT1_IRQn);
	DisableIRQ(UART0_RX_TX_IRQn);

	__WFI(); //enter to sleep mode
}

/*!
 * @brief  enter into Deep Sleep mode using CMSIS (STOP)
 */
void deepsleep(void)
{
	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

	__WFI(); //Enter to deep sleep mode
}
