#include <time.h>
#include <stdio.h>
#include <pigpio.h>

static const int _pin_trig = 18;
static const int _pin_echo = 23;

static void _start(int gpio, int level, uint32_t tick, void* user)
{
	uint32_t *millis;
	millis = user;
	if (1 == level)
       	{
		(*millis) = tick;
	} else if (0 == level && *millis != 0)
	{
		*millis = tick - *millis;
	}
}

void sense(uint32_t *millis)
{
	struct timespec ts;
	ts.tv_sec = 0;
	ts.tv_nsec = 20000;

	gpioWrite(_pin_trig, 1);
	nanosleep(&ts, NULL);
	gpioSetAlertFuncEx(_pin_echo, _start, (void*)millis);
	gpioWrite(_pin_trig, 0);
}

int main(int argc, char *argv[])
{
	uint32_t millis = 0;
	time_t rawtime;
	struct tm * timeinfo;
	struct timespec ts;
	int t;
	char datestr[200];
	ts.tv_sec = 1;
	ts.tv_nsec = 0;
	if (gpioInitialise() < 0) return 1;
	gpioSetMode(_pin_trig, PI_OUTPUT);
	gpioSetMode(_pin_echo, PI_INPUT);
	
	for (t = 0; t<10; t++) {
		sense(&millis);
		nanosleep(&ts, NULL);
		gpioSetAlertFunc(_pin_echo, NULL);
		if (millis > 0) break;
	}

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(datestr, sizeof(datestr), "%Y-%m-%d %T", timeinfo);
	printf("%s %i mm\n", datestr, (int)(millis*0.1715));

	return 0;
}
