const int IRledPin = 13;
const int cchannelPin = A3; // Listen: recebe da BBB um sinal de controle
const int dchannelPin = A0; // Listen: recebe da BBB um bit de dados
const int transmissionPin = 10; // Speak: transmite para a BBB um 'Ok, mande dados'

int controlSignal = 0; // Valor lido pelo canal de controle
int dataSignal = 0; // Valor lido pelo canal de dados
int transmissionSignal = 0; // Valor lido pelo canal de transmissão
int command[34]; // Vetor com os bits de dados a serem lidos que irão formar o comando
int i;

void setup() 
{
	pinMode(A3, INPUT);
	pinMode(A0, INPUT);
	pinMode(10, OUTPUT);
	pinMode(13, OUTPUT); //IR LED
	Serial.begin(9600); // initialize serial communications at 9600 bps:
	Serial.println("\nReady!\n");
}

void loop() 
{
	/* Receive bits from BBB */
	receive_from_bbb();
	/* Aciona o IR LED e modula bitstream pra mandar pra TV */
	send_samsung();
}

/**/
void receive_from_bbb() 
{
	for(i=0; i<34; i++) 
		command[i] = 1; // inicializa a bitstream antes de receber um novo

	i=0;
	while(i != 34) {
 		transmissionSignal = LOW;
		digitalWrite(transmissionPin, transmissionSignal);
		/* Listening: ei BBB, quer transmistir bit? */
		controlSignal = analogRead(cchannelPin);
		if(controlSignal > 690) { // BBB quer!
			/* Speaking: Ei BBB, pode mandar o bit! */
			transmissionSignal = HIGH;
			digitalWrite(transmissionPin, transmissionSignal); 
		}
		
		/* BBB quer transmitir .:. controlSignal = HIGH */
		/* UNO pronto pra receber .:. transmissionSignal = HIGH */
		while(transmissionSignal == HIGH) { 
			/* Listening: ei BBB, terminou de mandar o bit? */
			controlSignal = analogRead(cchannelPin); 
			/* Listening: ei BBB, manda o bit no canal de dados */
			dataSignal = analogRead(dchannelPin); 
			if(controlSignal<100) { // BBB terminou!
				/* Speaking: Ei BBB, nao manda bit, pois estou ocupado! */
				transmissionSignal = LOW;
				digitalWrite(transmissionPin, transmissionSignal);
				if(dataSignal<100) // se data eh LOW, bit eh 0
					command[i] = 0; 
				i++; // Jah que leu 1 bit, incrementa o contador e quebra o loop
				break;
			}
		}
	}

	/* Imprime comando recebido no Serial Monitor */
	for(i=0; i<34; i++)
		Serial.print(command[i]);
	Serial.println("");
}

void send_samsung() 
{
	delayMicroseconds(55000);
	mark(4300);
	delayMicroseconds(4500);
	for(i=1; i<33; i++) {
		mark(440);
		if(command[i]==0) {
			delayMicroseconds(640);
		} else {
			delayMicroseconds(1720);
		}
	}
	mark(440);
	delayMicroseconds(640);
}

/* Envia um pulso de 38kHz para o pin por x us, reproduzindo o comando decodificado (PWM) */
void mark(long microsecs) 
{
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
	sei();  // Reativa as interrupcoes de background
}
