#include "pongLib.h"
/* #include "commonLib.h" */
#include "ledDisplay.h"

//------------------------------------------------------
// FUNCIONES DE INICIALIZACION / RESET
//------------------------------------------------------

void InicializaPala2(tipo_pala *p_pala) {
	// Pala inicialmente en el centro de la pantalla
	InicializaPala(p_pala);
	p_pala->y = 0;
}

void ActualizaPantallaPong(tipo_pong* p_pong) {

    // Borro toda la pantalla
	ReseteaPantalla((tipo_pantalla*)(p_pong->p_pantalla));

    // Pinta la pala
	PintaPala(
		(tipo_pala*)(&(p_pong->pala)),
		(tipo_pantalla*)(p_pong->p_pantalla));
    // Pinta la otra pala
	PintaPala(
		(tipo_pala*)(&(p_pong->pala2)),
		(tipo_pantalla*)(p_pong->p_pantalla));

	// Pinta la pelota
	PintaPelota(
		(tipo_pelota*)(&(p_pong->pelota)),
		(tipo_pantalla*)(p_pong->p_pantalla));
}

void InicializaPong(tipo_pong *p_pong) {
	ResetPong(p_pong);
	ActualizaPantallaPong(p_pong);
}

void ResetPong(tipo_pong *p_pong) {
	/* printf("asdf\n"); */
	ReseteaPantalla((tipo_pantalla*)(p_pong->p_pantalla));
	InicializaPelota((tipo_pelota*)(&(p_pong->pelota)));
	InicializaPala((tipo_pala*)(&(p_pong->pala)));
	InicializaPala2((tipo_pala*)(&(p_pong->pala2)));
}

int CompruebaReboteParedesVerticalesPong (tipo_pong pong) {
	// Comprobamos si la nueva posicion de la pelota excede los limites de la pantalla
	if((pong.pelota.x + pong.pelota.trayectoria.xv >= NUM_COLUMNAS_DISPLAY) ||
		(pong.pelota.x + pong.pelota.trayectoria.xv < 0) ) {
		// La pelota rebota contra la pared derecha o izquierda
		return 1;
	}
	return 0;
}

int CompruebaRebotePalaPong (tipo_pong pong) {
	printf("%d", pong.pala2.y);
	if(pong.pelota.trayectoria.yv > 0) { // Esta condicion solo tiene sentido si la pelota va hacia abajo en la pantalla
		if ((pong.pelota.x + pong.pelota.trayectoria.xv >= pong.pala.x ) &&
			(pong.pelota.x + pong.pelota.trayectoria.xv < pong.pala.x + NUM_COLUMNAS_PALA)) {
				if ((pong.pelota.y + pong.pelota.trayectoria.yv >= pong.pala.y) &&
					(pong.pelota.y + pong.pelota.trayectoria.yv < pong.pala.y + NUM_FILAS_PALA)) {
					return 1;
				}
		}
	}
	return 0;
}
int CompruebaRebotePalaPong2 (tipo_pong pong) {
	if(pong.pelota.trayectoria.yv < 0) { // Esta condicion solo tiene sentido si la pelota va hacia abajo en la pantalla
		if ((pong.pelota.x + pong.pelota.trayectoria.xv >= pong.pala2.x ) &&
			(pong.pelota.x + pong.pelota.trayectoria.xv < pong.pala2.x + NUM_COLUMNAS_PALA)) {
				if ((pong.pelota.y + pong.pelota.trayectoria.yv <= pong.pala2.y)
					&& (pong.pelota.y + pong.pelota.trayectoria.yv > pong.pala2.y - NUM_FILAS_PALA)){
					return 1;
				}
		}
	}
	return 0;
}

int CompruebaPunto (tipo_pong pong) {
	// Comprobamos si no hemos conseguido devolver la pelota
	if(pong.pelota.y + pong.pelota.trayectoria.yv >= NUM_FILAS_DISPLAY 
		|| pong.pelota.y + pong.pelota.trayectoria.yv < 0) {
		// Hemos fallado
		return 1;
	}
	return 0;
}

//------------------------------------------------------
// FUNCIONES DE ACCION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

// void MuevePalaIzquierda (void): funcion encargada de ejecutar
// el movimiento hacia la izquierda contemplado para la pala.
// Debe garantizar la viabilidad del mismo mediante la comprobación
// de que la nueva posición correspondiente a la pala no suponga
// que ésta rebase o exceda los límites definidos para el área de juego
// (i.e. al menos uno de los leds que componen la raqueta debe permanecer
// visible durante todo el transcurso de la partida).

void MuevePalaIzquierdaPong (fsm_t* this) {
	tipo_pong* p_pong;
	p_pong = (tipo_pong*)(this->user_data);

	// A completar por el alumno
	// ...
	piLock(SYSTEM_FLAGS_KEY);
	flags &= (~FLAG_MOV_IZQUIERDA);
	piUnlock(SYSTEM_FLAGS_KEY);

	ActualizaPosicionPala(&(p_pong->pala), IZQUIERDA);
	fflush(stdout);

	piLock(MATRIX_KEY); // CLAVE PANTALLA
	ActualizaPantallaPong(p_pong);
	piUnlock(MATRIX_KEY); // CLAVE PANTALLA

	piLock(STD_IO_BUFFER_KEY); // CLAVE E/S STD
	PintaPantallaPorTerminal(p_pong->p_pantalla);
	piUnlock(STD_IO_BUFFER_KEY); // CLAVE E/S STD

}

void MuevePalaIzquierdaPong2 (fsm_t* this) {
	tipo_pong* p_pong;
	p_pong = (tipo_pong*)(this->user_data);

	// A completar por el alumno
	// ...
	piLock(SYSTEM_FLAGS_KEY);
	flags &= (~FLAG_MOV_IZQUIERDA2);
	piUnlock(SYSTEM_FLAGS_KEY);

	ActualizaPosicionPala(&(p_pong->pala2), IZQUIERDA);
	fflush(stdout);

	piLock(MATRIX_KEY); // CLAVE PANTALLA
	ActualizaPantallaPong(p_pong);
	piUnlock(MATRIX_KEY); // CLAVE PANTALLA

	piLock(STD_IO_BUFFER_KEY); // CLAVE E/S STD
	PintaPantallaPorTerminal(p_pong->p_pantalla);
	piUnlock(STD_IO_BUFFER_KEY); // CLAVE E/S STD

}

// void MuevePalaDerecha (void): función similar a la anterior
// encargada del movimiento hacia la derecha.

void MuevePalaDerechaPong (fsm_t* this) {
	tipo_pong* p_pong;
	p_pong = (tipo_pong*)(this->user_data);

	// A completar por el alumno
	// ...
	piLock(SYSTEM_FLAGS_KEY);
	flags &= (~FLAG_MOV_DERECHA);
	piUnlock(SYSTEM_FLAGS_KEY);

	ActualizaPosicionPala(&(p_pong->pala), DERECHA);

	piLock(MATRIX_KEY); // CLAVE PANTALLA
	ActualizaPantallaPong(p_pong);
	piUnlock(MATRIX_KEY); // CLAVE PANTALLA

	piLock(STD_IO_BUFFER_KEY); // CLAVE E/S STD
	PintaPantallaPorTerminal(p_pong->p_pantalla);
	piUnlock(STD_IO_BUFFER_KEY); // CLAVE E/S STD
}
void MuevePalaDerechaPong2 (fsm_t* this) {
	tipo_pong* p_pong;
	p_pong = (tipo_pong*)(this->user_data);

	// A completar por el alumno
	// ...
	piLock(SYSTEM_FLAGS_KEY);
	flags &= (~FLAG_MOV_DERECHA2);
	piUnlock(SYSTEM_FLAGS_KEY);

	ActualizaPosicionPala(&(p_pong->pala2), DERECHA);

	piLock(MATRIX_KEY); // CLAVE PANTALLA
	ActualizaPantallaPong(p_pong);
	piUnlock(MATRIX_KEY); // CLAVE PANTALLA

	piLock(STD_IO_BUFFER_KEY); // CLAVE E/S STD
	PintaPantallaPorTerminal(p_pong->p_pantalla);
	piUnlock(STD_IO_BUFFER_KEY); // CLAVE E/S STD
}

// void PausaJuego (void): función encargada de pausar el juego

void PausarJuegoPong (fsm_t* this) {
	tipo_pong *p_pong;
	p_pong = (tipo_pong*)(this->user_data);

	piLock(SYSTEM_FLAGS_KEY);
	flags &= (~FLAG_PAUSA_JUEGO);
	piUnlock(SYSTEM_FLAGS_KEY);

	int i,j;
	for(i=0;i<NUM_FILAS_DISPLAY;i++) {
		for(j=0;j<NUM_COLUMNAS_DISPLAY;j++) {
			p_pong->p_pantalla->matriz[i][j] = pantalla_pausa.matriz[i][j];
		}
	}


	printf("PAUSA\n");
	fflush(stdout);
}

// void ContinuaJuego (void): función encargada de continuar el juego tras una pausa

void ContinuarJuegoPong (fsm_t* this) {
	tipo_pong *p_pong;
	p_pong = (tipo_pong*)(this->user_data);

	printf("Continua en:\n");
	fflush(stdout);
	
	led_text_main("321",1000);


	piLock(SYSTEM_FLAGS_KEY);
	flags &= (~FLAG_PAUSA_JUEGO);
	piUnlock(SYSTEM_FLAGS_KEY);

	ActualizarJuegoPong(this);

}

//------------------------------------------------------
// FUNCIONES DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

int CompruebaMovimientoIzquierda2(fsm_t* this) {
	int result = 0;

	// A completar por el alumno
	// ...
	piLock(SYSTEM_FLAGS_KEY);
	result = (flags & FLAG_MOV_IZQUIERDA2);
	piUnlock(SYSTEM_FLAGS_KEY);

	return result;
}

int CompruebaMovimientoDerecha2(fsm_t* this) {
	int result = 0;

	// A completar por el alumno
	// ...
	piLock(SYSTEM_FLAGS_KEY);
	result = (flags & FLAG_MOV_DERECHA2);
	piUnlock(SYSTEM_FLAGS_KEY);

	return result;
}

//------------------------------------------------------
// FUNCIONES DE ACCION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

// void InicializaJuego (void): funcion encargada de llevar a cabo
// la oportuna inicialización de toda variable o estructura de datos
// que resulte necesaria para el desarrollo del juego.

void InicializaJuegoPong(fsm_t* this) {
	tipo_pong *p_pong;
	p_pong = (tipo_pong*)(this->user_data);

	p_pong->tmr_actualizacion_juego_isr = tmr_new(tmr_actualizacion_juego_isr);

	// A completar por el alumno
	// ...
	piLock(SYSTEM_FLAGS_KEY);
	flags &= (~FLAG_BOTON);
	piUnlock(SYSTEM_FLAGS_KEY);

	piLock (STD_IO_BUFFER_KEY);

	InicializaPong(p_pong);

	tmr_startms(p_pong->tmr_actualizacion_juego_isr, TIMEOUT_ACTUALIZA_JUEGO);

	PintaMensajeInicialPantalla(p_pong->p_pantalla, p_pong->p_pantalla);
	PintaPantallaPorTerminal(p_pong->p_pantalla);

	piUnlock (STD_IO_BUFFER_KEY);


	/* pseudoWiringPiEnableDisplay(1); */
}

// void ActualizarJuego (void): función encargada de actualizar la
// posición de la pelota conforme a la trayectoria definida para ésta.
// Para ello deberá identificar los posibles rebotes de la pelota para,
// en ese caso, modificar su correspondiente trayectoria (los rebotes
// detectados contra alguno de los ladrillos implicarán adicionalmente
// la eliminación del ladrillo). Del mismo modo, deberá también
// identificar las situaciones en las que se dé por finalizada la partida:
// bien porque el jugador no consiga devolver la pelota, y por tanto ésta
// rebase el límite inferior del área de juego, bien porque se agoten
// los ladrillos visibles en el área de juego.

void ActualizarJuegoPong (fsm_t* this) {
	tipo_pong* p_pong;
	p_pong = (tipo_pong*)(this->user_data);

	piLock (SYSTEM_FLAGS_KEY);
	flags &= (~ FLAG_TIMER_JUEGO);
	piUnlock (SYSTEM_FLAGS_KEY);

	if(CompruebaReboteParedesVerticalesPong(*p_pong)) {
		p_pong->pelota.trayectoria.xv = -p_pong->pelota.trayectoria.xv;
	}

	if(CompruebaPunto (*p_pong)) {
		piLock (SYSTEM_FLAGS_KEY);
		flags |= FLAG_FIN_JUEGO;
		piUnlock (SYSTEM_FLAGS_KEY);
		return;

	} else if (CompruebaRebotePalaPong (*p_pong)) {
		switch(p_pong->pelota.x + p_pong->pelota.trayectoria.xv - p_pong->pala.x) {

		case 0:
		CambiarDireccionPelota(&(p_pong->pelota),ARRIBA_IZQUIERDA);
		break;

		case 1:
		CambiarDireccionPelota(&(p_pong->pelota), ARRIBA);
		break;

		case 2:
		CambiarDireccionPelota(&(p_pong->pelota), ARRIBA_DERECHA);
		break;

		}
	} 
	else if (CompruebaRebotePalaPong2 (*p_pong)) {
		switch(p_pong->pelota.x + p_pong->pelota.trayectoria.xv - p_pong->pala2.x) {

		case 0:
		CambiarDireccionPelota(&(p_pong->pelota),ABAJO_IZQUIERDA);
		break;

		case 1:
		CambiarDireccionPelota(&(p_pong->pelota), ABAJO);
		break;

		case 2:
		CambiarDireccionPelota(&(p_pong->pelota), ABAJO_DERECHA);
		break;

		}
	}

	ActualizaPosicionPelota (&(p_pong->pelota));

	piLock(MATRIX_KEY);
	ActualizaPantallaPong (p_pong);
	piUnlock(MATRIX_KEY);

	piLock (STD_IO_BUFFER_KEY);
	PintaPantallaPorTerminal(p_pong->p_pantalla);
	piUnlock (STD_IO_BUFFER_KEY);

	tmr_startms(p_pong->tmr_actualizacion_juego_isr, TIMEOUT_ACTUALIZA_JUEGO);

}

// void FinalJuego (void): función encargada de mostrar en la ventana de
// terminal los mensajes necesarios para informar acerca del resultado del juego.

void FinalJuegoPong (fsm_t* this) {
	tipo_pong *p_pong;
	p_pong = (tipo_pong*)(this->user_data);

	// A completar por el alumno
	// ...
	piLock(SYSTEM_FLAGS_KEY);
	flags &= (~FLAG_FIN_JUEGO);
	piUnlock(SYSTEM_FLAGS_KEY);
	
	if(p_pong->pelota.y >= p_pong->pala.y){
		printf("point player 2\n");
	}
	else if(p_pong->pelota.y <= p_pong->pala2.y){
		printf("point player 1\n");
	}


	/* pseudoWiringPiEnableDisplay(0); */
}

//void ReseteaJuego (void): función encargada de llevar a cabo la
// reinicialización de cuantas variables o estructuras resulten
// necesarias para dar comienzo a una nueva partida.

void ReseteaJuegoPong (fsm_t* this) {
	tipo_pong *p_pong;
	p_pong = (tipo_pong*)(this->user_data);

	piLock(SYSTEM_FLAGS_KEY);
	flags &= (~FLAG_BOTON);
	piUnlock(SYSTEM_FLAGS_KEY);

	piLock (STD_IO_BUFFER_KEY);
	ResetPong(p_pong);

	piUnlock (STD_IO_BUFFER_KEY);

	// A completar por el alumno
	// ...
}

