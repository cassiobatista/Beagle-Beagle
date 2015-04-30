/* 
 * O programa le comandos enviados por um controle remoto (TV, DVD, Ar condicionado...) 
 * recebidos por um photosensor ligado ao arduino e redireciona-os usando um LED IR. 
 * A leitura eh feita captando a duracao em que o LED IR ficou ligado ou desligado, 
 * armazenando essa informacao em uma matriz de pulsos.
 */

/* Eh necessario usar a leitura direta dos pinos do arduino, ja que a funcao
 * digitalRead() eh lenta e o timing eh muito importante.
 */

#include <math.h>
#define IRpin_PIN      PIND
#define IRpin          2

int IRledPin = 13;

/* Pulso maximo de 65 ms */
#define MAXPULSE 65000

/* Se a resolucao for grande demais, o timing nao sera preciso */
#define RESOLUTION 20 

/* 100 pares de pulsos armazenados (on/off) */
uint16_t pulses[100][2];	// par de pulsos high e low 
uint8_t currentpulse = 0;	// index dos pulsos sendo armazenados

void setup(void) {
	pinMode(IRledPin, OUTPUT);
	Serial.begin(9600);
	Serial.println("Pronto para decodificar IR!");
}

void loop(void) {
	uint16_t highpulse, lowpulse;  
	highpulse = lowpulse = 0; 

	/* Leitura direta do pino */
	while (IRpin_PIN & (1 << IRpin)) {
		highpulse++;
		delayMicroseconds(RESOLUTION);

		/* Se pulso muito longo, ou nada ocorreu ou o codigo terminou */ 
		/* Mostra os tempos de pulso no monitor serial */
		if ((highpulse >= MAXPULSE) && (currentpulse != 0)) {
			printPulses();
			currentpulse=0;
			return;
		}
	}
	/* Pulso nao foi muito longo: armazena na matriz */
	pulses[currentpulse][0] = highpulse;

	/* Leitura direta do pino */
	while (! (IRpin_PIN & _BV(IRpin))) {
		lowpulse++; 
		delayMicroseconds(RESOLUTION);
		if ((lowpulse >= MAXPULSE)  && (currentpulse != 0)) {
			printPulses();
			currentpulse=0;
			return;
		}
	}
	pulses[currentpulse][1] = lowpulse;

	/* Pulso lido corretamente */
	currentpulse++;
}

/* Mostra o tempo dos pulsos no monitor serial */
void printPulses(void) {
	Serial.println("\n\r\n\rRecebido: \n\rOFF \tON");
	for (uint8_t i = 0; i < currentpulse; i++) {
		Serial.print(pulses[i][0] * RESOLUTION, DEC);
		Serial.print("\t");
		Serial.print(pulses[i][1] * RESOLUTION, DEC);
		Serial.println("");
	}
	delay(1000);

	/* Envia o comando pelo led IR */
	sendCommand(currentpulse);
}

/* Envia um pulso de 38kHz para o pin por x ms, reproduzindo o comando decodificado (PWM) */
void pulseIR(long microsecs) {
	cli();  // Desativa interrupcoes de background
	while (microsecs > 0) {
		// 38 kHz eh aproximadamente 13 us high e 13 us low
		digitalWrite(IRledPin, HIGH);	// digitalWrite dura 3 us
		delayMicroseconds(10);			// espera 10 us
		digitalWrite(IRledPin, LOW);	// 3 us
		delayMicroseconds(10);			// espera mais 10 us

		/* subtrai 26 us do pulso */
		microsecs -= 26;
	}
	sei();  // Ativa as interrupcoes
}

/* Reproduz o comando no led IR utilizando a matriz de ms `on' e `off' */
void sendCommand(uint8_t cp) {
	for (uint8_t i = 0; i < cp; i++){
		delayMicroseconds((pulses[i][0] * RESOLUTION));	// off
		pulseIR((pulses[i][1] * RESOLUTION));			// on
	}
	Serial.println("Comando enviado!");
}
