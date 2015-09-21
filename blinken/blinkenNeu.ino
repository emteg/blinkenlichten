/*** Statische Einstellungen ************************************/

/* Mit welcher Frequenz die Hauptschleife ausgefuehrt wird      */
#define FREQUENCY 100

/* Wieviele Lampen angeschlossen sind                           */
#define LAMP_COUNT 6

/****************************************************************/


#include "Tlc5940.h"
#include "Lamps.h"
#include "MoodLight.h"


LampManager lampManager(LAMP_COUNT);
MoodLight moodLight(&lampManager);
const int loopDelay = 1000 / FREQUENCY; // In Millisekunden

boolean slaveMode = false;


void setup() {
  
  Tlc.init();
  
  // Die 6 Lampen gleichmaessig auf beide TLCs verteilen
  lampManager.setLampLeg(1, 6);
  lampManager.setLampLeg(2, 12);
  lampManager.setLampLeg(3, 16);
  lampManager.setLampLeg(4, 22);
  lampManager.setLampLeg(5, 28);
  
  // Einstellungen fuer das Moodlight im Stand Alone Modus
  moodLight.setDeltaPhiDegrees(0.5);
  moodLight.setBrightness(0.75);
  moodLight.setSaturation(0.9);
  moodLight.resetDegrees(0, 135);
  
  Serial.begin(9600);
}


void loop() {
  
  if (!slaveMode) {
    moodLight.changeColors();
    delay(loopDelay);
  } else {
    delay(1); 
  }
    
  lampManager.sendToTLC();
  
  
  int channel = 0;
  
  while (Serial.available() > 0) {
    channel = Serial.parseInt();
    
    // Farbe einer Lampe einstellen, in Slave-Modus wechseln
    // int R
    // int G
    // int B
    if (channel < 251) {
      slaveMode = true;
      int r = Serial.parseInt();
      int g = Serial.parseInt();
      int b = Serial.parseInt();
      
      if (Serial.read() == 10) {
        lampManager.setLampRGB(channel, r, g, b);
      } else {
        break;
      }
     
    // Lampen an oder ausschalten
    // int lampNumber Nummer der Lampe, die geschaltet werden soll
    // int isOn Ob die Lampe ein oder ausgeschaltet werden soll  
    } else if (channel == 251) {
      int lampNumber = Serial.parseInt();
      int isOn = Serial.parseInt();
      
      if (Serial.read() == 10) {
        if (isOn == 0) {
          lampManager.setLampIsOn(lampNumber, false);
        } else {
          lampManager.setLampIsOn(lampNumber, true);
        }
      } else {
        break;
      }
     
    // Moodlight-Geschwindigkeit einstellen
    // float deltaPhi Winkel in Grad, um den die Farben pro Takt veraendert werden
    } else if (channel == 252) {
      float deltaPhi = Serial.parseFloat();
      
      if (Serial.read() == 10) {
        moodLight.setDeltaPhiDegrees(deltaPhi);
      } else {
        break;
      } 
     
    // Moodlight-Saettigung einstellen  
    // float saturation im Bereich 0..1
    } else if (channel == 253) {
      float saturation = Serial.parseFloat();
      
      if (Serial.read() == 10) {
        moodLight.setSaturation(saturation);
      } else {
        break;
      }
     
    // Moodlight-Helligkeit einstellen  
    // float brightness im Bereich 0..1
    } else if (channel == 254) {
      float brightness = Serial.parseFloat();
      
      if (Serial.read() == 10) {
        moodLight.setBrightness(brightness);
      } else {
        break;
      }
      
    // Auf Moodlight-Modus zurueck schalten
    // float startFarbe Farbe der ersten Lampe in Grad
    // float farbDifferenz Unterschied zwischen den Lampen in Grad
    } else if (channel == 255) {
      
      float hueStart = Serial.parseFloat();
      float hueDiff = Serial.parseFloat();
      
      if (Serial.read() == 10) {
        slaveMode = false;
        moodLight.resetDegrees(hueStart, hueDiff);
      } else {
        break;
      }
        
    }
  }
  
}
