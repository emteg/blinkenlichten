#include "Lamps.h"
#include "Tlc5940.h"

/* Lamps - Public Functions */

LampManager::LampManager(int aCount) {
  count = aCount;
  lamps = new lamp[count];
	gammaFactor = 1 / ((float) 4095);
  this->setupLamps();
}


LampManager::~LampManager() {
  
}


int LampManager::getCount() {
  return count; 
}


struct lamp* LampManager::getLamp(int index) {
	return &lamps[index]; 
}


void LampManager::sendToTLC() {
  Tlc.clear();
  
  for (int i = 0; i < count; i++) {
    this->sendLampToTLC(i);
  }
  
  Tlc.update();
}


void LampManager::setLampLeg(int index, int aLeg) {
	if (index >= 0 && index < count) {
		lamps[index].leg = aLeg;
	}
}


void LampManager::setLampRGB(int index, int aR, int aG, int aB) {
	if (index >= 0 && index < count) {
		lamps[index].R = this->RGBLimit(aR);
		lamps[index].G = this->RGBLimit(aG);
		lamps[index].B = this->RGBLimit(aB);
	}
}

void LampManager::setLampHSV(int index, float aH, float aS, float aV) {
	if (index >= 0 && index < count) {
		lamps[index].H = aH;
		lamps[index].S = aS;
		lamps[index].V = aV;
	}
}


void LampManager::setLampIsOn(int index, boolean isOn) {
	if (index >= 0 && index < count) {
		lamps[index].isOn = isOn;
	}
}


/* Lamps - Private Functions */

/**
 * gammaCorrect(int &value)
 *
 * Fuehrt eine Gamma-Korrektur fuer den uebergebenen Wert aus.
 * result = wert^2 / MAX_HELLIGKEIT
 */
int LampManager::gammaCorrect(int &value) {
  if (value > 0) {
    return gammaFactor * value * value;
  } else {
   return 0; 
  }
}


void LampManager::sendLampToTLC(int &index) {
  if (lamps[index].isOn) {
    Tlc.set(lamps[index].leg, this->gammaCorrect(lamps[index].R));
    Tlc.set(lamps[index].leg + 1, this->gammaCorrect(lamps[index].G));
    Tlc.set(lamps[index].leg + 2, this->gammaCorrect(lamps[index].B));
  }
}


/**
 * setupLamps()
 *
 * Setzt Standardwerte fuer die Lampen und weist den Lampen
 * deren jeweils erstes Bein an den TLCs zu.
 */
void LampManager::setupLamps() {
  int leg = 0;                             // erstes Bein der aktuellen Lampe
  int lampCounter = 0;                     // Zaehlt, wieviele Lampen schon
                                           // am aktuellen TLC angeschlossen sind

  for (int i = 0; i < count; i++) {    
    lamps[i].isOn = true;
    lamps[i].R = 0;
    lamps[i].G = 0;
    lamps[i].B = 0;
		lamps[i].H = 0;
    lamps[i].S = 0;
    lamps[i].V = 0;
    lamps[i].leg = leg;
        
    leg += 3;
    lampCounter++;
    
    if (lampCounter > 4) {                 // Wenn 5 Lampen am TLC angeschlossen
      lampCounter = 0;                     // sind: zum naechsten TLC springen
      leg++;                               // Zaehler zuruecksetzen
    }
  }
}

int LampManager::RGBLimit(int aValue) {
	if (aValue >= 0 && aValue <= 4095) {
		return aValue;
	} else if (aValue < 0) {
		return 0;
	} else {
		return 4095;
	}
}