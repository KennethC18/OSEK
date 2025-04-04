/*
 * SO.h
 *
 *  Created on: 3 feb. 2025
 *  Author: qcezs
 *
 *  Este archivo contiene las declaraciones de las funciones para la gestión de tareas
 *  en el sistema operativo.
 */

#ifndef SAE_SO_H_
#define SAE_SO_H_

#include <SAE/data_structures.h>
#include <Peripherics/PIT.h>

/*
 * Función: Create_Task
 * Descripción: Crea una nueva tarea y la agrega a la lista de tareas, ordenándola por prioridad.
 * Parámetros:
 * - number: Número de la tarea.
 * - task_ID: Identificador único de la tarea (un carácter).
 * - autostart: Indica si la tarea debe iniciar automáticamente (AUTOSTART_ON) o no (AUTOSTART_OFF).
 * - priority: Prioridad de la tarea. Un valor más alto indica mayor prioridad.
 * - start_address: Dirección de la función que representa la tarea.
 * Retorno: Ninguno
 */
void Create_Task(uint8_t number, char task_ID, Autostart autostart, uint8_t priority, void* start_address);

/*
 * Función: Activate_Task
 * Descripción: Activa una tarea especificada por su identificador. La tarea actual pasa de RUN a READY,
 *              y la tarea especificada pasa de SUSPENDED a READY. Guarda el contexto de la tarea actual.
 * Parámetros:
 * - task_ID: Identificador único de la tarea a activar.
 * Retorno: Ninguno
 */
void Activate_Task(char task_ID);

/*
 * Función: Activate_Task_ISR
 * Descripción: Activa una tarea especificada por su identificador desde una interrupción (ISR).
 *              La tarea actual pasa de RUN a READY, y la tarea especificada pasa de SUSPENDED a READY.
 *              No guarda el contexto de la tarea actual.
 * Parámetros:
 * - task_ID: Identificador único de la tarea a activar.
 * Retorno: Ninguno
 */
void Activate_Task_ISR(char task_ID);

/*
 * Función: Chain_Task
 * Descripción: Suspende la tarea actual y activa la tarea especificada por su identificador.
 *              La tarea actual pasa de RUN a SUSPENDED, y la tarea especificada pasa de SUSPENDED a READY.
 * Parámetros:
 * - task_ID: Identificador único de la tarea a activar.
 * Retorno: Ninguno
 */
void Chain_Task(char task_ID);

/*
 * Función: Terminate_Task
 * Descripción: Suspende la tarea actual. La tarea actual pasa de RUN a SUSPENDED.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void Terminate_Task(void);

/*
 * Función: Scheduler
 * Descripción: Selecciona y ejecuta la tarea con mayor prioridad que esté en estado READY.
 *              Si no hay tareas listas para ejecutarse, entra en modo de bajo consumo.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void Scheduler(void);

/*
 * Función: SO_Init
 * Descripción: Inicializa el sistema operativo y llama al Scheduler para comenzar la gestión de tareas.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void SO_Init(void);

/*
 * Función: Save_Context
 * Descripción: Guarda el contexto de la tarea que fue interrumpida por una ISR.
 *              Debe llamarse antes de Activate_Task_ISR.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void Save_Context(void);

/*
 * Función: Restore_Context
 * Descripción: Recupera el contexto de la tarea que fue interrumpida.
 *              Se llama dentro del Scheduler.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void Restore_Context(void);



/*
 * Funciones para tiempo
 */
void Set_Alarm(char task_ID, uint16_t time, Enable_Alarm enable);	// time en ticks
void Check_Alarms(void);


/*
 * Función: Queue_Init
 * Descripción: Inicializa una cola FIFO con una longitud máxima y tamaño de elemento fijos.
 * Parámetros:
 *   - length: Número máximo de elementos en la cola.
 *   - item_size: Tamaño en bytes de cada elemento.
 * Retorno:
 *   - Puntero a la cola inicializada o NULL si falla la asignación.
 */
Queue_t* Queue_Init(uint32_t length, uint32_t item_size);

/*
 * Función: Queue_IsEmpty
 * Descripción: Verifica si la cola está vacía.
 */
bool Queue_IsEmpty(Queue_t* q);

/*
 * Función: Queue_IsFull
 * Descripción: Verifica si la cola está llena.
 */
bool Queue_IsFull(Queue_t* q);

/*
 * Función: Queue_Enqueue
 * Descripción: Inserta un elemento en el final de la cola.
 * Parámetros:
 *   - q: Puntero a la cola.
 *   - item: Puntero a los datos a insertar (de tamaño item_size).
 * Retorno:
 *   - 0 si se encola correctamente o -1 si la cola está llena.
 */
int Queue_Enqueue(Queue_t* q, const void* item);

/*
 * Función: Queue_Dequeue
 * Descripción: Extrae el elemento del frente de la cola.
 * Parámetros:
 *   - q: Puntero a la cola.
 *   - item: Puntero al buffer donde se copiará el elemento extraído.
 * Retorno:
 *   - 0 si se desencola correctamente o -1 si la cola está vacía.
 */
int Queue_Dequeue(Queue_t* q, void* item);

/*
 * Función: Queue_Free
 * Descripción: Libera la memoria asignada a la cola.
 */
void Queue_Free(Queue_t* q);

#endif /* SAE_SO_H_ */
