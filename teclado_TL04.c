#include "teclado_TL04.h"

char tecladoTL04[4][4] = {
	{'1', '2', '3', 'C'},
	{'4', '5', '6', 'D'},
	{'7', '8', '9', 'E'},
	{'A', '0', 'B', 'F'}
};

// Maquina de estados: lista de transiciones
// {EstadoOrigen, CondicionDeDisparo, EstadoFinal, AccionesSiTransicion }
fsm_trans_t fsm_trans_excitacion_columnas[] = {
	{ TECLADO_ESPERA_COLUMNA, CompruebaTimeoutColumna, TECLADO_ESPERA_COLUMNA, TecladoExcitaColumna },
	{-1, NULL, -1, NULL },
};

fsm_trans_t fsm_trans_deteccion_pulsaciones[] = {
	{ TECLADO_ESPERA_TECLA, CompruebaTeclaPulsada, TECLADO_ESPERA_TECLA, ProcesaTeclaPulsada},
	{-1, NULL, -1, NULL },
};

//------------------------------------------------------
// PROCEDIMIENTOS DE INICIALIZACION DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------

void InicializaTeclado(TipoTeclado *p_teclado) {
	// A completar por el alumno...
	// ...
	/*int aux[NUM_COLUMNAS_TECLADO][NUM_FILAS_TECLADO][NUM_FILAS_TECLADO] = {{GPIO_KEYBOARD_COL_1, GPIO_KEYBOARD_COL_2, GPIO_KEYBOARD_COL_3, GPIO_KEYBOARD_COL_4},*/
				 /*{GPIO_KEYBOARD_ROW_1, GPIO_KEYBOARD_ROW_2, GPIO_KEYBOARD_ROW_3, GPIO_KEYBOARD_ROW_4},*/
				 /*{DEBOUNCE_TIME, DEBOUNCE_TIME, DEBOUNCE_TIME, DEBOUNCE_TIME}};*/
				
	/*int columnas[NUM_COLUMNAS_TECLADO] = {GPIO_KEYBOARD_COL_1, */
										/*GPIO_KEYBOARD_COL_2, */
										/*GPIO_KEYBOARD_COL_3, */
										/*GPIO_KEYBOARD_COL_4};*/
	/**p_teclado->columnas = columnas;*/
	
	/*int filas[NUM_FILAS_TECLADO] = {GPIO_KEYBOARD_ROW_1, */
									  /*GPIO_KEYBOARD_ROW_2, */
									  /*GPIO_KEYBOARD_ROW_3, */
									  /*GPIO_KEYBOARD_ROW_4};*/
	/*p_teclado->filas = filas;*/
	 
	/*int debounceTime[NUM_FILAS_TECLADO] = {DEBOUNCE_TIME,*/
												/*DEBOUNCE_TIME,*/
												/*DEBOUNCE_TIME,*/
												/*DEBOUNCE_TIME};*/
	/*p_teclado->debounceTime = debounceTime;*/

	p_teclado->tmr_duracion_columna = tmr_new (timer_duracion_columna_isr);
	tmr_startms((tmr_t*)(p_teclado->tmr_duracion_columna),TIMEOUT_COLUMNA_TECLADO);
}

//------------------------------------------------------
// OTROS PROCEDIMIENTOS PROPIOS DE LA LIBRERIA
//------------------------------------------------------

void ActualizaExcitacionTecladoGPIO (int columna) {
	// A completar por el alumno
	int gpio = 0;
	
	switch(columna){
		case COLUMNA_1:
			gpio = GPIO_KEYBOARD_COL_1;
			break;
		case COLUMNA_2:
			gpio = GPIO_KEYBOARD_COL_2;
			break;
		case COLUMNA_3:
			gpio = GPIO_KEYBOARD_COL_3;
			break;
		case COLUMNA_4:
			gpio = GPIO_KEYBOARD_COL_4;
			break;
		default:
			break;
	}
		pinMode(gpio,OUTPUT);
		digitalWrite(gpio,HIGH);
}

//------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

int CompruebaTimeoutColumna (fsm_t* this) {
	int result = 0;
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	// completado	
	piLock(SYSTEM_FLAGS_KEY);
	result = (flags & FLAG_TIMEOUT_COLUMNA_TECLADO);
	piUnlock(SYSTEM_FLAGS_KEY);

	return result;
}

int CompruebaTeclaPulsada (fsm_t* this) {
	int result = 0;
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	// A completado
	piLock(SYSTEM_FLAGS_KEY);
	result = (flags & FLAG_TECLA_PULSADA);
	piUnlock(SYSTEM_FLAGS_KEY);

	return result;
}

//------------------------------------------------------
// FUNCIONES DE SALIDA O DE ACCION DE LAS MAQUINAS DE ESTADOS
//------------------------------------------------------

void TecladoExcitaColumna (fsm_t* this) {
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	// A completar por el alumno
	// ...
	ActualizaExcitacionTecladoGPIO (p_teclado->columna_actual); //
	// Llamada a ActualizaExcitacionTecladoGPIO con columna a activar como argumento
}

void ProcesaTeclaPulsada (fsm_t* this) {
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	// completado
	piLock(SYSTEM_FLAGS_KEY);
	flags &= (~FLAG_TECLA_PULSADA);

	switch(p_teclado->teclaPulsada.col){
		case COLUMNA_1: //
			break;
		case COLUMNA_2: // tecla 0 (s14) movimiento izquierda
			if(tecladoTL04[p_teclado->teclaPulsada.row][p_teclado->teclaPulsada.col]=='0'){
			printf("\n Mov. izq.\n");
			fflush(stdout);
			}
			break;

		case COLUMNA_3: // tecla B (s15) movimiento derecha
			if(tecladoTL04[p_teclado->teclaPulsada.row][p_teclado->teclaPulsada.col]=='B'){
			printf("\n Mov. dcha.\n");
			fflush(stdout);
			}
			break;

		case COLUMNA_4: // tecla C (s4) "start"
			if(tecladoTL04[p_teclado->teclaPulsada.row][p_teclado->teclaPulsada.col]=='C'){
			printf("\n Mov. izq.\n");
			fflush(stdout);
			}
			break;
			
		default:
			printf("\nERROR!!!! invalid number of column (%d)!!!\n", p_teclado->teclaPulsada.col);
			fflush(stdout);

			//p_teclado->teclaPulsada.row = -1;
			//p_teclado->teclaPulsada.col = -1;

			break;
	}

	piUnlock(SYSTEM_FLAGS_KEY);
	
}


//------------------------------------------------------
// SUBRUTINAS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------


	// int wiringPiISR (int pin, int edgeType, void (*function)(void));
	// identificar interrupciones recibidas en el pin especificado

void teclado_fila_1_isr (void) {
	// A completar por el alumno
	// ...

	while(digitalRead(GPIO_KEYBOARD_ROW_1) ==HIGH){
		delay(1);
	}
	/*debouceTime[0] = millis () + DEBOUNCE_TIME*/

	
	piLock(SYSTEM_FLAGS_KEY);

	flags |= FLAG_TECLA_PULSADA;

	piUnlock (SYSTEM_FLAGS_KEY);
}
	

void teclado_f_2_isr (void) {

}

void teclado_fila_2_isr (void) {
	// A completar por el alumno
	// ...

}

void teclado_fila_3_isr (void) {
	// A completar por el alumno
	// ...
}

void teclado_fila_4_isr (void) {
	// A completar por el alumno
	// ...
}

void timer_duracion_columna_isr (union sigval value) {
	// A completar por el alumno
	// ...
}
