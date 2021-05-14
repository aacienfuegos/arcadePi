#ifndef _ARKANOPI_H_
#define _ARKANOPI_H_

#include "systemLib.h"
#include "kbhit.h" // para poder detectar teclas pulsadas sin bloqueo y leer las teclas pulsadas
#include "fsm.h"
#include "tmr.h"
#include "teclado_TL04.h"
#include "arkanoPiLib.h"
#include "pongLib.h"
#include "commonLib.h"
#include "ledDisplay.h"

#define NUM_JUEGOS 2

typedef struct {
	tipo_arkanoPi arkanoPi;
	tipo_pong pong;
	// Aqui podrian añadirse otros juegos...
	// tipo_otroJuego1 otroJuego1;
	// tipo_otroJuego2 otroJuego2;
	// ...
	int game; // Game selected 0 arkano, 1 pong...
	char icons[NUM_JUEGOS][2];
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

