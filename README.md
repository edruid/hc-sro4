# hc-sro4

## WiringPi

This was my first attempt, it seems that WiringPi is no longer maintained
though and the version I found didn't support rpi 1 B+. It worked with 
rpi 1 B though.

```
gcc -o hc-sr04-wiringPi hc-sr04-wiringPi.c -I. -I/include -I../ -lwiringPi -lcrypt -pthread -lm -lrt
./hc-sr04-wiringPi
```

## python

My second attempt, it seems to be working fine although I am a bit sceptical
about timing snsitive stuff in python which might want to do garbage collection
in the middle...

```
./ultrasonic_distance.py
```

## pigpio

The third attempt. For some reason the first measurment almost always failed,
so I made a retry loop.

```
gcc -Wall -pthread -o hc-sr04-pigpio hc-sr04-pigpio.c -lpigpio -lrt
sudo ./hc-sr04-pigpio
```
