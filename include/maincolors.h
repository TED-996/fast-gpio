#ifndef _FAST_GPIO_MAIN_H_
#define _FAST_GPIO_MAIN_H_

#include <stdlib.h>
#include <cstdio>
#include <cstring>
#include <poll.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#include <iostream>
#include <fstream>

#include <fastgpio.h>
#include <fastpwmcolors.h>

#include <fastgpioomega.h>
#include <fastgpioomega2.h>

#define FASTGPIO_VERBOSITY_QUIET		(0)
#define FASTGPIO_VERBOSITY_NORMAL		(1)
#define FASTGPIO_VERBOSITY_ALL			(2)
#define FASTGPIO_VERBOSITY_JSON			(3)

#define FASTGPIO_DEFAULT_VERBOSITY		(FASTGPIO_VERBOSITY_NORMAL)
#define FASTGPIO_DEFAULT_DEBUG			(0)

#define FASTGPIO_VERBOSE	0
#define	FASTGPIO_DEBUG 		0

struct colorSetup { 
	int freq;
	int pinR;
	int pinG;
	int pinB;

	int redDuty;
	int greenDuty;
	int blueDuty;

	int cmdFd;
	

	colorSetup(){
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



#endif 	// _FAST_GPIO_MAIN_H_