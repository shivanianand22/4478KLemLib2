#include "devices.h"
#include <string>
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