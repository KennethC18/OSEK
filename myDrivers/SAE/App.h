/*
 * App.h
 *
 *  Created on: 7 mar. 2025
 *  Author: qcezs
 *
 *  Este archivo contiene las declaraciones de las funciones y configuraciones
 *  utilizadas en la aplicación.
 */

#ifndef SAE_APP_H_
#define SAE_APP_H_

#include <Peripherics/GPIO.h>
#include <SAE/SO.h>
#include "fsl_gpio.h"
#include "fsl_port.h"

void Task_PWM1(void);
void Task_PWM2(void);
void Task_PWM3(void);
void Task_PWM4(void);
void Task_PWM5(void);

void Task_UART1(void);
void Task_UART2(void);
void Task_UART3(void);
void Task_UART4(void);
void Task_UART5(void);

/*
 * Función: PORTA_IRQHandler
 * Descripción: Manejador de interrupciones para el puerto A. Limpia las banderas de interrupción
 *              y activa la tarea correspondiente.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void PORTA_IRQHandler(void);
void PORTB_IRQHandler(void);
void PORTC_IRQHandler(void);
void PORTD_IRQHandler(void);
void PORTE_IRQHandler(void);

#endif /* SAE_APP_H_ */
