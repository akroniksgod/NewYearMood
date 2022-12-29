

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

void TerminateAll()
{
	digitalWrite (0, HIGH);
	digitalWrite (1, HIGH);
	digitalWrite (2, HIGH);
}

void SetRandomColor()
{
	int mudel = rand() % 300;
	int seklect1 = rand() % 3;
	int seklect2 = rand() % 3;
	int seklect3 = rand() % 3;
	
	digitalWrite (0, HIGH);
	digitalWrite (1, HIGH);
	digitalWrite (2, HIGH); 
	
	digitalWrite(seklect1, LOW);
	digitalWrite(seklect2, LOW);
	digitalWrite(seklect3, LOW);
	
	delay(mudel);
}

void OnlyRed()
{
	TerminateAll();
	digitalWrite(0, LOW);
}

void OnlyBlue()
{
	TerminateAll();
	digitalWrite(1, LOW);
}

void OnlyGreen()
{
	TerminateAll();
	digitalWrite(2, LOW);
}

int main (void)
{
	wiringPiSetup();
	pinMode (0, OUTPUT);
	pinMode (1, OUTPUT);
	pinMode (2, OUTPUT);
	
	TerminateAll();
	
	OnlyRed();
	delay(1000);
	OnlyBlue();
	delay(1000);
	OnlyGreen();
	delay(1000);
	
	for ( ; ; )
	{
		SetRandomColor();
	}
	return 0;
}
