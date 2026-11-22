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
#include <SAE/SO.h>
#include <fsl_debug_console.h>
#include <Peripherics/GPIO.h>
#include <stdlib.h>
#include <string.h>

/* Estructura global que contiene la lista de tareas y el puntero a la tarea actual */
Alarm_List list_a = {NULL};
Task_List list = {NULL, NULL, {NULL}};

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

    // Asignar stack para la tarea (512 bytes)
    uint32_t* stack = (uint32_t*)malloc(512);
    if (stack == NULL) {
        free(new_task);
        return;
    }

    // Inicializar los campos de la nueva tarea
    new_task->number = number;
    new_task->task_ID = task_ID;
    new_task->autostart = autostart;
    new_task->priority = priority;
    new_task->task_state = SUSPENDED;
    new_task->start_address = start_address;
    new_task->return_address = start_address;
    new_task->interrupted = 0;
    new_task->next = NULL;

    // Inicializar el contexto de la tarea
    memset(&new_task->context, 0, sizeof(Context));
    new_task->context.sp = (uint32_t)(stack + 512/4); // Apuntar al tope del stack

    // Si la tarea tiene AUTOSTART habilitado, cambiar su estado a READY
    if (autostart == AUTOSTART_ON) {
        new_task->task_state = READY;
    }

    // Agregar la tarea al diccionario para acceso rápido
    list.task_map[task_ID - 'A'] = new_task;

    // Insertar la tarea en la lista, ordenada por prioridad
    if (list.head == NULL || list.head->priority < priority) {
        new_task->next = list.head;
        list.head = new_task;
    } else {
        Task* current = list.head;
        while (current->next != NULL && current->next->priority >= priority) {
            current = current->next;
        }
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
        asm volatile (
            "ldr r5, [sp, #20]\n"    // Cargar dirección de retorno
            "sub r5, #1\n"           // Ajustar lr
            "orr r5, r5, #1\n"       // Asegurar modo Thumb
            "ldr r6, =list\n"        // Dirección de 'list'
            "ldr r6, [r6, #4]\n"     // Cargar list.current_task
            "str r5, [r6, #12]\n"    // Guardar en return_address
        );
        list.current_task->task_state = READY;
    }

    // Activar la tarea especificada por task_ID
    Task* task = list.task_map[task_ID - 'A'];
    if (task != NULL) {
        task->task_state = READY;
    }

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
        list.current_task->task_state = READY;
    }

    Task* task = list.task_map[task_ID - 'A'];
    if (task != NULL) {
        task->task_state = READY;
    }


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
    if (list.current_task != NULL) {
        list.current_task->task_state = SUSPENDED;
    }

    Task* task = list.task_map[task_ID - 'A'];
    if (task != NULL) {
        task->task_state = READY;
    }

    Scheduler();
}

/*
 * Función: Terminate_Task
 * Descripción: Suspende la tarea actual. La tarea actual pasa de RUN a SUSPENDED.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void Terminate_Task(void) {
    if (list.current_task != NULL) {
        list.current_task->task_state = SUSPENDED;
    }
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
            list.current_task = current;

            if (current->interrupted) {
                current->interrupted = 0;
                Restore_Context();
            } else {
                asm volatile (
                    "ldr r5, =list\n"
                    "ldr r5, [r5, #4]\n"     // Cargar list.current_task
                    "ldr r6, [r5, #12]\n"    // Cargar return_address
                    "mov lr, r6\n"
                    "bx lr\n"
                );
            }
            return;
        }
        current = current->next;
    }

    // Si no hay tareas listas, entrar en modo de bajo consumo
    asm volatile ("wfi");
}

/*
 * Función: SO_Init
 * Descripción: Inicializa el sistema operativo y llama al Scheduler para comenzar la gestión de tareas.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void SO_Init(void) {
    // Configurar el procesador para usar PSP en modo Thread
	asm volatile ("mov r0, sp");
	asm volatile ("msr psp, r0");
	asm volatile ("mrs r0, control");          // Establecer PSP
	asm volatile ("orrs r0, r0, #0x2");           // Usar PSP en modo Thread
	asm volatile ("msr control, r0");
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
    if (list.current_task != NULL) {
        list.current_task->interrupted = 1;
    }

    asm volatile ("mrs r6, psp\n");              // Cargar PSP en r6
	asm volatile ("ldr r8, =list\n");            // Dirección base de 'list'
	asm volatile ("ldr r8, [r8, #4]\n");         // Cargar list.current_task
	asm volatile ("add r8, #20\n");              // Apuntar a list.current_task->context
        // Guardar registros apilados automáticamente (stack frame)
	asm volatile ("ldmia r6!, {r0-r3}\n");       // Cargar R0-R3 desde PSP, incrementar r6
	asm volatile ("stmia r8!, {r0-r3}\n");       // Guardar R0-R3 en context.r0-r3
	asm volatile ("ldmia r6!, {r0-r3}\n");       // Cargar R12, LR, PC, xPSR
	asm volatile ("stmia r8!, {r0-r3}\n");       // Guardar R12, LR, PC
	asm volatile ("stmia r8, {r0}\n");           // Guardar xPSR
	asm volatile ("str r6, [r8, #12]\n");        // Guardar PSP actualizado en context.sp
}

/*
 * Función: Restore_Context
 * Descripción: Recupera el contexto de la tarea que fue interrumpida.
 *              Se llama dentro del Scheduler.
 * Parámetros: Ninguno
 * Retorno: Ninguno
 */
void Restore_Context(void) {
    asm volatile ("ldr r8, =list\n");            // Cargar dirección de 'list'
    asm volatile ("ldr r8, [r8, #4]\n");         // Cargar list.current_task
    asm volatile ("add r8, #20\n");              // Apuntar a list.current_task->context
    asm volatile ("ldmia r8!, {r0-r3}\n");       // Cargar R0-R3
	asm volatile ("ldmia r8!, {r4-r7}\n");       // Cargar R12, LR, PC, xPSR
	asm volatile ("ldr r8, [r8, #12]\n");        // Cargar context.sp en r8
	asm volatile ("msr psp, r8\n");              // Restaurar PSP
        // Preparar stack frame para retorno
	asm volatile ("stmia r8!, {r0-r3}\n");       // Apilar R0-R3
	asm volatile ("stmia r8!, {r4-r7}\n");       // Apilar R12, LR, PC, xPSR
	asm volatile ("msr psp, r8\n");              // Actualizar PSP
        // Configurar LR para retorno de interrupción
	asm volatile ("mov lr, #0xFFFFFFFD\n");      // EXC_RETURN para Thread mode, PSP
	asm volatile ("bx lr");
}

/*
 * Funciones para alarmas
 * time (ticks)
 */
void Set_Alarm(char task_ID, uint16_t time, Enable_Alarm enable) {
    Alarm* new_alarm = (Alarm*)malloc(sizeof(Alarm));
    if (new_alarm == NULL) {
        return;
    }

    new_alarm->task_ID = task_ID;
    new_alarm->original_time = time;
    new_alarm->remain_time = time;
    new_alarm->enable = enable;
    new_alarm->alarm_activated = 0;
    new_alarm->next = NULL;

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
        return -1;
    }
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
        return -1;
    }
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
 * Función: PIT0_IRQHandler
 * Descripción: Manejador de interrupciones para el temporizador PIT0.
 *              Procesa alarmas y activa tareas asociadas.
 */
void PIT0_IRQHandler(void) {
    Save_Context();

    Alarm* current = list_a.head;
    while (current != NULL) {
        if (current->enable == ENABLED) {
            current->remain_time--;
        }
        if (current->remain_time == 0) {
            Task* task = list.task_map[current->task_ID - 'A'];
            current->remain_time = current->original_time;
            if (task != NULL) {
                task->task_state = READY;
                PRINTF("Alarma activada para tarea %c\n\r", task->task_ID);
            }
            current->alarm_activated = 1;
        }
        current = current->next;
    }

    // Limpiar bandera de interrupción
    PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;

    Scheduler();
}
