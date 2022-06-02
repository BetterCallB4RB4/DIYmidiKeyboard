#include "MIDI.h"

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
double pitchMap[ 36 ][ 2 ] = {{60, 0.0}, {60, 0.0}, {60, 0.0}, {60, 0.0}, {60, 0.0}, {60, 0.0}, {60, 0.0}, {60, 0.0}, {60, 0.0}, {60, 0.0}, {60, 0.0}, {60, 0.0},
                              {60, 0.0}, {60, 0.0}, {60, 0.0}, {60, 0.0}, {60, 0.0}, {60, 0.0}, {60, 0.0}, {60, 0.0}, {60, 0.0}, {60, 0.0}, {60, 0.0}, {60, 0.0},
                              {60, 0.0}, {60, 0.0}, {60, 0.0}, {60, 0.0}, {60, 0.0}, {60, 0.0}, {60, 0.0}, {60, 0.0}, {60, 0.0}, {60, 0.0}, {60, 0.0}, {60, 0.0}};

void setup() {
    
  

}

void loop() {


  
  
}






void setDutyCycle(int midiNote){
  for(int i = 0; i < 36; i++){
    if(midiNote == pitchMap[i][0]){
      
    }
  }
}
