
#include "ledDisplay.h"

tipo_pantalla pantalla_inicial = {
	.matriz = {
	{0,0,0,0,0,0,0,0},
	{0,0,1,0,0,1,0,0},
	{0,0,1,0,0,1,0,0},
	{0,0,0,0,0,0,0,0},
	{0,1,0,0,0,0,1,0},
	{0,1,1,0,0,1,1,0},
	{0,0,1,1,1,1,0,0},
	}
};

tipo_pantalla pantalla_final = {
	.matriz = {
	{0,0,0,0,0,0,0,0},
	{0,0,1,0,0,1,0,0},
	{0,0,1,0,0,1,0,0},
	{0,0,0,0,0,0,0,0},
	{0,1,0,0,0,0,1,0},
	{0,1,1,0,0,1,1,0},
	{0,0,1,1,1,1,0,0},
	}
};

// Maquina de estados: lista de transiciones
// {EstadoOrigen, CondicionDeDisparo, EstadoFinal, AccionesSiTransicion }
fsm_trans_t fsm_trans_excitacion_display[] = {
	{ DISPLAY_ESPERA_COLUMNA, CompruebaTimeoutColumnaDisplay, DISPLAY_ESPERA_COLUMNA, ActualizaExcitacionDisplay },
	{-1, NULL, -1, NULL },
};

//------------------------------------------------------
// PROCEDIMIENTOS DE INICIALIZACION DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------

void InicializaLedDisplay (TipoLedDisplay *led_display) {
	// A completar por el alumno...
	// ...
	if (wiringPiSetupGpio() < 0) {
	    fprintf (stderr, "Unable to setup wiringPi: %s\n", strerror (errno)) ;
	    return;
	}

	/* pinMode (GPIO_LED_DISPLAY_ROW_1, OUTPUT); */
	/* digitalWrite(GPIO_LED_DISPLAY_ROW_1, HIGH); */

	/* pinMode (GPIO_LED_DISPLAY_ROW_2, OUTPUT); */
	/* digitalWrite(GPIO_LED_DISPLAY_ROW_2, HIGH); */

	/* pinMode (GPIO_LED_DISPLAY_ROW_3, OUTPUT); */
	/* digitalWrite(GPIO_LED_DISPLAY_ROW_3, HIGH); */

	/* pinMode (GPIO_LED_DISPLAY_ROW_4, OUTPUT); */
	/* digitalWrite(GPIO_LED_DISPLAY_ROW_4, HIGH); */

	/* pinMode (GPIO_LED_DISPLAY_ROW_5, OUTPUT); */
	/* digitalWrite(GPIO_LED_DISPLAY_ROW_4, HIGH); */

	/* pinMode (GPIO_LED_DISPLAY_ROW_6, OUTPUT); */
	/* digitalWrite(GPIO_LED_DISPLAY_ROW_6, HIGH); */

	/* pinMode (GPIO_LED_DISPLAY_ROW_7, OUTPUT); */
	/* digitalWrite(GPIO_LED_DISPLAY_ROW_7, HIGH); */

	/* pinMode (GPIO_LED_DISPLAY_COL_1, OUTPUT); */
	/* digitalWrite(GPIO_LED_DISPLAY_COL_1, LOW); */

	/* pinMode (GPIO_LED_DISPLAY_COL_2, OUTPUT); */
	/* digitalWrite(GPIO_LED_DISPLAY_COL_2, LOW); */

	/* pinMode (GPIO_LED_DISPLAY_COL_3, OUTPUT); */
	/* digitalWrite(GPIO_LED_DISPLAY_COL_3, LOW); */

	/* pinMode (GPIO_LED_DISPLAY_COL_4, OUTPUT); */
	/* digitalWrite(GPIO_LED_DISPLAY_COL_4, LOW); */

	pinMode(GPIO_LED_DISPLAY_ROW_1, OUTPUT);
	pinMode(GPIO_LED_DISPLAY_ROW_2, OUTPUT);
	pinMode(GPIO_LED_DISPLAY_ROW_3, OUTPUT);
	pinMode(GPIO_LED_DISPLAY_ROW_4, OUTPUT);
	pinMode(GPIO_LED_DISPLAY_ROW_5, OUTPUT);
	pinMode(GPIO_LED_DISPLAY_ROW_6, OUTPUT);
	pinMode(GPIO_LED_DISPLAY_ROW_7, OUTPUT);
	pinMode(GPIO_LED_DISPLAY_COL_1, OUTPUT);
	pinMode(GPIO_LED_DISPLAY_COL_2, OUTPUT);
	pinMode(GPIO_LED_DISPLAY_COL_3, OUTPUT);

	led_display->pantalla = pantalla_inicial;
	ActualizaLedDisplay(led_display);

	led_display->tmr_refresco_display = tmr_new (timer_refresco_display_isr);
	tmr_startms((tmr_t*)(led_display->tmr_refresco_display),TIMEOUT_COLUMNA_DISPLAY);
}

//------------------------------------------------------
// OTROS PROCEDIMIENTOS PROPIOS DE LA LIBRERIA
//------------------------------------------------------

void ApagaFilas (TipoLedDisplay *led_display){
	// A completar por el alumno...
	// ...  completado
	digitalWrite(GPIO_LED_DISPLAY_ROW_1, HIGH);
	digitalWrite(GPIO_LED_DISPLAY_ROW_2, HIGH);
	digitalWrite(GPIO_LED_DISPLAY_ROW_3, HIGH);
	digitalWrite(GPIO_LED_DISPLAY_ROW_4, HIGH);
	digitalWrite(GPIO_LED_DISPLAY_ROW_5, HIGH);
	digitalWrite(GPIO_LED_DISPLAY_ROW_6, HIGH);
	digitalWrite(GPIO_LED_DISPLAY_ROW_7, HIGH);
}

void ExcitaColumnas(int columna) {
	digitalWrite(GPIO_LED_DISPLAY_COL_1, LOW);
	digitalWrite(GPIO_LED_DISPLAY_COL_2, LOW);
	digitalWrite(GPIO_LED_DISPLAY_COL_3, LOW);

	switch(columna) {
		// A completar por el alumno...
		// ...
		case 1:
			digitalWrite(GPIO_LED_DISPLAY_COL_1, HIGH);
			break;
		case 2:
			digitalWrite(GPIO_LED_DISPLAY_COL_2, HIGH);
			break;
		case 3:
			digitalWrite(GPIO_LED_DISPLAY_COL_1, HIGH);
			digitalWrite(GPIO_LED_DISPLAY_COL_2, HIGH);
			break;
		case 4:
			digitalWrite(GPIO_LED_DISPLAY_COL_3, HIGH);
			break;
		case 5:
			digitalWrite(GPIO_LED_DISPLAY_COL_1, HIGH);
			digitalWrite(GPIO_LED_DISPLAY_COL_3, HIGH);
			break;
		case 6:
			digitalWrite(GPIO_LED_DISPLAY_COL_2, HIGH);
			digitalWrite(GPIO_LED_DISPLAY_COL_3, HIGH);
			break;
		case 7:
			digitalWrite(GPIO_LED_DISPLAY_COL_1, HIGH);
			digitalWrite(GPIO_LED_DISPLAY_COL_2, HIGH);
			digitalWrite(GPIO_LED_DISPLAY_COL_3, HIGH);
			break;

	}
}

void ActualizaLedDisplay (TipoLedDisplay *led_display) {
	// A completar por el alumno...
	// ...
	ApagaFilas(led_display);
	int i, j = 0;

	for(i=0;i<NUM_FILAS_DISPLAY;i++) {
		for(j=0;j<NUM_COLUMNAS_DISPLAY;j++) {
			ExcitaColumnas(j);
			digitalWrite(led_display->filas[i], !led_display->pantalla.matriz[i][j]);
		}
	}

	return;
}

//------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

int CompruebaTimeoutColumnaDisplay (fsm_t* this) {
	int result = 0;
	TipoLedDisplay *p_ledDisplay;
	p_ledDisplay = (TipoLedDisplay*)(this->user_data);

	// A completar por el alumno...
	// ... completado
	piLock(MATRIX_KEY);
	result = (p_ledDisplay->flags & FLAG_TIMEOUT_COLUMNA_DISPLAY);
	piUnlock(MATRIX_KEY);

	return result;
}

//------------------------------------------------------
// FUNCIONES DE SALIDA O DE ACCION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

void ActualizaExcitacionDisplay (fsm_t* this) {
	TipoLedDisplay *p_ledDisplay;
	p_ledDisplay = (TipoLedDisplay*)(this->user_data);


	// A completar por el alumno...
	// ...
	ActualizaLedDisplay(p_ledDisplay);

	tmr_startms(led_display.tmr_refresco_display, TIMEOUT_COLUMNA_DISPLAY);
}

//------------------------------------------------------
// SUBRUTINAS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------

void timer_refresco_display_isr (union sigval value) {
	// A completar por el alumno...
	// ...
	piLock(MATRIX_KEY);
	led_display.flags |= FLAG_TIMEOUT_COLUMNA_DISPLAY;
	piUnlock(MATRIX_KEY);
}

