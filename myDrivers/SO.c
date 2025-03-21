#include "SO.h"
#include "Low_Power.h"

/* Estructura global que contiene:
 * - Lista de tareas
 * - Puntero a la lista actual
 */
Task_List	list = {NULL, NULL, {NULL}};



/*
 * Crea una nueva tarea, la ordena por prioridad, y la agrega a la lista
 *
 * Recibe:
 * - number:			Número de tarea
 * - task_ID:			Identificador de la tarea. Solo admmite un caracter
 * - autostart: 		Indica si la tarea se debe de iniciar automáticamente
 * - priority:			Prioridad de la tarea. Un número más alto indica mayor prioridad.
 * - start_address: 	Dirección de la función
 */
void Create_Task(uint8_t number, char task_ID, Autostart autostart, uint8_t priority,
                 void* start_address) {
	/* Reservar memoria para la nueva tarea */
    Task* new_task = (Task*)malloc(sizeof(Task));
    if(new_task == NULL) {
        return;								// En caso de fallar, salir y terminar...
    }

    /* Llenar los campos de la nueva tarea */
    new_task->number        = number;
    new_task->task_ID       = task_ID;
    new_task->autostart     = autostart;
    new_task->priority      = priority;
    new_task->task_state    = SUSPENDED;
    new_task->start_address = start_address;
    new_task->return_address= start_address;	// En un inicio, 'return_address' = 'start_address'
    new_task->interrupted 	= 0;
    new_task->next          = NULL;

    /* Iniciar contexto de la tarea */
    memset(&new_task->context, 0, sizeof(Context));

    /* Si la tarea tiene AUTOSTART habilitado, cambiar su estado a READY */
    if(autostart == AUTOSTART_ON) new_task->task_state = READY;

    /* Agregar a la Hash */
    list.task_map[task_ID - 'A'] = new_task;

    /* Insertar a la lista. Se ordena por prioridad */
    if(list.head == NULL || list.head->priority < priority) {
    	/* Cuando la lista esta vacía o la nueva tarea tiene mayor prioridad, insertar al inicio */
        new_task->next = list.head;
        list.head = new_task;
    }
    else {
    	/* Buscar la posición correcta */
        Task* current = list.head;
        while((current->next != NULL) && (current->next->priority >= priority)) {
            current = current->next;
        }
        /* Insertar en la posición correcta */
        new_task->next = current->next;
        current->next = new_task;
    }
}



/*
 * Activa la tarea especificada por 'task_ID'
 *
 * Recibe:
 * - task_ID: Identificador único de cada tarea
 */
void Activate_Task(char task_ID) {
    if (list.current_task != NULL) {
    	/* Guardar la dirección de retorno de la tarea actual */
		asm("ldr r5,[sp, #20]");	// Cargar dirección de retorno en r5
		asm("sub r5, #1");			// Ajustar lr
		asm("orr r5, r5, #1");		// Para evitar Hardfault, hace OR con 1.

		asm("ldr r6,=list");		// Cargar dirección base de 'list'
		asm("ldr r6,[r6,#4]");		// Cargar 'list.current_task' (Offset 4)
		asm("str r5,[r6,#12]");		// Guardar en return_address (Offset 16 en 'Task')

		/* Cambiar el estado de la tarea ejecutándose. Solo puede haber una tarea en RUN */
        list.current_task->task_state = READY;
    }

    /* Activar la tarea que tiene 'task_ID' correspondiente */
    Task* task = list.task_map[task_ID - 'A'];
    if (task != NULL) {
        task->task_state = READY;
    }

    /* Ajustar el 'sp' para liberar espacio */
    asm("add sp, #16");

    /* Llamar al 'Scheduler' y que se encargue de ejecutar la tarea con mayor prioridad */
    Scheduler();
}



/*
 * Activa la tarea especificada por 'task_ID'
 *
 * Recibe:
 * - task_ID: Identificador único de cada tarea
 */
void Activate_Task_ISR(char task_ID) {
    if (list.current_task != NULL) {
		/* Cambiar el estado de la tarea ejecutándose. Solo puede haber una tarea en RUN */
        list.current_task->task_state = READY;
    }

    /* Activar la tarea que tiene 'task_ID' correspondiente */
    Task* task = list.task_map[task_ID - 'A'];
    if (task != NULL) {
        task->task_state = READY;
    }

    /* Ajustar el 'sp' para liberar espacio */
    asm("add sp, #16");

    /* Llamar al 'Scheduler' y que se encargue de ejecutar la tarea con mayor prioridad */
    Scheduler();
}


/*
 * Suspende la tarea actual (RUN -> SUSPENDED), y pone en estado READY a la especificada.
 *
 * Recibe:
 * - task_ID: Identificador único de cada tarea
 */
void Chain_Task(char task_ID) {
    /* Suspender/Terminar la tarea actual */
    if (list.current_task != NULL) {
        list.current_task->task_state = SUSPENDED;		// Terminar la tarea
    }

    /* Activar la tarea del 'task_ID' escrito */
    Task* task = list.task_map[task_ID - 'A'];
    if (task != NULL) {
        task->task_state = READY;
    }

    /* Ajustar el 'sp' para liberar espacio */
    asm("add sp, #16");

    /* Volver a llamar el Scheduler */
    Scheduler();
}



/*
 * Termina / Suspende la tarea actual y llama al Scheduler
 */
void Terminate_Task(void) {
    /* Suspende la tarea actual */
    if (list.current_task != NULL) {
        list.current_task->task_state = SUSPENDED;
    }

    /* Ajustar el 'sp' para liberar espacio */
    asm("add sp, #8");

    /* Volver a llamar al Scheduler */
    Scheduler();
}



/*
 * Selecciona y ejecuta la tarea con mayor prioridad Y en estado READY
 */
void Scheduler(void) {
    Task* current = list.head;

    /* Buscar la tarea con mayor prioridad Y en estado READY */
    while (current != NULL) {
        if (current->task_state == READY) {
            current->task_state = RUN;

            /* Guardar la nueva tarea actual en 'list.current_task' */
			asm("ldr r5,=list");      				// Dirección base de 'list'
			asm("str %0,[r5,#4]" :: "r"(current));	// Guardar current en list.current_task

            /* Verificar si la tarea fue interrumpida */
            if (current->interrupted) {
                /* Restaurar el contexto */
            	Restore_Context();

                /* Marcar la tarea como no interrumpida */
                current->interrupted = 0;
            }

			/* Guardar la dirección de retorno en list.current_task->return_address */
			if (current->return_address != NULL) {
				asm("ldr r5, = list");		// Dirección base de 'list'
				asm("ldr r5, [r5, #4]");	// Cargar 'list.current_task'
				asm("ldr r6, [r5, #12]");	// Cargar return_address (Offset 12)

				asm("mov lr,r6");			// Ajustar dirección de retorno
				asm("bx lr");				// Saltar a la dirección de retorno
			}
			return;
        }
        current = current->next;
    }

    /* En caso de que no haya tareas por ejecutar, entrar en modo de bajo consumo */
    while (1) {
    	sleep();
    }
}



/*
 * Inicia el Sistema Operativo y llama al Scheduler
 */
void SO_Init(void) {
    Scheduler();
}



/*
 * Guarda el contexto de la tarea que fue interrumpida
 */
void Save_Context(void) {
	/* Marcar a la tarea actual como interrumpida */
	if(list.current_task != NULL) list.current_task->interrupted = 1;

	/* Ajustar el sp. Debe de apuntar a donde se guardaron los datos antes de la interrupción */
	asm("mov r6, sp");
	asm("add r6, #0x2c");

	/* Cargar los datos, y guardar en la estructura de datos */
	asm("ldr r8, = list");		// Apuntar a dirección base de 'list'
	asm("add r8, #0x12c");		// Apuntar a context de la tarea actual / interrumpida

	/* Guardar XPSR */
	//asm("ldr r9, [r6, #-4]");
	asm("mrs r9, xpsr");
	asm("str r9, [r8]");

	/* Guardar PC */
	asm("ldr r9, [r6, #-8]");
	asm("str r9, [r8, #4]");

	/* Guardar LR */
	asm("ldr r9, [r6, #-12]");
	asm("str r9, [r8, #8]");

	/* Guardar SP */
	asm("ldr r9, [r6, #-40]");
	asm("str r9, [r8, #12]");

	/* Guardar R12 */
	asm("ldr r9, [r6, #-16]");
	asm("str r9, [r8, #16]");

	/* Guardar R3*/
	asm("ldr r9, [r6, #-20]");
	asm("str r9, [r8, #20]");

	/* Guardar R2*/
	asm("ldr r9, [r6, #-24]");
	asm("str r9, [r8, #24]");

	/* Guardar R1*/
	asm("ldr r9, [r6, #-28]");
	asm("str r9, [r8, #28]");

	/* Guardar R0*/
	asm("ldr r9, [r6, #-32]");
	asm("str r9, [r8, #32]");
}


/*
 * Recupera el contexto de la tarea que fue interrumpida
 */
void Restore_Context(void) {
	if(list.current_task != NULL) list.current_task->interrupted = 0;

    /* Cargar la dirección de list.current_task->context */
    asm("ldr r8, = list");  // Cargar la dirección de 'list'
	asm("add r8, #0x12c");	// Apuntar a context de la interrumpida

    /* Restaurar XPSR */
    asm("ldr r9, [r8]");            // Cargar XPSR
    //asm("msr xpsr, r9");            // Restaurar XPSR

    /* Restaurar LR */
    asm("ldr r9, [r8, #8]");		// Cargar LR
    asm("mov lr, r9");				// Restaurar LR

    /* Restaurar SP */
    asm("ldr r9, [r8, #12]");		// Cargar SP
    asm("mov sp, r9");				// Restaurar SP

    /* Restaurar R12 */
    asm("ldr r9, [r8, #16]");		// Cargar R12
    asm("mov r12, r9");				// Restaurar R12

    /* Restaurar R3 */
    asm("ldr r9, [r8, #20]");		// Cargar R3
    asm("mov r3, r9");				// Restaurar R3

    /* Restaurar R2 */
    asm("ldr r9, [r8, #24]");		// Cargar R2
    asm("mov r2, r9");				// Restaurar R2

    /* Restaurar R1 */
    asm("ldr r9, [r8, #28]");		// Cargar R1
    asm("mov r1, r9");				// Restaurar R1

    /* Restaurar R0 */
    asm("ldr r9, [r8, #32]");		// Cargar R0
    asm("mov r0, r9");				// Restaurar R0

    /* Restaurar PC */
    asm("ldr r9, [r8, #4]");        // Cargar PC
    asm("mov pc, r9");              // Restaurar PC
}
