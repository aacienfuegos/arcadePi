/*
 * tmr.c
 *
 *  Created on: 1 de mar. de 2016
 *      Author: Administrador
 */

#include "tmr.h"
#include <stdlib.h>
#include <signal.h>
#include <time.h>

tmr_t*
tmr_new (notify_func_t isr)
{
    tmr_t* this = (tmr_t*) malloc (sizeof (tmr_t));
    tmr_init (this, isr);
    return this;

}

void
tmr_init (tmr_t* this, notify_func_t isr) {
    this->se.sigev_notify = SIGEV_THREAD;
    this->se.sigev_value.sival_ptr = &(this->timerid);
    this->se.sigev_notify_function = isr;
    this->se.sigev_notify_attributes = NULL;
    timer_create (CLOCK_REALTIME, &(this->se), &(this->timerid));  /* o CLOCK_MONOTONIC si se soporta */
}

void
tmr_destroy(tmr_t* this)
{
    tmr_stop (this);
    free(this);
}

void
tmr_startms(tmr_t* this, int ms) {
    this->spec.it_value.tv_sec = ms / 1000;
    this->spec.it_value.tv_nsec = (ms % 1000) * 1000000;
    this->spec.it_interval.tv_sec = 0;
    this->spec.it_interval.tv_nsec = 0;
    timer_settime (this->timerid, 0, &(this->spec), NULL);
}

void
tmr_stop (tmr_t* this) {
    timer_delete (this->timerid);
}

