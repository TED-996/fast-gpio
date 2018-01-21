#include <fastpwmcolors.h>

FastPwmColors::FastPwmColors(int freq, int pinR, int pinG, int pinB)
{
	Reset();
	
	_SetupFreq(freq);

	this->pinR = pinR;
	this->pinG = pinG;
	this->pinB = pinB;

	SetupRgb(0, 0, 0);

	if (strcmp(DEVICE_TYPE, "ramips") == 0) {
		gpio = new FastGpioOmega2();
	} else {
		gpio = new FastGpioOmega();
	}

	gpio->SetDirection(pinR, 1);
	gpio->SetDirection(pinG, 1);
	gpio->SetDirection(pinB, 1);
}

FastPwmColors::~FastPwmColors(void)
{
	// nothing for now
}

void FastPwmColors::Reset(void)
{
	// nothing for now
}

void FastPwmColors::SetupRgb(int redDuty, int greenDuty, int blueDuty){
	_SetupDuties(pinR, redDuty, pinG, greenDuty, pinB, blueDuty);
}

void FastPwmColors::_SetupFreq(int frequency){
	this->freq 		= frequency;

	// find the period (in ms)
	this->period 		= (1.0f/freq) * 1000;
}


void FastPwmColors::_SetupDuties(int pin1, int duty1, int pin2, int duty2, int pin3, int duty3)
{
	// Fast sort
	if (duty2 < duty1){
		std::swap(pin1, pin2);
		std::swap(duty1, duty2);
	}
	if (duty3 < duty1){
		std::swap(pin1, pin3);
		std::swap(duty1, duty3);
	}
	if (duty3 < duty2){
		std::swap(pin2, pin3);
		std::swap(duty2, duty3);
	}
	


	this->pin1 = pin1;
	this->pin2 = pin2;
	this->pin3 = pin3;

	this->hi1 = this->period * duty1 / 1024;
	this->hi2 = this->period * (duty2 - duty1) / 1024;
	this->hi3 = this->period * (duty3 - duty2) / 1024;
	this->low = this->period * (1024 - duty3) / 1024;

	/*if (verbosityLevel > 0) {
		printf ( 	"PWMColor Setup: frequency = %d, duty1 = %d%%, duty2 = %d%%, duty2 = %d%%\n"
					"\n", 
					frequency, duty1, duty2, duty3
					TODO
				);
	}*/
}

double correctionMultiplier = 1.0;

void FastPwmColors::Run (double timeMs)
{
	// TODO: adjust the correctionMultiplier based on pre+post times
	while(timeMs > period){
		_PwmCycle(correctionMultiplier);
		timeMs -= period;
	}
	if (timeMs > 0.001){
		_PwmCycle(timeMs / period * correctionMultiplier);
	}
}

void FastPwmColors::_PwmCycle (double lengthMultiplier)
{
	// All HIGH part of cycle
	gpio->Set(pin1, 1);
	gpio->Set(pin2, 1);
	gpio->Set(pin3, 1);
	_Sleep(lengthMultiplier * hi1);

	// 2 + 3 HIGH part
	gpio->Set(pin1, 0);
	_Sleep(lengthMultiplier * hi2);
	
	// 3 HIGH part
	gpio->Set(pin2, 0);
	_Sleep(lengthMultiplier * hi3);

	// LOW part
	gpio->Set(pin3, 0);
	_Sleep(low);
}

inline void FastPwmColors::_Sleep (double length)
{
	if (length < 0.0001){
		return;
	}
	// sleep function uses microseconds
	int value	= (int)(length * 1000);	

	usleep(value);
}
