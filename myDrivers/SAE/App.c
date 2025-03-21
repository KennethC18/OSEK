/*
 * App.c
 *
 *  Created on: 7 mar. 2025
 *  Author: qcezs
 *
 *  Este archivo contiene la implementación de las funciones declaradas en App.h.
 */

#include <SAE/App.h>

/*
 * Función: GPIO_Config
 * Descripción: Configura el pin 10 del puerto A como entrada con pull-up, filtro pasivo habilitado
 *              y configura la interrupción en el flanco de bajada.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void GPIO_Config(void) {
    // Configuración del pin 10 del puerto A
    port_pin_config_t config = {
        kPORT_PullUp,                      // Resistencia pull-up habilitada
        kPORT_SlowSlewRate,                // Tasa de slew lenta
        kPORT_PassiveFilterEnable,         // Filtro pasivo habilitado
        kPORT_OpenDrainDisable,            // Modo open-drain deshabilitado
        kPORT_LowDriveStrength,            // Baja fuerza de manejo
        kPORT_MuxAsGpio,                   // Configuración como GPIO
        kPORT_UnlockRegister,              // Desbloqueo del registro
    };

    // Aplicar la configuración al pin 10 del puerto A
    uint32_t addr = (uint32_t)&PORTA->PCR[10];
    *(volatile uint16_t *)(addr) = *((const uint16_t *)(const void *)&config);

    // Configurar la interrupción en el pin 10 del puerto A
    PORTA->PCR[10] = (PORTA->PCR[10] & ~PORT_PCR_IRQC_MASK) | PORT_PCR_IRQC(10);

    // Habilitar la interrupción del puerto A
    EnableIRQ(PORTA_IRQn);
}

/*
 * Función: delay
 * Descripción: Implementa un retardo simple mediante un bucle vacío.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void delay(uint32_t x) {
    for (uint32_t i = 0; i < x; i++);
}

/*
 * Función: Task_A
 * Descripción: Ejecuta una secuencia de luces rojas, introduce un retardo y activa la Task_B.
 *              Luego, repite la secuencia de luces rojas y apaga las luces antes de finalizar.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void Task_A(void) {
//	Red();
//	delay();
//	Off();
//	delay();
//	Activate_Task('B');
//
//	Red();
//	delay();
//	Off();
//	Terminate_Task();

	GPIO_B->PSOR = (1 << 7) | (1 << 6) | (1 << 5) | (1 << 4) | (1 << 3);
	//delay(750);
	Terminate_Task();
}

/*
 * Función: Task_B
 * Descripción: Ejecuta una secuencia de luces azules, introduce un retardo y apaga las luces.
 *              Luego, encadena la ejecución de la Task_C.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void Task_B(void) {
//    Blue();         	// Encender luz azul
//    delay();        	// Introducir retardo
//    Off();          	// Apagar luces
//    Chain_Task('C');  	// Encadenar la ejecución de Task_C

	GPIO_B->PCOR = (1 << 7);
	Terminate_Task();
}

/*
 * Función: Task_C
 * Descripción: Ejecuta una secuencia de luces verdes, introduce un retardo y apaga las luces.
 *              Luego, finaliza la tarea.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void Task_C(void) {
//    Green();        	// Encender luz verde
//    delay();        	// Introducir retardo
//    Off();          	// Apagar luces
//    Terminate_Task();   // Finalizar la tarea

	GPIO_B->PCOR = (1 << 6);
	Terminate_Task();
}

void Task_D(void) {
	GPIO_B->PCOR = (1 << 5);
	Terminate_Task();
}

void Task_E(void) {
	GPIO_B->PCOR = (1 << 4);
	Terminate_Task();
}

void Task_F(void) {
	GPIO_B->PCOR = (1 << 3);
	Terminate_Task();
}

void Task_G(void) {
	GPIO_B->PTOR = (1 << 2);
	Terminate_Task();
}

/*
 * Función: Task_ISR
 * Descripción: Ejecuta una secuencia de luces blancas en respuesta a una interrupción,
 *              introduce un retardo y apaga las luces antes de finalizar.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void Task_ISR(void) {
    White();        	// Encender luz blanca
    //delay();        	// Introducir retardo
    Off();          	// Apagar luces
    Terminate_Task();   // Finalizar la tarea
}

/*
 * Función: PORTA_IRQHandler
 * Descripción: Manejador de interrupciones para el puerto A. Guarda el contexto, limpia las
 *              banderas de interrupción y activa la tarea correspondiente (Task_ISR).
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void PORTA_IRQHandler(void) {
    Save_Context();  // Guardar el contexto actual

    // Limpiar todas las banderas de interrupción
    PORTA->ISFR = 0xFFFFFFFF;
    __DSB();        // Barrera de memoria

    // Ajustar el puntero de la pila (asm inline)
    asm("add sp, #0x32");

    // Activar la tarea correspondiente a la interrupción
    Activate_Task_ISR('D');
}
