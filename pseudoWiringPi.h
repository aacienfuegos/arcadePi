/*
 * pseudoWiringPi.h
 *
 *  Created on: 31 de Mar. de 2020
 *      Author: FFM
 *      COMPATIBLE v4.0
 */

#ifndef _PSEUDO_WIRINGPI_H_
#define _PSEUDO_WIRINGPI_H_

#include <pthread.h>
#include "systemLib.h"

// C doesn't have true/false by default and I can never remember which
//	way round they are, so ...
//	(and yes, I know about stdbool.h but I like capitals for these and I'm old)

#ifndef	TRUE
#  define	TRUE	(1==1)
#  define	FALSE	(!TRUE)
#endif

// GCC warning suppressor

#define	UNU	__attribute__((unused))

// Mask for the bottom 64 pins which belong to the Raspberry Pi
//	The others are available for the other devices

#define	PI_GPIO_MASK	(0xFFFFFFC0)

// Handy defines

// wiringPi modes

#define	WPI_MODE_PINS		 0
#define	WPI_MODE_GPIO		 1
#define	WPI_MODE_GPIO_SYS	 2
#define	WPI_MODE_PHYS		 3
#define	WPI_MODE_PIFACE		 4
#define	WPI_MODE_UNINITIALISED	-1

// Pin modes

#define	INPUT			 0
#define	OUTPUT			 1
#define	PWM_OUTPUT		 2
#define	GPIO_CLOCK		 3
#define	SOFT_PWM_OUTPUT		 4
#define	SOFT_TONE_OUTPUT	 5
#define	PWM_TONE_OUTPUT		 6

#define	LOW			0
#define	HIGH		1

// Pull up/down/none

#define	PUD_OFF			 0
#define	PUD_DOWN		 1
#define	PUD_UP			 2

// PWM

#define	PWM_MODE_MS		0
#define	P				WM_MODE_BAL		1

// Interrupt levels

#define	INT_EDGE_SETUP		0
#define	INT_EDGE_FALLING	1
#define	INT_EDGE_RISING		2
#define	INT_EDGE_BOTH		3

#define	MAX_NUM_INPUT_KEYS 	4

// Core wiringPi functions
extern int  wiringPiSetupGpio   (void) ;

extern          void pinMode             (int pin, int mode) ;
extern          void pullUpDnControl     (int pin, int pud) ;
extern          void digitalWrite        (int pin, int value) ;

// Interrupts
//	(Also Pi hardware specific)
extern int  waitForInterruptSTDIN    (int mS) ;
extern int piHiPri (const int pri);
extern int  wiringPiISR         (int pin, int mode, void (*function)(void)) ;

// Threads

#define	PI_THREAD(X)	void *X (UNU void *dummy)

// Failure modes

#define	WPI_FATAL	(1==1)
#define	WPI_ALMOST	(1==2)

extern int  piThreadCreate      (void *(*fn)(void *)) ;
extern void piLock              (int key) ;
extern void piUnlock            (int key) ;

extern void         delay             (unsigned int howLong) ;
extern unsigned int millis            (void) ;

extern void pseudoWiringPiEnableDisplay(int estado);

#endif /* _PSEUDO_WIRINGPI_H_ */
