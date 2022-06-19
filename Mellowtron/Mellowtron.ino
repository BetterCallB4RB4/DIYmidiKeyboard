#include "MIDI.h"
#include "TimerOne.h"

#define PIN_CONTROL 9

MIDI_CREATE_DEFAULT_INSTANCE();

/*
 * since there is no mathematical correlation between 
 * a note and the corresponding voltage to be given to the motor 
 * (since the pitch is controlled by the quality of the motor) 
 * the notes must be mapped manually
 */

/*
 * We have estimated to reach 3 octaves, to move further in range 
 * it will be necessary to modify the pitch present on the cassette. 
 * 
 * a C must be recorded on the cassette
 */
int pitchMap[ 12 ][ 2 ] = {{61, 200}, {62, 250}, {63, 300}, {64, 350}, {65, 400}, {66, 450}, {67, 500}, {68, 550}, {69, 600}, {70, 650}, {71, 700}, {72, 750}};


void setup() {  
  MIDI.begin();
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  pinMode(PIN_CONTROL, OUTPUT);
  Timer1.initialize(1);
  Timer1.pwm(PIN_CONTROL, 512, 1); //first initialization - allows to use setPwmDuty() later wich is slitly faster function
}

String inString = "";
int value = 0;
int previus = 0;

void loop() {

  if (Serial.available()){
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
  }

  

  
  
  /*
  if (MIDI.read()){
    switch(MIDI.getType()){
      case midi::NoteOn:
        int note = int(MIDI.getData1());
        playNote(note);
        break;
    }
  }
  */



  /*
  //inside loop()
  // Read serial input:
  while (Serial.available() > 0) {
    int inChar = Serial.read();
    if (isDigit(inChar)) {
      // convert the incoming byte to a char and add it to the string:
      inString += (char)inChar;
    }
    // if you get a newline, print the string, then the string's value:
    if (inChar == '\n') {
      value = inString.toInt();
      Serial.println(value);
      inString = "";
    }
  }
  if(previus != value){
    Timer1.setPwmDuty(PIN_CONTROL, value);
    previus = value;
  }
  */
   
   /*
   delay(tempo);
   playNote(62);
   delay(tempo);
   playNote(63);
   delay(tempo);
   playNote(64);
   delay(tempo);
   playNote(65);
   delay(tempo);
   playNote(66);
   delay(tempo);
   playNote(67);
   delay(tempo);
   playNote(68);
   delay(tempo);
   playNote(69);
   delay(tempo);
   playNote(70);
   delay(tempo);
   playNote(71);
   delay(tempo);
   playNote(72);
   delay(tempo);
   */
   
  
   
}

/*
 * as this project is to be considered as an expansion of the minilogue, 
 * the played note does not have an ADSR control 
 * (which is delegated to the minilogue circuit) so the note is never stopped. 
 * The audio stream is continuous, it only change its pitch over time
 */
void playNote(int midiNote){
  for(int i = 0; i < 12; i++){
    if(midiNote == pitchMap[i][0]){
      Timer1.setPwmDuty(PIN_CONTROL, pitchMap[i][1]);
      Serial.println(pitchMap[i][1]);
      return;
    }
  }
  return;
}
