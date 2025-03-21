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

#include <Peripherics/RGB.h>
#include <SAE/SO.h>
#include "fsl_gpio.h"
#include "fsl_port.h"

// Librerías personalizadas

/*
 * Función: GPIO_Config
 * Descripción: Configura el pin del botón SW3 en el puerto A.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void GPIO_Config(void);

/*
 * Función: Task_A
 * Descripción: Tarea que ejecuta una secuencia de luces rojas y activa la Task_B.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void Task_A(void);

/*
 * Función: Task_B
 * Descripción: Tarea que ejecuta una secuencia de luces azules y activa la Task_C.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void Task_B(void);

/*
 * Función: Task_C
 * Descripción: Tarea que ejecuta una secuencia de luces verdes y finaliza.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void Task_C(void);
void Task_D(void);
void Task_E(void);
void Task_F(void);
void Task_G(void);


/*
 * Función: Task_ISR
 * Descripción: Tarea que se ejecuta en respuesta a una interrupción, activando una luz blanca.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void Task_ISR(void);

/*
 * Función: PORTA_IRQHandler
 * Descripción: Manejador de interrupciones para el puerto A. Limpia las banderas de interrupción
 *              y activa la tarea correspondiente.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void PORTA_IRQHandler(void);

#endif /* SAE_APP_H_ */
