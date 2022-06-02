#include "MIDIUSB.h"
#include "Trigger.h"

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

int kick = 0;
int snare = 0;
int hihat = 0;
int kickThreshold = 1020;
int snareThreshold = 1020;
int hihatThreshold = 1020;

int hihatStart = 0;
int snareStart = 0;
int kickStart = 0;

Trigger kickDrum(A0, 24, 100);

int startOctave = 48;

int currentMatrix[7][8];
int precMatrix[7][8];
int noteMatrix[7][8] = {{-1, -4, -7, -10, 0, 0, 0, 0},
                        {-2, -5, -8, -11, -3, -6, -9, -12},
                        {29, 32, 31, 30, 35, 36, 34, 33},
                        {13, 16, 15, 14, 19, 20, 18, 17},
                        {37, 40, 39, 38, 0, 0, 0, 41},
                        {21, 24, 23, 22, 27, 28, 26, 25},
                        {5, 8, 7, 6, 11, 12, 10, 9}};

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
          //numeri positivi identificano le note, numeri negativi identificano opzioni
          if(noteMatrix[i][j] < 0){
            int controlCode = noteMatrix[i][j];
            switch (controlCode) {
              case -1:
                octaveUp();
                break;
              case -2:
                octaveDown();
                break;
              case -3:
                resetOcatve();
                break;
              case -4:
                kickUp();
                break;
              case -5:
                kickDown();
                break;
              case -6:
                resetKick();
                break;
              case -7:
                snareUp();
                break;
              case -8:
                snareDown();
                break;
              case -9:
                resetSnare();
                break;
              case -10:
                hihatUp();
                break;
              case -11:
                hihatDown();
                break;
              case -12:
                resetHihat();
                break;
            }
          } else {
            noteOn(0, noteMatrix[i][j] + startOctave, 127);
          }
        }
      } else {
        currentMatrix[i][j] = 0;
        if(precMatrix[i][j] == 1){
          precMatrix[i][j] = 0;
          noteOff(0, noteMatrix[i][j] + startOctave, 127);
        }  
      }  
      //printPos(j, i);
    }
    digitalWrite(ctrlChannel[i], HIGH);
  }
  
  kick = analogRead(A3);
  snare = analogRead(A2);
  hihat = analogRead(A1);

  if (kick >= kickThreshold && kickStart == 0 ) {
    noteOn(0, 24, 127); 
    kickStart = millis();
  } else if((millis() - kickStart) > 25 && kickStart != 0){
    noteOff(0, 24, 127);
    kickStart = 0;
  }
  
  if (snare >= snareThreshold && snareStart == 0 ) {
    noteOn(0, 28, 127); 
    snareStart = millis();
  } else if((millis() - snareStart) > 25 && snareStart != 0){
    noteOff(0, 28, 127);
    snareStart = 0;
  }
  
  if (hihat >= hihatThreshold && hihatStart == 0 ) {
    noteOn(0, 30, 127);
    hihatStart = millis();
  } else if((millis() - hihatStart) > 25 && hihatStart != 0){
    noteOff(0, 30, 127);
    hihatStart = 0;
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

void printPos(int j, int i){
  if(digitalRead(ingChannel[j]) == 0){
    Serial.println("------------");
    Serial.print("index:");
    Serial.print(j);
    Serial.print(" -> ");
    Serial.print("pin:");
    Serial.println(ingChannel[j]);
    Serial.print("index:");
    Serial.print(i);
    Serial.print(" -> ");
    Serial.print("pin:");
    Serial.println(ctrlChannel[i]);
  }
}

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
  MidiUSB.flush();  
  /*
  Serial.print("suono la nota: ");
  Serial.println(noteMatrix[i][j]);
  Serial.print(ingChannel[j]);
  Serial.print(",");
  Serial.println(ctrlChannel[i]);
  */
}

void pitchBend(byte channel, int pitch) {
  int midiCode = map(pitch, 0, 1024, 0, 16384);
  byte pbl = midiCode & 0x7F;
  byte pbh = (midiCode >> 7) & 0x7F;
  midiEventPacket_t pitchBend = {0x0E, 0xE0 | channel, pbl, pbh};
  MidiUSB.sendMIDI(pitchBend);
  MidiUSB.flush();
  delay(20);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
  MidiUSB.flush();
  /*
  Serial.print("fermo la nota: ");
  Serial.println(noteMatrix[i][j]);
  Serial.print(ingChannel[j]);
  Serial.print(",");
  Serial.println(ctrlChannel[i]);
  */
}

//################################### -> OCTAVE SETTINGS
void octaveUp(){
  if(startOctave <= 119 ){
    startOctave += 8;
    Serial.print("octave: ");
    Serial.println(startOctave);
    delay(500);    
  }
}

void octaveDown(){
  if(startOctave >= 8 ){
    startOctave -= 8;
    Serial.print("octave: ");
    Serial.println(startOctave);
    delay(500);    
  }
}

void resetOcatve(){
  startOctave = 48;
  delay(500);    
  Serial.print("octave: ");
  Serial.println(startOctave);
}
//################################### -> KICKDRUM SETTINGS
void kickUp(){
  kickThreshold += 10;
  Serial.print("kick threshold: ");
  Serial.println(kickThreshold);
  delay(500);
}

void kickDown(){
  kickThreshold -= 10;
  Serial.print("kick threshold: ");
  Serial.println(kickThreshold);
  delay(500);
}

void resetKick(){
  kickThreshold = 100;
  Serial.print("kick threshold: ");
  Serial.println(kickThreshold);
  delay(500);
}
//################################### -> SNARE SETTINGS
void snareUp(){
  snareThreshold += 10;
  Serial.print("snare threshold: ");
  Serial.println(snareThreshold);
  delay(500);
}

void snareDown(){
  snareThreshold -= 10;
  Serial.print("snare threshold: ");
  Serial.println(snareThreshold);
  delay(500);
}

void resetSnare(){
  snareThreshold = 100;
  Serial.print("snare threshold: ");
  Serial.println(snareThreshold);
  delay(500);
}
//################################### -> HIHAT SETTINGS
void hihatUp(){
  hihatThreshold += 10;
  Serial.print("hihat threshold: ");
  Serial.println(hihatThreshold);
  delay(500);
}

void hihatDown(){
  hihatThreshold -= 10;
  Serial.print("hihat threshold: ");
  Serial.println(hihatThreshold);
  delay(500);
}

void resetHihat(){
  hihatThreshold = 100;
  Serial.print("hihat threshold: ");
  Serial.println(hihatThreshold);
  delay(500);
}
//################################### 
