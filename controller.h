#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "systemLib.h"
#include "fsm.h"
#include "commonLib.h"
#include "ledDisplayAux.h"

#define NUM_JUEGOS 2

typedef struct
{
	int game; 
	
} TipoController;

enum games {
	ARKANOPI,
	PONG
};

int CompruebaExitGames (fsm_t* this);

void SelectNextGame (fsm_t* this);
void SelectPrevGame (fsm_t* this);
void SelectGame (fsm_t* this);
void ExitGames (fsm_t* this);

extern TipoController controller;
extern fsm_trans_t fsm_trans_selector[];
extern int flags;

#endif /* _CONTROLLER_H_ */