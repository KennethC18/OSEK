#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

/* Incluir librerías hechas por mí */
#include "App.h"

/*
 * Código principal
 */
int main(void) {
    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    /* Configurar e inicializar pines GPIO para prender los LEDs */
    Config_Pins();
    Init_Pins();
    Off();
    GPIO_Config();

    /* Creación de tareas */
    Create_Task(1, 'A', AUTOSTART_ON,  1, Task_A);
    Create_Task(2, 'B', AUTOSTART_OFF, 2, Task_B);
    Create_Task(3, 'C', AUTOSTART_OFF, 3, Task_C);

    /* Tareas a ejecutar cuando suceda una interrupción */
    Create_Task(4, 'D', AUTOSTART_OFF, 4, Task_ISR);

    /* Inciar el SO (Llamar al Scheduler) */
    SO_Init();

    /* Nunca va a llegar a este punro (En teoría)... */
    while(1);
    return 0;
}
