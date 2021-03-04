/*
 * pseudoWiringPi.c
 *
 *  Created on: 31 de Mar. de 2020
 *      Author: FFM
 *      COMPATIBLE v4.0
 */


#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <poll.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <asm/ioctl.h>

#include <sched.h>
#include <string.h>

#include "pseudoWiringPi.h"

#define BACKUP_NEWLINE "\033[A\033[C\033[C\033[C\033[C\033[C\033[C\033[C\033[C"


char colors [] = {};

static int waitForInterruptSTDIN_process = 0;

pthread_t myThread ;
static pthread_mutex_t piMutexes [4] ;

// Misc

static int wiringPiMode = WPI_MODE_UNINITIALISED ;
static pthread_mutex_t pinMutex ;


// Debugging & Return codes

int wiringPiDebug       = FALSE ;
int wiringPiReturnCodes = FALSE ;

// Time for easy calculations

static uint64_t epochMilli ;

// ISR Data

static void (*isrFunctions [64])(void) ;

static int columnaTecladoActiva = -1;
static int columnaDisplayActiva = -1;

static char pseudoTecladoTL04[4][4] = {
	{'1', '2', '3', 'c'},
	{'4', '5', '6', 'd'},
	{'7', '8', '9', 'e'},
	{'a', '0', 'b', 'f'}
};

// Matriz interna de pseudoWiringPi para modelar el estado del display emulado
// NOTA: se emula el conjunto display mas HW acondicionamiento al completo, incluido el decoder correspondiente
// que traduce el estado de los 3 pines GPIO definidos al valor de la columna a excitar en cada momento
static int pseudoMatrizColor[7][8] = {
		{31,31,31,31,31,31,31,31},
		{31,31,31,31,31,31,31,31},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{34,34,34,34,34,34,34,34},
};

static int pseudoMatriz[7][8] = {
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0},
};

static int GPIO_to_cols[64]; // Array para el mapeo de pines GPIO a columnas del decoder (el valor codificado corresponde a la columna del display emulado)
static int GPIO_to_rows[64]; // Array para el mapeo de pines GPIO a filas del display

static int array_flags_check_columnas_teclado [4] = {0x01, 0x02, 0x04, 0x08};
static int array_flags_check_columnas_display [3] = {0x01, 0x02, 0x04};

static int primeraVezDisplay = 1; // Flag util para determinar cuando hay que borrar la pantalla antes de volver a escribirla
// Al ejecutarse desde la terminal (fuera del entorno) aporta la ventaja de evitar el scroll vertical hacia abajo propio del repintado de la pantalla

static int numWritesAfterColumnEnable[7]; // Array util para determinar cuando un digitalWrite debe actualizar el contenido de pseudoMatriz
// ACLARACION: el display real exige apagar todas las filas antes de proceder a un cambio de excitacion (i.e. de columna)
// De lo contrario, en el momento de aplicar dicha nueva excitacion, las filas podrian estar reflejando un estado diferente
// del que en realidad corresponde a la nueva columna a visualizar
// Ese apagado no plantea mayor problema desde el punto de vista del display real. Sin embargo, a efectos del emulado,
// cuyo comportamiento y estado esta modelado mediante pseudoMatriz, es MUY IMPORTANTE que dicho apagado NO ALTERE O
// MODIFIQUE el contenido de pseudoMatriz o de lo contrario SOLO VEREMOS UN DISPLAY EMULADO CUYO CONTENIDO ES BASICAMENTE 0s
// (TODO APAGADO)

// La emulación del display se apoya en la idea de emplear la salida estandar, la terminal, como sustituto
// del display real. Para ello se ha considerado que era IMPORTANTE que el uso de la terminal con dicho fin
// no debia impedir u obstaculizar el normal uso de la misma para presentar mensajes via printf.
// Por ese motivo, a efectos de la emulación del display se distingue entre dos posibles estados:
//
// 1) EMULACION DEL DISPLAY: la terminal se pone al servicio de la emulación. Bajo este estado
// NO ES POSIBLE HACER USO DE LA TERMINAL PARA LA ESCRITURA O PRESENTACION DE MENSAJES VIA PRINTF
// El estado viene determinado por JUGANDO = 1, para lo cual sera necesario hacer uso de la llamada a:
//
// 		pseudoWiringPiEnableDisplay(1);
//
// El lugar adecuado para realizar dicha llamada es al FINAL de:
//
// void InicializaJuego(fsm_t* this) {}
//
// 2) USO NORMAL DE LA TERMINAL: funcionamiento NORMAL de la terminal, sin emulacion del display.
// Se puede emplear la terminal del mismo modo en que venimos haciendolo sin mayor problema.
// Este estado viene determinado por JUGANDO = 0, para lo cual sera necesario hacer uso de la llamada a:
//
// 		pseudoWiringPiEnableDisplay(0);
//
// El lugar adecuado para realizar dicha llamada es al COMIENZO de:
//
// void FinalJuego (fsm_t* this) {}


static int JUGANDO = 0; // Flag general cuyo valor determina si estamos usando el display emulado o no

/*
 * wiringPiFailure:
 *	Fail. Or not.
 *********************************************************************************
 */

int wiringPiFailure (int fatal, const char *message, ...)
{
  va_list argp ;
  char buffer [1024] ;

  if (!fatal && wiringPiReturnCodes)
    return -1 ;

  va_start (argp, message) ;
    vsnprintf (buffer, 1023, message, argp) ;
  va_end (argp) ;

  fprintf (stderr, "%s", buffer) ;
  exit (EXIT_FAILURE) ;

  return 0 ;
}

/*
 * wiringPiSetupGpio:
 *	Must be called once at the start of your program execution.
 *
 * GPIO setup: Initialises the system into GPIO Pin mode and uses the
 *	memory mapped hardware directly.
 *********************************************************************************
 */

int wiringPiSetupGpio (void)
{
	int i = 0;

	if (wiringPiDebug)
		printf ("wiringPi: wiringPiSetupGpio called\n") ;

	wiringPiMode = WPI_MODE_GPIO ;

	for(i=0;i<64;i++) {
		GPIO_to_rows[i] = -1;
		GPIO_to_cols[i] = -1;
	}

	GPIO_to_rows[GPIO_LED_DISPLAY_ROW_1] = 0;
	GPIO_to_rows[GPIO_LED_DISPLAY_ROW_2] = 1;
	GPIO_to_rows[GPIO_LED_DISPLAY_ROW_3] = 2;
	GPIO_to_rows[GPIO_LED_DISPLAY_ROW_4] = 3;
	GPIO_to_rows[GPIO_LED_DISPLAY_ROW_5] = 4;
	GPIO_to_rows[GPIO_LED_DISPLAY_ROW_6] = 5;
	GPIO_to_rows[GPIO_LED_DISPLAY_ROW_7] = 6;

	GPIO_to_cols[GPIO_LED_DISPLAY_COL_1] = 0;
	GPIO_to_cols[GPIO_LED_DISPLAY_COL_2] = 1;
	GPIO_to_cols[GPIO_LED_DISPLAY_COL_3] = 2;

	columnaDisplayActiva = 0;

	for(i=0;i<7;i++)
		numWritesAfterColumnEnable[i] = 0;

	return 0 ;
}

/*
 * pinMode:
 *	Sets the mode of a pin to be input, output or PWM output
 *********************************************************************************
 */

void pinMode (int pin, int mode)
{
	if (wiringPiMode != WPI_MODE_GPIO)	// Sys mode
	{
	  printf("[pseudoWiringPi][ERROR!!!][Modo de configuración incorrecto!!!][Use wiringPiSetupGpio ()]\n");
	  fflush(stdout);
	  return;
	}

	if (mode == INPUT){
	  printf("[pseudoWiringPi][pinMode][pin %d][INPUT]\n", pin);
	  fflush(stdout);
	}
	else if (mode == OUTPUT){
	  printf("[pseudoWiringPi][pinMode][pin %d][OUTPUT]\n", pin);
	  fflush(stdout);
	}
}

/*
 * pullUpDownCtrl:
 *	Control the internal pull-up/down resistors on a GPIO pin.
 *********************************************************************************
 */

void pullUpDnControl (int pin, int pud)
{
  if ((pin & PI_GPIO_MASK) == 0)		// On-Board Pin
  {
	 if (wiringPiMode != WPI_MODE_GPIO)	// Sys mode
	  {
		printf("[pseudoWiringPi][ERROR!!!][Modo de configuración incorrecto!!!][Use wiringPiSetupGpio ()]\n");
		fflush(stdout);
		return;
	  }

		switch (pud)
		{
			case PUD_OFF:
				printf("[pseudoWiringPi][pullUpDnControl][pin %d][PUD_OFF]\n", pin);
				break;
			case PUD_UP:
				printf("[pseudoWiringPi][pullUpDnControl][pin %d][PUD_UP]\n", pin);
				break;
			case PUD_DOWN:
				printf("[pseudoWiringPi][pullUpDnControl][pin %d][PUD_DOWN]\n", pin);
				break;
			default:
				printf("[pseudoWiringPi][ERROR!!!][pullUpDnControl][Modo incorrecto!!!][Use PUD_OFF o PUD_DOWN]\n");
				fflush(stdout);
				return ; /* An illegal value */
		}
  }
}

/*
 * waitForInterrupt:
 *	Pi Specific.
 *	Wait for Interrupt on a GPIO pin.
 *	This is actually done via the /sys/class/gpio interface regardless of
 *	the wiringPi access mode in-use. Maybe sometime it might get a better
 *	way for a bit more efficiency.
 *********************************************************************************
 */

int waitForInterruptSTDIN (int mS)
{
  uint8_t c ;
  int i, flagsColumnsChecked;
  int pinesFilasTeclado[4] = {
		  GPIO_KEYBOARD_ROW_1,
		  GPIO_KEYBOARD_ROW_2,
		  GPIO_KEYBOARD_ROW_3,
		  GPIO_KEYBOARD_ROW_4};

  // Wait for it ...
  while(1) {
		delay(50); // Wiring Pi function that pauses program execution for at least 10 millisecond

		piLock (STD_IO_BUFFER_KEY);
		if(kbhit()) {
			c = kbread();
			piUnlock (STD_IO_BUFFER_KEY);
			break;
		}
		piUnlock (STD_IO_BUFFER_KEY);
  }

	flagsColumnsChecked = 0;
	while(flagsColumnsChecked<15) { // antes de tirar una pulsacion me aseguro de haber comprobado las 4 columnas
		piLock(KEYBOARD_KEY); // columnaTecladoActiva lo modifican los digitalWrite
		for(i=0;i<4;i++){
			if(tolower(c) == pseudoTecladoTL04[i][columnaTecladoActiva]){
				piUnlock(KEYBOARD_KEY);
				isrFunctions [pinesFilasTeclado[i]] () ;
				return c;
			}
		}

		flagsColumnsChecked |= array_flags_check_columnas_teclado[columnaTecladoActiva];
		piUnlock(KEYBOARD_KEY);
		// delay para permitir el cambio de excitacion
		delay(5);
	}

	return c ;
}

/*
 * interruptHandler:
 *	This is a thread and gets started to wait for the interrupt we're
 *	hoping to catch. It will call the user-function when the interrupt
 *	fires.
 *********************************************************************************
 */

static void *interruptHandlerSTDIN (UNU void *arg)
{
  //(void)piHiPri (55) ;	// Only effective if we run as root

  for (;;)
	waitForInterruptSTDIN (-1);

  return NULL ;
}

/*
 * piHiPri:
 *	Attempt to set a high priority schedulling for the running program
 *********************************************************************************
 */

int piHiPri (const int pri)
{
  struct sched_param sched ;

  memset (&sched, 0, sizeof(sched)) ;

  if (pri > sched_get_priority_max (SCHED_RR))
    sched.sched_priority = sched_get_priority_max (SCHED_RR) ;
  else
    sched.sched_priority = pri ;

  return sched_setscheduler (0, SCHED_RR, &sched) ;
}

/*
 * wiringPiISR:
 *	Pi Specific.
 *	Take the details and create an interrupt handler that will do a call-
 *	back to the user supplied function.
 *********************************************************************************
 */

int wiringPiISR (int pin, int mode, void (*function)(void))
{
  pthread_t threadId ;

  if ((pin < 0) || (pin > 63))
    return wiringPiFailure (WPI_FATAL, "wiringPiISR: pin must be 0-63 (%d)\n", pin) ;

  else if (wiringPiMode == WPI_MODE_UNINITIALISED)
    return wiringPiFailure (WPI_FATAL, "wiringPiISR: wiringPi has not been initialised. Unable to continue.\n") ;
  else if (wiringPiMode != WPI_MODE_GPIO)	// Sys mode
	  {
		printf("[pseudoWiringPi][ERROR!!!][Modo de configuración incorrecto!!!][Use wiringPiSetupGpio ()]\n");
		fflush(stdout);
		return wiringPiFailure (WPI_FATAL, "wiringPiISR: wiringPi has not been initialised properly. Unable to continue.\n") ;
	  }

  // Now export the pin and set the right edge
  isrFunctions [pin] = function ;

  if(!waitForInterruptSTDIN_process) {
	pthread_mutex_lock (&pinMutex) ;
	pthread_create (&threadId, NULL, interruptHandlerSTDIN, NULL) ;
	pthread_mutex_unlock (&pinMutex) ;
	waitForInterruptSTDIN_process = 1;
  }

  return 0 ;
}


/*
 * digitalWrite:
 *	Set an output bit
 *********************************************************************************
 */

void digitalWrite (int pin, int value)
{
	int i=0, j=0;

	if ((pin & PI_GPIO_MASK) == 0) {		// On-Board Pin
		if (wiringPiMode != WPI_MODE_GPIO) {	// Sys mode
			printf("[pseudoWiringPi][ERROR!!!][Modo de configuración incorrecto!!!][Use wiringPiSetupGpio ()]\n");
			fflush(stdout);
			return;
		}

		if (pin == GPIO_LED_DISPLAY_COL_1 || pin == GPIO_LED_DISPLAY_COL_2 || pin == GPIO_LED_DISPLAY_COL_3) { // Pines para control de columnas display
			if (value == HIGH)
				columnaDisplayActiva |= array_flags_check_columnas_display[GPIO_to_cols[pin]];
			else if (value == LOW)
				columnaDisplayActiva &= (~array_flags_check_columnas_display[GPIO_to_cols[pin]]);
			else {
				printf("[pseudoWiringPi][ERROR!!!][digitalWrite][parametro value incorrecto!!! (use HIGH o LOW)]\n");
				fflush(stdout);
				return;
			}

			// Si cambio de columna reseteo contadores de numero de escrituras
			for(i=0;i<7;i++)
				numWritesAfterColumnEnable[i] = 0;
		}

		// Emulacion teclado matricial
		if (value == HIGH) {
			if(pin >= GPIO_KEYBOARD_COL_1 && pin <= GPIO_KEYBOARD_COL_4)
				columnaTecladoActiva = pin;
		}

		if (pin == GPIO_LED_DISPLAY_ROW_1 ||
			pin == GPIO_LED_DISPLAY_ROW_2 ||
			pin == GPIO_LED_DISPLAY_ROW_3 ||
			pin == GPIO_LED_DISPLAY_ROW_4 ||
			pin == GPIO_LED_DISPLAY_ROW_5 ||
			pin == GPIO_LED_DISPLAY_ROW_6 ||
			pin == GPIO_LED_DISPLAY_ROW_7 ) { // Pines para filas display
			if (numWritesAfterColumnEnable[GPIO_to_rows[pin]] == 0) {
				pseudoMatriz[GPIO_to_rows[pin]][columnaDisplayActiva] = !value; // Para encender se escribe LOW, para apagar HIGH
				numWritesAfterColumnEnable[GPIO_to_rows[pin]]++;
			}
		}

		if(columnaDisplayActiva == 7 && pin == GPIO_LED_DISPLAY_ROW_7) { // Esta condicion equivale a comprobar cuando se termina de hacer un barrido COMPLETO del display
			// (se ha completado un barrido para todas las columnas) momento que aprovecharemos para repintar nuestro display emulado
			if(JUGANDO) {
				piLock (STD_IO_BUFFER_KEY);
				if (!primeraVezDisplay) { // Codigo que permite repintar la pantalla SIN SCROLL VERTICAL HACIA ABAJO
					// OJO! solo funciona bien desde la propia terminal de Ubuntu!
					// Desde la pseudo-terminal de Eclipse NO FUNCIONA ya que no procesa bien ni los \b ni los \n (como bien sabeis)
					for(i=0;i<7;i++)
						printf("%s", BACKUP_NEWLINE);

					printf("\033[A");
					printf("\033[A\033[C\033[C\033[C\033[C\033[C\033[C\033[C\033[C\033[C\033[C");
				}

				// Actualizo pantalla
				printf("\n[PANTALLA]\n\n");

				for(i=0;i<7;i++) {
					for(j=0;j<8;j++)
						if(pseudoMatriz[i][j]) {
							printf("\033[%dm", pseudoMatrizColor[i][j]);
							printf("%d\033[0m", pseudoMatriz[i][j]);
						}
						else
							printf("%d", pseudoMatriz[i][j]);
					if(i<6)
						printf("\n");
				}
				fflush(stdout);

				primeraVezDisplay = 0;
				piUnlock (STD_IO_BUFFER_KEY);
			}
		}
	}
}

/*
 * pseudoWiringPiEnableDisplay:
 *	Funcion que habilita o deshabilita el display emulado
 *********************************************************************************
 */

void pseudoWiringPiEnableDisplay(int estado) {
	if(estado) {
		primeraVezDisplay = 1;
		JUGANDO = 1;
	}
	else {
		JUGANDO = 0;
	}
}

/*
 * delay:
 *	Wait for some number of milliseconds
 *********************************************************************************
 */

void delay (unsigned int howLong)
{
  struct timespec sleeper, dummy ;

  sleeper.tv_sec  = (time_t)(howLong / 1000) ;
  sleeper.tv_nsec = (long)(howLong % 1000) * 1000000 ;

  nanosleep (&sleeper, &dummy) ;
}

/*
 * millis:
 *	Return a number of milliseconds as an unsigned int.
 *	Wraps at 49 days.
 *********************************************************************************
 */

unsigned int millis (void)
{
  uint64_t now ;

#ifdef	OLD_WAY
  struct timeval tv ;

  gettimeofday (&tv, NULL) ;
  now  = (uint64_t)tv.tv_sec * (uint64_t)1000 + (uint64_t)(tv.tv_usec / 1000) ;

#else
  struct  timespec ts ;

  clock_gettime (CLOCK_MONOTONIC_RAW, &ts) ;
  now  = (uint64_t)ts.tv_sec * (uint64_t)1000 + (uint64_t)(ts.tv_nsec / 1000000L) ;
#endif

  return (uint32_t)(now - epochMilli) ;
}

/*
 * piThreadCreate:
 *	Create and start a thread
 *********************************************************************************
 */

int piThreadCreate (void *(*fn)(void *))
{
  pthread_t myThread ;

  return pthread_create (&myThread, NULL, fn, NULL) ;
}

/*
 * piLock: piUnlock:
 *	Activate/Deactivate a mutex.
 *	We're keeping things simple here and only tracking 4 mutexes which
 *	is more than enough for out entry-level pthread programming
 *********************************************************************************
 */

void piLock (int key)
{
  pthread_mutex_lock (&piMutexes [key]) ;
}

void piUnlock (int key)
{
  pthread_mutex_unlock (&piMutexes [key]) ;
}
