#ifndef _SYSTEMLIB_H_
#define _SYSTEMLIB_H_

#include <time.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
//s#include <wiringPi.h>
#include "pseudoWiringPi.h"

#include "kbhit.h" // para poder detectar teclas pulsadas sin bloqueo y leer las teclas pulsadas
#include "fsm.h"
#include "tmr.h"

#define __SIN_PSEUDOWIRINGPI__
#define CLK_MS 					1

// ATENCION: Valores a modificar por el alumno
// INTERVALO DE GUARDA ANTI-REBOTES
#define	DEBOUNCE_TIME			0
#define TIMEOUT_ACTUALIZA_JUEGO 0

// A 'key' which we can lock and unlock - values are 0 through 3
//	This is interpreted internally as a pthread_mutex by wiringPi
//	which is hiding some of that to make life simple.

// CLAVES PARA MUTEX
// ATENCION: Valores a modificar por el alumno
#define	KEYBOARD_KEY		0
#define	SYSTEM_FLAGS_KEY	0
#define	MATRIX_KEY			0
#define	STD_IO_BUFFER_KEY	0

// Distribucion de pines GPIO empleada para el teclado y el display
// ATENCION: Valores a modificar por el alumno
#define GPIO_KEYBOARD_COL_1 	0
#define GPIO_KEYBOARD_COL_2 	0
#define GPIO_KEYBOARD_COL_3 	0
#define GPIO_KEYBOARD_COL_4 	0
#define GPIO_KEYBOARD_ROW_1 	0
#define GPIO_KEYBOARD_ROW_2 	0
#define GPIO_KEYBOARD_ROW_3 	0
#define GPIO_KEYBOARD_ROW_4 	0

#define GPIO_LED_DISPLAY_COL_1	0
#define GPIO_LED_DISPLAY_COL_2	0
#define GPIO_LED_DISPLAY_COL_3	0
#define GPIO_LED_DISPLAY_COL_4	0
#define GPIO_LED_DISPLAY_ROW_1	0
#define GPIO_LED_DISPLAY_ROW_2	0
#define GPIO_LED_DISPLAY_ROW_3	0
#define GPIO_LED_DISPLAY_ROW_4	0
#define GPIO_LED_DISPLAY_ROW_5	0
#define GPIO_LED_DISPLAY_ROW_6	0
#define GPIO_LED_DISPLAY_ROW_7	0

// FLAGS FSM CONTROL DE SERPIENTE Y GESTION JUEGO
// ATENCION: Valores a modificar por el alumno
#define FLAG_MOV_ARRIBA 	0x00
#define FLAG_MOV_ABAJO		0x00
#define FLAG_MOV_DERECHA 	0x00
#define FLAG_MOV_IZQUIERDA 	0x00
#define FLAG_TIMER_JUEGO	0x00
#define FLAG_BOTON 			0x00
#define FLAG_FIN_JUEGO		0x00

enum fsm_state {
	WAIT_START,
	WAIT_PUSH,
	WAIT_END};

extern int flags;

#endif /* SYSTEMLIB_H_ */
