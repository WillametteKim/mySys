//ledtest1.c
#include <stdio.h>
#include <wiringPi.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#define PIN 1

void INT_handler(int);
void init();

int main()
{
	printf("LED TEST\n");

	wiringPiSetup();
	signal(SIGINT, INT_handler);
	pinMode(PIN, OUTPUT);

	while(1)
	{
		digitalWrite(PIN, HIGH);
		usleep(1000*500);
		digitalWrite(PIN, LOW);
		usleep(1000*500);
	}
	return 0;
}

void INT_handler (int sig)
{
	printf("\nINT occured\n");
	init();
	exit(0);
}

void init()
{
	digitalWrite(PIN, LOW);
}