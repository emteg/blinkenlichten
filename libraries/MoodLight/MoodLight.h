#ifndef MoodLight_H
#define MoodLight_H

#include "Lamps.h"
#include <Arduino.h>

/*
 * Anleitung
 *
 * Im Konstruktor eine Referenz auf den LampManager uebergeben. Standardwerte
 * fuer Helligkeit, Saettigung und Winkeldifferenz werden eingestellt.
 *
 * Zum Benutzen einmal reset() aufrufen. Der Parameter gibt die Farbdifferenz
 * zwischen den einzelnen Lampen in Rad bzw Grad an. Die Farbwerte der Lampen
 * werden entsprechend berechnet, die erste Lampe wird immer auf 0 Grad
 * gesetzt. Es werden die zuletzt verwendeten Helligkeits- und Saettigungswerte
 * verwendet.
 *
 * Mit setBrightness() und setSaturation() werden die Helligkeits- und
 * Saettigungswerte aller Lampen geandert.
 *
 * calculateRGBValues() berechnet die RGB-Werte aus den aktuellen HSV-Werten.
 *
 * changeColors() aendert die Farbe jeder Lampe um die eingestellte Farb-
 * winkeldifferenz und ruft calculateRGBValues() auf.
 *
 */

class MoodLight {
	public:
		MoodLight(LampManager* lampManagerReference);
		~MoodLight();
		
		void reset(float hueStart, float lampHueDifference);
		void resetDegrees(float hueStart, float lampHueDifference);
		void calculateRGBValues();
		void changeColors();
		
		static int ratioToPercents(float aRatio);			// 0.0 .. 1.0 -> 0..100
		static float percentsToRatio(int percents);		// 0..100 -> 0.0 .. 0.1
		static float degreesToRadians(float degrees);		// 0..360 -> 0..2*Pi
		static float radiansToDegrees(float radians);		// 0..2*Pi -> 0..360
		
		float getBrightness();
		int getBrightnessPercent();
		
		float getSaturation();
		int getSaturationPercent();
		
		float getDeltaPhi();
		float getDeltaPhiDegrees();
		
		void setBrightness(float aBrightness);
		void setBrightnessPercent(int percents);
		
		void setSaturation(float aSaturation);
		void setSaturationPercent(int percents);
		
		void setDeltaPhi(float anAngle);
		void setDeltaPhiDegrees(float degrees);
	
	private:
		LampManager* lampManager;
		float deltaPhi;
		float brightness;
		float saturation;
		
		void calculateRGBValue(int lampIndex);
};

#endif