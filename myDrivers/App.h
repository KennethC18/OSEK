/*
 * App.h
 *
 *  Created on: 7 mar. 2025
 *      Author: qcezs
 */

#ifndef APP_H_
#define APP_H_

#include "fsl_gpio.h"
#include "fsl_port.h"

/*
 * Librerías hechas por mí
 */
#include "SO.h"
#include "RGB.h"

/*
 * Función para configurar el botón SW3
 */
void GPIO_Config(void);

/*
 * Tareas convencionales.
 */
void Task_A(void);
void Task_B(void);
void Task_C(void);

/*
 * Tareas a ejecutar cuando suceda una interrupción
 */
void Task_ISR(void);

void PORTA_IRQHandler(void);

#endif /* APP_H_ */
