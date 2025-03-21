/*
 * RGB.c
 *
 *  Created on: 6 mar. 2025
 *      Author: qcezs
 */
#include "RGB.h"

void Config_Pins()
{
	//Encender reloj de los puertos E(13), C(11), B(10) y A(9):
	SIM_SCGC5 |= (1 << 13) + (1 << 11) + (1 << 10) + (1 << 9);

	//Seleccionar el modo GPIO en los pines:
	PCR_PTA11	|= (1 << 8);
	PCR_PTC9	|= (1 << 8);
	PCR_PTE6	|= (1 << 8);
}

void Init_Pins()
{
	GPIO_A->PDDR |= (1 << 11);	//Color Azul
	GPIO_C->PDDR |= (1 << 9);	//Color Rojo
	GPIO_E->PDDR |= (1 << 6);	//Color Verde
}

void Red()
{
    GPIO_C->PCOR = (1 << 9);  // Encender LED Rojo (PTC9)
    GPIO_A->PSOR = (1 << 11); // Apagar LED Azul (PTA11)
    GPIO_E->PSOR = (1 << 6);  // Apagar LED Verde (PTE6)
}

void Blue()
{
    GPIO_A->PCOR = (1 << 11); // Encender LED Azul (PTA11)
    GPIO_C->PSOR = (1 << 9);  // Apagar LED Rojo (PTC9)
    GPIO_E->PSOR = (1 << 6);  // Apagar LED Verde (PTE6)
}

void Green()
{
    GPIO_E->PCOR = (1 << 6);  // Encender LED Verde (PTE6)
    GPIO_A->PSOR = (1 << 11); // Apagar LED Azul (PTA11)
    GPIO_C->PSOR = (1 << 9);  // Apagar LED Rojo (PTC9)
}

void White()
{
    GPIO_E->PCOR = (1 << 6);  // Encender LED Verde (PTE6)
    GPIO_A->PCOR = (1 << 11); // Encender LED Azul (PTA11)
    GPIO_C->PCOR = (1 << 9);  // Encender LED Rojo (PTC9)
}

void Off()
{
    GPIO_C->PSOR = (1 << 9);  // Apagar LED Rojo (PTC9)
    GPIO_A->PSOR = (1 << 11); // Apagar LED Azul (PTA11)
    GPIO_E->PSOR = (1 << 6);  // Apagar LED Verde (PTE6)
}
