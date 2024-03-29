#ifndef _LEDDISPLAYAUX_H_
#define _LEDDISPLAYAUX_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <wiringPiSPI.h>
#include "systemLib.h"

#define SPI_CHANNEL 0
#define CLOCK_SPEED 1000000

#define uchar unsigned char
#define uint unsigned int

enum icons_enum {
  ARKANOPI_ICON,
  PONG_ICON,
  HI_ICON
};

void InitSPI();

void display_clear();
void display_score(int score[], int nPlayers);
void display_countdown(int start, int delay);
void display_icon(int icon);
void display_text(char* text);

#endif /* _LEDDISPLAYAUX_H_ */