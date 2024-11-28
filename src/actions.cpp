#include "actions.h"
#include "devices.h"
//#include <coroutine>
#include <string>
pros::Task armMacro(macro, nullptr, "ARM_MACRO");
void macro(void* param){
	while(1){
	while(mArm.get_position() > -318 || mArm.get_position() < -322){
		if(mArm.get_position() < -322){
			mArm.move(100);
	}
		else{
			mArm.move(-100);
		}
	delay(20);
	}
	mArm.brake();
	armMacro.suspend();
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
	
void intakeColor(std::string color){ //type the color in all lowercase
    if(color.compare("red") ==0){
        if(sOpt.get_hue() == 0){
		    intakeDist();
	    }   
        else{
		    mIntake.brake();
	    }
    }
    else{
        if(sOpt.get_hue() == 240){
		    intakeDist();
	    }
        else{
		    mIntake.brake();
	    }
    }
}

void grab(){
	clamp.set_value(HIGH);
}
void release(){
	clamp.set_value(LOW);
}