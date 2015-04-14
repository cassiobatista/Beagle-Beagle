/*
 RC-5 Philips Protocol
The 36 kHz carrier frequency was chosen to render the system immune to
interference from TV scan lines. Since the repetition of the 36 kHz carrier is
27.778 μs and the duty factor is 25%-33%, the carrier pulse duration is 6.944 μs
Since the high half of each symbol (bit) of the RC-5 code word contains 32
carrier pulses, the symbol period is 64 x 27.778 μs = 1.778 ms, and the 14
symbols (bits) of a complete RC-5 code word takes 24.889 ms to transmit. The
code word is repeated every 113.778 ms (4096 / 36 kHz) as long as a key remains
pressed. (Again, please note that these timings are not strictly followed by all
manufacturers, due to a lack of widespread distribution of accurate information
on the RC-5 protocol.)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* sending */
void send() {
	char *bitstream = "11011100010001";
	short int f_khz = 38;
	short int i, j;
	short float duty = 25/100;

	if(strlen(bitstream)>14 || bitstream[0]!='1')
		return EXIT_FAILURE;

	for(i=0; i<strlen(bitstream); i++) {
		if(bitstream[i]=='0') {
			for(j=0; j<32; j++) {
				digitalWrite(GPIO, HIGH);
				delayMicroseconds(1000*duty/f_khz);
				digitalWrite(GPIO, LOW);
				delayMicroseconds(1000*(1-duty)/f_khz);
			}
			digitalWrite(GPIO, LOW);
			delayMicroseconds(1000*32*1/f_khz);
		} else if(bitstream[i]=='1') {
			digitalWrite(GPIO, LOW);
			delayMicroseconds(1000*32*1/f_khz);
			for(j=0; j<32; j++) {
				digitalWrite(GPIO, HIGH);
				delayMicroseconds(1000*duty/f_khz);
				digitalWrite(GPIO, LOW);
				delayMicroseconds(1000*(1-duty)/f_khz);
			}
		} else {
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}//close main
