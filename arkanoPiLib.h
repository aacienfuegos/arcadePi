
#ifndef _ARKANOPILIB_H_
#define _ARKANOPILIB_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "ledDisplay.h"
#include "commonLib.h"

typedef struct
{
	tipo_pantalla *p_pantalla; // Esta es nuestra pantalla de juego (matriz 10x7 de labo)
	tipo_pantalla ladrillos;
	tipo_pala pala;
	tipo_pelota pelota;
	tmr_t* tmr_actualizacion_juego_isr;

} tipo_arkanoPi;

//------------------------------------------------------------------------
// FUNCIONES DE INICIALIZACION / RESET  DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------------------------

void InicializaArkanoPi(tipo_arkanoPi *p_arkanoPi);
void ResetArkanoPi(tipo_arkanoPi *p_arkanoPi);

//------------------------------------------------------
// PROCEDIMIENTOS PARA LA GESTION DEL JUEGO
//------------------------------------------------------
int CompruebaReboteParedesVerticales(tipo_arkanoPi arkanoPi);
int CompruebaReboteTecho(tipo_arkanoPi arkanoPi);
int CompruebaRebotePala(tipo_arkanoPi arkanoPi);
int CompruebaFallo(tipo_arkanoPi arkanoPi);

//------------------------------------------------------
// PROCEDIMIENTOS PARA LA VISUALIZACION DEL JUEGO
//------------------------------------------------------
void ActualizaPantalla(tipo_arkanoPi *p_arkanoPi);
void PintaLadrillos(tipo_pantalla *p_ladrillos, tipo_pantalla *p_pantalla);

//------------------------------------------------------
// FUNCIONES DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------
int CompruebaIniciaArkano(fsm_t *this);

//------------------------------------------------------
// FUNCIONES DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------
void ExitArkano(fsm_t *this);
void MuevePalaIzquierda(fsm_t *this);
void MuevePalaDerecha(fsm_t *this);
void PausarJuego(fsm_t *this);
void ContinuarJuego(fsm_t *this);

//------------------------------------------------------
// FUNCIONES DE ACCION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------
void InicializaJuego(fsm_t *this);
void ActualizarJuego(fsm_t *this);
void FinalJuego(fsm_t *this);
void ReseteaJuego(fsm_t *this);

//------------------------------------------------------
// SUBRUTINAS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------

#endif /* _ARKANOPILIB_H_ */

