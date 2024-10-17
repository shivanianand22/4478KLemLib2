#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "pros/abstract_motor.hpp"
#include "pros/adi.h"
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/motors.hpp"
using namespace pros;
using namespace lemlib;

Controller controller(E_CONTROLLER_MASTER);
MotorGroup mLefts({-16, -20, -6}); // left motors forward (negitve)
MotorGroup mRights({18, 19, 8}); // right motors reverse (positve)
Motor mIntake(15);
Motor mArm(13);
Imu imu(17);
Distance sDist(1);
Optical sOpt(2);
adi::Port leftClamp ('A', E_ADI_DIGITAL_OUT); //basic ADI output port declaration. The syntax is unique in LemLib
adi::Port rightClamp ('B', E_ADI_DIGITAL_OUT); //even different than PROS + EZTemplate or PROS + OkapiLib

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
		pros::lcd::set_text(2, "Test auton");
		break;
	case 1:
		pros::lcd::set_text(2, "Blue RIGHT FullAWP");
		break;
	case 2:
		pros::lcd::set_text(2, "Red RIGHT FullAWP");
		break;
	case 3:
		pros::lcd::set_text(2, "Skills auton");
		break;
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	lcd::initialize();
	chassis.calibrate();

	lcd::set_text(1, "Press center button to select autonomous");
	lcd::register_btn1_cb(autonSelector);

	leftClamp.set_value(LOW);
	rightClamp.set_value(LOW);
	mArm.set_brake_mode(E_MOTOR_BRAKE_HOLD);
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
ASSET(redRightFullAWP_txt);

void test(){
	chassis.setPose(0,0,0);
	chassis.moveToPoint(0, 12, 100000);
}

void blueRightFull(){
	
}

void redRightFull(){
	chassis.setPose(0,0,0);
	chassis.follow(redRightFullAWP_txt, 15, 2000);
}

void progSkills(){
	
}

void intakeDist(){
	int i = 2;
	mIntake.move(127);
	if(sDist.get_distance() < 5 && sDist.get_object_velocity() > 0){
		mIntake.brake();
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
		test();
		break;
	case 1:
		blueRightFull();
		break;
	case 2:
		redRightFull();
		break;
	case 3:
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
	mIntake.set_brake_mode(MotorBrake::hold);

	while (true) {
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
		if(controller.get_digital_new_press(E_CONTROLLER_DIGITAL_A)){
			if(pressed){											  
				leftClamp.set_value(HIGH);							  
				rightClamp.set_value(HIGH);							  
			}	
			else if(!pressed){
				leftClamp.set_value(LOW);
				rightClamp.set_value(LOW);
			}
		}
		
		if(controller.get_digital(E_CONTROLLER_DIGITAL_R1)){
			mIntake.move(-127);
		}
		else if(controller.get_digital(E_CONTROLLER_DIGITAL_R2)){
			mIntake.move(127);
		}
		else{
			mIntake.brake();
		}

		if(controller.get_digital(E_CONTROLLER_DIGITAL_L1)){
			mArm.move(-127);
		}
		else if(controller.get_digital(E_CONTROLLER_DIGITAL_L2)){
			mArm.move(127);
		}
		else{
			mArm.brake();
		}
	}
}