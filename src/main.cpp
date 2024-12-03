#include "lemlib/api.hpp" // IWYU pragma: keep
#include "main.h"
#include "actions.h"
#include "devices.h"
#include "auton.h"
#include "lemlib/chassis/chassis.hpp"
#include "pros/abstract_motor.hpp"
#include "pros/adi.h"
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/motors.hpp"
#include "pros/rtos.hpp"
#include <cstddef>
#include <string>
using namespace pros;
using namespace lemlib;


void initialize() {
	lcd::initialize();
	chassis.calibrate();

	lcd::set_text(1, "Press center button to select autonomous");
	lcd::register_btn1_cb(autonSelector);
	armMacro = pros::Task (macro, nullptr, "ARM_MACRO");
	armMacro.suspend();

	clamp.set_value(LOW);
	mArm.set_brake_mode(E_MOTOR_BRAKE_HOLD);
	mArm.set_current_limit(2500);
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
void autonomous() {
	controller.rumble("-");
	mLefts.tare_position();
	mRights.tare_position();
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
	case 5: 
		redRush();
		break;
	case 6: 
		blueRush();
		break;
	}
	
}

void opcontrol() {
	bool pressed = false; //initializes as false so pneumatics don't start open
	mIntake.set_brake_mode(MotorBrake::coast);
	mArm.set_brake_mode(MotorBrake::hold);
	bool run = false;
	
	

	while (true) {
		//controller.set_text(0, 0, std::to_string(mArm.get_position()));
		controller.set_text(0, 0, std :: to_string(mIntake.get_actual_velocity()));
		lcd::print(0, "%d %d %d", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);  // Prints status of the emulated screen LCDs

		// Tank control scheme
		int leftVal = controller.get_analog(ANALOG_LEFT_Y);
		int rightVal = controller.get_analog(ANALOG_RIGHT_Y);
		chassis.tank(leftVal, rightVal);                     // Passes joystick values into tank drive
		delay(20);                               // Run for 20 ms then update

//variable pressed ensures we can use the same button on controller  
//to toggle pistons either way: true is open, false is closed
		if(controller.get_digital_new_press(E_CONTROLLER_DIGITAL_B)){
			pressed = !pressed;
			if(pressed){											  						  
				clamp.set_value(HIGH);							  
			}	
			else if(!pressed){
				clamp.set_value(LOW);
			}
		}
		if(controller.get_digital_new_press(E_CONTROLLER_DIGITAL_DOWN)){//armMacro
			//run = !run;
			armMacro.resume();
			
		}
		else{
			if(controller.get_digital(E_CONTROLLER_DIGITAL_R1)){
			mIntake.move(-127);
		}
			else if(controller.get_digital(E_CONTROLLER_DIGITAL_R2)){
			mIntake.move(127);
		}
			else{
			mIntake.brake();
		}
		}
		

		if(controller.get_digital(E_CONTROLLER_DIGITAL_L1)){
			mIntake.move_relative(90, 300);
			delay(100);
			mArm.move_absolute(-1600, 1000);
		}
		else if(controller.get_digital(E_CONTROLLER_DIGITAL_L2)){
			mArm.move(100);
		}
		else{
			mArm.brake();
		}
		/*
		if (run){
			if(mArm.get_position() < -325){
				mArm.move(100);
			}
			else if(mArm.get_position() > -318){
				mArm.move(-100);
			}
		}
		
		if(mArm.get_position() < -315 && mArm.get_position() > -325){
			run = false;
		}
		if(mArm.get_position() < -1365 && mArm.get_position() > -325){
			run = false;
		}
		*/
	}
}