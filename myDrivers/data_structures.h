/*
 * data_structures.h
 *
 *  Created on: 3 feb. 2025
 *      Author: qcezs
 */

#ifndef DATA_STRUCTURES_H_
#define DATA_STRUCTURES_H_

#include <stdint.h>
#include <stddef.h>

#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

/*
 * Estructuras de datos necesarias para las tareas
 */

/*
 * Enumeración: Autostart
 * Dos estados:
 * - AUTOSTART_ON:	La tarea está activa sin "Activate_Task"
 * - AUTOSTART_OFF:	Se necesita "Activate_Task" para poner en READY la tarea
 */
typedef enum {
	AUTOSTART_ON,
	AUTOSTART_OFF
} Autostart;



/*
 * Enumeración: Task_State
 * Tres estados:
 * - SUSPENDED: La tarea está en modo suspended. No compite en el Scheduler
 * - READY:		La tarea está compitiendo para ejecutarse.
 * - RUN:		La tarea se está ejecutando. Solo puede haber una tarea ejecutandose.
 */
typedef enum {
	SUSPENDED,
	READY,
	RUN
} Task_state;



/*
 * Lista para almacenar el contexto de la/las tareas interrumpidas
 */
typedef struct {
	uint32_t xpsr;
	uint32_t pc;
	uint32_t lr;
	uint32_t sp;
	uint32_t r12;
	uint32_t r3;
	uint32_t r2;
	uint32_t r1;
	uint32_t r0;
} Context;



/*
 * Estructura: Task
 * Datos:
 * - number:			Número de la tarea.
 * - task_ID:			Identificador único que tiene cada tarea.
 * - autostart:			Define si tiene AUTOSTART_ON u AUTOSTART_OFF
 * - priority:			Define el nivel de prioridad de la tarea. Un número más alto define
 * 						prioridad más alta.
 * - task_state:		Define el estado inicial de la tarea. Por defecto, todas inician en
 * 						SUSPENDED
 * - start_address:		Dirección inicial de cada tarea.
 * - return_address:	Dirección de retorno para cuando se tenga que regresar a la tarea.
 * 						En un inicio, return_adress = start_address.
 */
typedef struct Task{
	uint8_t		number;
	char		task_ID;
	Autostart	autostart;
	uint8_t		priority;
	Task_state	task_state;
	void*		start_address;
	void*		return_address;
	uint8_t		interrupted;
	Context		context;

	struct Task* next;
} Task;


/*
 * Lista donde se almacenan todas las tareas
 */
typedef struct {
    Task* head;				// Primer elemento de la lista
    Task* current_task; 	// Puntero a la tarea actual en ejecución
    Task* task_map[];		// Diccionario para acceso rápido por task_ID
} Task_List;



//typedef struct {
//	uint32_t r0;
//	uint32_t r1;
//	uint32_t r2;
//	uint32_t r3;
//	uint32_t r12;
//	uint32_t lr;
//	uint32_t pc;
//	uint32_t xpsr;
//	uint32_t sp;
//} Context;


#endif /* DATA_STRUCTURES_H_ */
