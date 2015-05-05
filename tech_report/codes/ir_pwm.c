/* RC-6 Handler

 * Author: May, 2015
 * Cassio Trindade Batista 
 * cassio.batista.13@gmail.com

 * Protocol Description
 * The 36 kHz carrier frequency was chosen to render the system immune to
 * interference from TV scan lines. Since the repetition of the 36 kHz carrier
 * is XXXX μs and the duty factor is 25%-33%, the carrier pulse duration is 
 * XXXX μs Since the high half of each symbol of the RC-6 code word contains 32
 * carrier pulses, the symbol period is XXXX, and the 22
 * symbols (bits) of a complete RC-6 code word takes XXXX ms to transmit. The
 * code word is repeated every XXXX ms (4096 / 36 kHz) as long as a key 
 * remains pressed. 

 *****************************************************************************
 *       .-----------.    .----.    .--------.    .----.    .--------.       *
 * RC -> | IR sensor | -> | Rx | -> | decode | -> | Tx | -> | IR Led | -> TV *
 *       '-----------'    '----'    '--------'    '----'    '--------'       *
 *                                                                           *
 *****************************************************************************

 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

/* Converts a duration array into a bitstream */
char *decode(uint16_t *dur_vec, uint8_t size);

/* Error handling */
void die(char *msg, uint8_t code);

/*Tx: Transmit info to IR Led (to TV) */
void send(char *stream);

/* Rx: Receive info read from Photosensor TSOP18xx (from RC) */
void receive(int gpio);

int main() {
	uint16_t x = 960; //eXtra: padding to fill duration (see func receive())

	// CANAL MAIS 
	uint16_t a[] = { 
			42864, 2500, 940, 340, 960, 320, 520, 320, 540, 1180, 1400, 320,
			540, 320, 520, 340, 520, 320, 540, 320, 540, 320, 520, 320, 540,
			320, 520, 760, 960, 340, 520, 760, 520, 320, 960, 320, 540, 320, x,
			15724, 2500, 940, 320, 960, 340, 520, 320, 540, 1180, 1400, 320,
			540, 320, 520, 340, 520, 320, 520, 340, 520, 320, 540, 320, 540,
			320, 520, 760, 960, 320, 540, 760, 520, 320, 960, 320, 540, 320
	};

	// CANAL MENOS
	uint16_t b[] = { 
			39128, 2500, 940, 340, 960, 320, 520, 340, 520, 1180, 1400, 320,
			540, 320, 540, 320, 520, 320, 540, 320, 520, 340, 520, 320, 540,
			320, 540, 740, 960, 340, 520, 760, 520, 320, 960, 760, x, 16164,
			2480, 960, 320, 960, 320, 540, 320, 520, 1200, 1380, 340, 520, 320,
			540, 320, 540, 320, 520, 340, 520, 320, 520, 340, 520, 320, 540,
			760, 960, 320, 520, 760, 520, 340, 940, 760
	};
 
	// LIGAR / DESLIGAR
	uint16_t c[] = { 
			34160, 2500, 960, 320, 960, 320, 520, 340, 520, 1200, 1380, 340,
			520, 320, 540, 320, 520, 340, 520, 320, 540, 320, 520, 340, 520,
			320, 540, 320, 520, 340, 520, 320, 540, 760, 520, 320, 960, 320,
			540, 320, x, 15704, 2480, 960, 320, 960, 340, 520, 320, 520, 1200,
			1400, 320, 520, 340, 520, 320, 540, 320, 520, 340, 520, 320, 540,
			320, 540, 320, 520, 320, 540, 320, 520, 340, 520, 760, 520, 340,
			940, 320, 540, 320
	};

	// VOLUME MAIS
	uint16_t d[] = { 
			46568, 2500, 960, 320, 940, 340, 520, 320, 540, 1200, 1380, 320,
			540, 320, 540, 320, 520, 320, 540, 320, 520, 340, 520, 320, 540,
			320, 520, 340, 520, 320, 540 , 760, 960, 320, 520, 340, 520, 320,
			540, 320, x, 15704, 2500, 940, 320, 960, 320, 540, 320, 540, 1180,
			1400, 320, 520, 340, 520, 320, 540, 320, 540, 320, 520, 320, 540,
			320, 520, 340, 520, 320, 540, 320, 520, 760, 960, 320, 540, 320,
			540, 320, 520, 340
	};

	// VOLUME MENOS
	uint16_t e[] = { 
			6124, 2480, 920, 360, 960, 320, 520, 340, 520, 1200, 1380, 320, 540,
			320, 540, 320, 520, 340, 520, 320, 520, 340, 520, 320, 540, 320,
			540, 320, 520, 340, 520, 760, 960, 320, 520, 340, 520, 760, x,
			16144, 2480, 940, 340, 960, 320, 540, 320, 520, 1200, 1400, 320,
			520, 320, 540, 320, 540, 320, 520, 340, 520, 320, 520, 340, 520,
			320, 540, 320, 540, 320, 520, 760, 960, 320, 540, 320, 520, 760
	};
	
	printf("CH+: %s\n", decode(a, (uint8_t)sizeof(a)/sizeof(uint16_t)));
	printf("CH-: %s\n", decode(b, (uint8_t)sizeof(b)/sizeof(uint16_t)));
	printf("ON : %s\n", decode(c, (uint8_t)sizeof(c)/sizeof(uint16_t)));
	printf("V+ : %s\n", decode(d, (uint8_t)sizeof(d)/sizeof(uint16_t)));
	printf("V- : %s\n", decode(e, (uint8_t)sizeof(e)/sizeof(uint16_t)));

	return EXIT_SUCCESS;
}//close main

/* receives an duration vector and outputs a bit stream */
char *decode(uint16_t *dur_vec, uint8_t size)
{
	uint8_t i, bitcount;
	char *bitstream = (char *) malloc((22+1) * sizeof(char));

	uint8_t stay = 1; //next bit must stay on the same index of duration vector
	uint8_t high = 1; //next bit will start on the high level

	bitcount = 0;
	for(i=0; i<size; i++) {
		/* check AGC bit */
		if((dur_vec[i]+dur_vec[i+1])>=3400) {
			if(dur_vec[i]>3000)
				i++;
			
			if (bitcount==0) {
				bitstream[bitcount++] = '1';
				stay = 0;
				high = 1;
			} else { //it is repeating
				break;
			}
		 }

		/* check toggle bit */
		if(bitcount==5) {
			high = 0;
			if(dur_vec[++i]>1000) {
				bitstream[bitcount++] = '1';
				stay = 1;
			} else {
				bitstream[bitcount++] = '0';
				stay = 0;
			} 
			i++;
		}

		/* check common bits */
		if((dur_vec[i]+dur_vec[i+1])>=(360+560)) {
			if(high) {
				bitstream[bitcount++] = '1';
				if(dur_vec[i+1]>560) {
					high = 0;
					stay = 1;
				} else {
					high = 1;
					stay = 0;
				}
			} else {
				bitstream[bitcount++] = '0';
				if(dur_vec[i+1]>360) {
					high = 1;
					stay = 1;
				} else {
					high = 0;
					stay = 0;
				}
			}
		} else {
			if(high) {
				high = 1;
				stay = 0;
				bitstream[bitcount++] = '1';
			} else {
				high = 0;
				stay = 0;
				bitstream[bitcount++] = '0';
			}
		}

		if(strlen(bitstream)==22) 
			break;

		if(!stay) 
			i++;
	}
	return bitstream;
}//close decode

/* die with error */
void die(char *msg, uint8_t code) {
	fprintf(stderr, "%s\n", msg);
	exit(code);
}

/* Gabriel: fprintf e fopen for GPIO*/
/* send IR info to TV */
void send(char *stream)
{
	if(strlen(stream)!=22)
		die("tamanho errado", 0);
	else if (stream[0]!='1' || stream[1]!='1')
		die("AGC ou START errado", 1);

	FILE* pin;
	if(!(pin = fopen("/crl/porra/buceta/pin_8===D~", "w")))
		die("impossivel abrir", 4);
	
	#define HIGH 5
	#define LOW 0

	uint8_t i, j, f_khz=36, pulses=16;
	float duty = 25/100;

	/* send AGC bit */
	for(j=0; j<6*pulses; j++) {
		fprintf(pin, "%d", HIGH);
		usleep(1000*duty/f_khz);
		fprintf(pin, "%d", LOW);
		usleep(1000*(1-duty)/f_khz);
	}
	fprintf(pin, "%d", LOW);
	usleep(1000*2*pulses*1/f_khz);

	for(i=1; i<strlen(stream); i++) {
		/* Toggle */
		if(i==5) 
			pulses=2*16;
		else 
			pulses=1*16;

		if(stream[i]=='1') {
			for(j=0; j<pulses; j++) {
				fprintf(pin, "%d", HIGH);
				usleep(1000*duty/f_khz);
				fprintf(pin, "%d", LOW);
				usleep(1000*(1-duty)/f_khz);
			}
			fprintf(pin, "%d", LOW);
			usleep(1000*pulses*1/f_khz);
		} else if(stream[i]=='0') {
			fprintf(pin, "%d", LOW);
			usleep(1000*pulses*1/f_khz);
			for(j=0; j<pulses; j++) {
				fprintf(pin, "%d", HIGH);
				usleep(1000*duty/f_khz);
				fprintf(pin, "%d", LOW);
				usleep(1000*(1-duty)/f_khz);
			}
		} else {
			die("wrong format", 3);
		}
	}
	fclose(pin);
}//close send

/* thiago: converts from arduino to c */
/* Receive IR info from Remote Control */
void receive(int gpio) 
{
	FILE* pin;
	if(!(pin = fopen("/crl/porra/buceta/pin_00", "r")))
		die("impossivel abrir", 5);

	uint8_t i, currentpulse = 0;
	uint16_t high = 0;
	uint16_t low  = 0;
	uint16_t *pulses; 

	/* sum(vector_of_durations) = 23111 */
	float t = 16.0 * (1/36.0);
	uint16_t dur = (6*t)+(2*t) + (2*2*t) + (2*20*t);

	pulses = /*(uint16_t *)*/ calloc(100*2, sizeof(uint16_t));
	
	while(1) {
		high = 0 ; low = 0;
		/* condition: fread returns a non null value */
		while(1) {
			high++;
			usleep(20);
			if((high >= 65000) && (currentpulse!=0)) 
				return;
		}

		/* condition: fread returns a null value */
		while(1) {
			low++;
			usleep(20);
			if((high >= 65000) && (currentpulse!=0)) 
				return;
		}

		pulses[currentpulse++] = high;
		pulses[currentpulse++] = low;
	}

	//save_to_db("philips", "volume+", 
	//		decode(pulses, sizeof(pulses)/sizeof(uint16_t)));
}
/* EOF */
