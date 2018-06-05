#ifndef _FAST_PWM_H_
#define _FAST_PWM_H_

#include <fastgpio.h>
#include <fastgpioomega2.h>
#include <module.h>
#include <unistd.h>
#include <utility>

#define DEFAULT_FREQ 200
#define DEFAULT_DUTY_CYCLE 50
#define US_PER_SEC 1000000

class FastPwmColors : public Module {
 public:
  FastPwmColors(void);
  FastPwmColors(int freq, int pinR, int pinG, int pinB);
  ~FastPwmColors(void);

  void Reset(void);

  void SetupRgb(int redDuty, int greenDuty, int blueDuty);
  void Run(double timeMs);

  double getPeriod() { return period; }

 private:
  // private functions
  void _SetupFreq(int frequency);
  void _SetupDuties(int pin1,
                    int duty1,
                    int pin2,
                    int duty2,
                    int pin3,
                    int duty3);
  void _Sleep(double length);

  void _PwmCycle(double lengthMultiplier);
  void _PwmCycleFull();

  FastGpioOmega2 gpio;

  int pinR;
  int pinG;
  int pinB;

  int pin1;
  int pin2;
  int pin3;
  uint32_t hi1;
  uint32_t hi2;
  uint32_t hi3;
  uint32_t low;

  int freq;
  uint32_t period;
};

#endif  // _FAST_PWM_H_