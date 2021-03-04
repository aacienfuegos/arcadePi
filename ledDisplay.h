#ifndef _LEDDISPLAY_H_
#define _LEDDISPLAY_H_

//#include <wiringPi.h>
#include "pseudoWiringPi.h"
#include "systemLib.h"
#include "tmr.h"

// REFRESCO DISPLAY
// ATENCION: Valor a modificar por el alumno
#define TIMEOUT_COLUMNA_DISPLAY 60

#define NUM_PINES_CONTROL_COLUMNAS_DISPLAY	3
#define NUM_COLUMNAS_DISPLAY	8
#define NUM_FILAS_DISPLAY		7

// FLAGS FSM CONTROL DE EXCITACION DISPLAY
// ATENCION: Valores a modificar por el alumno
#define FLAG_TIMEOUT_COLUMNA_DISPLAY 	0x00

enum estados_excitacion_display_fsm {
	DISPLAY_ESPERA_COLUMNA
};

typedef struct {
	int matriz[NUM_FILAS_DISPLAY][NUM_COLUMNAS_DISPLAY];
} tipo_pantalla;

typedef struct {
	int pines_control_columnas[NUM_PINES_CONTROL_COLUMNAS_DISPLAY]; // pines_control_columnas
	int filas[NUM_FILAS_DISPLAY];
	int p_columna;
	tipo_pantalla pantalla;
	tmr_t* tmr_refresco_display;
	int flags;
} TipoLedDisplay;

extern TipoLedDisplay led_display;
extern tipo_pantalla pantalla_inicial;
extern tipo_pantalla pantalla_final;
extern fsm_trans_t fsm_trans_excitacion_display[];

//------------------------------------------------------
// PROCEDIMIENTOS DE INICIALIZACION DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------

void InicializaLedDisplay (TipoLedDisplay *led_display);

//------------------------------------------------------
// OTROS PROCEDIMIENTOS PROPIOS DE LA LIBRERIA
//------------------------------------------------------

void ApagaFilas (TipoLedDisplay *led_display);
void ExcitaColumnas (int columna);
void ActualizaLedDisplay (TipoLedDisplay *led_display);

//------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

int CompruebaTimeoutColumnaDisplay (fsm_t* this);

//------------------------------------------------------
// FUNCIONES DE SALIDA O DE ACCION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

void ActualizaExcitacionDisplay (fsm_t* this);

//------------------------------------------------------
// SUBRUTINAS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------

void timer_refresco_display_isr (union sigval value);

#endif /* _LEDDISPLAY_H_ */
