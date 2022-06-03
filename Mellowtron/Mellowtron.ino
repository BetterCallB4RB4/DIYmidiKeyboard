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
int pitchMap[ 36 ][ 2 ] = {{60, 512}, {60, 512}, {60, 512}, {60, 512}, {60, 512}, {60, 512}, {60, 512}, {60, 512}, {60, 512}, {60, 512}, {60, 512}, {60, 512}, 
                           {60, 512}, {60, 512}, {60, 512}, {60, 512}, {60, 512}, {60, 512}, {60, 512}, {60, 512}, {60, 512}, {60, 512}, {60, 512}, {60, 512}, 
                           {60, 512}, {60, 512}, {60, 512}, {60, 512}, {60, 512}, {60, 512}, {60, 512}, {60, 512}, {60, 512}, {60, 512}, {60, 512}, {60, 512}};


void setup() {
  MIDI.begin();
  pinMode(PIN_CONTROL, OUTPUT);
  Timer1.initialize(1);
  Timer1.pwm(PIN_CONTROL, 512, 1); //first initialization - allows to use setPwmDuty() later wich is slitly faster function
}

void loop() {
  if (MIDI.read()){
    switch(MIDI.getType()){
      case midi::NoteOn:
        int note = int(MIDI.getData1());
        playNote(note);
        break;
    }
  }
}

/*
 * as this project is to be considered as an expansion of the minilogue, 
 * the played note does not have an ADSR control 
 * (which is delegated to the minilogue circuit) so the note is never stopped. 
 * The audio stream is continuous, it only change its pitch over time
 */
void playNote(int midiNote){
  for(int i = 0; i < 36; i++){
    if(midiNote == pitchMap[i][0]){
      Timer1.setPwmDuty(PIN_CONTROL, pitchMap[i][1]);
    }
  }
}
