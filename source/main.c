#include <stdio.h>
#include <SAE/App.h>
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
    PIT_Config();

    /* Crear alarmas */
    Set_Alarm('A', 100, ENABLED);
    Set_Alarm('B', 10, DISABLED);
    Set_Alarm('C', 20, DISABLED);
    Set_Alarm('D', 30, DISABLED);
	Set_Alarm('E', 40, DISABLED);
	Set_Alarm('F', 50, DISABLED);
	Set_Alarm('G', 80, DISABLED);


    /* Creación de tareas */
    Create_Task(1, 'A', AUTOSTART_OFF, 2, Task_A);
    Create_Task(2, 'B', AUTOSTART_OFF, 1, Task_B);
    Create_Task(3, 'C', AUTOSTART_OFF, 1, Task_C);
    Create_Task(4, 'D', AUTOSTART_OFF, 1, Task_D);
    Create_Task(5, 'E', AUTOSTART_OFF, 1, Task_E);
    Create_Task(6, 'F', AUTOSTART_OFF, 1, Task_F);
    Create_Task(7, 'G', AUTOSTART_OFF, 1, Task_G);


    /* Tareas a ejecutar cuando suceda una interrupción */
    //Create_Task(4, 'D', AUTOSTART_OFF, 4, Task_ISR);

    /* Inciar el SO (Llamar al Scheduler) */
    SO_Init();

    /* Nunca va a llegar a este punro (En teoría)... */
    while(1);
    return 0;
}
