/*
 * RGB.h
 *
 *  Created on: 6 mar. 2025
 *      Author: qcezs
 */

#ifndef RGB_H_
#define RGB_H_

//Se incluyen las líbrerías correspondientes:
	//Librería para "uint32_t
	#include <stdint.h>

	//Librería para usar las funciones del SDK:
	#include "board.h"
	#include "clock_config.h"
	#include "fsl_debug_console.h"
	#include "fsl_gpio.h"
	#include "peripherals.h"
	#include "pin_mux.h"


//Declarar apuntadores a las direcciones de memoria necesarias:
	//Apuntador al reloj que controla los GPIO de los puertos A, B, C, D y E:
	#define SIM_SCGC5			(*((volatile uint32_t *) 0x40048038))

	//Apuntadores a las direcciones de los pines que controlan los LEDs:
	#define PCR_PTA11			(*((volatile uint32_t*) 0x4004902C))	//LED AZUL
	#define PCR_PTC9			(*((volatile uint32_t*) 0x4004B024))	//LED ROJO
	#define PCR_PTE6			(*((volatile uint32_t*) 0x4004D018))	//LED VERDE

	//Apuntadores a las direcciones base de los puertos A, C y E:
	#define GPIO_A_BASE_ADDR	( ((volatile uint32_t *) 0x400FF000))	// Puerto A
	#define GPIO_C_BASE_ADDR	( ((volatile uint32_t *) 0x400FF080))	// Puerto C
	#define GPIO_E_BASE_ADDR	( ((volatile uint32_t *) 0x400FF100))	// Puerto E


//Definiones de estructuras y apuntadores a esta:
	//Estructura de los registros GPIO de cada puerto:
	typedef struct
	{
		uint32_t PDOR;
		uint32_t PSOR;
		uint32_t PCOR;
		uint32_t PTOR;
		uint32_t PDIR;
		uint32_t PDDR;
	} GPIO_t;

	//Apuntadores:
	#define GPIO_A ( (GPIO_t*) GPIO_A_BASE_ADDR)	//Apuntador para el LED Azul
	#define GPIO_C ( (GPIO_t*) GPIO_C_BASE_ADDR)	//Apuntador para el LED Rojo
	#define GPIO_E ( (GPIO_t*) GPIO_E_BASE_ADDR)	//Apuntador para el LED Verde


//Declaración de funciones:
	void Config_Pins();	//Configurar los GPIOs
	void Init_Pins();	//Inicializar pines como salida

	void Red();			//Encender Rojo
	void Blue();		//Encender Azul
	void Green();		//Encender Verde
	void White();		//Todos los LEDs encendidos

	void Off();			//Apagar todos los LEDs

#endif /* RGB_H_ */
