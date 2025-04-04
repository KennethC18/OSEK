/*
 * SO.c
 *
 *  Created on: 13 mar. 2025
 *  Author: qcezs
 *
 *  Este archivo contiene la implementación de las funciones para la gestión de tareas
 *  en el sistema operativo.
 */

#include <Peripherics/Low_Power.h>
#include <Peripherics/RGB.h>
#include <SAE/SO.h>
#include <fsl_debug_console.h>

/* Estructura global que contiene la lista de tareas y el puntero a la tarea actual */
Alarm_List	list_a	= {NULL};
Task_List	list	= {NULL, NULL, {NULL}};

/* Variables globales */
volatile uint8_t Alarm_Activated = 0;

/*
 * Función: Create_Task
 * Descripción: Crea una nueva tarea, la inicializa y la agrega a la lista de tareas,
 *              ordenándola por prioridad.
 * Parámetros:
 * - number: Número de la tarea.
 * - task_ID: Identificador único de la tarea (un carácter).
 * - autostart: Indica si la tarea debe iniciar automáticamente (AUTOSTART_ON) o no (AUTOSTART_OFF).
 * - priority: Prioridad de la tarea. Un valor más alto indica mayor prioridad.
 * - start_address: Dirección de la función que representa la tarea.
 * Retorno: Ninguno
 */
void Create_Task(uint8_t number, char task_ID, Autostart autostart, uint8_t priority, void* start_address) {
    // Reservar memoria para la nueva tarea
    Task* new_task = (Task*)malloc(sizeof(Task));
    if (new_task == NULL) {
        return;  // En caso de fallo, salir
    }

    // Inicializar los campos de la nueva tarea
    new_task->number = number;
    new_task->task_ID = task_ID;
    new_task->autostart = autostart;
    new_task->priority = priority;
    new_task->task_state = SUSPENDED;
    new_task->start_address = start_address;
    new_task->return_address = start_address;  // Inicialmente, return_address = start_address
    new_task->interrupted = 0;
    new_task->next = NULL;

    // Inicializar el contexto de la tarea
    memset(&new_task->context, 0, sizeof(Context));

    // Si la tarea tiene AUTOSTART habilitado, cambiar su estado a READY
    if (autostart == AUTOSTART_ON) {
        new_task->task_state = READY;
    }

    // Agregar la tarea al diccionario para acceso rápido
    list.task_map[task_ID - 'A'] = new_task;

    // Insertar la tarea en la lista, ordenada por prioridad
    if (list.head == NULL || list.head->priority < priority) {
        // Insertar al inicio si la lista está vacía o la nueva tarea tiene mayor prioridad
        new_task->next = list.head;
        list.head = new_task;
    } else {
        // Buscar la posición correcta en la lista
        Task* current = list.head;
        while (current->next != NULL && current->next->priority >= priority) {
            current = current->next;
        }
        // Insertar la tarea en la posición correcta
        new_task->next = current->next;
        current->next = new_task;
    }
}

/*
 * Función: Activate_Task
 * Descripción: Activa una tarea especificada por su identificador. La tarea actual pasa de RUN a READY,
 *              y la tarea especificada pasa de SUSPENDED a READY. Guarda el contexto de la tarea actual.
 * Parámetros:
 * - task_ID: Identificador único de la tarea a activar.
 * Retorno: Ninguno
 */
void Activate_Task(char task_ID) {
    if (list.current_task != NULL) {
        // Guardar la dirección de retorno de la tarea actual
        asm("ldr r5,[sp, #20]");  // Cargar dirección de retorno en r5
        asm("sub r5, #1");        // Ajustar lr
        asm("orr r5, r5, #1");    // Evitar Hardfault haciendo OR con 1

        asm("ldr r6,=list");      // Cargar dirección base de 'list'
        asm("ldr r6,[r6,#4]");    // Cargar 'list.current_task' (Offset 4)
        asm("str r5,[r6,#12]");   // Guardar en return_address (Offset 16 en 'Task')

        // Cambiar el estado de la tarea actual a READY
        list.current_task->task_state = READY;
    }

    // Activar la tarea especificada por task_ID
    Task* task = list.task_map[task_ID - 'A'];
    if (task != NULL) {
        task->task_state = READY;
    }

    // Ajustar el puntero de la pila para liberar espacio
    asm("add sp, #16");

    // Llamar al Scheduler para ejecutar la tarea con mayor prioridad
    Scheduler();
}

/*
 * Función: Activate_Task_ISR
 * Descripción: Activa una tarea especificada por su identificador desde una interrupción (ISR).
 *              La tarea actual pasa de RUN a READY, y la tarea especificada pasa de SUSPENDED a READY.
 *              No guarda el contexto de la tarea actual.
 * Parámetros:
 * - task_ID: Identificador único de la tarea a activar.
 * Retorno: Ninguno
 */
void Activate_Task_ISR(char task_ID) {
    if (list.current_task != NULL) {
        // Cambiar el estado de la tarea actual a READY
        list.current_task->task_state = READY;
    }

    // Activar la tarea especificada por task_ID
    Task* task = list.task_map[task_ID - 'A'];
    if (task != NULL) {
        task->task_state = READY;
    }

    // Ajustar el puntero de la pila para liberar espacio
    asm("add sp, #16");

    // Llamar al Scheduler para ejecutar la tarea con mayor prioridad
    Scheduler();
}

/*
 * Función: Chain_Task
 * Descripción: Suspende la tarea actual y activa la tarea especificada por su identificador.
 *              La tarea actual pasa de RUN a SUSPENDED, y la tarea especificada pasa de SUSPENDED a READY.
 * Parámetros:
 * - task_ID: Identificador único de la tarea a activar.
 * Retorno: Ninguno
 */
void Chain_Task(char task_ID) {
    // Suspender la tarea actual
    if (list.current_task != NULL) {
        list.current_task->task_state = SUSPENDED;
    }

    // Activar la tarea especificada por task_ID
    Task* task = list.task_map[task_ID - 'A'];
    if (task != NULL) {
        task->task_state = READY;
    }

    // Ajustar el puntero de la pila para liberar espacio
    asm("add sp, #16");

    // Llamar al Scheduler para ejecutar la tarea con mayor prioridad
    Scheduler();
}

/*
 * Función: Terminate_Task
 * Descripción: Suspende la tarea actual. La tarea actual pasa de RUN a SUSPENDED.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void Terminate_Task(void) {
    // Suspender la tarea actual
    if (list.current_task != NULL) {
        list.current_task->task_state = SUSPENDED;
    }

    // Ajustar el puntero de la pila para liberar espacio
    asm("add sp, #8");

    // Llamar al Scheduler para ejecutar la siguiente tarea
    Scheduler();
}

/*
 * Función: Scheduler
 * Descripción: Selecciona y ejecuta la tarea con mayor prioridad que esté en estado READY.
 *              Si no hay tareas listas para ejecutarse, entra en modo de bajo consumo.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void Scheduler(void) {
    Task* current = list.head;

    // Buscar la tarea con mayor prioridad en estado READY
    while (current != NULL) {
        if (current->task_state == READY) {
            current->task_state = RUN;

            // Guardar la nueva tarea actual en list.current_task
            asm("ldr r5,=list");                     // Dirección base de 'list'
            asm("str %0,[r5,#4]" :: "r"(current));   // Guardar current en list.current_task

            // Verificar si la tarea fue interrumpida
            if (current->interrupted) {
                // Marcar la tarea como no interrumpida
                current->interrupted = 0;
                // Restaurar el contexto de la tarea interrumpida
                Restore_Context();
            }

            // Guardar la dirección de retorno en list.current_task->return_address
            if (current->return_address != NULL) {
                asm("ldr r5, = list");      // Dirección base de 'list'
                asm("ldr r5, [r5, #4]");    // Cargar 'list.current_task'
                asm("ldr r6, [r5, #12]");   // Cargar return_address (Offset 12)

                asm("mov lr,r6");           // Ajustar dirección de retorno
                asm("bx lr");               // Saltar a la dirección de retorno
            }
            return;
        }
        current = current->next;
    }

    // Si no hay tareas listas para ejecutarse, entrar en modo de bajo consumo
    sleep();
}

/*
 * Función: SO_Init
 * Descripción: Inicializa el sistema operativo y llama al Scheduler para comenzar la gestión de tareas.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void SO_Init(void) {
    Scheduler();
}

/*
 * Función: Save_Context
 * Descripción: Guarda el contexto de la tarea que fue interrumpida por una ISR.
 *              Debe llamarse antes de Activate_Task_ISR.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void Save_Context(void) {
    // Marcar la tarea actual como interrumpida
    if (list.current_task != NULL) {
        list.current_task->interrupted = 1;
    }

    // Ajustar el puntero de la pila para apuntar a los datos guardados antes de la interrupción
    asm("mov r6, sp");
    asm("add r6, #0x2c");

    // Cargar los datos y guardar en la estructura de contexto
    asm("ldr r8, = list");  // Apuntar a la dirección base de 'list'
    asm("add r8, #0x12c");  // Apuntar al contexto de la tarea actual/interrumpida

    // Guardar XPSR
    asm("mrs r9, xpsr");
    asm("str r9, [r8]");

    // Guardar PC
    asm("ldr r9, [r6, #-8]");
    asm("str r9, [r8, #4]");

    // Guardar LR
    asm("ldr r9, [r6, #-12]");
    asm("str r9, [r8, #8]");

    // Guardar SP
    asm("ldr r9, [r6, #-40]");
    asm("str r9, [r8, #12]");

    // Guardar R12
    asm("ldr r9, [r6, #-16]");
    asm("str r9, [r8, #16]");

    // Guardar R3
    asm("ldr r9, [r6, #-20]");
    asm("str r9, [r8, #20]");

    // Guardar R2
    asm("ldr r9, [r6, #-24]");
    asm("str r9, [r8, #24]");

    // Guardar R1
    asm("ldr r9, [r6, #-28]");
    asm("str r9, [r8, #28]");

    // Guardar R0
    asm("ldr r9, [r6, #-32]");
    asm("str r9, [r8, #32]");
}

/*
 * Función: Restore_Context
 * Descripción: Recupera el contexto de la tarea que fue interrumpida.
 *              Se llama dentro del Scheduler.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void Restore_Context(void) {
    // Cargar la dirección de list.current_task->context
    asm("ldr r8, = list");  // Cargar la dirección de 'list'
    asm("add r8, #0x12c");  // Apuntar al contexto de la tarea interrumpida

    // Restaurar XPSR
    asm("ldr r9, [r8]");

    // Restaurar LR
    asm("ldr r9, [r8, #8]");
    asm("mov lr, r9");

    // Restaurar SP
    asm("ldr r9, [r8, #12]");
    asm("mov sp, r9");

    // Restaurar R12
    asm("ldr r9, [r8, #16]");
    asm("mov r12, r9");

    // Restaurar R3
    asm("ldr r9, [r8, #20]");
    asm("mov r3, r9");

    // Restaurar R2
    asm("ldr r9, [r8, #24]");
    asm("mov r2, r9");

    // Restaurar R1
    asm("ldr r9, [r8, #28]");
    asm("mov r1, r9");

    // Restaurar R0
    asm("ldr r9, [r8, #32]");
    asm("mov r0, r9");

    // Restaurar PC
    asm("ldr r9, [r8, #4]");
    asm("mov pc, r9");
}



/*
 * Funciones para alarmas
 * time (ticks)
 */
void Set_Alarm(char task_ID, uint16_t time, Enable_Alarm enable) {
    // Reservar memoria para la nueva tarea
    Alarm* new_alarm = (Alarm*)malloc(sizeof(Alarm));
    if (new_alarm == NULL) {
        return;  // En caso de fallo, salir
    }

    // Inicializar los campos de la nueva tarea
    new_alarm->task_ID			= task_ID;
    new_alarm->original_time	= time;
    new_alarm->remain_time		= time;
    new_alarm->enable			= enable;
    new_alarm->alarm_activated  = 0;

    new_alarm->next				= NULL;


    // Agregar la alarma a la lista
    if (list_a.head == NULL) {
        list_a.head = new_alarm;
    } else {
        Alarm* current = list_a.head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_alarm;
    }
}


/*
 * Función: Queue_Init
 * Descripción: Inicializa una cola FIFO con una longitud máxima y tamaño de elemento fijos.
 * Parámetros:
 *   - length: Número máximo de elementos en la cola.
 *   - item_size: Tamaño en bytes de cada elemento.
 * Retorno:
 *   - Puntero a la cola inicializada o NULL si falla la asignación.
 */
Queue_t* Queue_Init(uint32_t length, uint32_t item_size) {
    Queue_t* q = (Queue_t*)malloc(sizeof(Queue_t));
    if (q == NULL) {
        return NULL;
    }
    q->length = length;
    q->item_size = item_size;
    q->head = 0;
    q->tail = 0;
    q->count = 0;
    q->buffer = (uint8_t*)malloc(length * item_size);
    if (q->buffer == NULL) {
        free(q);
        return NULL;
    }
    return q;
}

/*
 * Función: Queue_IsEmpty
 * Descripción: Verifica si la cola está vacía.
 */
bool Queue_IsEmpty(Queue_t* q) {
    return (q->count == 0);
}

/*
 * Función: Queue_IsFull
 * Descripción: Verifica si la cola está llena.
 */
bool Queue_IsFull(Queue_t* q) {
    return (q->count == q->length);
}

/*
 * Función: Queue_Enqueue
 * Descripción: Inserta un elemento en el final de la cola.
 * Parámetros:
 *   - q: Puntero a la cola.
 *   - item: Puntero a los datos a insertar (de tamaño item_size).
 * Retorno:
 *   - 0 si se encola correctamente o -1 si la cola está llena.
 */
int Queue_Enqueue(Queue_t* q, const void* item) {
    if (Queue_IsFull(q)) {
        return -1; // Cola llena
    }
    // Copia el elemento al buffer en la posición tail
    memcpy(q->buffer + (q->tail * q->item_size), item, q->item_size);
    q->tail = (q->tail + 1) % q->length;
    q->count++;
    return 0;
}

/*
 * Función: Queue_Dequeue
 * Descripción: Extrae el elemento del frente de la cola.
 * Parámetros:
 *   - q: Puntero a la cola.
 *   - item: Puntero al buffer donde se copiará el elemento extraído.
 * Retorno:
 *   - 0 si se desencola correctamente o -1 si la cola está vacía.
 */
int Queue_Dequeue(Queue_t* q, void* item) {
    if (Queue_IsEmpty(q)) {
        return -1; // Cola vacía
    }
    // Copia el elemento del buffer en la posición head
    memcpy(item, q->buffer + (q->head * q->item_size), q->item_size);
    q->head = (q->head + 1) % q->length;
    q->count--;
    return 0;
}

/*
 * Función: Queue_Free
 * Descripción: Libera la memoria asignada a la cola.
 */
void Queue_Free(Queue_t* q) {
    if (q != NULL) {
        free(q->buffer);
        free(q);
    }
}



/*
 *
 */
void PIT0_IRQHandler(void) {
	Alarm* current = list_a.head;

	while(current != NULL) {
		if(current->enable == ENABLED) {
			current->remain_time--;
		}

		if(current->remain_time == 0) {
		    Task* task = list.task_map[current->task_ID - 'A'];

			current->remain_time = current->original_time;
			task->task_state = READY;
			PRINTF("Alarma activada para tarea %c\n\r", task->task_ID);

			current->alarm_activated = 1;
		}
		current = current->next;
	}

	/* Clean Interruption Flag*/
	*PIT_TFLG0 = 1;

//	if(Alarm_Activated) {
//		NVIC_DisableIRQ(PIT0_IRQn);
//		Save_Context();
//		asm("add sp, #0x28");
//		Scheduler();
//	}
	while(current != NULL) {
		if(current->alarm_activated == 1) {
			NVIC_DisableIRQ(PIT0_IRQn);
			current->alarm_activated = 0;
			Save_Context();
			asm("add sp, #0x28");
			Scheduler();
		}
	}
}
