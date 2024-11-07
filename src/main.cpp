#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "lemlib/chassis/chassis.hpp"
#include "pros/abstract_motor.hpp"
#include "pros/adi.h"
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include <string>
using namespace pros;
using namespace lemlib;

Controller controller(E_CONTROLLER_MASTER);
MotorGroup mLefts({-16, -20, -6}); // left motors forward (negitve)
MotorGroup mRights({18, 19, 8}); // right motors reverse (positve)
Motor mIntake(15, pros::MotorGearset::blue);
Motor mArm(13, pros::MotorGearset::green);
Imu imu(17);
Distance sDist(1);
Optical sOpt(2);
adi::Port clamp ('B', E_ADI_DIGITAL_OUT); //even different than PROS + EZTemplate or PROS + OkapiLib

// drivetrain settings
Drivetrain drivetrain(&mLefts, // left motor group
                              &mRights, // right motor group
                              11.25, // 11.25 inch track width
                              Omniwheel::NEW_275, // using new 2.75" omnis
                              450, // drivetrain rpm is 450
                              2 // horizontal drift is 2 (for now)
);

OdomSensors sensors(nullptr, // vertical tracking wheel 1, set to null
                            nullptr, // vertical tracking wheel 2, set to nullptr as we are using IMEs
                            nullptr, // horizontal tracking wheel 1
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu // inertial sensor
);


// lateral PID controller
ControllerSettings lateral_controller(10, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              3, // derivative gain (kD)
                                              0, // anti windup
                                              0, // small error range, in inches
                                              0, // small error range timeout, in milliseconds
                                              0, // large error range, in inches
                                              0, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);

// angular PID controller
ControllerSettings angular_controller(2, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              10, // derivative gain (kD)
                                              0, // anti windup
                                              0, // small error range, in degrees
                                              0, // small error range timeout, in milliseconds
                                              0, // large error range, in degrees
                                              0, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);

// input curve for throttle input during driver control
ExpoDriveCurve throttleCurve(3, // joystick deadband out of 127
                                     10, // minimum output where drivetrain will move out of 127
                                     1.019 // expo curve gain
);

// input curve for steer input during driver control
ExpoDriveCurve steerCurve(3, // joystick deadband out of 127
                                  10, // minimum output where drivetrain will move out of 127
                                  1.019 // expo curve gain
);

// create the chassis
Chassis chassis(drivetrain, // drivetrain settings
                        lateral_controller, // lateral PID settings
                        angular_controller, // angular PID settings
                        sensors, // odometry sensors
						&throttleCurve, 
						&steerCurve
);

int selection = 0;

void autonSelector(){
	if(selection <= 2){
		selection++;
	} else {
		selection = 0;
	}

	switch(selection){
	case 0:
		pros::lcd::set_text(2, "MOGO Side");
		break;
	case 1:
		pros::lcd::set_text(2, "Ring Side");
		break;
	case 2:
		pros::lcd::set_text(2, "Prog Skills");
		break;
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
 void macro(){
	while(mArm.get_position() > -315 || mArm.get_position() < -325){
		if(mArm.get_position() < -325){
			mArm.move(100);
	}
		else{
			mArm.move(-100);
		}
	}
}
void setArm() {
	if(mArm.get_position() < -325){
			mArm.move(100);
		}
	else if(mArm.get_position() > -315){
			mArm.move(-100);
		}
	
 }

void initialize() {
	lcd::initialize();
	chassis.calibrate();

	lcd::set_text(1, "Press center button to select autonomous");
	lcd::register_btn1_cb(autonSelector);

	clamp.set_value(LOW);
	mArm.set_brake_mode(E_MOTOR_BRAKE_HOLD);
	Task armMacro(setArm);
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

void grab(){
	clamp.set_value(HIGH);
}
void release(){
	clamp.set_value(LOW);
}


//FRONT OF THE ROBOT IS THE INTAKE
void mogoSide(){
	//code with allaince stake
	//chassis.setPose(-52.638, -17, 180);
	chassis.setPose(-58.5, -16.5, 180);
	chassis.moveToPose(-63.5,  2.3, 90, 2000, {.forwards = false});
	delay(2000);
	mIntake.move(-120);
	delay(1400); //score allaince stake
	chassis.moveToPoint(-47.938, -15.951, 1000);
	chassis.moveToPose(-28.5, -23.999, -45, 2000, {.forwards = false}); //get to mogo
	delay(2500);
	grab();
	chassis.moveToPoint(-21, -60.914, 3000);

	//chassis.turnToHeading(90, 4000);
	//chassis.moveToPoint(-60,  -1, 1000, {.forwards = false});
	// chassis.setPose(-52.638, -17.826, 180);
	// chassis.moveToPose(-61, -0.5, -90, 2000);

	/* Rayhaan Code
	chassis.setPose(-53.638, -27.826, 270);
	chassis.moveToPose(-27.037, -25.926, 250, 9500);
	
	chassis.moveToPoint(-24,  26.325, 8000, {.forwards = true});
	// chassis.setPose(-53.638, -27.826, 270); test code
	// chassis.moveToPose(-27.037, -25.926, 250, 9500);
	*/

	/*shivani code
	chassis.moveToPoint(-23, -24.999, 2700, {.forwards = false}, false);
	grab();
	mIntake.move(-200);
	delay(2000);
	chassis.moveToPoint(-19.825, -45.5, 3000);
	delay(4000);
	chassis.moveToPoint(-23.712, -1.5, 4000);
	mIntake.brake();*/
}

void ringSide(){
	chassis.setPose(-58.625, 36.3, 90);
	chassis.moveToPoint(-24, 26.325, 6000, {.forwards = false}, false);
	grab();
}

void progSkills(){
	chassis.setPose(-53, 0, 90);
	mIntake.move(127);
	chassis.moveToPose(-40.375, 13.262, 45, 1000);
	chassis.moveToPose(-76.7, 66.2, 320, 2500); //score 1st goal
	chassis.moveToPoint(17.138, 37.25, 2000);
	chassis.moveToPoint(56.564, 18,  1300); //get behind 2nd goal
	chassis.moveToPoint((67.5), 65, 2500);
	chassis.moveToPoint(59, 57, 2500, {.forwards = false});
	chassis.moveToPoint(49, 4, 4000);
	chassis.moveToPose(88.864, -59.77, 85, 4000); //score 3rd goal
	chassis.moveToPoint(56, -47, 2500, {.forwards = false});
	chassis.moveToPose(-19.793, -30.913, 320, 4000);
	chassis.moveToPose(-43, -15, 200, 4000);
	chassis.moveToPoint(-64.088, -74.139,2000);//score 4th goal
	chassis.moveToPoint(-64, -53, 2500, {.forwards = false});
	chassis.moveToPoint(-22, -22, 1000);
	mIntake.brake();

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

void intakeColorBlue(){
	if(sOpt.get_hue() == 240){
		intakeDist();
	}
	else{
		mIntake.brake();
	}
}

void autonomous() {
	switch (selection) {
	case 0:
		mogoSide();
		break;
	case 1:
		ringSide();
		break;
	case 2:
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
	bool pressed = false; //initializes as false so pneumatics don't start open
	bool slow = false; 
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
		if(controller.get_digital_new_press(E_CONTROLLER_DIGITAL_DOWN)){
			run = !run;
			
		}
		if(!slow){
			if(controller.get_digital(E_CONTROLLER_DIGITAL_R1)){
			mIntake.move(-200);
		}
			else if(controller.get_digital(E_CONTROLLER_DIGITAL_R2)){
			mIntake.move(200);
		}
			else{
			mIntake.brake();
		}
		}
		else{
			if(controller.get_digital(E_CONTROLLER_DIGITAL_R1)){
			mIntake.move(-65);
		}
			else if(controller.get_digital(E_CONTROLLER_DIGITAL_R2)){
			mIntake.move(65);
		}
			else{
			mIntake.brake();
		}
		}
		

		if(controller.get_digital(E_CONTROLLER_DIGITAL_L1)){
			//mArm.move(-127);
			mArm.move_absolute(-1600, 1000);
		}
		else if(controller.get_digital(E_CONTROLLER_DIGITAL_L2)){
			mArm.move(127);
		}
		else{
			mArm.brake();
		}
		
		if (run){
			if(mArm.get_position() < -325){
		mArm.move(100);
	}
			else if(mArm.get_position() > -315){
		mArm.move(-100);
	}
		
		}
		if(mArm.get_position() < -315 && mArm.get_position() > -325){
			run = false;
		}
		if(mArm.get_position() < -1365 && mArm.get_position() > -325){
			run = false;
		}
	}
}