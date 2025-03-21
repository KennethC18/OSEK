/*
 * Low_Power.h
 *
 *  Created on: 7 mar. 2025
 *  Author: qcezs
 *
 *  Este archivo contiene las declaraciones de las funciones para manejar modos de bajo consumo
 *  (sleep y deep sleep) en el sistema.
 */

#ifndef PERIPHERICS_LOW_POWER_H_
#define PERIPHERICS_LOW_POWER_H_

// Incluir archivos de cabecera necesarios
#include "MK66F18.h"
#include "cmsis_gcc.h"
#include "fsl_common_arm.h"

/*
 * Función: sleep
 * Descripción: Coloca el sistema en modo de bajo consumo (sleep mode) utilizando CMSIS.
 *              En este modo, el sistema se despierta ante una interrupción.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void sleep(void);

/*
 * Función: deepsleep
 * Descripción: Coloca el sistema en modo de bajo consumo profundo (deep sleep mode) utilizando CMSIS.
 *              En este modo, el sistema consume menos energía pero requiere una interrupción específica
 *              para despertar.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void deepsleep(void);

#endif /* PERIPHERICS_LOW_POWER_H_ */
