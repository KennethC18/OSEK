/*
 * RGB.h
 *
 *  Created on: 6 mar. 2025
 *  Author: qcezs
 *
 *  Este archivo contiene las declaraciones de las funciones y definiciones necesarias
 *  para controlar los LEDs RGB en el sistema.
 */

#ifndef PERIPHERICS_GPIO_H_
#define PERIPHERICS_GPIO_H_

// Incluir librerías necesarias
#include <stdint.h>  // Para tipos de datos como uint32_t
#include "board.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"
#include "peripherals.h"
#include "pin_mux.h"

/*
 * Definiciones de direcciones de memoria para los registros de control de GPIO.
 */

// Registro de control de reloj para los puertos A, B, C, D y E
#define SIM_SCGC5 (*((volatile uint32_t *) 0x40048038))

// Registros de control de pines para los LEDs
#define PCR_PTA11 (*((volatile uint32_t*) 0x4004902C))  // LED Azul (PTA11)
#define PCR_PTC9  (*((volatile uint32_t*) 0x4004B024))  // LED Rojo (PTC9)
#define PCR_PTE6  (*((volatile uint32_t*) 0x4004D018))  // LED Verde (PTE6)

// Direcciones base de los puertos GPIO
#define GPIO_A_BASE_ADDR ((volatile uint32_t *) 0x400FF000)  // Puerto A
#define GPIO_B_BASE_ADDR ((volatile uint32_t *) 0x400FF040)  // Puerto B
#define GPIO_C_BASE_ADDR ((volatile uint32_t *) 0x400FF080)  // Puerto C
#define GPIO_E_BASE_ADDR ((volatile uint32_t *) 0x400FF100)  // Puerto E

// GPIOs para la generación de PWM:
#define PCR_PTB2 (*((volatile uint32_t*) 0x4004A008))	// PTB3
#define PCR_PTB3 (*((volatile uint32_t*) 0x4004A00C))	// PTB3
#define PCR_PTB4 (*((volatile uint32_t*) 0x4004A010))	// PTB4
#define PCR_PTB5 (*((volatile uint32_t*) 0x4004A014))	// PTB5
#define PCR_PTB6 (*((volatile uint32_t*) 0x4004A018))	// PTB6
#define PCR_PTB7 (*((volatile uint32_t*) 0x4004A01C))	// PTB7

/*
 * Estructura: GPIO_t
 * Descripción: Representa los registros de un puerto GPIO.
 * Campos:
 * - PDOR: Registro de datos de salida.
 * - PSOR: Registro de set de salida (encender).
 * - PCOR: Registro de clear de salida (apagar).
 * - PTOR: Registro de toggle de salida.
 * - PDIR: Registro de datos de entrada.
 * - PDDR: Registro de dirección de datos (entrada/salida).
 */
typedef struct {
    uint32_t PDOR;
    uint32_t PSOR;
    uint32_t PCOR;
    uint32_t PTOR;
    uint32_t PDIR;
    uint32_t PDDR;
} GPIO_t;

// Punteros a los puertos GPIO
#define GPIO_A ((GPIO_t*) GPIO_A_BASE_ADDR)  // Puerto A (LED Azul)
#define GPIO_B ((GPIO_t*) GPIO_B_BASE_ADDR)  // Puerto B (Generador PWM)
#define GPIO_C ((GPIO_t*) GPIO_C_BASE_ADDR)  // Puerto C (LED Rojo)
#define GPIO_E ((GPIO_t*) GPIO_E_BASE_ADDR)  // Puerto E (LED Verde)

/*
 * Declaraciones de funciones.
 */

/*
 * Función: Config_Pins
 * Descripción: Configura los pines GPIO para los LEDs RGB.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void Config_Pins(void);

/*
 * Función: GPIO_Config
 * Descripción: Configura el pin del botón SW3 en el puerto A.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void GPIO_Config(void);

/*
 * Función: Init_Pins
 * Descripción: Inicializa los pines GPIO como salidas.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void Init_Pins(void);

/*
 * Función: Red
 * Descripción: Enciende el LED rojo y apaga los demás LEDs.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void Red(void);

/*
 * Función: Blue
 * Descripción: Enciende el LED azul y apaga los demás LEDs.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void Blue(void);

/*
 * Función: Green
 * Descripción: Enciende el LED verde y apaga los demás LEDs.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void Green(void);

/*
 * Función: White
 * Descripción: Enciende todos los LEDs (rojo, verde y azul) para generar luz blanca.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void White(void);

/*
 * Función: Off
 * Descripción: Apaga todos los LEDs.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void Off(void);

#endif /* PERIPHERICS_GPIO_H_ */
