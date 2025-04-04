/*
 * App.c
 *
 *  Created on: 7 mar. 2025
 *  Author: qcezs
 *
 *  Este archivo contiene la implementación de las funciones declaradas en App.h.
 */

#include "SAE/App.h"
#include "fsl_debug_console.h"
#include <stdlib.h>
#include <string.h>
#include "SAE/data_structures.h"   // Incluye la definición de Queue_t

/* Declaración de la cola de prueba de forma global.
   Se utilizará para almacenar datos de tamaño uint32_t y tendrá capacidad para 10 elementos. */
Queue_t* testQueue = NULL;

/*
 * Función: GPIO_Config
 * Configura el pin 10 del puerto A como entrada con pull-up y habilita la interrupción.
 */
void GPIO_Config(void) {
    port_pin_config_t config = {
        kPORT_PullUp,
        kPORT_SlowSlewRate,
        kPORT_PassiveFilterEnable,
        kPORT_OpenDrainDisable,
        kPORT_LowDriveStrength,
        kPORT_MuxAsGpio,
        kPORT_UnlockRegister,
    };

    uint32_t addr = (uint32_t)&PORTA->PCR[10];
    *(volatile uint16_t *)(addr) = *((const uint16_t *)(const void *)&config);

    PORTA->PCR[10] = (PORTA->PCR[10] & ~PORT_PCR_IRQC_MASK) | PORT_PCR_IRQC(10);
    EnableIRQ(PORTA_IRQn);
}

/*
 * Función: delay
 * Implementa un retardo simple mediante un bucle vacío.
 */
void delay(uint32_t x) {
    for (uint32_t i = 0; i < x; i++);
}

/*
 * Función: Task_A
 * Esta tarea encola un dato en la cola de prueba y activa la Task_B.
 */
void Task_A(void) {
    uint32_t dataToEnqueue = 12345;  // Ejemplo de dato a encolar
    PRINTF("Task_A: Encolando dato %lu\n\r", dataToEnqueue);

    /* Si la cola no fue inicializada, se inicializa aquí */
    if (testQueue == NULL) {
        testQueue = Queue_Init(10, sizeof(uint32_t));
        if (testQueue == NULL) {
            PRINTF("Task_A: Error al inicializar la cola\n\r");
            Terminate_Task();
        }
    }

    /* Intentar encolar el dato */
    if (Queue_Enqueue(testQueue, &dataToEnqueue) == 0) {
        PRINTF("Task_A: Dato encolado correctamente\n\r");
    } else {
        PRINTF("Task_A: Cola llena, no se pudo encolar el dato\n\r");
    }

    /* Activa la Task_B para extraer el dato */
    Activate_Task('B');

    PRINTF("Task_A: Finalizando\n\r");
    Terminate_Task();
}

/*
 * Función: Task_B
 * Esta tarea intenta extraer (desencolar) un dato de la cola de prueba y lo imprime.
 */
void Task_B(void) {
    uint32_t dequeuedData = 0;
    PRINTF("Task_B: Intentando desencolar dato\n\r");

    if (testQueue == NULL) {
        PRINTF("Task_B: Cola no inicializada\n\r");
    } else if (Queue_Dequeue(testQueue, &dequeuedData) == 0) {
        PRINTF("Task_B: Dato desencolado: %lu\n\r", dequeuedData);
    } else {
        PRINTF("Task_B: Cola vacía\n\r");
    }

    Terminate_Task();
}

/*
 * Función: Task_C
 * Esta tarea se utiliza para finalizar la secuencia de prueba.
 */
void Task_C(void) {
    PRINTF("Task_C: Finalizando secuencia de prueba\n\r");
    Terminate_Task();
}

/*
 * Función: Task_ISR
 * Ejecuta una secuencia de luces blancas en respuesta a una interrupción y finaliza.
 */
void Task_ISR(void) {
    PRINTF("Interrupcion alarma 1\n\r");
    Terminate_Task();
}

/*
 * Función: Task_ISR_2
 * Ejecuta una secuencia de luces blancas en respuesta a una interrupción y finaliza.
 */
void Task_ISR_2(void) {
    PRINTF("Interrupcion alarma 2\n\r");
    Terminate_Task();
}

/*
 * Función: PORTA_IRQHandler
 * Manejador de interrupciones para el puerto A.
 */
void PORTA_IRQHandler(void) {
    Save_Context();  // Guardar el contexto actual
    PORTA->ISFR = 0xFFFFFFFF;  // Limpiar las banderas de interrupción
    __DSB();  // Barrera de memoria

    asm("add sp, #0x32");
    Activate_Task_ISR('D');
}
