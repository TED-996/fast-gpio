#ifndef _FAST_PWM_H_
#define _FAST_PWM_H_

#include <module.h>
#include <fastgpio.h>
#include <fastgpioomega.h>
#include <fastgpioomega2.h>
#include <unistd.h>
#include <utility>


#define DEFAULT_FREQ				200
#define DEFAULT_DUTY_CYCLE			50


class FastPwmColors : public Module {
public:
	FastPwmColors(void);
	FastPwmColors(int freq, int pinR, int pinG, int pinB);
	~FastPwmColors(void);

	void 	Reset 			(void);

	void 	SetupRgb		(int redDuty, int greenDuty, int blueDuty);
	void 	Run				(double timeMs);

	double  getPeriod		() {
		return period;
	}

private:
	// private functions
	void 	_SetupFreq		(int frequency);
	void 	_SetupDuties	(int pin1, int duty1, int pin2, int duty2, int pin3, int duty3);
	void 	_Sleep 			(double length);

	void 	_PwmCycle		(double lengthMultiplier);

	FastGpio * gpio;

	int pinR;
	int pinG;
	int pinB;

	int pin1;
	int pin2;
	int pin3;
	double hi1;
	double hi2;
	double hi3;
	double low;

	int		freq;
	double period;
};


#endif 	// _FAST_PWM_H_