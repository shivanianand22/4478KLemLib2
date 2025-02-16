#include "devices.h"
#include "pros/abstract_motor.hpp"
#include "pros/abstract_motor.hpp"
#include "pros/llemu.hpp"
double armTargets[] = {5, 45,165,  228};
double target = 0;
bool loadLB= false;
bool blueAllaince= false;
bool primed = false;
bool loaded = false;
// 0 is lower, 1 is prime, 2 is neutral, 3 is allaince

// 0 is lower, 1 is prime, 2 is neutral, 3 is allaince

// 0 is lower, 1 is prime, 2 is neutral, 3 is allaince

void grab(){
	Clamper.set_value(HIGH); //actiavtes piston to clamp
}
void release(){
	Clamper.set_value(LOW);
}
void moveArm(void* param){
	rotation_sensor.reverse();
	rotation_sensor.reverse();
	double setError = 0;
	double armSpeed = 0;
	while(true){
	//	lcd::print(1, "currPos: %i, target: %f", rotation_sensor.get_position(), target);
		//lcd::print(2, "error: %.4f, speed: %f",setError, armSpeed);
	//	lcd::print(1, "currPos: %i, target: %f", rotation_sensor.get_position(), target);
		//lcd::print(2, "error: %.4f, speed: %f",setError, armSpeed);
		setError = target - rotation_sensor.get_position()/100.0; 
		if(fabs(setError) > .5){
			
			
			armSpeed = armSetPid.update(setError);
			armSpeed = std :: clamp(int(armSpeed), -127, 127);
			mArm.move(armSpeed);
		}
		else{ //resets boolean values to prepare for next action
			mArm.brake(); 
			
		}
		if(fabs(armTargets[1]- (rotation_sensor.get_position()/100.0)) < 6){
			primed = true;
		}
		else{
			primed = false;
		}
		//above handels arm movement, below handles loading ring in ladyBrown
		if(loadLB){ //when true primes the lb and loads the ring into it
			//lcd::print(3, "Dist: %i, prime: %i", sDist.get_distance(), primed);
			target = armTargets[1]; //set the target to prime
			lcd::print(3, "Dist: %i, Prime: %i, load: %i", sDist.get_distance(), primed, loadLB);
			if(primed){ //if the arm is primed
				if(sDist.get_distance() < 120){
					delay(500);
					mIntake.move(0); //stop the arm when ring is in lady brown
					mIntake.set_brake_mode(MotorBrake::coast);
					mIntake.brake();
					loadLB = false; //reset loadLB
					controller.rumble(".");

				}
				else{
					mIntake.move(127); //move the arm until ring is in lady brown
					
				}
			}
		}
			
		}
		if(fabs(armTargets[1]- (rotation_sensor.get_position()/100.0)) < 6){
			primed = true;
		}
		else{
			primed = false;
		}
		//above handels arm movement, below handles loading ring in ladyBrown
		if(loadLB){ //when true primes the lb and loads the ring into it
			//lcd::print(3, "Dist: %i, prime: %i", sDist.get_distance(), primed);
			target = armTargets[1]; //set the target to prime
			lcd::print(3, "Dist: %i, Prime: %i, load: %i", sDist.get_distance(), primed, loadLB);
			if(primed){ //if the arm is primed
				if(sDist.get_distance() < 120){
					delay(500);
					mIntake.move(0); //stop the arm when ring is in lady brown
					mIntake.set_brake_mode(MotorBrake::coast);
					mIntake.brake();
					loadLB = false; //reset loadLB
					controller.rumble(".");

				}
				else{
					mIntake.move(127); //move the arm until ring is in lady brown
					
				}
			}
		}
		delay(20);


	}


void intakeColor(void* param){ //to implement for driver it is just while the butotn is being held
	rotation_sensor.reset_position();
	rotation_sensor.set_position(0);
	rotation_sensor.reverse();
	while(1){
		sOpt.set_led_pwm(50);
		if(blueAllaince){																//if the alliance is blue
			if(sOpt.get_hue() > 0 && sOpt.get_hue() < 50){ //if the ring is red
										//if ring is at top of intake
					mIntake.brake();													//fling ring
					delay(500);
					mIntake.move(127);		
					controller.rumble(".");		
				
			}
		}
		if(!blueAllaince){															//if the alliance is red
			if(sOpt.get_hue() > 75 && sOpt.get_hue() < 360){ //if the ring is blue
					delay(60);
					mIntake.brake();													//fling ring
					delay(100);
					mIntake.move(127);
					controller.rumble(".");				
				
			}
		}
		
		

		delay(20);
	}
	
}