
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


/*static TipoTeclado teclado;*/
int debounceTime[NUM_FILAS_TECLADO] = {0,0,0,0};
int rowPulsada;

//------------------------------------------------------
// PROCEDIMIENTOS DE INICIALIZACION DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------

void InicializaTeclado(TipoTeclado *p_teclado) {
	// A completar por el alumno...
	// ...
	/*wiringPiSetupGpio*/
	if (wiringPiSetupGpio() < 0) {
	    fprintf (stderr, "Unable to setup wiringPi: %s\n", strerror (errno)) ;
	    return;
	}

	pinMode (GPIO_KEYBOARD_ROW_1, INPUT);
	pullUpDnControl(GPIO_KEYBOARD_ROW_1, PUD_DOWN);
	wiringPiISR (GPIO_KEYBOARD_ROW_1, INT_EDGE_RISING, p_teclado->rutinas_ISR[0]);
	//p_teclado->debounceTime [teclado.filas[0]]=0;

	pinMode (GPIO_KEYBOARD_ROW_2, INPUT);
	pullUpDnControl(GPIO_KEYBOARD_ROW_2, PUD_DOWN);
	wiringPiISR (GPIO_KEYBOARD_ROW_2, INT_EDGE_RISING,  p_teclado->rutinas_ISR[1]);
	//p_teclado->debounceTime [teclado.filas[1]]=0;

	pinMode (GPIO_KEYBOARD_ROW_3, INPUT);
	pullUpDnControl(GPIO_KEYBOARD_ROW_3, PUD_DOWN);
	wiringPiISR (GPIO_KEYBOARD_ROW_3, INT_EDGE_RISING,  p_teclado->rutinas_ISR[2]);
	//p_teclado->debounceTime [teclado.filas[2]]=0;

	pinMode (GPIO_KEYBOARD_ROW_4, INPUT);
	pullUpDnControl(GPIO_KEYBOARD_ROW_4, PUD_DOWN);
	wiringPiISR (GPIO_KEYBOARD_ROW_4, INT_EDGE_RISING,  p_teclado->rutinas_ISR[3]);
	//p_teclado->debounceTime [teclado.filas[3]]=0;

	pinMode (GPIO_KEYBOARD_COL_1, OUTPUT);
	digitalWrite(GPIO_KEYBOARD_COL_1, LOW);

	pinMode (GPIO_KEYBOARD_COL_2, OUTPUT);
	digitalWrite(GPIO_KEYBOARD_COL_2, LOW);

	pinMode (GPIO_KEYBOARD_COL_3, OUTPUT);
	digitalWrite(GPIO_KEYBOARD_COL_3, LOW);

	pinMode (GPIO_KEYBOARD_COL_4, OUTPUT);
	digitalWrite(GPIO_KEYBOARD_COL_4, LOW);

	p_teclado->tmr_duracion_columna = tmr_new (timer_duracion_columna_isr);
	tmr_startms((tmr_t*)(p_teclado->tmr_duracion_columna),TIMEOUT_COLUMNA_TECLADO);
	/*tmr_startms(teclado.tmr_duracion_columna, TIMEOUT_COLUMNA_TECLADO);*/
}

//------------------------------------------------------
// OTROS PROCEDIMIENTOS PROPIOS DE LA LIBRERIA
//------------------------------------------------------

void ActualizaExcitacionTecladoGPIO (int columna) {
	// A completar por el alumno
	int gpio[NUM_COLUMNAS_TECLADO];
	gpio[COLUMNA_1] = GPIO_KEYBOARD_COL_1;
	gpio[COLUMNA_2] = GPIO_KEYBOARD_COL_2;
	gpio[COLUMNA_3] = GPIO_KEYBOARD_COL_3;
	gpio[COLUMNA_4] = GPIO_KEYBOARD_COL_4;

	switch(columna){
		case COLUMNA_1:
			digitalWrite(gpio[COLUMNA_4],0);
			digitalWrite(gpio[COLUMNA_1],1);
			break;
		case COLUMNA_2:
			digitalWrite(gpio[COLUMNA_1],0);
			digitalWrite(gpio[COLUMNA_2],1);
			break;
		case COLUMNA_3:
			digitalWrite(gpio[COLUMNA_2],0);
			digitalWrite(gpio[COLUMNA_3],1);
			break;
		case COLUMNA_4:
			digitalWrite(gpio[COLUMNA_3],0);
			digitalWrite(gpio[COLUMNA_4],1);
			break;
		default:
			break;
	}

	/*int i=0;*/
	/*for(i=COLUMNA_1; i<=COLUMNA_4; i++){*/
		/*pinMode(gpio[i], OUTPUT);*/
		/*digitalWrite(gpio[i],output[i]);*/
	/*}*/
}

//------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

int CompruebaTimeoutColumna (fsm_t* this) {
	int result = 0;
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	// completado
	piLock(KEYBOARD_KEY);
	result = (teclado.flags & FLAG_TIMEOUT_COLUMNA_TECLADO);
	piUnlock(KEYBOARD_KEY);

	return result;
}

int CompruebaTeclaPulsada (fsm_t* this) {
	int result = 0;
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	// A completado
	piLock(KEYBOARD_KEY);
	result = (teclado.flags & FLAG_TECLA_PULSADA);
	piUnlock(KEYBOARD_KEY);

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
	piLock (KEYBOARD_KEY);
	teclado.flags &= (~ FLAG_TIMEOUT_COLUMNA_TECLADO);
	piUnlock (KEYBOARD_KEY);

	p_teclado->columna_actual += 1;
	if(p_teclado->columna_actual > COLUMNA_4) {
		p_teclado->columna_actual=COLUMNA_1;
		}
	// Llamada a ActualizaExcitacionTecladoGPIO con columna a activar como argumento
	ActualizaExcitacionTecladoGPIO (p_teclado->columna_actual);
	/*printf("%i",p_teclado->columna_actual);*/

	tmr_startms(teclado.tmr_duracion_columna, TIMEOUT_COLUMNA_TECLADO);


}

void ProcesaTeclaPulsada (fsm_t* this) {
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	piLock(KEYBOARD_KEY);
	teclado.flags &= (~FLAG_TECLA_PULSADA);
	piUnlock(KEYBOARD_KEY);

	//p_teclado->teclaPulsada.col = p_teclado->columna_actual;
	//p_teclado->teclaPulsada.row = rowPulsada;

	/*piLock(STD_IO_BUFFER_KEY);*/
	/*printf("%i",p_teclado->teclaPulsada.row);*/
	/*printf("%i",p_teclado->teclaPulsada.col);*/
	/*fflush(stdout);*/
	/*piUnlock(STD_IO_BUFFER_KEY);*/

	// completado


	switch(p_teclado->teclaPulsada.col){
		case COLUMNA_1: //
			if(p_teclado->teclaPulsada.row==0){
				piLock(SYSTEM_FLAGS_KEY);
				flags |= FLAG_PAUSA_JUEGO;
				piUnlock(SYSTEM_FLAGS_KEY);
				/* printf("\n Mov. izq.\n"); */
				fflush(stdout);
			}
			break;
		case COLUMNA_2: // tecla 0 (s14) movimiento izquierda
			if(p_teclado->teclaPulsada.row==0){
				piLock(SYSTEM_FLAGS_KEY);
				flags |= FLAG_CONTINUA_JUEGO;
				piUnlock(SYSTEM_FLAGS_KEY);
				/* printf("\n Mov. izq.\n"); */
				fflush(stdout);
			}
			else if(p_teclado->teclaPulsada.row==3){
				piLock(SYSTEM_FLAGS_KEY);
				flags |= FLAG_MOV_IZQUIERDA;
				piUnlock(SYSTEM_FLAGS_KEY);
				printf("\n Mov. izq.\n");
				fflush(stdout);
			}
			break;

		case COLUMNA_3: // tecla B (s15) movimiento derecha
			//p_teclado->teclaPulsada.col = COLUMNA_3;
			/*printf("2");*/
			//fflush(stdout);
			if(p_teclado->teclaPulsada.row==3){
				//printf("\n Mov. dcha.\n");
				piLock(SYSTEM_FLAGS_KEY);
				flags |= FLAG_MOV_DERECHA;
				piUnlock(SYSTEM_FLAGS_KEY);
				fflush(stdout);
			}
			break;

		case COLUMNA_4: // tecla C (s4) "start"
			//p_teclado->teclaPulsada.col = COLUMNA_4;
			/*printf("3");*/
			//fflush(stdout);
			if(p_teclado->teclaPulsada.row==0){
				piLock(SYSTEM_FLAGS_KEY);
				flags |= FLAG_BOTON;
				piUnlock(SYSTEM_FLAGS_KEY);
				//printf("\n Inicio juego.\n");
				fflush(stdout);
			}
			break;

		default:
			printf("\nERROR!!!! invalid number of column (%d)!!!\n", p_teclado->columna_actual);
			fflush(stdout);

			//p_teclado->teclaPulsada.row = -1;
			//p_teclado->teclaPulsada.col = -1;

			break;
	}



}


//------------------------------------------------------
// SUBRUTINAS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------


	// int wiringPiISR (int pin, int edgeType, void (*function)(void));
	// identificar interrupciones recibidas en el pin especificado

	//int wiringPiISR (GPIO_KEYBOARD_ROW_1, INT_EDGE_RISING, (*teclado_fila_1_isr)(void));
	//int wiringPiISR (GPIO_KEYBOARD_ROW_2, INT_EDGE_RISING, (*teclado_fila_2_isr)(void));
	//int wiringPiISR (GPIO_KEYBOARD_ROW_3, INT_EDGE_RISING, (*teclado_fila_3_isr)(void));
	//int wiringPiISR (GPIO_KEYBOARD_ROW_4, INT_EDGE_RISING, (*teclado_fila_4_isr)(void));

void teclado_fila_1_isr (void) {
	// A completar por el alumno
	// ...

	if (millis() < teclado.debounceTime[FILA_1]){
		teclado.debounceTime[FILA_1]= millis() + DEBOUNCE_TIME;
		return;
	}

	piLock(KEYBOARD_KEY);

	teclado.flags |= FLAG_TECLA_PULSADA;

	teclado.teclaPulsada.row = FILA_1;
	teclado.teclaPulsada.col = teclado.columna_actual;
	piUnlock(KEYBOARD_KEY);

	debounceTime[FILA_1] = millis () + DEBOUNCE_TIME;
	//piLock(SYSTEM_FLAGS_KEY);
	/*flags |= FLAG_TECLA_PULSADA;*/
	//piUnlock (SYSTEM_FLAGS_KEY);

	//while(digitalRead(GPIO_KEYBOARD_ROW_1) ==HIGH){
	//	delay(debounceTime[FILA_1]);
		//delay(1);
	//}
};


void teclado_fila_2_isr (void) {
	// A completar por el alumno
	// ...
	if (millis() < teclado.debounceTime[FILA_2]){
		teclado.debounceTime[FILA_2]= millis() + DEBOUNCE_TIME;
		return;
	}

	piLock(KEYBOARD_KEY);
	teclado.teclaPulsada.row = FILA_2;
	teclado.teclaPulsada.col = teclado.columna_actual;
	teclado.flags |= FLAG_TECLA_PULSADA;
	piUnlock(KEYBOARD_KEY);

	teclado.debounceTime[FILA_2]= millis() + DEBOUNCE_TIME;

	//while(digitalRead(GPIO_KEYBOARD_ROW_2) ==HIGH){
		//delay(1);
	//	delay(debounceTime[FILA_2]);
	//}

};

void teclado_fila_3_isr (void) {
	// A completar por el alumno
	// ...
	if (millis() < teclado.debounceTime[FILA_3]){
		teclado.debounceTime[FILA_3]= millis() + DEBOUNCE_TIME;
		return;
	}

	piLock(KEYBOARD_KEY);
	teclado.flags |= FLAG_TECLA_PULSADA;
	teclado.teclaPulsada.row = FILA_3;
	teclado.teclaPulsada.col = teclado.columna_actual;
	piUnlock(KEYBOARD_KEY);


	teclado.debounceTime[FILA_3]= millis() + DEBOUNCE_TIME;
	//while(digitalRead(GPIO_KEYBOARD_ROW_3) ==HIGH){
		//delay(1);
	//	delay(debounceTime[FILA_3]);
	//}

};

void teclado_fila_4_isr (void) {
	// A completar por el alumno
	// ...
	if (millis() < teclado.debounceTime[FILA_4]){
		teclado.debounceTime[FILA_4]= millis() + DEBOUNCE_TIME;
		return;
	}

	piLock(KEYBOARD_KEY);
	teclado.teclaPulsada.row = FILA_4;
	teclado.teclaPulsada.col = teclado.columna_actual;
	teclado.flags |= FLAG_TECLA_PULSADA;
	piUnlock(KEYBOARD_KEY);

	debounceTime[FILA_4] = millis () + DEBOUNCE_TIME;

	//while(digitalRead(GPIO_KEYBOARD_ROW_4) ==HIGH){
		//delay(1);
	//	delay(debounceTime[FILA_4]);
	//}

};

void timer_duracion_columna_isr (union sigval value) {
	// A completar por el alumno
	// ...
	piLock(KEYBOARD_KEY);
	teclado.flags |= FLAG_TIMEOUT_COLUMNA_TECLADO;
	piUnlock(KEYBOARD_KEY);
};

