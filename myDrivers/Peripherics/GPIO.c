/*
 * RGB.c
 *
 *  Created on: 6 mar. 2025
 *  Author: qcezs
 *
 *  Este archivo contiene la implementación de las funciones para controlar los LEDs RGB.
 */

#include <Peripherics/GPIO.h>

/*
 * Función: Config_Pins
 * Descripción: Configura los pines GPIO para los LEDs RGB, habilitando el reloj de los puertos
 *              y seleccionando el modo GPIO para los pines correspondientes.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void Config_Pins(void) {
    // Habilitar el reloj para los puertos E, D, C, B y A
    SIM_SCGC5 |= (1 << 13) | (1 << 12) | (1 << 11) | (1 << 10) | (1 << 9);

    // Configurar los pines como GPIO
    PCR_PTA11 |= (1 << 8);  // PTA11 (LED Azul)
    PCR_PTC9  |= (1 << 8);  // PTC9  (LED Rojo)
    PCR_PTE6  |= (1 << 8);  // PTE6  (LED Verde)

    /* PWM pins */
    PCR_PTB3 |= (1 << 8);	// PTB3
    PCR_PTB4 |= (1 << 8);	// PTB4
    PCR_PTB5 |= (1 << 8);	// PTB5
    PCR_PTB6 |= (1 << 8);	// PTB6
    PCR_PTB7 |= (1 << 8);	// PTB7

    Init_Pins();
}

/*
 * Función: GPIO_Config
 * Descripción: Configura el pin del botón SW3 en el puerto A.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void GPIO_Config(void){
	Config_Pins();

	port_pin_config_t config = {
		kPORT_PullUp,
		kPORT_SlowSlewRate,
		kPORT_PassiveFilterEnable,
		kPORT_OpenDrainDisable,
		kPORT_LowDriveStrength,
		kPORT_MuxAsGpio,
		kPORT_UnlockRegister,
	};

	uint32_t addr = 0;

	addr = (uint32_t)&PORTA->PCR[25];
	*(volatile uint16_t *)(addr) = *((const uint16_t *)(const void *)&config);

//	addr = (uint32_t)&PORTB->PCR[18];
//	*(volatile uint16_t *)(addr) = *((const uint16_t *)(const void *)&config);
//
//	addr = (uint32_t)&PORTC->PCR[2];
//	*(volatile uint16_t *)(addr) = *((const uint16_t *)(const void *)&config);
//
//	addr = (uint32_t)&PORTD->PCR[0];
//	*(volatile uint16_t *)(addr) = *((const uint16_t *)(const void *)&config);
//
//	addr = (uint32_t)&PORTE->PCR[7];
//	*(volatile uint16_t *)(addr) = *((const uint16_t *)(const void *)&config);

	PORTA->PCR[25] = (PORTA->PCR[25] & ~PORT_PCR_IRQC_MASK) | PORT_PCR_IRQC(kPORT_InterruptFallingEdge);
//	PORTB->PCR[18] = (PORTA->PCR[18] & ~PORT_PCR_IRQC_MASK) | PORT_PCR_IRQC(kPORT_InterruptFallingEdge);
//	PORTC->PCR[2]  = (PORTA->PCR[2]  & ~PORT_PCR_IRQC_MASK) | PORT_PCR_IRQC(kPORT_InterruptFallingEdge);
//	PORTD->PCR[0]  = (PORTA->PCR[0]  & ~PORT_PCR_IRQC_MASK) | PORT_PCR_IRQC(kPORT_InterruptFallingEdge);
//	PORTE->PCR[7]  = (PORTA->PCR[7]  & ~PORT_PCR_IRQC_MASK) | PORT_PCR_IRQC(kPORT_InterruptFallingEdge);

	EnableIRQ(PORTA_IRQn);
//	EnableIRQ(PORTB_IRQn);
//	EnableIRQ(PORTC_IRQn);
//	EnableIRQ(PORTD_IRQn);
//	EnableIRQ(PORTE_IRQn);

}

/*
 * Función: Init_Pins
 * Descripción: Inicializa los pines GPIO como salidas.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void Init_Pins(void) {
    GPIO_A->PDDR |= (1 << 11);  // PTA11 como salida (LED Azul)
    GPIO_B->PDDR |= (1 << 3) | (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7);
    GPIO_C->PDDR |= (1 << 9);   // PTC9  como salida (LED Rojo)
    GPIO_E->PDDR |= (1 << 6);   // PTE6  como salida (LED Verde)

    Off();
}

/*
 * Función: Red
 * Descripción: Enciende el LED rojo y apaga los demás LEDs.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void Red(void) {
    GPIO_C->PCOR = (1 << 9);   // Encender LED Rojo (PTC9)
    GPIO_A->PSOR = (1 << 11);  // Apagar LED Azul (PTA11)
    GPIO_E->PSOR = (1 << 6);   // Apagar LED Verde (PTE6)
}

/*
 * Función: Blue
 * Descripción: Enciende el LED azul y apaga los demás LEDs.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void Blue(void) {
    GPIO_A->PCOR = (1 << 11);  // Encender LED Azul (PTA11)
    GPIO_C->PSOR = (1 << 9);   // Apagar LED Rojo (PTC9)
    GPIO_E->PSOR = (1 << 6);   // Apagar LED Verde (PTE6)
}

/*
 * Función: Green
 * Descripción: Enciende el LED verde y apaga los demás LEDs.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void Green(void) {
    GPIO_E->PCOR = (1 << 6);   // Encender LED Verde (PTE6)
    GPIO_A->PSOR = (1 << 11);  // Apagar LED Azul (PTA11)
    GPIO_C->PSOR = (1 << 9);   // Apagar LED Rojo (PTC9)
}

/*
 * Función: White
 * Descripción: Enciende todos los LEDs (rojo, verde y azul) para generar luz blanca.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void White(void) {
    GPIO_E->PCOR = (1 << 6);   // Encender LED Verde (PTE6)
    GPIO_A->PCOR = (1 << 11);  // Encender LED Azul (PTA11)
    GPIO_C->PCOR = (1 << 9);   // Encender LED Rojo (PTC9)
}

/*
 * Función: Off
 * Descripción: Apaga todos los LEDs.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void Off(void) {
    GPIO_C->PSOR = (1 << 9);   // Apagar LED Rojo (PTC9)
    GPIO_A->PSOR = (1 << 11);  // Apagar LED Azul (PTA11)
    GPIO_E->PSOR = (1 << 6);   // Apagar LED Verde (PTE6)
}
