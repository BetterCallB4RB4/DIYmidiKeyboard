#include "MIDI.h"

#define ING_0 2
#define ING_1 3
#define ING_2 4
#define ING_3 5
#define ING_4 6
#define ING_5 7
#define ING_6 8
#define ING_7 9

#define CTRL_0 0
#define CTRL_1 1
#define CTRL_2 10
#define CTRL_3 14
#define CTRL_4 15
#define CTRL_5 16
#define CTRL_6 18

int ctrlChannel[] = {CTRL_0, CTRL_1, CTRL_2, CTRL_3, CTRL_4, CTRL_5, CTRL_6};
int ingChannel[] = {ING_0, ING_1, ING_2, ING_3, ING_4, ING_5, ING_6, ING_7};

int startOctave = 48;
int precPitch = 0;
int channel = 0;

int currentMatrix[7][8];
int precMatrix[7][8];
int noteMatrix[7][8] = {{0, 0, 0, 0, 0, 0, 0, 0},
                        {0, 0, 0, 0, 0, 0, 0, 0},
                        {29, 32, 31, 30, 35, 36, 34, 33},
                        {13, 16, 15, 14, 19, 20, 18, 17},
                        {37, 40, 39, 38, 0, 0, 0, 41},
                        {21, 24, 23, 22, 27, 28, 26, 25},
                        {5, 8, 7, 6, 11, 12, 10, 9}};

//SETUP MIDI INTERFACE
MIDI_CREATE_DEFAULT_INSTANCE();

void setup() {
  Serial.begin(115200);
  
  pinMode(ING_0, INPUT);
  pinMode(ING_1, INPUT);
  pinMode(ING_2, INPUT);
  pinMode(ING_3, INPUT);
  pinMode(ING_4, INPUT);
  pinMode(ING_5, INPUT);
  pinMode(ING_6, INPUT);
  pinMode(ING_7, INPUT);

  pinMode(CTRL_0, OUTPUT);
  pinMode(CTRL_1, OUTPUT);
  pinMode(CTRL_2, OUTPUT);
  pinMode(CTRL_3, OUTPUT);
  pinMode(CTRL_4, OUTPUT);
  pinMode(CTRL_5, OUTPUT);
  pinMode(CTRL_6, OUTPUT);

  for(int i = 0; i < (sizeof(ctrlChannel) / sizeof(ctrlChannel[0])); i++){
    digitalWrite(ctrlChannel[i], HIGH);
  }

  int lowerOctav = 0;
  for(int i = 0; i < (sizeof(ctrlChannel) / sizeof(ctrlChannel[0])); i++){
    for(int j = 0; j < (sizeof(ingChannel) / sizeof(ingChannel[0])); j++){
      currentMatrix[i][j] = 0;  
      precMatrix[i][j] = 0;
      //noteMatrix[i][j] = lowerOctav++;
    }
  }
}

void loop() {
  for(int i = 0; i < (sizeof(ctrlChannel) / sizeof(ctrlChannel[0])); i++){
    digitalWrite(ctrlChannel[i], LOW);
    for(int j = 0; j < (sizeof(ingChannel) / sizeof(ingChannel[0])); j++){
      
      if(digitalRead(ingChannel[j]) == 0){
        currentMatrix[i][j] = 1;
        if(precMatrix[i][j] == 0){
          precMatrix[i][j] = 1;
          noteOn(channel, noteMatrix[i][j] + startOctave);
        }  
      } else {
        currentMatrix[i][j] = 0;
        if(precMatrix[i][j] == 1){
          precMatrix[i][j] = 0;
          noteOff(channel, noteMatrix[i][j] + startOctave);
        }  
      }
    }
    digitalWrite(ctrlChannel[i], HIGH);
  }
  
  if(analogRead(A3) != precPitch){
    precPitch = analogRead(A3);
    pitchBend(0, precPitch);
    delay(20);
  }
  
}

void printMatrix(){
  Serial.println("-- current matrix --");
  for(int i = 0; i < (sizeof(ctrlChannel) / sizeof(ctrlChannel[0])); i++){
    for(int j = 0; j < (sizeof(ingChannel) / sizeof(ingChannel[0])); j++){
      Serial.print(currentMatrix[i][j]);
    }
    Serial.println();
  }
  Serial.println();
  Serial.println("-- prec matrix --");
  for(int i = 0; i < (sizeof(ctrlChannel) / sizeof(ctrlChannel[0])); i++){
    for(int j = 0; j < (sizeof(ingChannel) / sizeof(ingChannel[0])); j++){
      Serial.print(precMatrix[i][j]);
    }
    Serial.println();
  }
  
}

void noteOn(byte channel, byte pitch) {
  MIDI.sendNoteOn(127, pitch, channel);
}

void noteOff(byte channel, byte pitch) {
  MIDI.sendNoteOff(127, pitch, channel);
}

void pitchBend(byte channel, int pitch) {
  int midiCode = map(pitch, 0, 1024, 0, 16384);
  MIDI.sendPitchBend(midiCode, channel);
}
