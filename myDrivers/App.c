/*
 * App.c
 *
 *  Created on: 7 mar. 2025
 *      Author: qcezs
 */
#include "App.h"

/*
 * Configuración del botón SW3
 */
void GPIO_Config(void) {
	port_pin_config_t config = {
			kPORT_PullUp,
			kPORT_SlowSlewRate,
			kPORT_PassiveFilterEnable,	// Activer el filtro pasivo
			kPORT_OpenDrainDisable,
			kPORT_LowDriveStrength,
			kPORT_MuxAsGpio,
			kPORT_UnlockRegister,
	};

    uint32_t addr                = (uint32_t)&PORTA->PCR[10];
    *(volatile uint16_t *)(addr) = *((const uint16_t *)(const void *)&config);
	PORTA->PCR[10] = (PORTA->PCR[10] & ~PORT_PCR_IRQC_MASK) | PORT_PCR_IRQC(10);
	EnableIRQ(PORTA_IRQn);
}

/* Delay simple. Para fines... no hay fines */
void delay(void) {
	for(uint32_t i = 0; i < 20000000; i++);
}

/*
 * Tareas a ejecutar
*/
void Task_A(void) {
	Red();

	//for(uint32_t i = 0; i < 10; i++);

	delay();
	Activate_Task('B');

	Red();
	delay();
	Off();
	Terminate_Task();
}

void Task_B(void) {
	Blue();
	delay();
	Off();
	Chain_Task('C');
}

void Task_C(void) {
	Green();
	delay();
	Off();
	Terminate_Task();
}

void Task_ISR(void) {
	White();
	delay();
	Off();
	Terminate_Task();
}

/*
 * Función: PORTA_IRQHandler
 * Descripción: Manejador de interrupciones del puerto A.
 */
void PORTA_IRQHandler(void) {
	/* Guardar contexto */
	Save_Context();

	/* Limpiar todas las banderas */
	PORTA->ISFR = 0xFFFFFFFF;
	__DSB();

	asm("add sp, #0x32");		// 0x28

	/* Activar la tarea correspondiente */
	Activate_Task_ISR('D');
}
