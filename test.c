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
if(val == 0){
digitalWrite(LEDPin, HIGH);
delay(1000);
printf("Entered Knock..\n");
}
else{
digitalWrite(LEDPin, LOW);
delay(1000);
printf("No Knock...\n");
}
}
return 0;
}
