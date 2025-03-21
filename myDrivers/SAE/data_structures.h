/*
 * data_structures.h
 *
 *  Created on: 3 feb. 2025
 *  Author: qcezs
 *
 *  Este archivo contiene las definiciones de las estructuras de datos y enumeraciones
 *  utilizadas para la gestión de tareas en el sistema.
 */

#ifndef SAE_DATA_STRUCTURES_H_
#define SAE_DATA_STRUCTURES_H_

#include <stdint.h>
#include <stddef.h>

// Incluir archivos de configuración del hardware
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

/*
 * Enumeración: Autostart
 * Descripción: Define si una tarea debe iniciar automáticamente o no.
 * Valores:
 * - AUTOSTART_ON:  La tarea se activa automáticamente sin necesidad de llamar a "Activate_Task".
 * - AUTOSTART_OFF: La tarea requiere una llamada a "Activate_Task" para ser puesta en estado READY.
 */
typedef enum {
    AUTOSTART_ON,
    AUTOSTART_OFF
} Autostart;

typedef enum {
	ENABLED,
	DISABLED
} Enable_Alarm;

/*
 * Enumeración: Task_State
 * Descripción: Define los posibles estados de una tarea.
 * Valores:
 * - SUSPENDED: La tarea está suspendida y no compite en el scheduler.
 * - READY:     La tarea está lista para ejecutarse y compite en el scheduler.
 * - RUN:       La tarea se está ejecutando actualmente. Solo una tarea puede estar en este estado.
 */
typedef enum {
    SUSPENDED,
    READY,
    RUN
} Task_state;

/*
 * Estructura: Context
 * Descripción: Almacena el contexto de una tarea interrumpida, incluyendo los registros del procesador.
 * Campos:
 * - xpsr: Registro de estado del programa.
 * - pc:  Contador de programa (Program Counter).
 * - lr:  Registro de enlace (Link Register).
 * - sp:  Puntero de pila (Stack Pointer).
 * - r12: Registro de uso general R12.
 * - r3:  Registro de uso general R3.
 * - r2:  Registro de uso general R2.
 * - r1:  Registro de uso general R1.
 * - r0:  Registro de uso general R0.
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
 * Descripción: Representa una tarea en el sistema.
 * Campos:
 * - number:         Número de la tarea.
 * - task_ID:        Identificador único de la tarea.
 * - autostart:      Indica si la tarea debe iniciar automáticamente (AUTOSTART_ON) o no (AUTOSTART_OFF).
 * - priority:       Prioridad de la tarea. Un valor más alto indica mayor prioridad.
 * - task_state:     Estado actual de la tarea (SUSPENDED, READY, RUN).
 * - start_address:  Dirección de inicio de la tarea.
 * - return_address: Dirección de retorno de la tarea. Inicialmente igual a start_address.
 * - interrupted:    Indica si la tarea fue interrumpida.
 * - context:        Contexto de la tarea (registros del procesador).
 * - next:           Puntero a la siguiente tarea en la lista.
 */
typedef struct Task {
    uint8_t     number;
    char        task_ID;
    Autostart   autostart;
    uint8_t     priority;
    Task_state  task_state;
    void*       start_address;
    void*       return_address;
    uint8_t     interrupted;
    Context     context;
    struct Task* next;
} Task;

/*
 * Estructura: Task_List
 * Descripción: Representa una lista de tareas en el sistema.
 * Campos:
 * - head:          Puntero al primer elemento de la lista.
 * - current_task:  Puntero a la tarea actual en ejecución.
 * - task_map:      Diccionario para acceso rápido a las tareas por su task_ID.
 */
typedef struct {
    Task* head;             // Primer elemento de la lista
    Task* current_task;     // Tarea actual en ejecución
    Task* task_map[];       // Diccionario para acceso rápido por task_ID
} Task_List;



/*
 * Estructuras para alarma
 */
typedef struct Alarm {
	char			task_ID;
	uint16_t		original_time;
	uint16_t		remain_time;
	Enable_Alarm	enable;

	struct Alarm*	next;
} Alarm;

typedef struct {
	Alarm* head;
} Alarm_List;

#endif /* SAE_DATA_STRUCTURES_H_ */
