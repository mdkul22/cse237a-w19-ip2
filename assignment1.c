#include "assignment1.h"
#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <stdint.h>

void init_shared_variable(SharedVariable* sv) {
	// You can initialize the shared variable if needed.
	sv->bProgramExit = 0;
	sv->small_mic = 0;
	sv->big_mic = 0;
	sv->touch = 0;
	sv->pause = 0;
}

void init_sensors(SharedVariable* sv) {
	pinMode(PIN_BUTTON, INPUT);
	pinMode(PIN_BIG, INPUT);
	pinMode(PIN_SMALL, INPUT);
	// using metal touch sensor which acts the same way
	pinMode(PIN_TOUCH, INPUT);
	softPwmCreate(PIN_DIP_BLU, 0, 0xFF);
	softPwmCreate(PIN_DIP_RED, 0, 0xFF);
	softPwmCreate(PIN_DIP_GRN, 0, 0xFF);
	softPwmCreate(PIN_SMD_BLU, 0, 0xFF);
	softPwmCreate(PIN_SMD_RED, 0, 0xFF);
	softPwmCreate(PIN_SMD_GRN, 0, 0xFF);
	pinMode(PIN_ALED, OUTPUT);
	pinMode(PIN_BUZZER, OUTPUT);
}

void body_button(SharedVariable* sv) {
	int value = digitalRead(PIN_BUTTON);
	if(value==LOW){
	sv->pause = (sv->pause+1)%2;
	printf("body button toggled\n");
	delay(1000);
	}
}

void body_threecolor(SharedVariable* sv) {
	if(sv->pause){
		softPwmWrite(PIN_DIP_BLU, 0);
		softPwmWrite(PIN_DIP_RED, 0);
		softPwmWrite(PIN_DIP_GRN, 0);
	}
	else{
		if(sv->small_mic){
		  softPwmWrite(PIN_DIP_BLU, 0x00);
		  softPwmWrite(PIN_DIP_RED, 0xFF);
		  softPwmWrite(PIN_DIP_GRN, 0x00);
		}
		else{
		  softPwmWrite(PIN_DIP_BLU, 0xFF);
		  softPwmWrite(PIN_DIP_RED, 0x00);
		  softPwmWrite(PIN_DIP_GRN, 0x00);
		}
	}
}

void body_big(SharedVariable* sv) {
	int val = digitalRead(PIN_BIG);
	if(val){
		sv->big_mic = 1;
	}
	else{
		sv->big_mic = 0;
	}
}

void body_small(SharedVariable* sv) {
	int val = digitalRead(PIN_SMALL);
	if(val){
		sv->small_mic = 1;
		printf("Small mic active\n");
	}
	else{
		sv->small_mic = 0;
	}
}

void body_touch(SharedVariable* sv) {
	int val = digitalRead(PIN_TOUCH);
	if(val){
		sv->touch = 1;
	}
	else{
		sv->touch = 0;
	}
}

void body_rgbcolor(SharedVariable* sv) {
	if(sv->pause){
		softPwmWrite(PIN_SMD_BLU, 0);
		softPwmWrite(PIN_SMD_RED, 0);
		softPwmWrite(PIN_SMD_GRN, 0);
	}
	else{
		if(sv->small_mic==0&&sv->touch==0){
			softPwmWrite(PIN_SMD_BLU, 0x00);
			softPwmWrite(PIN_SMD_RED, 0xFF);
			softPwmWrite(PIN_SMD_GRN, 0x00);
		}
		else if(sv->small_mic==1&&sv->touch==0){
			softPwmWrite(PIN_SMD_BLU, 0xC8);
			softPwmWrite(PIN_SMD_RED, 0xEE);
			softPwmWrite(PIN_SMD_GRN, 0x00);
		}
		else if(sv->small_mic==0&&sv->touch==1){
			softPwmWrite(PIN_SMD_BLU, 0x00);
			softPwmWrite(PIN_SMD_RED, 0x80);
			softPwmWrite(PIN_SMD_GRN, 0xFF);
		}
		else{
			softPwmWrite(PIN_SMD_BLU, 0xFF);
			softPwmWrite(PIN_SMD_RED, 0x00);
			softPwmWrite(PIN_SMD_GRN, 0xFF);
		}
	}
}

void body_aled(SharedVariable* sv) {
	if(sv->pause)
	{	
		printf("LED OFF\n");
		digitalWrite(PIN_ALED, LOW);
	}
	else{
		digitalWrite(PIN_ALED, HIGH);
	}
}

void body_buzzer(SharedVariable* sv){
	if(sv->big_mic){
		for(int i =0; i < 280; i++)
		{
			digitalWrite(PIN_BUZZER, HIGH);
			delay(1);
			digitalWrite(PIN_BUZZER, LOW);
			delay(1);
		}
	}
	else{
		digitalWrite(PIN_BUZZER, LOW);
	}
}
