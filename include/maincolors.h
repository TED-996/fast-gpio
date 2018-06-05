#ifndef _FAST_GPIO_MAIN_H_
#define _FAST_GPIO_MAIN_H_

#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>

#include <fstream>
#include <iostream>

#include <fastgpio.h>
#include <fastpwmcolors.h>

#include <fastgpioomega2.h>

struct colorSetup {
  int freq;
  int pinR;
  int pinG;
  int pinB;

  int redDuty;
  int greenDuty;
  int blueDuty;

  int cmdFd;

  colorSetup() {
    freq = 200;
    pinR = 3;
    pinG = 2;
    pinB = 11;

    redDuty = 0;
    greenDuty = 0;
    blueDuty = 0;

    // Set cmdFd to stdin
    cmdFd = 0;
  }
};

#endif  // _FAST_GPIO_MAIN_H_