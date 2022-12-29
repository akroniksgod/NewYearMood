

#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

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

int main (void)
{
	wiringPiSetup();
	pinMode (0, OUTPUT);
	pinMode (1, OUTPUT);
	pinMode (2, OUTPUT);
	
	digitalWrite (0, HIGH);
	digitalWrite (1, HIGH);
	digitalWrite (2, HIGH);
	
	for ( ; ; )
	{
		SetRandomColor();
	}
	return 0;
}
