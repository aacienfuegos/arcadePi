
#ifndef _PONGLIB_H_
#define _PONGLIB_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "controller.h"

#include "ledDisplay.h"
#include "commonLib.h"

typedef struct
{
	tipo_pantalla *p_pantalla; // Esta es nuestra pantalla de juego (matriz 10x7 de labo)
	tipo_pala pala;
	tipo_pala pala2;
	tipo_pelota pelota;
	tmr_t* tmr_actualizacion_juego_isr;
	int score1;
	int score2;

} tipo_pong;

//------------------------------------------------------------------------
// FUNCIONES DE INICIALIZACION / RESET  DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------------------------

void InicializaPala2(tipo_pala *p_pala);
void InicializaPong(tipo_pong *p_pong);
void ResetPong(tipo_pong *p_pong);

//------------------------------------------------------
// PROCEDIMIENTOS PARA LA GESTION DEL JUEGO
//------------------------------------------------------
int CompruebaReboteParedesVerticalesPong(tipo_pong pong);
int CompruebaRebotePalaPong(tipo_pong pong);
int CompruebaRebotePalaPong2(tipo_pong pong);
int CompruebaPunto(tipo_pong *p_pong);

//------------------------------------------------------
// PROCEDIMIENTOS PARA LA VISUALIZACION DEL JUEGO
//------------------------------------------------------
void ActualizaPantallaPong(tipo_pong *p_pong);

//------------------------------------------------------
// FUNCIONES DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------
int CompruebaIniciaPong(fsm_t *this);
int CompruebaMovimientoIzquierda2(fsm_t *this);
int CompruebaMovimientoDerecha2(fsm_t *this);

//------------------------------------------------------
// FUNCIONES DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------
void ExitPong(fsm_t *this);
void MuevePalaIzquierdaPong(fsm_t *this);
void MuevePalaDerechaPong(fsm_t *this);
void MuevePalaIzquierdaPong2(fsm_t *this);
void MuevePalaDerechaPong2(fsm_t *this);
void PausarJuegoPong(fsm_t *this);
void ContinuarJuegoPong(fsm_t *this);

//------------------------------------------------------
// FUNCIONES DE ACCION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------
void InicializaJuegoPong(fsm_t *this);
void StartJuegoPong(fsm_t *this);
void ActualizarJuegoPong(fsm_t *this);
void FinalJuegoPong(fsm_t *this);
void VictoriaPong(tipo_pong *p_pong);
void ReseteaJuegoPong(fsm_t *this);

//------------------------------------------------------
// SUBRUTINAS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------

#endif /* _PONGLIB_H_ */

