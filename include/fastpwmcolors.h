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
#define FREQ 200
#define INPUT_FREQ 100
#define PERIOD_US (US_PER_SEC / FREQ)
#define PIN_R 3
#define PIN_G 2
#define PIN_B 11

class FastPwmColors {
 public:
  FastPwmColors();
  void Reset(void);

  void SetupRgb(int redDuty, int greenDuty, int blueDuty);
  void Run(uint32_t timeUs);

 private:
  void _SetupDuties(int dutyR, int dutyG, int dutyB);
  void _Sleep(uint32_t lengthUs);

  void _PwmCycle(double lengthMultiplier);
  void _PwmCycleFull();

  FastGpioOmega2 gpio;

  uint32_t cycle[4][4];
};

#endif  // _FAST_PWM_H_