void clockGenerator();
void digitalPatternGenerator();

void setup() {
  Serial.begin(9600);

  Serial.print("----------------------------\n0 - Generate clock pulses\n1 - Generate specific digital pattern\n\nEnter 0 or 1:  ");

  char c;
  do {
    c = -1;
    if (Serial.available()){
      c = Serial.read();
      if (c == '0') {
        Serial.println("0\nEntering clock mode...");
        clockGenerator();
      } else if (c == '1') {
        Serial.println("1\nEntering digital pattern mode...");
//        digitalPatternGenerator();
      } else if (c != '\r' && c != '\n') {
        Serial.println(c);
        Serial.print("No valid choice, please enter 0 or 1:  ");
      }
    }
  } while (c != '0' && c != '1');
}

void loop() {
  // put your main code here, to run repeatedly:

}


void clockGenerator(){
  Serial.print("How many clock cycles? [0 for infinite]:  ");
  long cycles = inputNumber();
  Serial.print("Frequency [Hz]:  ");
  float freq = inputNumber();
  if (cycles){
    Serial.print("Generating ");
    Serial.print(cycles);
    Serial.print(" clock pulses at ");
    Serial.print(freq);   // TODO: format frequency as kHz, MHz...
    Serial.println(" Hz on pin 2 ...");
  } else {
    Serial.print("Generating infinite clock pulses at ");
    Serial.print(freq);
    Serial.println(" Hz on pin 2 ...");
  }

  unsigned long generatedPulses = 0;
  bool pinState = false;
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  unsigned long beginTime = millis();

  while (generatedPulses < cycles || cycles == 0){
    if (millis() >= (generatedPulses/freq + pinState/(2*freq))*1000 + beginTime){
      pinState = !pinState;
      digitalWrite(2, pinState);

      if (pinState == 0){
        generatedPulses++;          // Temporary ignoring long overflow (at least after 1000 sec ~2:45 h)
      }
    }
  }

  Serial.println("Done!");
}

float inputNumber(){
  float n = 0;

  delay(50);

  serialFlush();
  
  do {
    if (Serial.available()){
      n = Serial.parseFloat();

      if (n == 0){
        Serial.print("INVALID\nInvalid input, the value must be a number (use dot '.' for decimal):  ");
      } else {
        Serial.println(n, 4);
      }
    }
  } while (n == 0);

  serialFlush();
  
  return n;
}

void serialFlush(){
  while(Serial.available()) {
    Serial.read();
  }
}   
