/* Codigo arduino de decodificacao de comandos infravermelhos
 *
 * O programa decodifica os comandos enviados por um controle remoto (TV, DVD,
 * Ar condicionado...) recebidos por um receptor IR ligado ao arduino e 
 * redireciona o comando usando um transmissor IR `imitando' os comandos 
 * decodificados. A decodificacao eh feita captando a duracao em que o LED IR
 * ficou ligado transmitindo ou desligado e armazenando essa informacao em uma
 * matriz.
 * */

/* Eh necessario usar a leitura direta dos pinos do arduino, ja que a funcao
 * digitalRead() eh lenta e o timing eh muito importante aqui.
 * */

#include <math.h>
#define IRpin_PIN      PIND
#define IRpin          2

int IRledPin = 13;

// Pulso maximo de 65 milisegundos
#define MAXPULSE 65000

// Se a resolucao for grande demais o timing nao sera preciso
#define RESOLUTION 20 

// 100 pares de pulsos armazenados (on-off)
uint16_t pulses[100][2];	// par de pulsos high e low 
uint8_t currentpulse = 0;	// index dos pulsos sendo armazenados
int pulsedb[200]; // vetor de pulsos em milisegundos que sera usado para enviar os comandos

void setup(void) {
	pinMode(IRledPin, OUTPUT);
	Serial.begin(9600);
	Serial.println("Pronto para decodificar IR!");
}

void loop(void) {
	uint16_t highpulse, lowpulse;  
	highpulse = lowpulse = 0; 

	//  IRpin_PIN & (1 << IRpin) = leitura direta do pino 
	while (IRpin_PIN & (1 << IRpin)) {
		// Pino ainda high

		highpulse++;
		delayMicroseconds(RESOLUTION);

		/* Se o pulso for muito longo, ou nada ocorreu ou o codigo
		 * terminou. Mostra os tempos de pulso no monitor serial.
		 * */
		if ((highpulse >= MAXPULSE) && (currentpulse != 0)) {
			printPulses();
			currentpulse=0;
			return;
		}
	}
	// Pulso nao foi muito longo, armazena na matriz
	pulses[currentpulse][0] = highpulse;

	// Mesmo que o anterior
	while (! (IRpin_PIN & _BV(IRpin))) {
		// Pino ainda low
		lowpulse++;
		delayMicroseconds(RESOLUTION);
		if ((lowpulse >= MAXPULSE)  && (currentpulse != 0)) {
			printPulses();
			currentpulse=0;
			return;
		}
	}
	pulses[currentpulse][1] = lowpulse;

	/* Armazenando a matriz de duas colunas em um vetor com os valores
	 * alternados de cada coluna para, possivelmente, serem enviados
	 * a um banco de dados.
	 * */
	int i=0;
	for(int j=0; j<100; j++){
		pulsedb[i] = pulses[j][0]*RESOLUTION;
		i+=2; 
	}
	i=0;
	for(int j=0; j<100; j++){
		pulsedb[i+1] = pulses[j][1]*RESOLUTION;
		i+=2; 
	}

	// Pulso lido corretamente. 
	currentpulse++;

}

// Funcao para mostrar o tempo dos pulsos no monitor serial
void printPulses(void) {
	Serial.println("\n\r\n\rRecebido: \n\rOFF \tON");
	for (uint8_t i = 0; i < currentpulse; i++) {
		Serial.print(pulses[i][0] * RESOLUTION, DEC);
		Serial.print(" usec, ");
		Serial.print(pulses[i][1] * RESOLUTION, DEC);
		Serial.print(" usec\t");
	}

	delay(1000);
	// Chama a funcao que manda o comando pelo led IR
	sendCommand(currentpulse);
}

/* Esse procedimento manda um pulso de 38KHz para o IRledPin durande alguns 
 * microsegundos para reproduzir o comando decodificado (PWM).
 * */
void pulseIR(long microsecs) {

	cli();  // Desliga interrupcoes de background

	while (microsecs > 0) {
		// 38 kHz eh aproximadamente 13 microsegundos high e 13 microsegundos low
		digitalWrite(IRledPin, HIGH);	// digitalWrite dura 3 microsegundos
		delayMicroseconds(10);			// esperar 10 microsegundos
		digitalWrite(IRledPin, LOW);	// 3 microsegundos
		delayMicroseconds(10);			// espeera mais 10 microsegundos

		// entao diminuímos 26 microsegundos do pulso
		microsecs -= 26;
	}

	sei();  // Liga as interrupcoes
}

/* Funcao que reproduz o comando no led IR utilizando a matriz de microsegundos
 * `on' e `off'
 */
void sendCommand(uint8_t cp) {
	for (uint8_t i = 0; i < cp; i++){
		delayMicroseconds((pulses[i][0] * RESOLUTION));	// off
		pulseIR((pulses[i][1] * RESOLUTION));			// on
	}
	Serial.println("Comando enviado!");
}
