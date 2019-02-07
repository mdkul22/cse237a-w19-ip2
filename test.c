#include <wiringPi.h>
#include <stdio.h>

#define LEDPin 0
#define KNOCKPin 7
int main(void)
{
int val;
if(wiringPiSetup() == -1)
{
printf("setup wiringPi failed!");
return 1;
}
pinMode(KNOCKPin, INPUT);
pinMode(LEDPin, OUTPUT);

while(1){
val = digitalRead(KNOCKPin);
if(val == 1){
digitalWrite(LEDPin, 1);
delay(500);
}
else{
digitalWrite(LEDPin, 0);
delay(500);
}
}
return 0;
}
