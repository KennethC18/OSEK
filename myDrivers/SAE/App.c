/*
 * App.c
 *
 *  Created on: 7 mar. 2025
 *  Author: qcezs
 *
 *  Este archivo contiene la implementación de las funciones declaradas en App.h.
 */

#include "SAE/App.h"
#include "fsl_debug_console.h"
#include <stdlib.h>
#include <string.h>
#include "SAE/data_structures.h"   // Incluye la definición de Queue_t

#define UART_BAUDRATE    9600U


/* Declaración de la cola de prueba de forma global.
   Se utilizará para almacenar datos de tamaño uint32_t y tendrá capacidad para 10 elementos. */
Queue_t* testQueue = NULL;


/*
 * Función: delay
 * Implementa un retardo simple mediante un bucle vacío.
 */
void delay(uint32_t x) {
    for (volatile uint32_t i = 0; i < x; i++);
}

void Task_PWM1(void){

}

void Task_PWM2(void){

}

void Task_PWM3(void){

}

void Task_PWM4(void){

}

void Task_PWM5(void){

}





volatile uint8_t UART1_data;
void Task_UART1(void) {
    const uint32_t core_clk = CLOCK_GetCoreSysClkFreq();
    uint32_t bit_cycles = core_clk / UART_BAUDRATE;
    bit_cycles /= 11;

    DisableIRQ(PORTA_IRQn);

    // Confirmar start‑bit
    delay(bit_cycles / 2);
//    while(1){
//        GPIOB->PTOR = 1 << 7;
//        delay(bit_cycles);
//    }

    GPIOB->PTOR = 1 << 7;

//    if ( (GPIOA->PDIR & (1<<25)) != 0 ) {
//        // No había start‑bit → framing error
//        EnableIRQ(PORTA_IRQn);
//        Terminate_Task();
//    }

    // Leer los 8 bits de datos
    UART1_data = 0;
    for (uint8_t bit_index = 0; bit_index < 8; bit_index++) {
        delay(bit_cycles);
        GPIOB->PTOR = 1 << 7;
        if ( GPIOA->PDIR & (1<<25) ) {
            UART1_data |= (1 << bit_index);
        }
    }

    // Verificar stop‑bit
    delay(bit_cycles);
    GPIOB->PTOR = 1 << 7;
    if ( (GPIOA->PDIR & (1<<25)) == 0 ) {
        // framing error
    }

    // Restaurar IRQ y finalizar
    EnableIRQ(PORTA_IRQn);
    Terminate_Task();
}

void Task_UART2(void){
	DisableIRQ(PORTB_IRQn);
}

void Task_UART3(void){
	DisableIRQ(PORTC_IRQn);
}

void Task_UART4(void){
	DisableIRQ(PORTD_IRQn);
}

void Task_UART5(void){
	DisableIRQ(PORTE_IRQn);
}

/*
 * Función: PORTA_IRQHandler
 * Manejador de interrupciones para el puerto A.
 */
void PORTA_IRQHandler(void) {
	// Ajustar el puntero de la pila para apuntar a los datos guardados antes de la interrupción
	asm("mov r6, sp");
	asm("add r6, #0x2c");

	// Cargar los datos y guardar en la estructura de contexto
	asm("ldr r8, = list");  // Apuntar a la dirección base de 'list'
	asm("add r8, #0x12c");  // Apuntar al contexto de la tarea actual/interrumpida

	// Guardar XPSR
	asm("mrs r9, xpsr");
	asm("str r9, [r8]");

	// Guardar PC
	asm("ldr r9, [r6, #-8]");
	asm("str r9, [r8, #4]");

	// Guardar LR
	asm("ldr r9, [r6, #-12]");
	asm("str r9, [r8, #8]");

	// Guardar SP
	asm("ldr r9, [r6, #-40]");
	asm("str r9, [r8, #12]");

	// Guardar R12
	asm("ldr r9, [r6, #-16]");
	asm("str r9, [r8, #16]");

	// Guardar R3
	asm("ldr r9, [r6, #-20]");
	asm("str r9, [r8, #20]");

	// Guardar R2
	asm("ldr r9, [r6, #-24]");
	asm("str r9, [r8, #24]");

	// Guardar R1
	asm("ldr r9, [r6, #-28]");
	asm("str r9, [r8, #28]");

	// Guardar R0
	asm("ldr r9, [r6, #-32]");
	asm("str r9, [r8, #32]");

	Save_Context();

    PORTA->ISFR = 0xFFFFFFFF;  // Limpiar las banderas de interrupción

	Activate_Task_ISR('F');
}

void PORTB_IRQHandler(void) {
    Save_Context();  // Guardar el contexto actual

    PORTB->ISFR = 0xFFFFFFFF;  // Limpiar las banderas de interrupción

	Activate_Task_ISR('G');
}

void PORTC_IRQHandler(void) {
    Save_Context();  // Guardar el contexto actual

    PORTC->ISFR = 0xFFFFFFFF;  // Limpiar las banderas de interrupción

	Activate_Task_ISR('H');
}

void PORTD_IRQHandler(void) {
    Save_Context();  // Guardar el contexto actual

    PORTD->ISFR = 0xFFFFFFFF;  // Limpiar las banderas de interrupción

	Activate_Task_ISR('I');
}

void PORTE_IRQHandler(void) {
    Save_Context();  // Guardar el contexto actual

    PORTE->ISFR = 0xFFFFFFFF;  // Limpiar las banderas de interrupción

	Activate_Task_ISR('J');
}
