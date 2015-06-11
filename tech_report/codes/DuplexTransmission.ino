const int IRledPin = 13;
const int cchannelPin = A3; // Pino do canal de controle
const int dchannelPin = A0; // Pino do canal de dados
const int transmissionPin = 10; // Pino digital para comunicar para a Beagle que o canal
								// de dados está livre para transmissão

int controlSignal = 0; // Valor lido pelo canal de controle
int dataSignal = 0; // Valor lido pelo canal de dados
int transmissionSignal = 0; // Valor lido pelo canal de transmissão
int command[34]; // Vetor com os bits de dados a serem lidos que irão formar o comando
int i;

void setup() {
	pinMode(A3, INPUT);
	pinMode(A0, INPUT);
	pinMode(10, OUTPUT);
	pinMode(13, OUTPUT); //IR LED
	Serial.begin(9600); // initialize serial communications at 9600 bps:
	Serial.println("\nReady!\n");
}

void loop() {
	bbb();
}//loop

void bbb() {
	for(i=0; i<34; i++) 
		command[i] = 1; // Zera o comando antes de receber um novo

	i=0;
	while(i != 34) {
  		transmissionSignal = LOW;
		digitalWrite(transmissionPin, transmissionSignal);
		// Listening: ei BBB, quer meter?
		controlSignal = analogRead(cchannelPin);
		if(controlSignal > 690) { // BBB quer meter!
			transmissionSignal = HIGH;	//avisa que o canal de dados está livre 
			digitalWrite(transmissionPin, transmissionSignal); // Ok! Meta
		}
		
		// Se a Beagle quer transmitir (controlSignal = HIGH) e
		// o arduino tá pronto pra receber (transmissionSignal = HIGH)
		// ele lê do canal de dados (dataSignal = analogRead)
		while(transmissionSignal == HIGH) { 
			controlSignal = analogRead(cchannelPin); // BBB gozou: LOW
			dataSignal = analogRead(dchannelPin);
			if(controlSignal<100) { // se BBB gozou: LOW
				transmissionSignal = LOW;
				digitalWrite(transmissionPin, transmissionSignal); //Nao mete ainda que eu to ocupado
				if(dataSignal<100) // se data é LOW ...
					command[i] = 0; // ... bit é 0
				i++; // Já que leu 1 bit, incrementa o contador e vê se a Beagle quer transmitir de novo.
				break;
			}
		}
	}

	for(i=0; i<34; i++)
		Serial.print(command[i]);
	Serial.println("");

	send_samsung();
}

void send_samsung() {
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

/* Envia um pulso de 38kHz para o pin por x ms, reproduzindo o comando decodificado (PWM) */
void mark(long microsecs) {
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
/*
void outrarc6() {
  int i;
  int x[] = {42864, 2500, 940, 340, 960, 320, 520, 320, 540, 1180, 1400, 320, 540, 320, 520, 340, 520, 320, 
            540, 320, 540, 320, 520, 320, 540, 320, 520, 760, 960, 340, 520, 760, 520, 320, 960, 320, 540, 320, 
            15724, 2500, 940, 320, 960, 340, 520, 320, 540, 1180, 1400, 320, 540, 320, 520, 340, 520, 320, 520, 
            340, 520, 320, 540, 320, 540, 320, 520, 760, 960, 320, 540, 760, 520, 320, 960, 320, 540, 320 };
    
    for(i=0; i<sizeof(x)/sizeof(int); i+=2) {
      delayMicroseconds(x[i]);
      mark(x[i+1]);    
    }
}
*/
