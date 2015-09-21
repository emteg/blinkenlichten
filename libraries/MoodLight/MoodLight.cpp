#include "MoodLight.h"
#include "Lamps.h"

#define MAX_BRIGHTNESS 4095

MoodLight::MoodLight(LampManager* lampManagerReference) {

	this->lampManager = lampManagerReference;
	this->deltaPhi = 0;
	this->brightness = 0.75;
	this->saturation = 1;
	
}


MoodLight::~MoodLight() {

}

/* Aktions-Funktionen */

void MoodLight::reset(float hueStart, float lampHueDifference) {

	if (lampHueDifference < 0) {
		lampHueDifference = 0;
	} else if (lampHueDifference > 2 * PI) {
		lampHueDifference = lampHueDifference - 2 * PI;
	}
	
	float hue = hueStart;

	for (int i = 0; i < this->lampManager->getCount(); i++) {
	
		lamp* l = this->lampManager->getLamp(i);	
		l -> H = hue;
		l -> S = this->saturation;
		l -> V = this->brightness;
	
		hue += lampHueDifference;
		
		if (hue > 2*PI) {
			hue -= 2*PI;
		}
		
	}
	
}


void MoodLight::resetDegrees(float hueStart, float lampHueDifference) {

	this->reset(
		MoodLight::degreesToRadians(hueStart),
		MoodLight::degreesToRadians(lampHueDifference));
	
}


void MoodLight::calculateRGBValues() {
	
	for (int i = 0; i < this->lampManager->getCount(); i++) {
		this->calculateRGBValue(i);
	}
	
}


void MoodLight::calculateRGBValue(int lampIndex) {
	lamp* l = this->lampManager->getLamp(lampIndex);
	
	double r, g, b, h;
	h = l -> H / 2.0 / PI;
	
	int i = int(h * 6);
	double f = h * 6 - i;
	double p = l -> V * (1 - l -> S);
	double q = l -> V * (1 - f * l -> S);
	double t = l -> V * (1 - (1 - f) * l -> S);
	
	switch(i % 6) {
		case 0: r = l -> V, g = t, b = p; break;
		case 1: r = q, g = l -> V, b = p; break;
		case 2: r = p, g = l -> V, b = t; break;
		case 3: r = p, g = q, b = l -> V; break;
		case 4: r = t, g = p, b = l -> V; break;
		case 5: r = l -> V, g = p, b = q; break;
	}
	
	l -> R = r * MAX_BRIGHTNESS;
	l -> G = g * MAX_BRIGHTNESS;
	l -> B = b * MAX_BRIGHTNESS;
	
}


void MoodLight::changeColors() {

	for (int i = 0; i < this->lampManager->getCount(); i++) {
	
		lamp* l = this->lampManager->getLamp(i);
		l -> H += this->deltaPhi;
		
		if (l -> H > 2*PI) {
			l -> H -= 2*PI;
		}
		
	}
	calculateRGBValues();
	
}

/* Static-Funktionen */

int MoodLight::ratioToPercents(float aRatio) {

	return aRatio * 100;
	
}


float MoodLight::percentsToRatio(int percents) {

	return percents / 100;
	
}


float MoodLight::degreesToRadians(float degrees) {

	return degrees * PI / 180.0;
	
}


float MoodLight::radiansToDegrees(float radians) {

	return radians * 180.0 / PI;
	
}



/* Getters */

float MoodLight::getBrightness() {

	return this->brightness;
	
}


int MoodLight::getBrightnessPercent() {

	return MoodLight::ratioToPercents(this->brightness);
	
}


float MoodLight::getSaturation() {

	return this->saturation;
	
}


int MoodLight::getSaturationPercent() {

	return MoodLight::ratioToPercents(this->saturation);
	
}


float MoodLight::getDeltaPhi() {

	return this->deltaPhi;
	
}


float MoodLight::getDeltaPhiDegrees() {

	return MoodLight::radiansToDegrees(this->deltaPhi);
	
}

/* Setters */

void MoodLight::setBrightness(float aBrightness) {

	if (aBrightness > 1) {
		aBrightness = 1;
	} else if (aBrightness < 0) {
		aBrightness = 0;
	}
	
	this->brightness = aBrightness;
	
	for (int i = 0; i < this->lampManager->getCount(); i++) {
	
		lamp* l = this->lampManager->getLamp(i);
		l -> V = aBrightness;
		
	}
	
}


void MoodLight::setBrightnessPercent(int percents) {

	this->setBrightness(MoodLight::percentsToRatio(percents));
	
}


void MoodLight::setSaturation(float aSaturation) {

	if (aSaturation > 1) {
		aSaturation = 1;
	} else if (aSaturation < 0) {
		aSaturation = 0;
	}

	this->saturation = aSaturation;
	
	for (int i = 0; i < this->lampManager->getCount(); i++) {
	
		lamp* l = this->lampManager->getLamp(i);
		l -> S = aSaturation;
		
	}

}


void MoodLight::setSaturationPercent(int percents) {

	this->setSaturation(MoodLight::percentsToRatio(percents));

}


void MoodLight::setDeltaPhi(float anAngle) {

	if (anAngle < 0) {
		anAngle = 0;
	} else if (anAngle > 2 * PI) {
		anAngle = anAngle - 2 * PI;
	}
	
	this->deltaPhi = anAngle;
	
}


void MoodLight::setDeltaPhiDegrees(float degrees) {

	this->setDeltaPhi(MoodLight::degreesToRadians(degrees));

}



