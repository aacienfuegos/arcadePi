#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "systemLib.h"
#include "fsm.h"
#include "commonLib.h"

#define NUM_JUEGOS 2

typedef struct
{
	int game; // Game selected 0 arkano, 1 pong...
	char icons[NUM_JUEGOS][2];
} TipoController;


int CompruebaExitGames (fsm_t* this);

void SelectNextGame ();
void SelectPrevGame ();
void SelectGame ();

extern TipoController controller;
extern fsm_trans_t fsm_trans_selector[];
/* extern int flags; */
/* extern int flags_controller; */

#endif /* _CONTROLLER_H_ */