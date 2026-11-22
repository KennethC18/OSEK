/*
 * RGB.c
 *
 *  Created on: 6 mar. 2025
 *  Author: qcezs
 *
 *  Este archivo contiene la implementación de las funciones para controlar los LEDs RGB y PWM.
 */

#include <Peripherics/GPIO.h>
#include "fsl_port.h"

/*
 * Función: Config_Pins
 * Descripción: Configura los pines GPIO para los LEDs RGB y los pines PWM.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void Config_Pins(void) {
    SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTC_MASK |
                  SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTA_MASK;

    PORTA->PCR[11] = PORT_PCR_MUX(1); // PTA11 (LED Azul)
    PORTC->PCR[9]  = PORT_PCR_MUX(1); // PTC9  (LED Rojo)
    PORTE->PCR[6]  = PORT_PCR_MUX(1); // PTE6  (LED Verde)

    PORTB->PCR[3] = PORT_PCR_MUX(1); // PTB3 (PWM1)
    PORTB->PCR[4] = PORT_PCR_MUX(1); // PTB4 (PWM2)
    PORTB->PCR[5] = PORT_PCR_MUX(1); // PTB5 (PWM3)

    Init_Pins();
}

/*
 * Función: GPIO_Config
 * Descripción: Configura los pines de entrada para UART (PTA25, PTB18, PTC2).
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void GPIO_Config(void) {
    Config_Pins();

    port_pin_config_t config = {
        .pullSelect = kPORT_PullUp,
        .slewRate = kPORT_SlowSlewRate,
        .passiveFilterEnable = kPORT_PassiveFilterEnable,
        .openDrainEnable = kPORT_OpenDrainDisable,
        .driveStrength = kPORT_LowDriveStrength,
        .mux = kPORT_MuxAsGpio,
        .lockRegister = kPORT_UnlockRegister
    };

    PORTA->PCR[25] = *(uint32_t*)&config;
    PORTB->PCR[18] = *(uint32_t*)&config;
    PORTC->PCR[2]  = *(uint32_t*)&config;

    PORTA->PCR[25] = (PORTA->PCR[25] & ~PORT_PCR_IRQC_MASK) | PORT_PCR_IRQC(kPORT_InterruptFallingEdge);
    PORTB->PCR[18] = (PORTB->PCR[18] & ~PORT_PCR_IRQC_MASK) | PORT_PCR_IRQC(kPORT_InterruptFallingEdge);
    PORTC->PCR[2]  = (PORTC->PCR[2]  & ~PORT_PCR_IRQC_MASK) | PORT_PCR_IRQC(kPORT_InterruptFallingEdge);

    EnableIRQ(PORTA_IRQn);
    EnableIRQ(PORTB_IRQn);
    EnableIRQ(PORTC_IRQn);
}

/*
 * Función: Init_Pins
 * Descripción: Inicializa los pines GPIO como salidas para LEDs y PWM.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void Init_Pins(void) {
    GPIOA->PDDR |= (1 << 11); // PTA11 como salida (LED Azul)
    GPIOC->PDDR |= (1 << 9);  // PTC9  como salida (LED Rojo)
    GPIOE->PDDR |= (1 << 6);  // PTE6  como salida (LED Verde)

    GPIOB->PDDR |= (1 << 3) | (1 << 4) | (1 << 5); // PTB3-5 como salidas (PWM)
    PWM_Set_Low_All();
}

/*
 * Función: PWM_Set_High_All
 * Descripción: Pone todos los pines PWM en alto.
 */
void PWM_Set_High_All(void) {
    GPIOB->PCOR = (1 << 3) | (1 << 4) | (1 << 5);
}

/*
 * Función: PWM_Set_Low_All
 * Descripción: Pone todos los pines PWM en bajo.
 */
void PWM_Set_Low_All(void) {
    GPIOB->PSOR = (1 << 3) | (1 << 4) | (1 << 5);
}

/*
 * Función: PWM_Pin_Set_Toggle
 * Descripción: Conmuta el estado de un pin PWM.
 * Parámetros:
 * - pin: Número del pin (3-5 para PTB3-PTB5).
 */
void PWM_Pin_Set_Toggle(uint8_t pin) {
    GPIOB->PTOR = (1 << pin);
}

/*
 * Función: Red
 * Descripción: Enciende el LED rojo.
 */
void Red(void) {
    GPIOC->PCOR = (1 << 9);
    GPIOA->PSOR = (1 << 11);
    GPIOE->PSOR = (1 << 6);
}

/*
 * Función: Blue
 * Descripción: Enciende el LED azul.
 */
void Blue(void) {
    GPIOA->PCOR = (1 << 11);
    GPIOC->PSOR = (1 << 9);
    GPIOE->PSOR = (1 << 6);
}

/*
 * Función: Green
 * Descripción: Enciende el LED verde.
 */
void Green(void) {
    GPIOE->PCOR = (1 << 6);
    GPIOA->PSOR = (1 << 11);
    GPIOC->PSOR = (1 << 9);
}

/*
 * Función: White
 * Descripción: Enciende todos los LEDs.
 */
void White(void) {
    GPIOE->PCOR = (1 << 6);
    GPIOA->PCOR = (1 << 11);
    GPIOC->PCOR = (1 << 9);
}

/*
 * Función: Off
 * Descripción: Apaga todos los LEDs.
 */
void Off(void) {
    GPIOC->PSOR = (1 << 9);
    GPIOA->PSOR = (1 << 11);
    GPIOE->PSOR = (1 << 6);
}
