#include "devices.h"
#include "pros/rtos.h"
bool primeArm = false;
void grab(){
	Clamper.set_value(HIGH);
}
void release(){
	Clamper.set_value(LOW);
}
void actions(void* param){
	double setError = 0;
	double armSpeed = 0;
	while(true){
		if(primeArm){
			arm.move(armSpeed);
			delay(50);
			setError = 35 - rotation_sensor.get_position()/100.0 ; //change for macro
            armSpeed = armSetPid.update(setError);
			std :: clamp(int(armSpeed), -600, 600);
			delay(20);
			if(fabs(setError) < 1){ //when target reach turn off pid
				primeArm = false;
			}
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