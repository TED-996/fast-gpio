#include <maincolors.h>


int pwmRun(colorSetup* setup);
void usage(char* progname);

int main(int argc, char* argv[])
{
	colorSetup setup;

	// TODO parse arguments

	int status = pwmRun(&setup);

	return status;
}

int cleanupWhitespace(char* buffer, int size);
void processRequest(FastPwmColors& colors, char* request);

// function to run pwm commands
int pwmRun(colorSetup* setup)
{	
	FastPwmColors pwmObj(setup->freq, setup->pinR, setup->pinG, setup->pinB);

	pwmObj.SetVerbosity(0);
	pwmObj.SetDebugMode(0);
	pwmObj.SetupRgb(setup->redDuty, setup->greenDuty, setup->blueDuty);

	int cmdFd = setup->cmdFd;
	pollfd pfd;
	pfd.fd = cmdFd;
	pfd.events = POLLIN;
	pfd.revents = 0;

	char request[7];
	memset(request, 0, sizeof(request));
	int requestBytes = 0;

	double targetLoop = pwmObj.getPeriod();

	while(true){
		pwmObj.Run(targetLoop);

		int pollResult = poll(&pfd, 1, 0);

		if (pollResult == 0){
			continue;
		}
		if (pollResult == -1){
			fprintf(stderr, "Poll returned error: errno %d\n", errno);
			return EXIT_FAILURE; 
		}

		// All OK, new command
		int readResult = read(cmdFd, request + requestBytes, 6 - requestBytes);

		if (readResult == -1) {
			fprintf(stderr, "Read returned error: errno %d\n", errno);
			return EXIT_FAILURE;
		}
		if (readResult == 0){
			// Finished.
			break;
		}

		requestBytes = cleanupWhitespace(request, readResult + requestBytes);

		if (requestBytes == 6){
			// New request!
			processRequest(pwmObj, request);
			
			memset(request, 0, 6);
			requestBytes = 0;
		}
	}

	return EXIT_SUCCESS;
}

inline bool isWhitespace(char ch){
	return ch == ' ' || ch == '\r' || ch == '\n' || ch == '\t';
}

int cleanupWhitespace(char* buffer, int size){
	int offset = 0;
	char* end = buffer + size;
	for (char* ptr = buffer; ptr != end; ptr++){
		if (isWhitespace(*ptr)){
			offset++;
		}
		else if (offset != 0){
			*(ptr - offset) = *ptr;
		}
		
	}
	if (offset != 0){
		*(buffer + size - offset) = '\0';
	}

	return size - offset;
}

inline int fromHex(char* ch){
	int d1 = ch[0] - '0';
	if (d1 > 9){
		d1 -= ('A' - '0') - 10;
	}
	if (d1 > 15){
		d1 -= ('a' - 'A');
	}
	int d2 = ch[1] - '0';
	if (d2 > 9){
		d2 -= ('A' - '0') - 10;
	}
	if (d2 > 15){
		d2 -= ('a' - 'A');
	}

	// No sanity assertions for performance.
	return d1 * 16 + d2;
}

void processRequest(FastPwmColors& colors, char* request){
	int r = fromHex(request);
	int g = fromHex(request + 2);
	int b = fromHex(request + 4);

	colors.SetupRgb(r * 1024 / 255, g * 1024 / 255, b * 1024 / 255);
}