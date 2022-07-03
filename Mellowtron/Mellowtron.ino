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
//int pitchMap[ 12 ][ 2 ] = {{0, 200}, {1, 250}, {2, 300}, {3, 350}, {4, 400}, {5, 450}, {6, 500}, {7, 550}, {8, 600}, {9, 650}, {10, 700}, {11, 750}};
int pitchMap[ 12 ][ 2 ] = {{0, 20}, {1, 40}, {2, 60}, {3, 80}, {4, 100}, {5, 120}, {6, 150}, {7, 170}, {8, 190}, {9, 210}, {10, 230}, {11, 250}};

void setup() { 
  /* var for midi*/
  Serial.begin(31250);
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.setHandleNoteOn(handleNoteOn);

  /* var for feedback live led */
  pinMode (LED_BUILTIN, OUTPUT); 
  digitalWrite(LED_BUILTIN, LOW);

  /* var for pwm pitch control */
  
  pinMode(PIN_CONTROL, OUTPUT);
  /*
  Timer1.initialize(1);
  Timer1.pwm(PIN_CONTROL, 512, 1);  //first initialization - allows to use setPwmDuty() later wich is slitly faster function 
  */
}

void loop() {
   MIDI.read();
}

/*
 * as this project is to be considered as an expansion of the minilogue, 
 * the played note does not have an ADSR control 
 * (which is delegated to the minilogue circuit) so the note is never stopped. 
 * The audio stream is continuous, it only change its pitch over time
 */
void handleNoteOn(byte channel, byte pitch, byte velocity){
  /* pitch control */
  int midiNote = int(pitch)%12;
  for(int i = 0; i < 12; i++){
    if(midiNote == pitchMap[i][0]){
      //Timer1.setPwmDuty(PIN_CONTROL, pitchMap[i][1]);
      //Timer1.pwm(PIN_CONTROL, 512, 1); less optimized as the doc tells
      analogWrite(PIN_CONTROL, pitchMap[i][1]);
      return;
    }
  }
  return;
}
