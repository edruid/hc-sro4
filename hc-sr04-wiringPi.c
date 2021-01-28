#include <wiringPi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <syslog.h>
#include <sys/sysinfo.h>
#include <sys/types.h>
#include <sys/stat.h>

static const int _pin_trig = 1;
static const int _pin_echo = 4;

static void do_init(void);

static void quit_handler(int sig)
{
    signal(sig, SIG_IGN);
    exit(0);
}

static void do_init(void)
{
    if (wiringPiSetup() == -1) {
        printf("wiringPi-Error\n");
        exit(1);
    }

    signal (SIGTERM, quit_handler);
    signal (SIGHUP, quit_handler);

    piHiPri(20);

    pinMode(_pin_echo, INPUT);
    pinMode(_pin_trig, OUTPUT);
}

static int wait_change(int mode, unsigned int tmo)
{
    int v1, v2, v3;
    unsigned int now = micros();

    do {
       /* Primitive low-pass filter */
       v1 = digitalRead(_pin_echo);
       v2 = digitalRead(_pin_echo);
       v3 = digitalRead(_pin_echo);
       if (v1 == v2 && v2 == v3 && v3 == mode) {
           return (micros() - now);
       }
    } while ((micros() - now) < tmo);
    return -1;
}

static int sense_distance()
{
    int micros;
    digitalWrite(_pin_trig, HIGH);

    delayMicroseconds(10);
    digitalWrite(_pin_trig, LOW);

    micros = wait_change(HIGH, 10000);
    micros = wait_change(LOW, 10000);
    return micros * 0.1715;
}

int main(int argc, char *argv[])
{
    int ret;
    do_init();

    ret = sense_distance();
    printf("%i\n", ret);

    return 0;
}
