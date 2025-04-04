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
    Config_Pins();
    GPIO_Config();
    PIT_Config();

    /* Creación de Alarmas */
//    Set_Alarm('D', 1000, ENABLED);
//    Set_Alarm('E', 1000, ENABLED);

    /* Creación de tareas:
       - Task_A escribirá un dato en la cola.
       - Task_B extraerá el dato y lo imprimirá.
       - Task_C puede utilizarse para otras pruebas o finalizar el test.
    */
    Create_Task(1, 'A', AUTOSTART_ON, 3, Task_A);
    Create_Task(2, 'B', AUTOSTART_ON, 2, Task_B);
    Create_Task(3, 'C', AUTOSTART_ON, 1, Task_C);

    /* Tareas a ejecutar cuando suceda una interrupción */
//    Create_Task(4, 'D', AUTOSTART_OFF, 4, Task_ISR);
//    Create_Task(5, 'E', AUTOSTART_OFF, 4, Task_ISR_2);

    /* Iniciar el SO (Llamar al Scheduler) */
    SO_Init();

    /* Nunca va a llegar a este punto (En teoría)... */
    while(1);
    return 0;
}
