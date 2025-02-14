#include "devices.h"
#include "pros/llemu.hpp"
double armTargets[] = {5, 47,165,  210};
double target = 0;
extern bool loadLB;
bool blueAllaince= false;
bool primed = false;
// 0 is lower, 1 is prime, 2 is neutral, 3 is allaince
void grab(){
	Clamper.set_value(HIGH);
}
void release(){
	Clamper.set_value(LOW);
}
void moveArm(void* param){
	double setError = 0;
	double armSpeed = 0;
	while(true){
		lcd::print(1, "currPos: %i, target: %f", rotation_sensor.get_position(), target);
		lcd::print(2, "error: %.4f, speed: %f",setError, armSpeed);
		setError = target - rotation_sensor.get_position()/100.0; 
		if(fabs(setError) > .5){
			
			armSpeed = armSetPid.update(setError);
			armSpeed = std :: clamp(int(armSpeed), -127, 127);
			mArm.move(armSpeed);
		}
		else{ //resets boolean values to prepare for next action
			mArm.brake(); 
			
		}
		if(fabs(armTargets[1]- (rotation_sensor.get_position()/100.0)) < .5){
			primed = true;
		}
		else{
			primed = false;
		}
		delay(20);
	}
}

void intakeColor(void * param){ //to implement for driver it is just while the butotn is being held
	while(1){
		lcd::print(3, "Hue: %f, Dist: %i", sOpt.get_hue(), sDist.get_distance());
		sOpt.set_led_pwm(50);
		mIntake.move(127);
		if(blueAllaince){																//if the alliance is blue
			if(sOpt.get_hue() > 0 && sOpt.get_hue() < 60){ //if the ring is red
				if(sDist.get_distance() > 50){							//if ring is at top of intake
					mIntake.brake();													//fling ring
					delay(50);				
				}
			}
		}
		if(!blueAllaince){															//if the alliance is red
			if(sOpt.get_hue() > 240 && sOpt.get_hue() < 360){ //if the ring is blue
				if(sDist.get_distance() > 50){							//if ring is at top of intake
					mIntake.brake();													//fling ring
					delay(50);				
				}
			}
		}
		//above handels color Sorting, below handles loading ring in ladyBrown
		if(loadLB){ //when true primes the lb and loads the ring into it
			target = armTargets[1]; //set the target to prime
			if(primed){ //if the arm is primed
				if(sDist.get_distance() > 50 && sDist.get_distance() < 70){
					mIntake.move(0); //stop the arm when ring is in lady brown
					loadLB = false; //reset loadLB
				}
				else{
					mIntake.move(127); //move the arm until ring is in lady brown
				}
			}
		}


		delay(20);
	}
	
}


