#ifndef _TECLADO_TL04_H_
#define _TECLADO_TL04_H_

#include "systemLib.h"

// REFRESCO TECLADO
#define TIMEOUT_COLUMNA_TECLADO	25

#define NUM_COLUMNAS_TECLADO 	4
#define NUM_FILAS_TECLADO 		4

// FLAGS FSM CONTROL DE EXCITACION TECLADO Y FSM GESTION TECLAS PULSADAS
// ATENCION: Valores a modificar por el alumno
#define FLAG_TIMEOUT_COLUMNA_TECLADO  	0x01
#define FLAG_TECLA_PULSADA 				0x02

enum columns_values {
	COLUMNA_1,
	COLUMNA_2,
	COLUMNA_3,
	COLUMNA_4,
};

enum rows_values {
	FILA_1,
	FILA_2,
	FILA_3,
	FILA_4
};

enum estados_excitacion_teclado_fsm {
	TECLADO_ESPERA_COLUMNA
};

enum estados_deteccion_pulsaciones_teclado_fsm {
	TECLADO_ESPERA_TECLA
};

typedef struct {
	int col;
	int row;
} TipoTecla;

typedef struct {
	int columnas[NUM_COLUMNAS_TECLADO]; // Array con los valores BCM de los pines GPIO empleados para cada columna
	int filas[NUM_FILAS_TECLADO]; // Array con los valores BCM de los pines GPIO empleados para cada fila
	int debounceTime[NUM_FILAS_TECLADO]; // // Array de variables auxiliares para la implementacion de mecanismos anti-rebotes para cada entrada de interrupcion
	void (*rutinas_ISR[NUM_FILAS_TECLADO]) (void); // Array de punteros a procedimientos de atencion a las interrupciones ligados al teclado
	int columna_actual; // Variable que almacena el valor de la columna que esta activa
	TipoTecla teclaPulsada; // Variable que almacena la ultima tecla pulsada
	tmr_t* tmr_duracion_columna; // Temporizador responsable de medir el tiempo de activacion de cada columna
	int flags; // Variable para gestion de flags especificamente ligados a la gestion del teclado
} TipoTeclado;

extern TipoTeclado teclado;
extern fsm_trans_t fsm_trans_excitacion_columnas[];
extern fsm_trans_t fsm_trans_deteccion_pulsaciones[];
extern int flags; // Flags generales de sistema (necesario para comunicacion inter-FMs)

//------------------------------------------------------
// PROCEDIMIENTOS DE INICIALIZACION DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------

void InicializaTeclado(TipoTeclado *p_teclado);

//------------------------------------------------------
// OTROS PROCEDIMIENTOS PROPIOS DE LA LIBRERIA
//------------------------------------------------------

void ActualizaExcitacionTecladoGPIO (int columna);

//------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LAS MAQUINAS DE ESTADOS
//------------------------------------------------------

int  CompruebaTimeoutColumna (fsm_t* this);
int  CompruebaTeclaPulsada (fsm_t* this);

//------------------------------------------------------
// FUNCIONES DE SALIDA O DE ACCION DE LAS MAQUINAS DE ESTADOS
//------------------------------------------------------

void TecladoExcitaColumna (fsm_t* this);
void ProcesaTeclaPulsada (fsm_t* this);

//------------------------------------------------------
// SUBRUTINAS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------

void teclado_fila_1_isr (void);
void teclado_fila_2_isr (void);
void teclado_fila_3_isr (void);
void teclado_fila_4_isr (void);
void timer_duracion_columna_isr (union sigval value);

#endif /* _TECLADO_TL04_H_ */
