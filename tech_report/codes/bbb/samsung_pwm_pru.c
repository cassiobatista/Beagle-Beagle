/*! \file pwm_cap.c
  \brief Example: PWM output and CAP input.

  This file contains an example on how to measure the frequency and duty
  cycle of a pulse train with a eCAP module input. The program sets
  another pin as eHRPWM output to generate a pulse width modulated signal
  as source for the measurement. The output can be changed by some keys,
  the frequency and duty cycle of the input is shown continously in the
  terminal output.

Licence: GPLv3

Copyright 2014 by Thomas{ dOt ]Freiherr[ At ]gmx[ DoT }net
          2015 by Cassio{ dOt ]Batista{ dOt ]13[ At ]gmail[ DoT }com

Compile by: `gcc -Wall -o pwm_cap pwm_cap.c -lpruio`

 */

//! Message for the compiler.
#define _GNU_SOURCE 1
#include "stdio.h"
#include <termios.h>
#include <unistd.h>
#include <errno.h>

#include "pruio.h"
#include "pruio_pins.h"

//! The pin for PWM output.
#define PIN P8_13
#define FREQ 37900


int mark(pruIo* Io, int us);
int space(pruIo* Io, int us);

int pruio_error(pruIo* Io, char *msg);

int main(int argc, char **argv)
{
	pruIo *Io = pruio_new(PRUIO_DEF_ACTIVE, 0x98, 0, 1); //! create new driver structure

	int i, j;
	char *stream = "1111000001110000011100000000111110";
	//char *stream = "1111000001110000001000000101111110";

	if (Io->Errr) 
		return pruio_error(Io, "Initialisation failed ");

	if (pruio_pwm_setValue(Io, PIN, FREQ, .0)) 
		return pruio_error(Io, "Failed setting output @PIN ");

	// pin config OK, transfer local settings to PRU and start
	if (pruio_config(Io, 1, 0x1FE, 0, 4)) 
		return pruio_error(Io, "Config failed ");

	for(j=0; j<3; j++) {
		mark(Io, 4500);
		space(Io, 4500);
		for(i=1; i<34; i++) {
			mark(Io, 560);
			if(stream[i]=='1') {
				space(Io, 1690);
			} else {
				space(Io, 560);
			}
		}
		usleep(200000); //200ms, since signal lasts 108ms
	}

	pruio_destroy(Io);       /* destroy driver structure */
	printf("killed");
	return 0;
}

int mark(pruIo* Io, int us) 
{
	if (pruio_pwm_setValue(Io, PIN, FREQ, 0.5)) 
		return pruio_error(Io, "failed setting output @PIN "); 
	return usleep(us-150);
}

int space(pruIo* Io, int us) 
{
	if (pruio_pwm_setValue(Io, PIN, FREQ, 0.0)) 
		return pruio_error(Io, "failed setting output @PIN ");
	return usleep(us-150);
}

int pruio_error(pruIo *Io, char* msg) 
{
	printf("%s %s\n", msg, Io->Errr);
	pruio_destroy(Io);       /* destroy driver structure */
	return -1;
}
