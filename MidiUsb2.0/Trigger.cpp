#include "MIDIUSB.h"
#include "Trigger.h"

bool Trigger::isTriggered(){
  int value = analogRead(this->analogPort);
  return value > this->threshold;
}

void Trigger::playDrum(){
  midiEventPacket_t noteOn = {0x09, 0x90 | 0, this->drumID, 127};
  MidiUSB.sendMIDI(noteOn);
  MidiUSB.flush();

  midiEventPacket_t noteOff = {0x08, 0x80 | 0, this->drumID, 127};
  MidiUSB.sendMIDI(noteOff);
  MidiUSB.flush();
}

void Trigger::upThreshold(){
  this->threshold += 10;
  delay(500);
}

void Trigger::downThreshold(){
  this->threshold -= 10;
  delay(500);
}

void Trigger::resetThreshold(){
  this->threshold = this->startThreshold;
  delay(500);
}
