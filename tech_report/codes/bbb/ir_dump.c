/*! \file ir_dump.c
  \brief Example: get state of a button.

  This file contains an example on how to use libpruio to get the state
  of a button connetect to a GPIO pin on the beaglebone board. Here pin 7
  on header P8 is used as input with pullup resistor. Connect the button
  between P8_07 (GPIO input) and P8_01 (GND).

Licence: GPLv3

Copyright 2014 by Thomas{ dOt ]Freiherr[ At ]gmx[ DoT }net
          2015 by Cassio{ dOt ]Batista{ dOt ]13[ At ]gmail[ DoT }com

Compile by: `gcc -Wall -o ir_dump ir_dump.c -lpruio`

 */


//! Message for the compiler.
#define _GNU_SOURCE 1
#include "stdio.h"
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>

#include "pruio.h"
#include "pruio_pins.h"

//! The header pin to use.
#define PIN P8_16

/*! \brief Wait for keystroke or timeout.
  \param mseconds Timeout value in milliseconds.
  \returns 0 if timeout, 1 if input available, -1 on error.

  Wait for a keystroke or timeout and return which of the events happened.

 */
int isleep(unsigned int mseconds)
{
	fd_set set;
	struct timeval timeout;

	/* Initialize the file descriptor set. */
	FD_ZERO(&set);
	FD_SET(STDIN_FILENO, &set);

	/* Initialize the timeout data structure. */
	timeout.tv_sec = 0;
	timeout.tv_usec = mseconds * 1000;

	return TEMP_FAILURE_RETRY(select(FD_SETSIZE, &set, NULL, NULL, &timeout));
}

//! The main function.
int main(int argc, char **argv)
{
	pruIo *io = pruio_new(PRUIO_DEF_ACTIVE, 0x98, 0, 1); //! create new driver structure

	struct timeval stop_l, start_l;
	struct timeval stop_h, start_h;

	do {
		if (io->Errr) {
			printf("initialisation failed (%s)\n", io->Errr); break;}

		if (pruio_config(io, 1, 0x1FE, 0, 4)) {
			printf("config failed (%s)\n", io->Errr); break;}

		//while(!isleep(1)) { //                      run loop until keystroke
		while(1) { 
			gettimeofday(&start_l, NULL);
			while(((int) pruio_gpio_Value(io, PIN)) == 1);
			gettimeofday(&stop_l, NULL);

			gettimeofday(&start_h, NULL);
			while(((int) pruio_gpio_Value(io, PIN)) == 0);
			gettimeofday(&stop_h, NULL);

			printf("%lu %lu\n", (stop_l.tv_usec-start_l.tv_usec), (stop_h.tv_usec-start_h.tv_usec));
		}//close while sleep
	} while (0);

	printf("killed!\n");
	pruio_destroy(io);       /* destroy driver structure */
	return 0;
}
