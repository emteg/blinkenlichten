#ifndef Lamps_H
#define Lamps_H

#include <Arduino.h>

struct lamp {
  boolean isOn;
  int R;
  int G;
  int B;
	float H;
	float S;
	float V;
  int leg;
};

class LampManager {
	public:
    LampManager(int aCount);
    ~LampManager();
    
    int getCount();
    struct lamp* getLamp(int index);
    void sendToTLC();
		void setLampLeg(int index, int aLeg);
		void setLampRGB(int index, int aR, int aG, int aB);
		void setLampHSV(int index, float aH, float aS, float aV);
		void setLampIsOn(int index, boolean isOn);
		

  private:
    int count;
    lamp *lamps;
    float gammaFactor;
    
    int gammaCorrect(int &value);
		void sendLampToTLC(int &index);
    void setupLamps();	
		int RGBLimit(int aValue);
};

#endif