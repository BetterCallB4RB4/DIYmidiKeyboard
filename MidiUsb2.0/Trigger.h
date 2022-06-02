#ifndef __TRIGGER__
#define __TRIGGER__

class Trigger {

  public:
  Trigger(int analogPort, int drumID, int startThreshold){
    this->analogPort = analogPort;
    this->drumID = drumID;
    this->startThreshold = startThreshold;
    this->threshold = startThreshold;
  }     

  bool isTriggered();
  void playDrum();
  
  void upThreshold();
  void downThreshold();
  void resetThreshold();
  
  private:
    int analogPort;
    int drumID;
    int threshold;
    int startThreshold;

};

#endif
