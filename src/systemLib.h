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
#include <wiringPi.h>

#include "kbhit.h" 
#include "fsm.h"
#include "tmr.h"

/* #define __SIN_wiringPi__ */
#define CLK_MS 					1

// ATENCION: Valores a modificar por el alumno
// INTERVALO DE GUARDA ANTI-REBOTES
#define	DEBOUNCE_TIME			300
#define TIMEOUT_ACTUALIZA_JUEGO 1000

// A 'key' which we can lock and unlock - values are 0 through 3
//	This is interpreted internally as a pthread_mutex by wiringPi
//	which is hiding some of that to make life simple.

// CLAVES PARA MUTEX
// ATENCION: Valores a modificar por el alumno
#define	KEYBOARD_KEY		0
#define	SYSTEM_FLAGS_KEY	1
#define	MATRIX_KEY			2
#define	STD_IO_BUFFER_KEY	3

// Distribucion de pines GPIO empleada para el teclado y el display
// ATENCION: Valores a modificar por el alumno
#define GPIO_KEYBOARD_COL_1 	0
#define GPIO_KEYBOARD_COL_2 	1
#define GPIO_KEYBOARD_COL_3 	2
#define GPIO_KEYBOARD_COL_4 	3
#define GPIO_KEYBOARD_ROW_1 	5
#define GPIO_KEYBOARD_ROW_2 	6
#define GPIO_KEYBOARD_ROW_3 	12
#define GPIO_KEYBOARD_ROW_4 	13


// cambiar gpios por los que estamos usando
#define GPIO_LED_DISPLAY_COL_1	9
#define GPIO_LED_DISPLAY_COL_2	14
#define GPIO_LED_DISPLAY_COL_3	17
#define GPIO_LED_DISPLAY_ROW_1	4
#define GPIO_LED_DISPLAY_ROW_2	7
#define GPIO_LED_DISPLAY_ROW_3	16
#define GPIO_LED_DISPLAY_ROW_4	15
#define GPIO_LED_DISPLAY_ROW_5	22
#define GPIO_LED_DISPLAY_ROW_6	27
#define GPIO_LED_DISPLAY_ROW_7	24

// GPIOS LED AUXILIAR
#define LED_AUX_CLK  11
#define LED_AUX_CS  8
#define LED_AUX_DIN  10

// FLAGS FSM CONTROL DE JUEGOS Y GESTION JUEGO
// ATENCION: Valores a modificar por el alumno
#define FLAG_MOV_ARRIBA 	0x01
#define FLAG_MOV_ABAJO		0x02
#define FLAG_MOV_DERECHA 	0x04
#define FLAG_MOV_IZQUIERDA 	0x08
#define FLAG_MOV_DERECHA2 	0x10
#define FLAG_MOV_IZQUIERDA2	0x20

#define FLAG_TIMER_JUEGO	0x40
#define FLAG_BOTON 			0x80

#define FLAG_PAUSA_JUEGO	0x100
#define FLAG_FIN_JUEGO		0x200

// FLAGS CONTROL DE INTERACCION ENTRE JEUGOS
#define FLAG_JUEGO_ARKANOPI	0x400
#define FLAG_JUEGO_PONG		0x800
#define FLAG_EXIT			0x1000

enum fsm_state {
	WAIT_INICIO,
	WAIT_START,
	WAIT_PUSH,
	WAIT_PAUSE,
	WAIT_END};

extern int flags;

#endif /* SYSTEMLIB_H_ */

