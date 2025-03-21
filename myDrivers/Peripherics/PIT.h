/*
 * PIT.h
 *
 *  Created on: 11 mar. 2025
 *      Author: qcezs
 */

#ifndef PERIPHERICS_PIT_H_
#define PERIPHERICS_PIT_H_

#include "fsl_pit.h"

static uint32_t * const SIM_SCGC6	= (uint32_t*)(0x4004803c);	// System Clock Gating Control Register 6
static uint32_t * const PIT_MCR		= (uint32_t*)(0x40037000);	// PIT Module Control Register
static uint32_t * const PIT_LDVAL0	= (uint32_t*)(0x40037100);	// Timer Load Value Register
static uint32_t * const PIT_TCTRL0	= (uint32_t*)(0x40037108);	// Timer Control Register
static uint32_t * const PIT_TFLG0	= (uint32_t*)(0x4003710C);	// Timer Flag Contol Register
static uint32_t * const NVIC_ISER	= (uint32_t*)(0xE000E100 + (PIT0_IRQn >> 5) * 4);	// NVIC Interrupt Set Enable Register

void PIT_Config(void);
void PIT0_IRQHandler(void);

#endif /* PERIPHERICS_PIT_H_ */
