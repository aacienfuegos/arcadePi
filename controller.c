# include "controller.h"

fsm_trans_t fsm_trans_selector[] = {
    { WAIT_PUSH, CompruebaMovimientoAbajo, WAIT_PUSH, SelectPrevGame  },
    { WAIT_PUSH, CompruebaMovimientoArriba, WAIT_PUSH, SelectNextGame },
    { WAIT_PUSH, CompruebaBotonPulsado, WAIT_END, SelectGame },
    { WAIT_END, CompruebaExitGames, WAIT_PUSH, NULL },
    {-1, NULL, -1, NULL },
};

int CompruebaExitGames(fsm_t* this) {
    TipoController *p_controller;
    p_controller = (TipoController*)(this->user_data);

	int result = 0;
	int flags_inversed = flags ^ (FLAG_JUEGO_ARKANOPI | FLAG_JUEGO_PONG);

	piLock(SYSTEM_FLAGS_KEY);
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

	/* p_controller->game++; */
	/* if(p_controller->game >= NUM_JUEGOS) p_controller->game = 0; */
	p_controller->game = ARKANOPI;
	led_text_main(p_controller->icons[p_controller->game]);
	printf("Game %d chosen\n", p_controller->game);
}

void SelectPrevGame(fsm_t *this){
    TipoController *p_controller;
    p_controller = (TipoController*)(this->user_data);

	piLock(SYSTEM_FLAGS_KEY);
	flags &= (~FLAG_MOV_ABAJO);
	piUnlock(SYSTEM_FLAGS_KEY);

	/* p_controller->game--; */
	/* if(p_controller->game < 0) p_controller->game = NUM_JUEGOS-1; */
	p_controller->game = PONG;
	led_text_main(p_controller->icons[p_controller->game]);
	printf("Game %d chosen\n", p_controller->game);
}

void SelectGame(fsm_t *this){
    TipoController *p_controller;
    p_controller = (TipoController*)(this->user_data);

	piLock(SYSTEM_FLAGS_KEY);
	flags &= (~FLAG_BOTON);
	piUnlock(SYSTEM_FLAGS_KEY);

	/* printf("%d\n", p_controller->game); */
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
