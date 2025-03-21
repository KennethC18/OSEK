/*
 * SO.h
 *
 *  Created on: 3 feb. 2025
 *      Author: qcezs
 */

#ifndef SO_H_
#define SO_H_

#include "data_structures.h"

/* Función para crear una tarea */
void Create_Task(uint8_t number, char task_ID, Autostart autostart, uint8_t priority,
				 void* start_address);


/* Función para activar una tarea especificada por 'task_ID'
 * - La tarea actual pasa de RUN -> READY
 * - La tarea especificada pasa de SUSPENDED -> READY
 * - Activate_Task GUARDA el contexto dentro de la función
 * */
void Activate_Task(char task_ID);


/* Función para activar una tarea especificada por 'task_ID' DESDE la ISR
 * - La tarea actual pasa de RUN -> READY
 * - La tarea especificada pasa de SUSPENDED -> READY
 * - Activate_Task_ISR NO GUARDA el contexto dentro de la función
 * */
void Activate_Task_ISR(char task_ID);


/* Función para terminar la tarea actual, y activar la tarea especificada por 'task_ID'
 * - La tarea actual pasa de RUN -> SUSPENDED
 * - La tarea especificada pasa de SUSPENDED -> READY
 * */
void Chain_Task(char task_ID);


/* Función para terminar la tarea actual
 * - La tarea actual pasa de RUN -> SUSPENDED
 * */
void Terminate_Task(void);


/* Función para determinar cuál tarea se va a ejecutar
 * - Siempre se tiene que ejecutar la tarea que cumpla las dos condiciones:
 * 		1. La tarea tiene que tener la MAYOR prioridad
 * 		2. La tarea tiene que estar en estado READY
 * */
void Scheduler(void);


/* Función que llama al Scheduler e inicia la gestión de tareas */
void SO_Init(void);


/*
 * Función para guardar el contexto de la tarea que fue interrumpida por la ISR
 * Se tiene que llamar ANTES que Activate_Task_ISR
 */
void Save_Context(void);

/*
 * Función para recuperar el contexto de la tarea que fue interrumpida
 * Se llama dentro del Scheduler.
 */
void Restore_Context(void);

#endif /* SO_H_ */
