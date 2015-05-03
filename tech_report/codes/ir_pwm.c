/* RC-6 Handler

 * Author: May, 2015
 * Cassio Trindade Batista 
 * cassio.batista.13@gmail.com

 * Protocol Description
 * The 36 kHz carrier frequency was chosen to render the system immune to
 * interference from TV scan lines. Since the repetition of the 36 kHz carrier
 * is 27.778 μs and the duty factor is 25%-33%, the carrier pulse duration is 
 * 6.944 μs Since the high half of each symbol of the RC-5 code word contains 32
 * carrier pulses, the symbol period is 64 x 27.778 μs = 1.778 ms, and the 22
 * symbols (bits) of a complete RC-5 code word takes 24.889 ms to transmit. The
 * code word is repeated every 113.778 ms (4096 / 36 kHz) as long as a key 
 * remains pressed. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

char *decode(int dur[]);
void die(char *msg, uint8_t code);
/* Tx and Rx */
void send(char *stream);
void receive(int gpio);

int main() {
	// CANAL MAIS 
	unsigned int a[] = { 
			42864, 2500, 940, 340, 960, 320, 520, 320, 540, 1180, 1400, 320,
			540, 320, 520, 340, 520, 320, 540, 320, 540, 320, 520, 320, 540,
			320, 520, 760, 960, 340, 520, 760, 520, 320, 960, 320, 540, 320,
			15724, 2500, 940, 320, 960, 340, 520, 320, 540, 1180, 1400, 320,
			540, 320, 520, 340, 520, 320, 520, 340, 520, 320, 540, 320, 540,
			320, 520, 760, 960, 320, 540, 760, 520, 320, 960, 320, 540, 320
	};

	// CANAL MENOS
	int b[] = { 
			39128, 2500, 940, 340, 960, 320, 520, 340, 520, 1180, 1400, 320,
			540, 320, 540, 320, 520, 320, 540, 320, 520, 340, 520, 320, 540,
			320, 540, 740, 960, 340, 520, 760, 520, 320, 960, 760, 16164, 2480,
			960, 320, 960, 320, 540, 320, 520, 1200, 1380, 340, 520, 320, 540,
			320, 540, 320, 520, 340, 520, 320, 520, 340, 520, 320, 540, 760,
			960, 320, 520, 760, 520, 340, 940, 760 
	};
 
	// LIGAR / DESLIGAR
	int c[] = { 
			34160, 2500, 960, 320, 960, 320, 520, 340, 520, 1200, 1380, 340,
			520, 320, 540, 320, 520, 340, 520, 320, 540, 320, 520, 340, 520,
			320, 540, 320, 520, 340, 520, 320, 540, 760, 520, 320, 960, 320,
			540, 320, 15704, 2480, 960, 320, 960, 340, 520, 320, 520, 1200,
			1400, 320, 520, 340, 520, 320, 540, 320, 520, 340, 520, 320, 540,
			320, 540, 320, 520, 320, 540, 320, 520, 340, 520, 760, 520, 340,
			940, 320, 540, 320
	};

	// VOLUME MAIS
	int d[] = { 
			46568, 2500, 960, 320, 940, 340, 520, 320, 540, 1200, 1380, 320,
			540, 320, 540, 320, 520, 320, 540, 320, 520, 340, 520, 320, 540,
			320, 520, 340, 520, 320, 540 , 760, 960, 320, 520, 340, 520, 320,
			540, 320, 15704, 2500, 940, 320, 960, 320, 540, 320, 540, 1180,
			1400, 320, 520, 340, 520, 320, 540, 320, 540, 320, 520, 320, 540,
			320, 520, 340, 520, 320, 540, 320, 520, 760, 960, 320, 540, 320,
			540, 320, 520, 340
	};

	// VOLUME MENOS
	int e[] = { 
			6124, 2480, 920, 360, 960, 320, 520, 340, 520, 1200, 1380, 320, 540,
			320, 540, 320, 520, 340, 520, 320, 520, 340, 520, 320, 540, 320,
			540, 320, 520, 340, 520, 760, 960, 320, 520, 340, 520, 760, 16144,
			2480, 940, 340, 960, 320, 540, 320, 520, 1200, 1400, 320, 520, 320,
			540, 320, 540, 320, 520, 340, 520, 320, 520, 340, 520, 320, 540,
			320, 540, 320, 520, 760, 960, 320, 540, 320, 520, 760
	};

	printf("%s\n", decode(a));
	//printf("%s\n", decode(b));
	//printf("%s\n", decode(c));
	//printf("%s\n", decode(d));
	//printf("%s\n", decode(e));

	return EXIT_SUCCESS;
}

/* receives an duration vector and outputs a bit stream */
char *decode(int dur[])
{
	//unsigned int dur[] = { 
	//		42864, 2500, 940, 340, 960, 320, 520, 320, 540, 1180, 1400, 320,
	//		540, 320, 520, 340, 520, 320, 540, 320, 540, 320, 520, 320, 540,
	//		320, 520, 760, 960, 340, 520, 760, 520, 320, 960, 320, 540, 320,
	//		15724, 2500, 940, 320, 960, 340, 520, 320, 540, 1180, 1400, 320,
	//		540, 320, 520, 340, 520, 320, 520, 340, 520, 320, 540, 320, 540,
	//		320, 520, 760, 960, 320, 540, 760, 520, 320, 960, 320, 540, 320
	//};

	uint8_t i, j;
	char *bitstream = (char *) malloc((22+1) * sizeof(char));

	uint8_t stay = 1; 
	uint8_t high = 1;

	for(i=0,j=0; i<sizeof(dur)/sizeof(int); i++) {
		/* check AGC bit */
		if((dur[i]+dur[i+1])>3400) {
			if(dur[i]>3000){ 
				goto termina;
			}
			if(j==0) {
				bitstream[j++] = '1';
				stay = 0;
				high = 1;
			} else { //it is repeating
				break;
			}
		 }

		/* check toggle bit */
		if(j==5) {
			high = 0;
			if(dur[i]>1000) {
				bitstream[j++] = '1';
				stay = 1;
			} else {
				bitstream[j++] = '0';
				stay = 0;
			}
		goto termina;
		}

		if(strlen(bitstream)==22) 
			break;

		if((dur[i]+dur[i+1])>=(340+540)) {
			if((high==1) && (dur[i+1]>(540))) {
				high = 0;
				stay = 1;
				bitstream[j++] = '1';
			}

			else if((high==1) && (dur[i+1]<=(540))) {
				stay = 0;
				high = 1;
				bitstream[j++] = '1';
			}

			else if((high==0) && (dur[i+1]>(340))) {
				high = 1;
				stay = 1;
				bitstream[j++] = '0';
			}

			else if((high==0) && (dur[i+1]<=(340))) {
				high = 0;
				stay = 0;
				bitstream[j++] = '0';
			}
		} else {
			if(high) {
				stay = 0;
				high = 1;
				bitstream[j++] = '1';
			} else {
				stay = 0;
				high = 0;
				bitstream[j++] = '0';
			}
		}

		termina:
		if(stay==0) 
			i++;
	}
	return bitstream;
}//close decode

/* die with error */
void die(char *msg, uint8_t code) {
	fprintf(stderr, "%s\n", msg);
	exit(code);
}

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
			pulses=32;
		else 
			pulses=16;

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
void receive(int gpio) 
{
/* thiago */
/* thiago */
/* thiago */
/* thiago */
/* thiago */
/* thiago */
/* thiago */
/* thiago */
/* thiago */
}
/* EOF */
