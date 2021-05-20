#ifndef _COMMONLIB_H_
#define _COMMONLIB_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "ledDisplay.h"

enum t_direccion
{
	ARRIBA_IZQUIERDA,
	ARRIBA,
	ARRIBA_DERECHA,
	ABAJO_DERECHA,
	ABAJO,
	ABAJO_IZQUIERDA,
	IZQUIERDA, // NO PERMITIDA
	DERECHA,   // NO PERMITIDA
};

// CONSTANTES DEL JUEGO
#define NUM_COLUMNAS_PALA 3
#define NUM_FILAS_PALA 1
#define MAX_NUM_TRAYECTORIAS 8

typedef struct
{
	int ancho;
	int alto;
	int x;
	int y;
} tipo_pala;

typedef struct
{
	int xv;
	int yv;
} tipo_trayectoria;

typedef struct
{
	tipo_trayectoria posibles_trayectorias[MAX_NUM_TRAYECTORIAS];
	int num_posibles_trayectorias;
	tipo_trayectoria trayectoria;
	int x;
	int y;
} tipo_pelota;


//------------------------------------------------------------------------
// FUNCIONES DE INICIALIZACION / RESET  DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------------------------
void InicializaPelota(tipo_pelota *p_pelota);
void InicializaPala(tipo_pala *p_pala);
void InicializaPosiblesTrayectorias(tipo_pelota *p_pelota);
void ReseteaMatriz(tipo_pantalla *p_pantalla);

//------------------------------------------------------
// PROCEDIMIENTOS PARA LA GESTION DEL JUEGO
//------------------------------------------------------
void CambiarDireccionPelota(tipo_pelota *p_pelota, enum t_direccion direccion);
void ActualizaPosicionPala(tipo_pala *p_pala, enum t_direccion direccion);
void ActualizaPosicionPelota(tipo_pelota *p_pelota);

//------------------------------------------------------
// PROCEDIMIENTOS PARA LA VISUALIZACION DEL JUEGO
//------------------------------------------------------
void PintaMensajeInicialPantalla(tipo_pantalla *p_pantalla, tipo_pantalla *p_pantalla_inicial);
void PintaPantallaPorTerminal(tipo_pantalla *p_pantalla);
void PintaPala(tipo_pala *p_pala, tipo_pantalla *p_pantalla);
void PintaPelota(tipo_pelota *p_pelota, tipo_pantalla *p_pantalla);

//------------------------------------------------------
// FUNCIONES DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------
int CompruebaBotonPulsado(fsm_t *this);
int CompruebaExit(fsm_t *this);
int CompruebaMovimientoArriba(fsm_t *this);
int CompruebaMovimientoAbajo(fsm_t *this);
int CompruebaMovimientoIzquierda(fsm_t *this);
int CompruebaMovimientoDerecha(fsm_t *this);
int CompruebaPausaJuego(fsm_t *this);
int CompruebaContinuaJuego(fsm_t *this);
int CompruebaTimeoutActualizacionJuego(fsm_t *this);
int CompruebaFinalJuego(fsm_t *this);

//------------------------------------------------------
// SUBRUTINAS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------
void tmr_actualizacion_juego_isr(union sigval value);

#endif /* _COMMONLIB_H_ */

