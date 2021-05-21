
#include "arkanoPi.h"
#include <stdio.h>
#include <string.h>

int flags = 0;
/* int flags = 0; */

TipoSistema sistema;

// Declaracion del objeto teclado
TipoTeclado teclado = {
	// Completado
	.columnas = {
		GPIO_KEYBOARD_COL_1,
		GPIO_KEYBOARD_COL_2,
		GPIO_KEYBOARD_COL_3,
		GPIO_KEYBOARD_COL_4

	},
	.filas = {
		GPIO_KEYBOARD_ROW_1,
		GPIO_KEYBOARD_ROW_2,
		GPIO_KEYBOARD_ROW_3,
		GPIO_KEYBOARD_ROW_4
	},
	.rutinas_ISR = {
		teclado_fila_1_isr,
		teclado_fila_2_isr,
		teclado_fila_3_isr,
		teclado_fila_4_isr
	},

	// Completado
	.columna_actual = COLUMNA_1,

	.teclaPulsada.col = -1,
	.teclaPulsada.row = -1,
};

// Declaracion del objeto display
TipoLedDisplay led_display = {
	.pines_control_columnas = {
		// A completar por el alumno...
		// ...
		GPIO_LED_DISPLAY_COL_1,
		GPIO_LED_DISPLAY_COL_2,
		GPIO_LED_DISPLAY_COL_3
		/* GPIO_LED_DISPLAY_COL_4 */

	},
	.filas = {
		// A completar por el alumno...
		// ...
		GPIO_LED_DISPLAY_ROW_1,
		GPIO_LED_DISPLAY_ROW_2,
		GPIO_LED_DISPLAY_ROW_3,
		GPIO_LED_DISPLAY_ROW_4,
		GPIO_LED_DISPLAY_ROW_5,
		GPIO_LED_DISPLAY_ROW_6,
		GPIO_LED_DISPLAY_ROW_7

	},

	.p_columna = 0,
	// A completar por el alumno...
	// ...
};

TipoController controller = {
	.game = 0,

};

//------------------------------------------------------
// FUNCIONES DE CONFIGURACION/INICIALIZACION
//------------------------------------------------------

// int ConfiguracionSistema (TipoSistema *p_sistema): procedimiento de configuracion
// e inicializacion del sistema.
// Realizará, entra otras, todas las operaciones necesarias para:
// configurar el uso de posibles librerías (e.g. Wiring Pi),
// configurar las interrupciones externas asociadas a los pines GPIO,
// configurar las interrupciones periódicas y sus correspondientes temporizadores,
// la inicializacion de los diferentes elementos de los que consta nuestro sistema,
// crear, si fuese necesario, los threads adicionales que pueda requerir el sistema
// como el thread de exploración del teclado del PC
int ConfiguraInicializaSistema (TipoSistema *p_sistema) {
	int result = 0;
	// Compeltado
	wiringPiSetupGpio();
	InicializaTeclado(&teclado);
	InicializaLedDisplay(&led_display);
	InitSPI();

	// Lanzamos thread para exploracion del teclado convencional del PC
	result = piThreadCreate (thread_explora_teclado_PC);

	if (result != 0) {
		printf ("Thread didn't start!!!\n");
		return -1;
	}

	return result;
}

//------------------------------------------------------
// FUNCIONES LIGADAS A THREADS ADICIONALES
//------------------------------------------------------

PI_THREAD (thread_explora_teclado_PC) {
	int teclaPulsada;

	while(1) {
		delay(10); // Wiring Pi function: pauses program execution for at least 10 ms

		piLock (STD_IO_BUFFER_KEY);

		if(kbhit()) {
			teclaPulsada = kbread();

			switch(teclaPulsada) {
				//completado
				case 'a':
					piLock(SYSTEM_FLAGS_KEY);
					flags |= FLAG_MOV_IZQUIERDA;
					piUnlock(SYSTEM_FLAGS_KEY);
					break;

				case 'd':
					piLock(SYSTEM_FLAGS_KEY);
					flags |= FLAG_MOV_DERECHA;
					piUnlock(SYSTEM_FLAGS_KEY);
					break;

				case 'c':
					piLock(SYSTEM_FLAGS_KEY);
					flags |= FLAG_TIMER_JUEGO;
					piUnlock(SYSTEM_FLAGS_KEY);
					break;

				case 's':
					piLock(SYSTEM_FLAGS_KEY);
					flags |= FLAG_BOTON;
					piUnlock(SYSTEM_FLAGS_KEY);
					break;

				case 'p':
					piLock(SYSTEM_FLAGS_KEY);
					flags |= FLAG_PAUSA_JUEGO;
					piUnlock(SYSTEM_FLAGS_KEY);
					break;

				case '1':
					piLock(SYSTEM_FLAGS_KEY);
					flags |= FLAG_MOV_ABAJO;
					piUnlock(SYSTEM_FLAGS_KEY);
					break;

				case '0':
					piLock(SYSTEM_FLAGS_KEY);
					flags |= FLAG_MOV_ARRIBA;
					piUnlock(SYSTEM_FLAGS_KEY);
					break;

				case 'j':
					piLock(SYSTEM_FLAGS_KEY);
					flags |= FLAG_MOV_IZQUIERDA2;
					piUnlock(SYSTEM_FLAGS_KEY);
					break;

				case 'l':
					piLock(SYSTEM_FLAGS_KEY);
					flags |= FLAG_MOV_DERECHA2;
					piUnlock(SYSTEM_FLAGS_KEY);
					break;

				case 'x':
					piLock(SYSTEM_FLAGS_KEY);
					flags |= FLAG_EXIT;
					/* flags |= FLAG_EXIT; */
					piUnlock(SYSTEM_FLAGS_KEY);
					break;

				case 'q':
					exit(0);
					break;

				default:
					printf("INVALID KEY!!!\n");
					break;
			}
		}

		piUnlock (STD_IO_BUFFER_KEY);
	}
}

// wait until next_activation (absolute time)
void delay_until (unsigned int next) {
	unsigned int now = millis();
	if (next > now) {
		delay (next - now);
	}
}

int main () {
	unsigned int next;

	// Maquina de estados: lista de transiciones
	// {EstadoOrigen, CondicionDeDisparo, EstadoFinal, AccionesSiTransicion }
	fsm_trans_t arkanoPi[] = {
		{ WAIT_INICIO, CompruebaInicioArkanoPi, WAIT_START, InicializaJuego },
		{ WAIT_START, CompruebaBotonPulsado, WAIT_PUSH, StartJuego },

		{ WAIT_PUSH, CompruebaTimeoutActualizacionJuego, WAIT_PUSH, ActualizarJuego },
		{ WAIT_PUSH, CompruebaMovimientoIzquierda, WAIT_PUSH, MuevePalaIzquierda },
		{ WAIT_PUSH, CompruebaMovimientoDerecha, WAIT_PUSH, MuevePalaDerecha },

		{ WAIT_PUSH, CompruebaPausaJuego, WAIT_PAUSE, PausarJuego },
		{ WAIT_PAUSE, CompruebaPausaJuego, WAIT_PUSH, ContinuarJuego },

		{ WAIT_PUSH, CompruebaFinalJuego, WAIT_END, FinalJuego },
		{ WAIT_END,  CompruebaBotonPulsado, WAIT_START, ReseteaJuego },

		{ WAIT_START,  CompruebaExit, WAIT_INICIO, ExitArkano },
		{ WAIT_PAUSE,  CompruebaExit, WAIT_INICIO, ExitArkano },
		{ WAIT_END,  CompruebaExit, WAIT_INICIO, ExitArkano },
		{-1, NULL, -1, NULL },
	};
	fsm_trans_t pong[] = {
		{ WAIT_INICIO, CompruebaInicioPong, WAIT_START, InicializaJuegoPong },
		{ WAIT_START, CompruebaBotonPulsado, WAIT_PUSH, StartJuegoPong },

		{ WAIT_PUSH, CompruebaTimeoutActualizacionJuego, WAIT_PUSH, ActualizarJuegoPong },
		{ WAIT_PUSH, CompruebaMovimientoIzquierda, WAIT_PUSH, MuevePalaIzquierdaPong },
		{ WAIT_PUSH, CompruebaMovimientoDerecha, WAIT_PUSH, MuevePalaDerechaPong },
		{ WAIT_PUSH, CompruebaMovimientoIzquierda2, WAIT_PUSH, MuevePalaIzquierdaPong2 },
		{ WAIT_PUSH, CompruebaMovimientoDerecha2, WAIT_PUSH, MuevePalaDerechaPong2 },

		{ WAIT_PUSH, CompruebaPausaJuego, WAIT_PAUSE, PausarJuegoPong },
		{ WAIT_PAUSE, CompruebaPausaJuego, WAIT_PUSH, ContinuarJuegoPong },

		{ WAIT_PUSH, CompruebaFinalJuego, WAIT_END, FinalJuegoPong },
		{ WAIT_END,  CompruebaBotonPulsado, WAIT_START, ReseteaJuegoPong },

		{ WAIT_START,  CompruebaExit, WAIT_INICIO, ExitPong },
		{ WAIT_PAUSE,  CompruebaExit, WAIT_INICIO, ExitPong },
		{ WAIT_END,  CompruebaExit, WAIT_INICIO, ExitPong },
		{-1, NULL, -1, NULL },
	};

	// Configuracion e incializacion del sistema
	ConfiguraInicializaSistema (&sistema);
	sistema.arkanoPi.p_pantalla = &(led_display.pantalla);
	sistema.pong.p_pantalla = &(led_display.pantalla);

	fsm_t* arkanoPi_fsm = fsm_new (WAIT_INICIO, arkanoPi, &sistema);
	fsm_t* pong_fsm = fsm_new (WAIT_INICIO, pong, &sistema);

	// controller
	fsm_t* selector_fsm = fsm_new (WAIT_PUSH, fsm_trans_selector, &(controller));

	// teclado
	fsm_t* teclado_fsm = fsm_new ( TECLADO_ESPERA_COLUMNA, fsm_trans_excitacion_columnas, &(teclado));
	fsm_t* tecla_fsm = fsm_new (TECLADO_ESPERA_TECLA, fsm_trans_deteccion_pulsaciones, &(teclado));
	teclado.tmr_duracion_columna = tmr_new (timer_duracion_columna_isr);

	// display
	fsm_t* display_fsm = fsm_new (DISPLAY_ESPERA_COLUMNA, fsm_trans_excitacion_display, &(led_display));
	led_display.tmr_refresco_display =  tmr_new (timer_refresco_display_isr);

	// Start screen 
	display_text("Welcome");
	display_icon(HI_ICON);

	next = millis();
	while (1) {
		fsm_fire (selector_fsm);
		fsm_fire (arkanoPi_fsm);
		fsm_fire (pong_fsm);

		fsm_fire (teclado_fsm);
		fsm_fire (tecla_fsm);
		fsm_fire (display_fsm);

		next += CLK_MS;
		delay_until (next);

	}
	tmr_destroy((tmr_t*)(tmr_actualizacion_juego_isr));
	tmr_destroy((tmr_t*)(timer_duracion_columna_isr));

	fsm_destroy (selector_fsm);
	fsm_destroy (arkanoPi_fsm);
	fsm_destroy (pong_fsm);

	fsm_destroy (teclado_fsm);
	fsm_destroy (tecla_fsm);
		fsm_fire (display_fsm);
}

