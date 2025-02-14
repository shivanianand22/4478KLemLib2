#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "devices.h"
#include "auton.h"
#include "actions.h"
#include "lemlib/chassis/chassis.hpp"
#include "pros/abstract_motor.hpp"
#include "pros/adi.h"
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include "drive.h"
#include <string>
using namespace pros;
using namespace lemlib;



/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */

void initialize() {
	lcd::initialize();
	chassis.calibrate();
    imu.reset();
    Rotation rotation_sensor(10);
	lcd::set_text(1, "Press center button to select autonomous");
	lcd::register_btn1_cb(autonSelector);
	mArm.set_gearing(pros::E_MOTOR_GEARSET_36);
	Clamper.set_value(LOW);
	mArm.set_brake_mode(MotorBrake::hold);
	Task armTask(moveArm, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT,"controls arm tasks" );
    rotation_sensor.reset_position();
    rotation_sensor.set_position(0);
    //rotation_sensor.reverse();

}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
	pros::lcd::register_btn1_cb(autonSelector);
    
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
ASSET(skills_txt);
ASSET(rightAwpp1_txt);




//FRONT OF THE ROBOT IS THE INTAKE



void autonomous() {
	controller.rumble("-");
	mLefts.tare_position();
	mRights.tare_position();
    release();
    
	switch (selection) {
	case 0:
		redRight();
		break;
	case 1:
		redLeft();
		break;
	case 2:
		blueLeft();
		break;
	case 3:
		blueRight();
		break;
	case 4:
		progSkills();
		break;
	}
	
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	mIntake.set_brake_mode(MotorBrake::coast);
	rotation_sensor.reset_position();
	rotation_sensor.set_position(0);
	rotation_sensor.reverse();

	while (true) {
		controller.set_text(0, 0, std :: to_string(mIntake.get_actual_velocity()));
		lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
									(pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
									(pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);  // Prints status of the emulated screen LCDs

	// Tank control scheme
	// Passes joystick values into tank drive
	delay(20);     // Run for 20 ms then update
	drive();

	if(controller.get_digital_new_press(E_CONTROLLER_DIGITAL_B)){toggleClamp();}
	if(controller.get_digital_new_press(E_CONTROLLER_DIGITAL_DOWN)){primeArm = true;}
	if(controller.get_digital(E_CONTROLLER_DIGITAL_L1)){neutralArm = true;}
	if(controller.get_digital(E_CONTROLLER_DIGITAL_L2)){lowerArm = true;}
	if(controller.get_digital(E_CONTROLLER_DIGITAL_UP)){allainceArm = true;}

	if(controller.get_digital(E_CONTROLLER_DIGITAL_R1)){    
		mIntake.move(127);//move intake forward
	} 
	else if(controller.get_digital(E_CONTROLLER_DIGITAL_R2)){   
		mIntake.move(-127);//move intake backward
	}
	else{   
		mIntake.brake();//stop intake
	}    

	lcd::print(1, "currPos: %i", rotation_sensor.get_position());
	
	

	
		
/*	
		if(controller.get_digital(E_CONTROLLER_DIGITAL_L1)){
			//mArm.move(-127);
            armSpeed = armScorePID.update(rotation_sensor.get_position()/100.0 - -810);
			mIntake.move(60);
            delay(10);
            std :: clamp(int(scoreError), -600 , 600);
            if(rotation_sensor.get_position() > -790 * 100){ //this line
			    mArm.move_velocity(-armSpeed);
			    mArm.move_velocity(-armSpeed);
            }
            
		}
		else if(controller.get_digital(E_CONTROLLER_DIGITAL_L2)){
			mArm.move(60);
		}
		else{
			mArm.brake();
		}
		
		if (run){ 
            //possible PID for arm
            setError = rotation_sensor.get_position()/100.0 - -119; //change for macro
            //possible PID for arm
            setError = rotation_sensor.get_position()/100.0 - -119; //change for macro
            armSpeed = armSetPid.update(setError);
			std :: clamp(int(armSpeed), -600, 600);
            if(fabs(setError) > 1){
				//mArm.move(80);
                mArm.move_velocity(-armSpeed);
                std :: cout << "Running";
				//mArm.move(-80);
                
			}
            
            
		}
		
		if(fabs(setError) < 1){ 
			run = false;
		}
		
        controller.set_text(0, 0, "Positon: %f", (rotation_sensor.get_position()));
        lcd::print(1, "value: %f", armSpeed);
        //controller.set_text(0,0,"%d",rotation_sensor.get_position() );*/
	}
}

