#include "devices.h"
void grab(){
	Clamper.set_value(HIGH);
}
void release(){
	Clamper.set_value(LOW);
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