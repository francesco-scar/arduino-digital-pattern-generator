void setup() {
  Serial.begin(9600);

  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
}

void loop() {
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
        Serial.println("FUNCTION NOT AVAILABLE, I WILL ADD THAT SOON (I HOPE)")
//        digitalPatternGenerator();
      } else if (c != '\r' && c != '\n') {
        Serial.println(c);
        Serial.print("No valid choice, please enter 0 or 1:  ");
      }
    }
  } while (c != '0' && c != '1');
}


void clockGenerator(){
  clockFunctionBegin:
  Serial.print("How many clock cycles? [-1 for infinite]:  ");
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

  delay(50);

  serialFlush();
  
  Serial.println("Done!\n\nEnter 1 to select clock option again other to return to main menu");
  while (!Serial.available());
  if (Serial.read() == '1'){
    goto clockFunctionBegin;
  }
}

/*
void digitalPatternGenerator(){
  patternFunctionBegin:
  Serial.print("Frequency [Hz]:  ");
  float freq = inputNumber();
  Serial.println("Pattern string (256 byte max, truncated if more) in binary (introduced by B) or hexadecimal (introduced by X) [eg. B101110 or X2e]");
  uint8_t patern[64];
  uint8_t patternLength; 
//  getPattern(&patternLength, pattern);
  
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

  delay(50);

  serialFlush();
  
  Serial.println("Done!\n\nEnter 1 to select clock option again other to return to main menu");
  while (!Serial.available());
  if (Serial.read() == '1'){
    goto patternFunctionBegin;
  }
}




void getPattern(uint8_t ){
  
}
*/

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
