#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "lemlib/chassis/chassis.hpp"
#include "pros/abstract_motor.hpp"
#include "pros/adi.h"
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "pros/misc.hpp"
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
                            nullptr, // vertical tracking wheel 2, set to nullptr
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

int selection = 4;

void autonSelector(){
	if(selection <= 3){
		selection++;
	} else {
		selection = 0;
	}

	switch(selection){
	case 0:
		pros::lcd::set_text(2, "RedRight(MOGO Side)");
		break;
	case 1:
		pros::lcd::set_text(2, "RedLeft(Ring Side)");
		break;
	case 2:
		pros::lcd::set_text(2, "BlueLeft(MOGO side)");
		break;
	case 3:
		pros::lcd::set_text(2, "BlueRight(Ring Side)");
		break;
	case 4:
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
    imu.reset();
    Rotation rotation_sensor(10);
	lcd::set_text(1, "Press center button to select autonomous");
	lcd::register_btn1_cb(autonSelector);

	clamp.set_value(LOW);
	mArm.set_brake_mode(E_MOTOR_BRAKE_HOLD);
	Task armMacro(setArm);
	mArm.set_current_limit(2500);
    rotation_sensor.set_position(0);
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
void redRight(){ //mogo side
    chassis.setPose(-51.7, -18.0, 90);
    chassis.moveToPoint(-50.5,  0.2, 1500, {.forwards = false});
	chassis.waitUntilDone();
	chassis.setPose(-50.5,  0.2, chassis.getPose().theta);
    chassis.moveToPose(-57.4,  0.2, 90, 1500, {.forwards = false});


    chassis.waitUntilDone();
    delay(100);
    mIntake.move(-220);
    chassis.setPose(-63.5, 0.2, 90);
    delay(900); //score alliance stake
    chassis.moveToPoint(-47.938, -15.951, 1000);
    chassis.moveToPose(-28.5, -23.999, -55, 2000, {.forwards = false}); //get to mogo
    chassis.waitUntilDone();
    delay(75);
    grab();
    delay(75);
	chassis.setPose(-28.5,  -23.999, chassis.getPose().theta);
    chassis.moveToPoint(-19.5, -61.914, 3000);//pickup ring to put on Mogo
    chassis.moveToPose(-18, -1.98, 20, 3000); //touch bar
}

void blueLeft(){ //mogo side
    chassis.setPose(-51.9, 16.0, 90);
    chassis.moveToPoint(-54.5,  0.2, 1500, {.forwards = false});
    chassis.moveToPose(-63.5,  0.2, 90, 1500, {.forwards = false});
    chassis.waitUntilDone();
    delay(100);
    mIntake.move(-220);
    delay(1100); //score alliance stake
    chassis.moveToPoint(-47.938, 15.951, 1000);
    chassis.moveToPose(-28.5, 23.999, -145, 2000, {.forwards = false}); //get to mogo
    chassis.waitUntilDone();
    delay(75);
    grab();
    delay(75);
	chassis.setPose(-28.5,  23.999, chassis.getPose().theta);
    chassis.moveToPoint(-26.0, 53.114, 2400);//pickup ring to put on Mogo
    chassis.moveToPoint(-24, -2, 3000); //touch bar
}


void redLeft(){ //ring side
    chassis.setPose(-51.9, 16.0, 90);
    chassis.moveToPoint(-54.5,  0.2, 1500, {.forwards = false});
    chassis.moveToPose(-63.5,  0.2, 90, 1500, {.forwards = false});
    chassis.waitUntilDone();
    delay(100);
    mIntake.move(-220);
    delay(1100); //score alliance stake
    chassis.setPose(-63.5,  0.2, chassis.getPose().theta);
    chassis.moveToPoint(-47.938, 15.951, 1000);
    chassis.moveToPose(-28.5, 23.999, -145, 2000, {.forwards = false}); //get to mogo
    chassis.waitUntilDone();
    delay(75);
    grab();
    delay(75);
    chassis.setPose(-28.5,  23.999, chassis.getPose().theta);
    chassis.moveToPoint(-26.0, 57, 2000);//pickup ring to put on Mogo
    chassis.moveToPoint(-21.0, 52.114, 1000, {.forwards=false});//quickly backup to avoid 2nd ring
    chassis.moveToPose(-19.561, 51.9, 90, 1000); //prepare to rush
    chassis.moveToPoint(-14.17, 57, 1500); //rush center rings
    delay(500);
   /*
    chassis.moveToPoint(-17.561, -51.9, 1000, {.forwards = false}); //back up
    chassis.waitUntilDone();
    controller.rumble("--");
    
    chassis.moveToPoint(-18.17, 49,1700); //go for second ring
    chassis.waitUntilDone();
    delay(500);
    */

    chassis.moveToPoint(-20.468, -6, 1100); //touch bar
}


void blueRight(){ //ring side
    chassis.setPose(-51.7, -18.0, 90);
    chassis.moveToPoint(-50.5,  0.2, 1500, {.forwards = false});
	chassis.waitUntilDone();
	chassis.setPose(-50.5,  0.2, chassis.getPose().theta);
    chassis.moveToPose(-57.4,  0.2, 90, 1500, {.forwards = false});


    chassis.waitUntilDone();
    delay(100);
    mIntake.move(-220);
    chassis.setPose(-63.5, 0.2, 90);
    delay(900); //score alliance stake
    chassis.moveToPoint(-47.938, -15.951, 1000);
    chassis.moveToPose(-28.5, -23.999, -55, 2000, {.forwards = false}); //get to mogo
    chassis.waitUntilDone();
    delay(75);
    grab();
    delay(75);
	chassis.setPose(-28.5,  -23.999, chassis.getPose().theta);
    chassis.moveToPoint(-19.5, -61.914, 2800);//pickup ring to put on Mogo
    chassis.moveToPose(-12.561, -60.9, 90, 1000); //prepare to rush
    chassis.moveToPoint(-6, -60.9, 1400); //rush center rings
    chassis.moveToPose(-18, -1.98, 20, 3000); //touch bar
   
}

void progSkills(){
    controller.set_text(1, 1, "running skills");
    mLefts.tare_position();
    mRights.tare_position();
    chassis.setPose(-64.241, -0.705, 90);
    mIntake.move(-280);
    delay(200);
    chassis.moveToPoint(-64.241, 1.705, 1000);
   
    chassis.moveToPose(-46.897, 19.678, 160, 3000, {.forwards = false}); //go to goal
    chassis.waitUntilDone();
    delay(100);
    grab();
    chassis.moveToPoint(-22.593, 28.722, 2000);//pick up first ring
    chassis.moveToPose(-25.912, 49.993, 270, 3000); //go to 2nd ring
    chassis.moveToPose(-39.912, 49.653, 270, 750); //pick up 2nd ring
    chassis.waitUntilDone();
    delay(2000);
    chassis.moveToPoint(-58.972, 49.622, 2000); //3rd & 4th ring
    chassis.moveToPose(-58.257, 64.489, 325, 1000, {.forwards = false});//drop 1st goal into corner
    chassis.waitUntilDone();
    controller.clear();
    controller.set_text(1, 1, std:: to_string(chassis.getPose().theta));
    release();
    delay(100);
     //start of auton that just pushes goal
    mLefts.tare_position();
    mRights.tare_position();
    chassis.setPose(-62.9, 60.675, chassis.getPose().theta);
    delay(50);
    chassis.moveToPoint(17.138, 45.25, 2000,{.forwards = true});    
    chassis.moveToPoint(50.564, 18,  2500); //get behind 2nd goal
    chassis.moveToPoint((67.5), 65, 2500);
    chassis.moveToPoint(61, 57, 2500, {.forwards = false});
    chassis.moveToPoint(59, 4, 4000);
    chassis.moveToPose(90.864, -59.77, 85, 3900); //score 3rd goal
    chassis.moveToPoint(56, -47, 2500, {.forwards = false});
    chassis.moveToPose(-19.793, -30.913, 320, 4000);
    chassis.moveToPose(-39, -12, 200, 4000);
    chassis.moveToPoint(-64.088, -74.139,2000);//score 4th goal
    chassis.moveToPoint(-64, -53, 2500, {.forwards = false});
    chassis.moveToPoint(-22, -22, 1000);
    mIntake.brake();
    //start to go to second goal
    /*
    chassis.moveToPose(-47.175,-18.129,270, 3500);
    grab();*/
    //end of good auton to start push goal auton
    /*
   
    */
    /*
    //code to finish for good auton skills
    //go towards 2ng Mogo
    chassis.moveToPoint(-55.674, -0.515, 2000, {.forwards = false});
    chassis.moveToPose(-59.864, -19.983, 30, 1000, {.forwards=false});
    chassis.waitUntilDone();
    delay(1000);
    grab();
    delay(100);
    chassis.moveToPoint(-22.593, -24.395, 2000); //1st ring
    chassis.moveToPose(-35.936, -49.326,270,  2000);//2nd ring
    chassis.moveToPoint(-63.351, -49.136, 1000); //3rd & 4th ring
    chassis.moveToPose(-65.057, -63.814,45, 1000, {.forwards = false}); //drop off 2nd mogo in corner
    chassis.waitUntilDone();
    release();
    */
    /* old "20pt" skills
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
    mIntake.brake();*/
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
	bool pressed = false; //initializes as false so pneumatics don't start open
	bool slow = false; 
	mIntake.set_brake_mode(MotorBrake::coast);
	mArm.set_brake_mode(MotorBrake::hold);
	bool run = false;
	mArm.tare_position();
	

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
		// if(!slow){
		// 	if(controller.get_digital(E_CONTROLLER_DIGITAL_R1)){
		// 	mIntake.move(-200);
		// }
		// 	else if(controller.get_digital(E_CONTROLLER_DIGITAL_R2)){
		// 	mIntake.move(200);
		// }
		// 	else{
		// 	mIntake.brake();
		// }
		// }
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
			//mArm.move(-127);
			mIntake.move_relative(90, 400);
			delay(50);
			mArm.move_absolute(-1770, 1000);
		}
		else if(controller.get_digital(E_CONTROLLER_DIGITAL_L2)){
			mArm.move(100);
		}
		else{
			mArm.brake();
		}
		
		if (run){
			if(mArm.get_position() < -322){
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
        controller.set_text(0, 0, std::to_string(mArm.get_position()));
	}
}