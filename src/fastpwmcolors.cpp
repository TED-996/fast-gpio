#include <fastpwmcolors.h>

FastPwmColors::FastPwmColors() {
  Reset();

  SetupRgb(0, 0, 0);

  gpio.SetDirection(PIN_R, 1);
  gpio.SetDirection(PIN_G, 1);
  gpio.SetDirection(PIN_B, 1);
}

void FastPwmColors::Reset(void) {
  // nothing for now
}

inline void FastPwmColors::SetupRgb(int redDuty, int greenDuty, int blueDuty) {
  _SetupDuties(redDuty, greenDuty, blueDuty);
}

void FastPwmColors::_SetupDuties(int dutyR, int dutyG, int dutyB) {
  // Fast sort
  int duty1 = dutyR;
  int duty2 = dutyG;
  int duty3 = dutyB;

  if (duty2 < duty1) {
    std::swap(duty1, duty2);
  }
  if (duty3 < duty1) {
    std::swap(duty1, duty3);
  }
  if (duty3 < duty2) {
    std::swap(duty2, duty3);
  }

  int duties[4] = { 0, duty1, duty2, duty3 };

  cycle[0][0] = (dutyR != 0);
  cycle[0][1] = (dutyG != 0);
  cycle[0][2] = (dutyB != 0);

  // pls compiler unroll this thanks
  for (int line = 1; line <= 2; line++){
    int duty = duties[line];
    cycle[line - 1][3] = PERIOD_US * (duty - duties[line - 1]) / 256;
    
    if (duty == dutyR){
      cycle[line][0] = 0;
    }
    else{
      cycle[line][0] = cycle[line - 1][0];
    }
    if (duty == dutyG){
      cycle[line][1] = 0;
    }
    else{
      cycle[line][1] = cycle[line - 1][1];
    }
    if (duty == dutyB){
      cycle[line][2] = 0;
    }
    else{
      cycle[line][2] = cycle[line - 1][2];
    }
  }

  // correct for last duty == 255, which should not get a 0 in cycle matrix
  // it would get a 0 otherwise.
  // no need to update before if there are more with 255
  // because cycle[2][3] would be 0 which would ignore the line
  cycle[3][0] = (dutyR == 255);
  cycle[3][1] = (dutyG == 255);
  cycle[3][2] = (dutyB == 255);

  // set second-to-last time
  cycle[2][3] = PERIOD_US * (duties[3] - duties[2]) / 256;
  // set last time
  cycle[3][3] = PERIOD_US * (256 - duties[3]) / 256;
}

void FastPwmColors::Run(uint32_t timeUs) {
  while (timeUs > PERIOD_US) {
    _PwmCycleFull();
    timeUs -= PERIOD_US;
  }
  if (timeUs > 0) {
    _PwmCycle(timeUs / (double)PERIOD_US);
  }
}

void FastPwmColors::_PwmCycle(double lengthMultiplier) {
  for (int l = 0; l < 4; l++){
    uint32_t* line = cycle[l];
    if (cycle[l][3] == 0){
      // no sleep so ignore line
      continue;
    }

    gpio.Set(PIN_R, line[0]);
    gpio.Set(PIN_G, line[1]);
    gpio.Set(PIN_B, line[2]);
    
    _Sleep(uint32_t(line[3] * lengthMultiplier));
  }
}

void FastPwmColors::_PwmCycleFull() {
  for (int l = 0; l < 4; l++){
    uint32_t* line = cycle[l];
    if (cycle[l][3] == 0){
      // no sleep so ignore line
      continue;
    }

    gpio.Set(PIN_R, line[0]);
    gpio.Set(PIN_G, line[1]);
    gpio.Set(PIN_B, line[2]);
    
    _Sleep(line[3]);
  }
}

inline void FastPwmColors::_Sleep(uint32_t lengthUs) {
  if (lengthUs == 0) {
    return;
  }

  usleep(lengthUs);
}
