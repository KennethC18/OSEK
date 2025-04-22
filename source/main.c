#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
#include "SAE/App.h"      // Incluye las funciones de tareas y test de queue
#include "SAE/data_structures.h"  // Asegurarse que incluye la definición de Queue_t y sus funciones

/* Código principal */
int main(void) {
    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    BOARD_InitDebugConsole();
#endif

    /* Configurar e inicializar pines GPIO para prender los LEDs y demás periféricos */
    GPIO_Config();
//    PIT_Config();

    /* Creación de Alarmas */
//    Set_Alarm('D', 1000, ENABLED);
//    Set_Alarm('E', 1000, ENABLED);

    /* Creación de tareas:
       Parámetros:
 * - number: Número de la tarea.
 * - task_ID: Identificador único de la tarea (un carácter).
 * - autostart: Indica si la tarea debe iniciar automáticamente (AUTOSTART_ON) o no (AUTOSTART_OFF).
 * - priority: Prioridad de la tarea. Un valor más alto indica mayor prioridad.
 * - start_address: Dirección de la función que representa la tarea.
 * */
//    Create_Task(1, 'A', AUTOSTART_OFF, 1, Task_PWM1);
//    Create_Task(2, 'B', AUTOSTART_OFF, 1, Task_PWM2);
//    Create_Task(3, 'C', AUTOSTART_OFF, 1, Task_PWM3);
//    Create_Task(4, 'D', AUTOSTART_OFF, 1, Task_PWM4);
//    Create_Task(5, 'E', AUTOSTART_OFF, 1, Task_PWM5);

    Create_Task(6, 'F', AUTOSTART_OFF, 1, Task_UART1);
//	Create_Task(7, 'G', AUTOSTART_OFF, 1, Task_UART2);
//	Create_Task(8, 'H', AUTOSTART_OFF, 1, Task_UART3);
//	Create_Task(9, 'I', AUTOSTART_OFF, 1, Task_UART4);
//	Create_Task(10, 'J', AUTOSTART_OFF, 1, Task_UART5);

    /* Iniciar el SO (Llamar al Scheduler) */
    SO_Init();

    /* Nunca va a llegar a este punto (En teoría)... */
    while(1);
    return 0;
}
