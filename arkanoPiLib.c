#include "arkanoPiLib.h"
/* #include "commonLib.h" */
#include "ledDisplay.h"
#include <stdio.h>
#include <string.h>


int ladrillos_basico[NUM_FILAS_DISPLAY][NUM_COLUMNAS_DISPLAY] = {
	{1,1,1,1,1,1,1,1}, // 0xFF
	{1,1,1,1,1,1,1,1}, // 0xFF
	{0,0,0,0,0,0,0,0}, // 0x00
	{0,0,0,0,0,0,0,0}, // 0x00
	{0,0,0,0,0,0,0,0}, // 0x00
	{0,0,0,0,0,0,0,0}, // 0x00
	{0,0,0,0,0,0,0,0}, // 0x00

};

int score = 0;
char c_score[17] = "0123456789ABCDEFG";
//"ABCDEFG";

//------------------------------------------------------
// FUNCIONES DE INICIALIZACION / RESET
//------------------------------------------------------

void InicializaLadrillos(tipo_pantalla *p_ladrillos) {
	int i=0, j=0;

	for(i=0;i<NUM_FILAS_DISPLAY;i++) {
		for(j=0;j<NUM_COLUMNAS_DISPLAY;j++) {
			p_ladrillos->matriz[i][j] = ladrillos_basico[i][j];
		}
	}
}

void PintaLadrillos(tipo_pantalla *p_ladrillos, tipo_pantalla *p_pantalla) {
	int i=0, j=0;

	for(i=0;i<NUM_FILAS_DISPLAY;i++) {
		for(j=0;j<NUM_COLUMNAS_DISPLAY;j++) {
			p_pantalla->matriz[i][j] = p_ladrillos->matriz[i][j];
		}
    }
}

void ActualizaPantalla(tipo_arkanoPi* p_arkanoPi) {

    // Borro toda la pantalla
	ReseteaPantalla((tipo_pantalla*)(p_arkanoPi->p_pantalla));

    // Pinta los ladrillos
	PintaLadrillos(
		(tipo_pantalla*)(&(p_arkanoPi->ladrillos)),
		(tipo_pantalla*)(p_arkanoPi->p_pantalla));

    // Pinta la pala
	PintaPala(
		(tipo_pala*)(&(p_arkanoPi->pala)),
		(tipo_pantalla*)(p_arkanoPi->p_pantalla));

	// Pinta la pelota
	PintaPelota(
		(tipo_pelota*)(&(p_arkanoPi->pelota)),
		(tipo_pantalla*)(p_arkanoPi->p_pantalla));
}

void InicializaArkanoPi(tipo_arkanoPi *p_arkanoPi) {
	ResetArkanoPi(p_arkanoPi);
	ActualizaPantalla(p_arkanoPi);
}

void ResetArkanoPi(tipo_arkanoPi *p_arkanoPi) {
	ReseteaPantalla((tipo_pantalla*)(p_arkanoPi->p_pantalla));
	InicializaLadrillos((tipo_pantalla*)(&(p_arkanoPi->ladrillos)));
	InicializaPelota((tipo_pelota*)(&(p_arkanoPi->pelota)));
	InicializaPala((tipo_pala*)(&(p_arkanoPi->pala)));
}

int CompruebaReboteLadrillo (tipo_arkanoPi *p_arkanoPi) {
	int p_posible_ladrillo_x = 0;
	int p_posible_ladrillo_y = 0;

	p_posible_ladrillo_x = p_arkanoPi->pelota.x + p_arkanoPi->pelota.trayectoria.xv;

	if ( ( p_posible_ladrillo_x < 0) || ( p_posible_ladrillo_x >= NUM_COLUMNAS_DISPLAY ) ) {
		printf("\n\nERROR GRAVE!!! p_posible_ladrillo_x = %d!!!\n\n", p_posible_ladrillo_x);
		fflush(stdout);
		exit(-1);
	}

	p_posible_ladrillo_y = p_arkanoPi->pelota.y + p_arkanoPi->pelota.trayectoria.yv;

	if ( ( p_posible_ladrillo_y < 0) || ( p_posible_ladrillo_y >= NUM_FILAS_DISPLAY ) ) {
		printf("\n\nERROR GRAVE!!! p_posible_ladrillo_y = %d!!!\n\n", p_posible_ladrillo_y);
		fflush(stdout);
	}

	if(p_arkanoPi->ladrillos.matriz[p_posible_ladrillo_y][p_posible_ladrillo_x] > 0 ) {
		// La pelota ha entrado en el area de ladrillos
		// y descontamos el numero de golpes que resta para destruir el ladrillo
		p_arkanoPi->ladrillos.matriz[p_posible_ladrillo_y][p_posible_ladrillo_x] = p_arkanoPi->ladrillos.matriz[p_posible_ladrillo_y][p_posible_ladrillo_x] - 1;

		return 1;
	}
	return 0;
}

int CompruebaReboteParedesVerticales (tipo_arkanoPi arkanoPi) {
	// Comprobamos si la nueva posicion de la pelota excede los limites de la pantalla
	if((arkanoPi.pelota.x + arkanoPi.pelota.trayectoria.xv >= NUM_COLUMNAS_DISPLAY) ||
		(arkanoPi.pelota.x + arkanoPi.pelota.trayectoria.xv < 0) ) {
		// La pelota rebota contra la pared derecha o izquierda
		return 1;
	}
	return 0;
}

int CompruebaReboteTecho (tipo_arkanoPi arkanoPi) {
	// Comprobamos si la nueva posicion de la pelota excede los limites de la pantalla
	if(arkanoPi.pelota.y + arkanoPi.pelota.trayectoria.yv < 0) {
		// La pelota rebota contra la pared derecha o izquierda
		return 1;
	}
	return 0;
}

int CompruebaRebotePala (tipo_arkanoPi arkanoPi) {
	if(arkanoPi.pelota.trayectoria.yv > 0) { // Esta condicion solo tiene sentido si la pelota va hacia abajo en la pantalla
		if ((arkanoPi.pelota.x + arkanoPi.pelota.trayectoria.xv >= arkanoPi.pala.x ) &&
			(arkanoPi.pelota.x + arkanoPi.pelota.trayectoria.xv < arkanoPi.pala.x + NUM_COLUMNAS_PALA)) {
				if ((arkanoPi.pelota.y + arkanoPi.pelota.trayectoria.yv >= arkanoPi.pala.y) &&
					(arkanoPi.pelota.y + arkanoPi.pelota.trayectoria.yv < arkanoPi.pala.y + NUM_FILAS_PALA)) {
					return 1;
				}
		}
	}
	return 0;
}

int CompruebaFallo (tipo_arkanoPi arkanoPi) {
	// Comprobamos si no hemos conseguido devolver la pelota
	if(arkanoPi.pelota.y + arkanoPi.pelota.trayectoria.yv >= NUM_FILAS_DISPLAY) {
		// Hemos fallado
		return 1;
	}
	return 0;
}

int CalculaLadrillosRestantes(tipo_pantalla *p_ladrillos) {
	int i=0, j=0;
	int numLadrillosRestantes;

	numLadrillosRestantes = 0;

	for(i=0;i<NUM_FILAS_DISPLAY;i++) {
		for(j=0;j<NUM_COLUMNAS_DISPLAY;j++) {
			if(p_ladrillos->matriz[i][j] != 0) {
				numLadrillosRestantes++;
			}
		}
	}

	return numLadrillosRestantes;
}

//------------------------------------------------------
// FUNCIONES DE ACCION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

void ExitArkano (fsm_t* this) {
	tipo_arkanoPi* p_arkanoPi;
	p_arkanoPi = (tipo_arkanoPi*)(this->user_data);

	piLock(SYSTEM_FLAGS_KEY);
	flags &= (~FLAG_JUEGO_ARKANOPI);
	piUnlock(SYSTEM_FLAGS_KEY);
}

// void MuevePalaIzquierda (void): funcion encargada de ejecutar
// el movimiento hacia la izquierda contemplado para la pala.
// Debe garantizar la viabilidad del mismo mediante la comprobación
// de que la nueva posición correspondiente a la pala no suponga
// que ésta rebase o exceda los límites definidos para el área de juego
// (i.e. al menos uno de los leds que componen la raqueta debe permanecer
// visible durante todo el transcurso de la partida).

void MuevePalaIzquierda (fsm_t* this) {
	tipo_arkanoPi* p_arkanoPi;
	p_arkanoPi = (tipo_arkanoPi*)(this->user_data);

	// A completar por el alumno
	// ...
	piLock(SYSTEM_FLAGS_KEY);
	flags &= (~FLAG_MOV_IZQUIERDA);
	piUnlock(SYSTEM_FLAGS_KEY);

	ActualizaPosicionPala(&(p_arkanoPi->pala), IZQUIERDA);
	fflush(stdout);

	piLock(MATRIX_KEY); // CLAVE PANTALLA
	ActualizaPantalla(p_arkanoPi);
	piUnlock(MATRIX_KEY); // CLAVE PANTALLA

	piLock(STD_IO_BUFFER_KEY); // CLAVE E/S STD
	PintaPantallaPorTerminal(p_arkanoPi->p_pantalla);
	piUnlock(STD_IO_BUFFER_KEY); // CLAVE E/S STD

}

// void MuevePalaDerecha (void): función similar a la anterior
// encargada del movimiento hacia la derecha.

void MuevePalaDerecha (fsm_t* this) {
	tipo_arkanoPi* p_arkanoPi;
	p_arkanoPi = (tipo_arkanoPi*)(this->user_data);

	// A completar por el alumno
	// ...
	piLock(SYSTEM_FLAGS_KEY);
	flags &= (~FLAG_MOV_DERECHA);
	piUnlock(SYSTEM_FLAGS_KEY);

	ActualizaPosicionPala(&(p_arkanoPi->pala), DERECHA);

	piLock(MATRIX_KEY); // CLAVE PANTALLA
	ActualizaPantalla(p_arkanoPi);
	piUnlock(MATRIX_KEY); // CLAVE PANTALLA

	piLock(STD_IO_BUFFER_KEY); // CLAVE E/S STD
	PintaPantallaPorTerminal(p_arkanoPi->p_pantalla);
	piUnlock(STD_IO_BUFFER_KEY); // CLAVE E/S STD
}

// void PausaJuego (void): función encargada de pausar el juego

void PausarJuego (fsm_t* this) {
	tipo_arkanoPi *p_arkanoPi;
	p_arkanoPi = (tipo_arkanoPi*)(this->user_data);

	piLock(SYSTEM_FLAGS_KEY);
	flags &= (~FLAG_PAUSA_JUEGO);
	piUnlock(SYSTEM_FLAGS_KEY);

	int i,j;
	for(i=0;i<NUM_FILAS_DISPLAY;i++) {
		for(j=0;j<NUM_COLUMNAS_DISPLAY;j++) {
			p_arkanoPi->p_pantalla->matriz[i][j] = pantalla_pausa.matriz[i][j];
		}
	}


	printf("PAUSA\n");
	fflush(stdout);
}

// void ContinuaJuego (void): función encargada de continuar el juego tras una pausa

void ContinuarJuego (fsm_t* this) {
	tipo_arkanoPi *p_arkanoPi;
	p_arkanoPi = (tipo_arkanoPi*)(this->user_data);

	printf("Continua en:\n");
	fflush(stdout);

	led_text_main("321",1000);

	piLock(SYSTEM_FLAGS_KEY);
	flags &= (~FLAG_PAUSA_JUEGO);
	piUnlock(SYSTEM_FLAGS_KEY);

	ActualizarJuego(this);

}

//------------------------------------------------------
// FUNCIONES DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

int CompruebaIniciaArkano(fsm_t* this) {
	int result = 0;

	// A completar por el alumno
	// ...
	piLock(SYSTEM_FLAGS_KEY);
	result = (flags & FLAG_JUEGO_ARKANOPI);
	piUnlock(SYSTEM_FLAGS_KEY);

	return result;
}
//------------------------------------------------------
// FUNCIONES DE ACCION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

// void InicializaJuego (void): funcion encargada de llevar a cabo
// la oportuna inicialización de toda variable o estructura de datos
// que resulte necesaria para el desarrollo del juego.

void InicializaJuego(fsm_t* this) {
	tipo_arkanoPi *p_arkanoPi;
	p_arkanoPi = (tipo_arkanoPi*)(this->user_data);

	p_arkanoPi->tmr_actualizacion_juego_isr = tmr_new(tmr_actualizacion_juego_isr);

	// A completar por el alumno
	// ...
	piLock (STD_IO_BUFFER_KEY);
	InicializaArkanoPi(p_arkanoPi);

	tmr_startms(p_arkanoPi->tmr_actualizacion_juego_isr, TIMEOUT_ACTUALIZA_JUEGO);

	PintaMensajeInicialPantalla(p_arkanoPi->p_pantalla, p_arkanoPi->p_pantalla);
	PintaPantallaPorTerminal(p_arkanoPi->p_pantalla);

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

void ActualizarJuego (fsm_t* this) {
	tipo_arkanoPi* p_arkanoPi;
	p_arkanoPi = (tipo_arkanoPi*)(this->user_data);

	piLock (SYSTEM_FLAGS_KEY);
	flags &= (~ FLAG_TIMER_JUEGO);
	piUnlock (SYSTEM_FLAGS_KEY);

	if(CompruebaReboteParedesVerticales(*p_arkanoPi)) {
		p_arkanoPi->pelota.trayectoria.xv = -p_arkanoPi->pelota.trayectoria.xv;
	}

	if(CompruebaReboteTecho(*p_arkanoPi)) {
		p_arkanoPi->pelota.trayectoria.yv = -p_arkanoPi->pelota.trayectoria.yv;
	}

	if(CompruebaFallo (*p_arkanoPi)) {
		piLock (SYSTEM_FLAGS_KEY);
		flags |= FLAG_FIN_JUEGO;
		piUnlock (SYSTEM_FLAGS_KEY);
		return;

	} else if (CompruebaRebotePala (*p_arkanoPi)) {
		switch(p_arkanoPi->pelota.x + p_arkanoPi->pelota.trayectoria.xv - p_arkanoPi->pala.x) {

		case 0:
		CambiarDireccionPelota(&(p_arkanoPi->pelota),ARRIBA_IZQUIERDA);
		break;

		case 1:
		CambiarDireccionPelota(&(p_arkanoPi->pelota), ARRIBA);
		break;

		case 2:
		CambiarDireccionPelota(&(p_arkanoPi->pelota), ARRIBA_DERECHA);
		break;

		}
	}
	if (CompruebaReboteLadrillo(p_arkanoPi)) {
		p_arkanoPi->pelota.trayectoria.yv = -p_arkanoPi->pelota.trayectoria.yv;
		score ++;
		if(CalculaLadrillosRestantes(&(p_arkanoPi->ladrillos))<= 0) {
			piLock (SYSTEM_FLAGS_KEY);
			flags |= FLAG_FIN_JUEGO;
			piUnlock (SYSTEM_FLAGS_KEY);
			return;
	}
	}

	ActualizaPosicionPelota (&(p_arkanoPi->pelota));

	char s_Score[2];
		
		s_Score[0] = c_score[score];
		s_Score[1]= '\0';

	led_text_main(s_Score,0);

	piLock(MATRIX_KEY);
	ActualizaPantalla (p_arkanoPi);
	piUnlock(MATRIX_KEY);

	piLock (STD_IO_BUFFER_KEY);
	PintaPantallaPorTerminal(p_arkanoPi->p_pantalla);
	piUnlock (STD_IO_BUFFER_KEY);

	tmr_startms(p_arkanoPi->tmr_actualizacion_juego_isr, TIMEOUT_ACTUALIZA_JUEGO);

}

// void FinalJuego (void): función encargada de mostrar en la ventana de
// terminal los mensajes necesarios para informar acerca del resultado del juego.

void FinalJuego (fsm_t* this) {
	tipo_arkanoPi *p_arkanoPi;
	p_arkanoPi = (tipo_arkanoPi*)(this->user_data);

	// A completar por el alumno
	// ...
	piLock(SYSTEM_FLAGS_KEY);
	flags &= (~FLAG_FIN_JUEGO);
	piUnlock(SYSTEM_FLAGS_KEY);


	if(CalculaLadrillosRestantes(&(p_arkanoPi->ladrillos))==0){
		piLock(STD_IO_BUFFER_KEY);
		printf("FIN DEL JUEGO: HAS GANADO\n");
		piUnlock(STD_IO_BUFFER_KEY);
	} else {
		piLock(STD_IO_BUFFER_KEY);
		printf("FIN DEL JUEGO: HAS PERDIDO\n");
		piUnlock(STD_IO_BUFFER_KEY);

		int i,j;
		for(i=0;i<NUM_FILAS_DISPLAY;i++) {
			for(j=0;j<NUM_COLUMNAS_DISPLAY;j++) {
				p_arkanoPi->p_pantalla->matriz[i][j] = pantalla_final.matriz[i][j];
			}
		}
	}

	/* pseudoWiringPiEnableDisplay(0); */
}

//void ReseteaJuego (void): función encargada de llevar a cabo la
// reinicialización de cuantas variables o estructuras resulten
// necesarias para dar comienzo a una nueva partida.

void ReseteaJuego (fsm_t* this) {
	tipo_arkanoPi *p_arkanoPi;
	p_arkanoPi = (tipo_arkanoPi*)(this->user_data);
	
	piLock(SYSTEM_FLAGS_KEY);
	flags &= (~FLAG_BOTON);
	piUnlock(SYSTEM_FLAGS_KEY);

	piLock (STD_IO_BUFFER_KEY);
	ResetArkanoPi(p_arkanoPi);

	piUnlock (STD_IO_BUFFER_KEY);

	score = 0;
	// A completar por el alumno
	// ...
}

