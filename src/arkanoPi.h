#ifndef _ARKANOPI_H_
#define _ARKANOPI_H_

#include "systemLib.h"
#include "kbhit.h" // deteccion de teclas sin bloqueo
#include "fsm.h"
#include "tmr.h"
#include "teclado_TL04.h"
#include "controller.h"
#include "arkanoPiLib.h"
#include "pongLib.h"
#include "commonLib.h"
#include "ledDisplay.h"

typedef struct {
	tipo_arkanoPi arkanoPi;
	tipo_pong pong;
} TipoSistema;

//------------------------------------------------------
// FUNCIONES DE CONFIGURACION/INICIALIZACION
//------------------------------------------------------
int ConfiguraInicializaSistema (TipoSistema *p_sistema);

//------------------------------------------------------
// SUBRUTINAS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------
void tmr_actualizacion_juego_isr(union sigval value);

//------------------------------------------------------
// FUNCIONES LIGADAS A THREADS ADICIONALES
//------------------------------------------------------
PI_THREAD(thread_explora_teclado_PC);

#endif /* ARKANOPI_H_ */

