

#include <wiringPi.h>
int main (void)
{
wiringPiSetup ()  ;
pinMode (0, OUTPUT);
pinMode (1, OUTPUT);
pinMode (2, OUTPUT);
for ( ; ; )
{
digitalWrite (0, HIGH);
digitalWrite (1, HIGH);
digitalWrite (2, HIGH);
delay (2000);
digitalWrite (0, LOW);
digitalWrite (1, LOW);
digitalWrite (2, LOW); 
delay (2000);
}
return 0;
}
