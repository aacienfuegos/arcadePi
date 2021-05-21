# include "controller.h"

fsm_trans_t fsm_trans_selector[] = {
    { WAIT_PUSH, CompruebaMovimientoAbajo, WAIT_PUSH, SelectPrevGame  },
    { WAIT_PUSH, CompruebaMovimientoArriba, WAIT_PUSH, SelectNextGame },
    { WAIT_PUSH, CompruebaBotonPulsado, WAIT_END, SelectGame },
    { WAIT_END, CompruebaExitGames, WAIT_PUSH, ExitGames },
    {-1, NULL, -1, NULL },
};

/* CompruebaExitGames(fsm_t* this) se encarga de comprobar que ninguno */
/* de los juegos se está corriendo */
int CompruebaExitGames(fsm_t* this) {
    TipoController *p_controller;
    p_controller = (TipoController*)(this->user_data);

	int result = 0;
	
	// Invertimos las flags de los juegos
	int flags_inversed = flags ^ (FLAG_JUEGO_ARKANOPI | FLAG_JUEGO_PONG);
	// De esta forma observamos una flag HIGH cuando NO estamos jugando

	piLock(SYSTEM_FLAGS_KEY);
	// Comprobamos si esa flag de NO jugar está activada
	result = (flags_inversed & FLAG_JUEGO_ARKANOPI) && (flags_inversed & FLAG_JUEGO_PONG);
	piUnlock(SYSTEM_FLAGS_KEY);
	
	return result;
}

void SelectNextGame(fsm_t* this){
    TipoController *p_controller;
    p_controller = (TipoController*)(this->user_data);

	piLock(SYSTEM_FLAGS_KEY);
	flags &= (~FLAG_MOV_ARRIBA);
	piUnlock(SYSTEM_FLAGS_KEY);

	// seleccionar el juego siguiente 
	p_controller->game++;
	if(p_controller->game >= NUM_JUEGOS) p_controller->game = 0;

	// Mostrar el icono del juego seleccionado
	display_icon(p_controller->game);
}

void SelectPrevGame(fsm_t *this){
    TipoController *p_controller;
    p_controller = (TipoController*)(this->user_data);

	piLock(SYSTEM_FLAGS_KEY);
	flags &= (~FLAG_MOV_ABAJO);
	piUnlock(SYSTEM_FLAGS_KEY);

	// seleccionar el juego anterior
	p_controller->game--;
	if(p_controller->game < 0) p_controller->game = NUM_JUEGOS-1;

	// Mostrar el icono del juego seleccionado
	display_icon(p_controller->game);
	printf("Game %d chosen\n", p_controller->game);
}

// Función para confirmar la elección del juego
void SelectGame(fsm_t *this){
    TipoController *p_controller;
    p_controller = (TipoController*)(this->user_data);

	piLock(SYSTEM_FLAGS_KEY);
	flags &= (~FLAG_BOTON);
	piUnlock(SYSTEM_FLAGS_KEY);

	// Activamos la flag del juego correspondiente
	switch(p_controller->game){
		case ARKANOPI:
			piLock(SYSTEM_FLAGS_KEY);
			flags |= FLAG_JUEGO_ARKANOPI;
			piUnlock(SYSTEM_FLAGS_KEY);
			break;
		case PONG:
			piLock(SYSTEM_FLAGS_KEY);
			flags |= FLAG_JUEGO_PONG;
			piUnlock(SYSTEM_FLAGS_KEY);
			break;
	}
}

void ExitGames(fsm_t* this){
    TipoController *p_controller;
    p_controller = (TipoController*)(this->user_data);
	
	// volvemos a mostrar el icono inicial
	display_icon(HI_ICON);
}