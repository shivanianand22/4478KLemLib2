#include "devices.h"
#include "pros/llemu.hpp"
bool primeArm = false;
bool neutralArm = false;
bool allainceArm = false;
bool lowerArm = false;
void grab(){
	Clamper.set_value(HIGH);
}
void release(){
	Clamper.set_value(LOW);
}
void moveArm(void* param){
	double setError = 0;
	double armSpeed = 0;
	double target = 0;
	while(true){
		
		if(primeArm){target = 35;}// sets target value depending on which button clicked
		if(allainceArm){}
		if(neutralArm){}
		if(lowerArm){target = 5;}
		setError = target - rotation_sensor.get_position()/100.0; 
		if(fabs(setError) > .5){
			lcd::print(1, "currPos: %i, target: %f", rotation_sensor.get_position(), target);
			lcd::print(2, "error: %.4f, speed: %f",setError, armSpeed);
			armSpeed = armSetPid.update(setError);
			armSpeed = std :: clamp(int(armSpeed), -127, 127);
			mArm.move(armSpeed);
		}
		else{ //resets boolean values to prepare for next action
			mArm.brake(); 
			primeArm = false;
			lowerArm = false;
			allainceArm = false;
			neutralArm = false;
		}

		delay(20);
	}
}
void intakeDist(){
	int i = 2;
	mIntake.move(127);
	while(i > 0){
		if(sDist.get_distance()*10 < 5 && sDist.get_object_velocity() > 0){ //5 is an estimate in cm. don't know where dist sensor will be
			mIntake.brake();
			i--;
		}
	}
}

void intakeColorRed(){
	if(sOpt.get_hue() == 0){
		intakeDist();
	}
	else{
		mIntake.brake();
	}
	
}
void intakeInArm(){
	mIntake.move(-127);
	delay(2000);
	while(mIntake.get_actual_velocity() > 400){}
	mIntake.brake();

}
void intakeColorBlue(){
	if(sOpt.get_hue() == 240){
		intakeDist();
	}
	else{
		mIntake.brake();
	}
}