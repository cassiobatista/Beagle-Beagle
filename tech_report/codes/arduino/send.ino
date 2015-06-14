// This sketch will send out a Nikon D50 trigger signal (probably works with // most Nikons)
// See the full tutorial at http://www.ladyada.net/learn/sensors/ir.html
 
void setup()   {                
	// initialize the IR digital pin as an output:
	pinMode(13, OUTPUT);      
	Serial.begin(9600);
}
 
void loop()                     {
  Serial.println("Sending IR signal");
  SendChannelUpCode();
  delay(10000);  // wait twenty seconds (20 seconds * 1000 milliseconds)
}
 
// This procedure sends a 38KHz pulse to the 13 
// for a certain # of microseconds. We'll use this whenever we need to send
// codes
void pulseIR(long microsecs) {
	// we'll count down from the number of microseconds we are told to wait
	cli();  // this turns off any background interrupts
	while (microsecs > 0) {
		// 38 kHz is about 13 microseconds high and 13 microseconds low
		digitalWrite(13, HIGH);  // this takes about 3 microseconds to happen
		delayMicroseconds(10);   // hang out for 10 microseconds
		digitalWrite(13, LOW);   // this also takes about 3 microseconds
		delayMicroseconds(10);   // hang out for 10 microseconds
		
		// so 26 microseconds altogether
		microsecs -= 26;
	}
	sei();  // this turns them back on
}
 
// This is the code for the CHANNEL + for the downstairs TV COMCAST
void SendChannelUpCode() {
  delayMicroseconds(); //Time off (Left Column on serial monitor)
  pulseIR();           //Time on  (Right Column on serial monitor)
  delayMicroseconds();
  pulseIR();
  delayMicroseconds();
  pulseIR();
  delayMicroseconds();
  pulseIR();
  delayMicroseconds();
  pulseIR();
  delayMicroseconds();
  pulseIR();
  delayMicroseconds();
  pulseIR();
  delayMicroseconds();
  pulseIR();
}
