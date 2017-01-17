const int buttonPin = 2;
const int greenLed = 13;
const int redLed = 12;
const int piezo = 8;

/*
 * Morse rules:
 * 
 * 1. Length of dot is one unit
 * 2. Length of dash is three units
 * 3. The space between parts of the letter is one unit
 * 4. The space between letters is three units
 * 5. The space between words is seven units 
 */

int multiplier = 100;
int delayPeriod = 10;
int lastReading = LOW;

int dotLength = multiplier;
int dashLength = multiplier * 3;
int charPause = multiplier;
int letterPause = multiplier * 3;
int wordPause = multiplier * 7;

int dotLengthTolerance = dotLength / 2;
int dashLengthTolerance = dashLength / 2; 
int charPauseTolerance = charPause / 2;
int letterPauseTolerance = letterPause / 2;
int wordPauseTolerance = wordPause / 2;

int timeOn = 0;
int timeOff = 0;
int lastTimeOn = 0;
int lastTimeOff = 0;

String morse = "";

int counter = 0;
int remainder;
int buttonStatus [4];

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  Serial.begin(9600);
}


void lateOrEarly(int diffs[]){
  for (int i; i < sizeof(diffs)-1 ; i++){
    Serial.print(diffs[i]); 
  }
}

void loop() {
  delay(delayPeriod);
  
  if (digitalRead(buttonPin) == HIGH){
    tone(piezo, 800);
    digitalWrite(redLed, LOW);
    timeOn += delayPeriod;
    lastTimeOff = timeOff;
    timeOff = 0;
  }
  else{
    noTone(piezo);
    digitalWrite(greenLed, LOW);
    digitalWrite(redLed, LOW);
    timeOff += delayPeriod;
    lastTimeOn = timeOn;
    timeOn = 0;
  }

  if (lastReading != digitalRead(buttonPin) && buttonStatus[0] == lastReading && buttonStatus[1] == lastReading && buttonStatus[2] == lastReading && buttonStatus[3] == lastReading && buttonStatus[4] == lastReading){
    if (lastReading == LOW){
      if ((charPause + charPauseTolerance) > lastTimeOff && lastTimeOff > (charPause - charPauseTolerance)){
        digitalWrite(greenLed, HIGH);
        Serial.println("Charpause");
      }
      else if((letterPause + letterPauseTolerance) > lastTimeOff && lastTimeOff > (letterPause - letterPauseTolerance)){
        digitalWrite(greenLed, HIGH);
        Serial.println("Letterpause");
        morse += " ";
      }
      else if((wordPause + wordPauseTolerance) > lastTimeOff && lastTimeOff > (wordPause - wordPauseTolerance)){
        digitalWrite(greenLed, HIGH);
        Serial.println("Wordpause");
        morse += "/";
      }
      else {
        digitalWrite(redLed, HIGH);
        Serial.print("Invalid pause length: ");
        Serial.println(lastTimeOff);

      }
    }
    else {
      if ((dotLength + dotLengthTolerance) > lastTimeOn && lastTimeOn > (dotLength - dotLengthTolerance)){
        digitalWrite(greenLed, HIGH);
        Serial.println("dot");
        morse += ".";
        Serial.println(morse);
      }
      else if((dashLength + dashLengthTolerance) > lastTimeOn && lastTimeOn > (dashLength - dashLengthTolerance)){
        digitalWrite(greenLed, HIGH);
        Serial.println("dash");
        morse += "-";
        Serial.println(morse);
      }
      else {
        digitalWrite(redLed, HIGH);
        Serial.print("Invalid press length: ");
        Serial.print(lastTimeOn);

        int dotDiff = dotLength - lastTimeOn;
        int dashDiff = dashLength - lastTimeOn;
        int pushedDiff[] = {dotDiff, dashDiff};

        int lowestDiff = abs(pushedDiff[0]);
        for(int i = 0; i < 2; i++){
          if (abs(pushedDiff[i]) < lowestDiff){
            (pushedDiff[i]) = lowestDiff;
          }
        }
        if (lowestDiff > 0){
          Serial.println(": Yellow LED");
        }
        else{
          Serial.println(": Blue LED");
        }
        
      }
    }
  }
  lastReading = digitalRead(buttonPin);

  remainder = counter % 4;
  buttonStatus [remainder] = digitalRead(buttonPin); 
  counter++;
  
}
