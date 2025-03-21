/*
 * Low_Power.c
 *
 *  Created on: 7 mar. 2025
 *  Author: qcezs
 *
 *  Este archivo contiene la implementación de las funciones para manejar modos de bajo consumo
 *  (sleep y deep sleep) en el sistema.
 */

#include <Peripherics/Low_Power.h>

/*
 * Función: sleep
 * Descripción: Coloca el sistema en modo de bajo consumo (sleep mode) utilizando CMSIS.
 *              Deshabilita todas las interrupciones excepto las de GPIO para que el sistema
 *              solo se despierte ante una interrupción de PORTC (entrada de onda cuadrada de 1Hz).
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void sleep(void) {
    // Configurar el sistema para entrar en modo sleep (no deep sleep)
    SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;

    // Entrar en modo sleep (Wait For Interrupt)
    EnableIRQ(PIT0_IRQn);
    //__WFI();
}

/*
 * Función: deepsleep
 * Descripción: Coloca el sistema en modo de bajo consumo profundo (deep sleep mode) utilizando CMSIS.
 *              En este modo, el sistema consume menos energía pero requiere una interrupción específica
 *              para despertar.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void deepsleep(void) {
    // Configurar el sistema para entrar en modo deep sleep
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

    // Entrar en modo deep sleep (Wait For Interrupt)
    __WFI();
}
