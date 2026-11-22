/*
 * App.c
 *
 *  Created on: 7 mar. 2025
 *  Author: qcezs
 *
 *  Este archivo contiene la implementación de las funciones declaradas en App.h.
 */

#include "SAE/App.h"
#include <stdlib.h>
#include <string.h>
#include "SAE/data_structures.h"

#define UART_BAUDRATE 9600U

/* Variables globales para datos UART */
volatile uint8_t uart1_data = 0;
volatile uint8_t uart2_data = 0;
volatile uint8_t uart3_data = 0;

/*
 * Función: delay
 * Descripción: Implementa un retardo simple.
 */
void delay(uint32_t x) {
    for (volatile uint32_t i = 0; i < x; i++);
}

/*
 * Función: Task_PWM1
 * Descripción: Conmuta PTB3, ajustando el período según el dato de UART1.
 */
void Task_PWM1(void) {
    // Mapear uart1_data (0-255) a un período de 1 a 100 ticks
    uint16_t period = (uart1_data == 0) ? 10 : (1 + (uart1_data * 99 / 255));
    Set_Alarm('A', period);
    PWM_Pin_Set_Toggle(3); // PTB3
    Terminate_Task();
}

/*
 * Función: Task_PWM2
 * Descripción: Conmuta PTB4, ajustando el período según el dato de UART2.
 */
void Task_PWM2(void) {
    uint16_t period = (uart2_data == 0) ? 10 : (1 + (uart2_data * 99 / 255));
    Set_Alarm('B', period);
    PWM_Pin_Set_Toggle(4); // PTB4
    Terminate_Task();
}

/*
 * Función: Task_PWM3
 * Descripción: Conmuta PTB5, ajustando el período según el dato de UART3.
 */
void Task_PWM3(void) {
    uint16_t period = (uart3_data == 0) ? 10 : (1 + (uart3_data * 99 / 255));
    Set_Alarm('C', period);
    PWM_Pin_Set_Toggle(5); // PTB5
    Terminate_Task();
}

/*
 * Función: Task_UART1
 * Descripción: Lee datos de PTA25 y los almacena en uart1_data.
 */
void Task_UART1(void) {
    const uint32_t core_clk = CLOCK_GetCoreSysClkFreq();
    uint32_t bit_cycles = core_clk / UART_BAUDRATE;
    bit_cycles /= 11;

    DisableIRQ(PORTA_IRQn);

    delay(bit_cycles / 2);

    uint8_t data = 0;
    for (uint8_t bit_index = 0; bit_index < 8; bit_index++) {
        delay(bit_cycles);
        if (GPIOA->PDIR & (1 << 25)) {
            data |= (1 << bit_index);
        }
    }

    delay(bit_cycles);
    if (GPIOA->PDIR & (1 << 25)) {
        uart1_data = data;
    }

    EnableIRQ(PORTA_IRQn);
    Terminate_Task();
}

/*
 * Función: Task_UART2
 * Descripción: Lee datos de PTB18 y los almacena en uart2_data.
 */
void Task_UART2(void) {
    const uint32_t core_clk = CLOCK_GetCoreSysClkFreq();
    uint32_t bit_cycles = core_clk / UART_BAUDRATE;
    bit_cycles /= 11;

    DisableIRQ(PORTB_IRQn);

    delay(bit_cycles / 2);

    uint8_t data = 0;
    for (uint8_t bit_index = 0; bit_index < 8; bit_index++) {
        delay(bit_cycles);
        if (GPIOB->PDIR & (1 << 18)) {
            data |= (1 << bit_index);
        }
    }

    delay(bit_cycles);
    if (GPIOB->PDIR & (1 << 18)) {
        uart2_data = data;
    }

    EnableIRQ(PORTB_IRQn);
    Terminate_Task();
}

/*
 * Función: Task_UART3
 * Descripción: Lee datos de PTC2 y los almacena en uart3_data.
 */
void Task_UART3(void) {
    const uint32_t core_clk = CLOCK_GetCoreSysClkFreq();
    uint32_t bit_cycles = core_clk / UART_BAUDRATE;
    bit_cycles /= 11;

    DisableIRQ(PORTC_IRQn);

    delay(bit_cycles / 2);

    uint8_t data = 0;
    for (uint8_t bit_index = 0; bit_index < 8; bit_index++) {
        delay(bit_cycles);
        if (GPIOC->PDIR & (1 << 2)) {
            data |= (1 << bit_index);
        }
    }

    delay(bit_cycles);
    if (GPIOC->PDIR & (1 << 2)) {
        uart3_data = data;
    }

    EnableIRQ(PORTC_IRQn);
    Terminate_Task();
}

/*
 * Función: PORTA_IRQHandler
 * Descripción: Manejador de interrupciones para el puerto A.
 */
void PORTA_IRQHandler(void) {
    Save_Context();
    PORTA->ISFR = 0xFFFFFFFF;
    Activate_Task_ISR('F');
}

/*
 * Función: PORTB_IRQHandler
 * Descripción: Manejador de interrupciones para el puerto B.
 */
void PORTB_IRQHandler(void) {
    Save_Context();
    PORTB->ISFR = 0xFFFFFFFF;
    Activate_Task_ISR('G');
}

/*
 * Función: PORTC_IRQHandler
 * Descripción: Manejador de interrupciones para el puerto C.
 */
void PORTC_IRQHandler(void) {
    Save_Context();
    PORTC->ISFR = 0xFFFFFFFF;
    Activate_Task_ISR('H');
}
