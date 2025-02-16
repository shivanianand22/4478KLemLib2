#include "../include/main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "devices.h"
#include "auton.h"
#include "actions.h"
#include "lemlib/chassis/chassis.hpp"
#include "liblvgl/core/lv_disp.h"
#include "liblvgl/core/lv_event.h"
#include "liblvgl/core/lv_obj.h"
#include "liblvgl/core/lv_obj_style.h"
#include "liblvgl/lv_api_map.h"
#include "liblvgl/misc/lv_area.h"
#include "liblvgl/misc/lv_color.h"
#include "liblvgl/widgets/lv_img.h"
#include "pros/abstract_motor.hpp"
#include "pros/adi.h"
#include "pros/adi.hpp"
#include "lvgl_visuals.h"
#include "pros/misc.h"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include "drive.h"
#include "pros/apix.h"
#include "lvgl_actions.h"
#include "custom.h"
#include <string>
#include <vector>
using namespace pros;
using namespace lemlib;


LV_IMG_DECLARE(RedLeft);
LV_IMG_DECLARE(RedRight);
LV_IMG_DECLARE(BlueLeft);
LV_IMG_DECLARE(BlueRight);
LV_IMG_DECLARE(Rings);
LV_IMG_DECLARE(Mogo);
//above declared the images used in the program
bool blueAuton = false;
bool mogoAuton = false;
bool leftSide = false;

//booleans that dictate which path to show

extern void displayPaths();

// In a .cpp file (e.g., lvgl_screens.cpp)
lv_obj_t *autonTypeScreen ; //user picks between mogo and ring side
lv_obj_t *allianceScreen ;
lv_obj_t *pathingScreen ;
//user picks the color allaince they are on
//Screen displayed by the program
std::vector<void(*)()> autonPath;
//vector keeping track of the functions needed to be run

lv_obj_t * pathLabel;
//label object which diplays the path

// Callback function for blue allaince button
void blue_btn_event_handler(lv_event_t * e) {
  lv_obj_t * btn = lv_event_get_target(e);
  blueAuton = true; 
  lv_scr_load_anim(autonTypeScreen, LV_SCR_LOAD_ANIM_OVER_BOTTOM, 500, 0, true); 
} ///^^^ loads next screen and deltes previous one to conserve RAM

// Callback function for red allaince button
void red_btn_event_handler(lv_event_t * e) {
  lv_obj_t * btn = lv_event_get_target(e);
  blueAuton = false;
  lv_scr_load_anim(autonTypeScreen, LV_SCR_LOAD_ANIM_OVER_BOTTOM, 500, 0, true); 
}///^^^ loads next screen and deltes previous one to conserve RAM

// Callback function for mogo button
void mogo_side_event_handler(lv_event_t * e) {
  lv_obj_t * btn = lv_event_get_target(e);
  mogoAuton = true;
  displayPaths();
} //^^^^^^finalized the path region and calls the function to load the auton screen

// Callback function menu rings button
void ring_side_event_handler(lv_event_t * e) {
  lv_obj_t * btn = lv_event_get_target(e);
  mogoAuton = false;
  displayPaths();
}//^^^^^^finalized the path region and calls the function to load the auton screen
int count=0;
std::string pathText = "Path:\n"; 
void updatePathLabel(std::string movement) {//updates the current path on the brain screen
    count++;
    //creates string path to be printed
    pathText += (std::to_string(count) + "." + movement + " \n"); //adds the movement to the path
    lv_label_set_text(pathLabel, pathText.c_str()); //updates the brain screen
}

//6 below functions add the respective function to the path vector when respective button is clicked
void allianceStakeButtonHandler(lv_event_t * e) {
  autonPath.push_back(leftSide ? leftAutonFunctions[0] : rightAutonFunctions[0]);
  updatePathLabel("Alliance Stake");
}

void mogoButtonHandler(lv_event_t * e) {
  
  autonPath.push_back(leftSide ? leftAutonFunctions[1] : rightAutonFunctions[1]);
  updatePathLabel("Mobile Goal");
}

void ringsButtonHandler(lv_event_t * e) {
  autonPath.push_back(leftSide ? leftAutonFunctions[3] : rightAutonFunctions[3]);
  updatePathLabel(" Middle Rings");
}

void farMogoButtonHandler(lv_event_t * e) {
  autonPath.push_back(leftSide ? leftAutonFunctions[4] : rightAutonFunctions[4]);
  updatePathLabel("Middle Mogo");
}

void ringButtonHandler(lv_event_t * e) {
  controller.rumble(".");
  autonPath.push_back(leftSide ? leftAutonFunctions[2] : rightAutonFunctions[2]);
  updatePathLabel("Ring");
}

void cornerButtonHandler(lv_event_t * e) {
  autonPath.push_back(leftSide ? leftAutonFunctions[5] : rightAutonFunctions[5]);
  updatePathLabel("Corner");
}

void barButtonHandler(lv_event_t * e) {
  autonPath.push_back(leftSide ? leftAutonFunctions[6] : rightAutonFunctions[6]);
  updatePathLabel("touch bar");
}

//callback functions for startButton which toggles wheter the robot is going forwards or backwards
void startPositionButtonHandler(lv_event_t * e) {
    startForward = !startForward;
    lv_obj_t * btn = lv_event_get_target(e);
    if (startForward) {
        lv_obj_set_style_bg_color(btn, lv_color_hex(0x00FF00), LV_PART_MAIN); // Green for forwards
        lv_label_set_text(lv_obj_get_child(btn, 0), "Forwards"); //print forwards on the button
    } else {
        lv_obj_set_style_bg_color(btn, lv_color_hex(0xFF0000), LV_PART_MAIN); // Red for backwards
        lv_label_set_text(lv_obj_get_child(btn, 0), "Backwards");//print backwards on the button
    }
}

//buttons generated for RedLeft path more to come
void alignButton(lv_obj_t *alliance_btn,lv_obj_t *mogo_btn, lv_obj_t *ring_btn, lv_obj_t *mid_btn, lv_obj_t *corner_btn, lv_obj_t *bar_btn){
  pathLabel = lv_label_create(pathingScreen);
  if(blueAuton && mogoAuton){ //aligns for blueLeft
    leftSide = true;
    lv_obj_align(alliance_btn, LV_ALIGN_BOTTOM_RIGHT, -10, -145);//aligns button
    lv_obj_align(mogo_btn, LV_ALIGN_BOTTOM_RIGHT, -90, -90);
    lv_obj_align(mid_btn, LV_ALIGN_BOTTOM_RIGHT, -140, -45);
    lv_obj_align(ring_btn, LV_ALIGN_BOTTOM_RIGHT, -91, -45);
    lv_obj_align(corner_btn, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
    lv_obj_align(bar_btn, LV_ALIGN_BOTTOM_RIGHT, -110, -105);
    lv_obj_add_event_cb(mid_btn, farMogoButtonHandler, LV_EVENT_CLICKED, NULL);
    lv_obj_align(pathLabel, LV_ALIGN_TOP_LEFT, 10, 10);
    
  }
  else if(blueAuton && !mogoAuton){//aligns for blueRight
    leftSide = false;
    lv_obj_align(alliance_btn, LV_ALIGN_TOP_RIGHT, -10, 145);//aligns button
    lv_obj_align(mogo_btn, LV_ALIGN_TOP_RIGHT, -90, 90);
    lv_obj_align(mid_btn, LV_ALIGN_TOP_RIGHT, -135, 40);
    lv_obj_align(ring_btn, LV_ALIGN_TOP_RIGHT, -91, 40);
    lv_obj_align(corner_btn, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_obj_align(bar_btn, LV_ALIGN_TOP_RIGHT, -128, 115);
    lv_obj_add_event_cb(mid_btn, ringsButtonHandler, LV_EVENT_CLICKED, NULL);
    lv_obj_align(pathLabel, LV_ALIGN_TOP_LEFT, 10, 10);
  }
  else if(!blueAuton && !mogoAuton){//allights for redLeft
    leftSide = true;
    lv_obj_align(alliance_btn, LV_ALIGN_TOP_LEFT, 10, 145);//aligns button
    lv_obj_align(mogo_btn, LV_ALIGN_TOP_LEFT, 90, 90);
    lv_obj_align(mid_btn, LV_ALIGN_TOP_LEFT, 145, 40);
    lv_obj_align(ring_btn, LV_ALIGN_TOP_LEFT, 83, 40);
    lv_obj_align(corner_btn, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_align(bar_btn, LV_ALIGN_TOP_LEFT, 125, 125);
    lv_obj_add_event_cb(mid_btn, ringsButtonHandler, LV_EVENT_CLICKED, NULL);
    lv_obj_align(pathLabel, LV_ALIGN_TOP_RIGHT, -10, 10);
  }
  else{//red right
    leftSide = false;
    lv_obj_align(alliance_btn, LV_ALIGN_BOTTOM_LEFT, 10, -145);//aligns button
    lv_obj_align(mogo_btn, LV_ALIGN_BOTTOM_LEFT, 85, -90);
    lv_obj_align(mid_btn, LV_ALIGN_BOTTOM_LEFT, -50, -51);
    lv_obj_align(ring_btn, LV_ALIGN_BOTTOM_LEFT, 91, -45);
    lv_obj_align(corner_btn, LV_ALIGN_BOTTOM_LEFT, 0, 0);
    lv_obj_align(bar_btn, LV_ALIGN_BOTTOM_LEFT, 120, -120);
    lv_obj_add_event_cb(mid_btn, farMogoButtonHandler, LV_EVENT_CLICKED, NULL);
    lv_obj_align(pathLabel, LV_ALIGN_TOP_RIGHT, -10, 10);
  }
}
void createButtons() {
    // Alliance Stake Button
    lv_obj_t * allianceStakeButton = lv_btn_create(pathingScreen); //create button
    lv_obj_set_size(allianceStakeButton, 30, 30); //set button size
    lv_obj_set_style_bg_opa(allianceStakeButton, LV_OPA_TRANSP, LV_PART_MAIN); // Bright green
  
    lv_obj_add_event_cb(allianceStakeButton, allianceStakeButtonHandler, LV_EVENT_CLICKED, NULL);
    //^^^registers the call back function to the button 

    // Mogo Button
    lv_obj_t * mogoButton = lv_btn_create(pathingScreen);
    lv_obj_set_size(mogoButton, 30, 30);
    lv_obj_set_style_bg_opa(mogoButton, LV_OPA_TRANSP, LV_PART_MAIN); // Bright green
    lv_obj_add_event_cb(mogoButton, mogoButtonHandler, LV_EVENT_CLICKED, NULL);

    // Rings or mogo Button
    lv_obj_t * middleButton = lv_btn_create(pathingScreen);
    lv_obj_set_size(middleButton, 30, 30);
    lv_obj_set_style_bg_opa(middleButton, LV_OPA_TRANSP, LV_PART_MAIN); // Bright green

    // Ring Button
    lv_obj_t * ringButton = lv_btn_create(pathingScreen);
    lv_obj_set_size(ringButton, 30, 30);
    lv_obj_set_style_bg_opa(ringButton, LV_OPA_TRANSP, LV_PART_MAIN); // Bright green
    lv_obj_add_event_cb(ringButton, ringButtonHandler, LV_EVENT_CLICKED, NULL);

    // Corner Button
    lv_obj_t * cornerButton = lv_btn_create(pathingScreen);
    lv_obj_set_size(cornerButton, 30, 30);
    lv_obj_set_style_bg_opa(cornerButton, LV_OPA_TRANSP, LV_PART_MAIN); // Bright green
    lv_obj_add_event_cb(cornerButton, cornerButtonHandler, LV_EVENT_CLICKED, NULL);

    // Bar Button
    lv_obj_t * barButton = lv_btn_create(pathingScreen);
    lv_obj_set_size(barButton, 30, 30);
    lv_obj_set_style_bg_opa(barButton, LV_OPA_TRANSP, LV_PART_MAIN); // Bright green
    
    lv_obj_add_event_cb(barButton, barButtonHandler, LV_EVENT_CLICKED, NULL);

    alignButton(allianceStakeButton, mogoButton, middleButton, ringButton, cornerButton, barButton);
    // Start Position Button, to change starting orientation of the robot
    lv_obj_t * startPositionButton = lv_btn_create(pathingScreen);
    lv_obj_set_size(startPositionButton, 100, 50);
    lv_obj_set_style_bg_color(startPositionButton, lv_color_hex(0x00FF00), LV_PART_MAIN); // Bright green
    if(!blueAuton){ lv_obj_align(startPositionButton, LV_ALIGN_BOTTOM_RIGHT, -10, -10);}
    else{         lv_obj_align(startPositionButton, LV_ALIGN_BOTTOM_LEFT, 10, -10);}
    lv_obj_add_event_cb(startPositionButton, startPositionButtonHandler, LV_EVENT_CLICKED, NULL);
    lv_obj_t * label = lv_label_create(startPositionButton);//Putting text on the button
    lv_label_set_text(label, "Forwards"); 
    lv_obj_center(label);
}
//the functions below load the differnt auton paths

void displayPaths(){
  createButtons();
  lv_obj_t * img = lv_img_create(pathingScreen); //create an image object
  
  if(blueAuton && mogoAuton){
    lv_img_set_src(img, &BlueLeft); //set the image of the field on the brain
    lv_obj_align(img,  LV_ALIGN_BOTTOM_RIGHT, 0, 0);
    
  }
  else if(blueAuton && !mogoAuton){
    lv_img_set_src(img, &BlueRight); //set the image of the field on the brain
    lv_obj_align(img, LV_ALIGN_TOP_RIGHT, 0,0);
  }
  else if(!blueAuton && !mogoAuton){
    lv_img_set_src(img, &RedLeft); //set the image of the field on the brain
    lv_obj_align(img, LV_ALIGN_TOP_LEFT, 0, 0);//align the image to the top left of the screen
  }
  else{
    lv_img_set_src(img, &RedRight); //set the image of the field on the brain
    lv_obj_align(img, LV_ALIGN_BOTTOM_LEFT, 0, 0);
  }
  lv_obj_move_background(img);
  lv_scr_load_anim(pathingScreen, LV_SCR_LOAD_ANIM_OVER_BOTTOM, 500, 0, true);
}


//intializes the screens and sets the background color
void createScreens(){
    allianceScreen = lv_obj_create(NULL); // Create a new screen
    autonTypeScreen = lv_obj_create(NULL); // Create a new screen
    pathingScreen = lv_obj_create(NULL); // Create a new screen
    // redLeftScreen = lv_obj_create(NULL); // Create a new screen
    // redRightScreen = lv_obj_create(NULL); // Create a new screen
    // blueLeftScreen = lv_obj_create(NULL); // Create a new screen
    // blueRightScreen = lv_obj_create(NULL); // Create a new screen
    lv_obj_set_style_bg_color(allianceScreen, lv_color_hex(0x003a57), LV_PART_MAIN); // Dark blue
    lv_obj_set_style_bg_color(autonTypeScreen, lv_color_hex(0x003a57), LV_PART_MAIN); // Dark blue
    lv_obj_set_style_bg_color(pathingScreen, lv_color_hex(0x003a57), LV_PART_MAIN); // Dark blue
    // lv_obj_set_style_bg_color(redLeftScreen, lv_color_hex(0x003a57), LV_PART_MAIN); // Dark blue
    // lv_obj_set_style_bg_color(redRightScreen, lv_color_hex(0xFF4500), LV_PART_MAIN); // Deep orange
    // lv_obj_set_style_bg_color(blueLeftScreen, lv_color_hex(0xFF4500), LV_PART_MAIN); // Deep orange
    // lv_obj_set_style_bg_color(blueRightScreen, lv_color_hex(0xFF4500), LV_PART_MAIN); // Deep orange
    //^^^^sets the backgrund color for the screens
    // Load the initial screen
    lv_scr_load(allianceScreen);

}
//decided which autonmous region to display based on the booleans

//creates all the menu buttons
void create_buttons() {
  // Create a blue button
  lv_obj_t * blue_btn = lv_btn_create(allianceScreen);                         
  lv_obj_add_flag(blue_btn, LV_OBJ_FLAG_CHECKABLE);  // Make the button checkable

  // Default Blue Style
  static lv_style_t blueStyle; //create a style for the button
  lv_style_init(&blueStyle);
  lv_style_set_bg_color(&blueStyle, lv_palette_main(LV_PALETTE_BLUE)); //Blue

  // Dark Blue Checked Style
  static lv_style_t darkBlueStyle;
  lv_style_init(&darkBlueStyle);//create secondary style for when button is checked
  lv_style_set_bg_color(&darkBlueStyle, lv_palette_darken(LV_PALETTE_BLUE, 3));  // Darker blue

  // Apply default style
  lv_obj_add_style(blue_btn, &blueStyle, LV_STATE_DEFAULT);      // Apply normal blue with not checked
  lv_obj_add_style(blue_btn, &darkBlueStyle, LV_STATE_CHECKED);  // Apply dark blue when checked
  lv_obj_add_event_cb(blue_btn, blue_btn_event_handler, LV_EVENT_CLICKED, NULL);
  lv_obj_set_size(blue_btn, 120, 240); //set size of blue button
  lv_obj_align(blue_btn, LV_ALIGN_LEFT_MID, 0, 0); //allign to left side of screen

  // Create a red button
  lv_obj_t * red_btn = lv_btn_create(allianceScreen);               
  lv_obj_add_flag(red_btn, LV_OBJ_FLAG_CHECKABLE);  // Make the button checkable

  // Default Red Style
  static lv_style_t redStyle;//create a style for the button
  lv_style_init(&redStyle);
  lv_style_set_bg_color(&redStyle, lv_palette_main(LV_PALETTE_RED));//Red

  // Dark Red Checked Style
  static lv_style_t darkRedStyle;//create secondary style for when button is checked
  lv_style_init(&darkRedStyle);
  lv_style_set_bg_color(&darkRedStyle, lv_palette_darken(LV_PALETTE_RED, 3));  // Darker red

  // Apply default style
  lv_obj_add_style(red_btn, &redStyle, LV_STATE_DEFAULT);// Apply normal red with not checked
  lv_obj_add_style(red_btn, &darkRedStyle, LV_STATE_CHECKED);  // Apply dark red when checked
  lv_obj_add_event_cb(red_btn, red_btn_event_handler, LV_EVENT_CLICKED, NULL);
  lv_obj_set_size(red_btn, 120, 240);//set size of red button
  lv_obj_align(red_btn, LV_ALIGN_RIGHT_MID, 0, 0);//allign to right side of screen


  // Create the buttons with images

  //Create the mogo button
  lv_obj_t * mogoSide = lv_imgbtn_create(autonTypeScreen); //vvvsets button to mogo Image
  lv_imgbtn_set_src(mogoSide, LV_IMGBTN_STATE_RELEASED, NULL, &Mogo, NULL);
  lv_obj_align(mogoSide, LV_ALIGN_LEFT_MID, 0, 0);//alligns the button to the left side of the screen
  lv_obj_add_event_cb(mogoSide, mogo_side_event_handler, LV_EVENT_CLICKED, NULL);
  //^^^registers the call back function to the button

  // Create the rings button
  lv_obj_t * ringSide = lv_imgbtn_create(autonTypeScreen);//vvvvsets button to ring Image
  lv_imgbtn_set_src(ringSide, LV_IMGBTN_STATE_RELEASED, NULL, &Rings, NULL);
  lv_obj_align(ringSide, LV_ALIGN_RIGHT_MID, 0, 0);//alligns the button to the right side of the screen
  lv_obj_add_event_cb(ringSide, ring_side_event_handler, LV_EVENT_CLICKED, NULL);
  //^^^registers the call back function to the button
  
}



/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */

//Task colorTask(intakeColor, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT,"controls color sort" );

void initialize() {
	lv_init(); //initalize  lvgl
	chassis.calibrate();
  imu.reset();
  Rotation rotation_sensor(10);
	createScreens();
	create_buttons();
	mArm.set_gearing(pros::E_MOTOR_GEARSET_36);
	Clamper.set_value(LOW);
	mArm.set_brake_mode(MotorBrake::hold);
	Task armTask(moveArm, (void*)"PROS", TASK_PRIORITY_DEFAULT, TASK_STACK_DEPTH_DEFAULT,"controls arm tasks" );
	rotation_sensor.set_position(0);
	selection--;
	autonSelector();
	
		
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
	
    
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




void autonomous() {
	
	controller.rumble("-");
	mLefts.tare_position();
	mRights.tare_position();
	for (auto& motion : autonPath) { //loops through the vector of functions 
		motion(); //runs the functions in the vector in order
		chassis.waitUntilDone();//ensures each command runs seprately
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
	//colorTask.remove();
	mIntake.set_brake_mode(MotorBrake::coast);

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
	if(controller.get_digital_new_press(E_CONTROLLER_DIGITAL_L2)){target = armTargets[0];}
	if(controller.get_digital_new_press(E_CONTROLLER_DIGITAL_DOWN)){target = armTargets[1];}
	if(controller.get_digital_new_press(E_CONTROLLER_DIGITAL_L1)){target = armTargets[2];}
	if(controller.get_digital_new_press(E_CONTROLLER_DIGITAL_UP)){target = armTargets[3];}

	if(controller.get_digital_new_press(E_CONTROLLER_DIGITAL_L2)){target = armTargets[0];}
	if(controller.get_digital_new_press(E_CONTROLLER_DIGITAL_DOWN)){target = armTargets[1];}
	if(controller.get_digital_new_press(E_CONTROLLER_DIGITAL_L1)){target = armTargets[2];}
	if(controller.get_digital_new_press(E_CONTROLLER_DIGITAL_UP)){target = armTargets[3];}


	if(controller.get_digital(E_CONTROLLER_DIGITAL_R1)){  

		if(primed){ //if the arm is primed
			if(sDist.get_distance() > 50 && sDist.get_distance() < 70){
				mIntake.move(0); //stop the arm when ring is in lady brown
			}
			else{
				mIntake.move(127); //move the arm until ring is in lady brown
			}
		}
		else{
			mIntake.move(127);
		}

	if(controller.get_digital(E_CONTROLLER_DIGITAL_R1)){  

		if(primed){ //if the arm is primed
			if(sDist.get_distance() > 50 && sDist.get_distance() < 70){
				mIntake.move(0); //stop the arm when ring is in lady brown
			}
			else{
				mIntake.move(127); //move the arm until ring is in lady brown
			}
		}
		else{
			mIntake.move(127);
		}

	} 
	else if(controller.get_digital(E_CONTROLLER_DIGITAL_R2)){   
		mIntake.move(-127);//move intake backward
	}
	else{   
		mIntake.brake();//stop intake
	}    
	if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)){
		if(!blueAllaince){
			controller.set_text(1,1,"BLUE TEAM");
			blueAllaince = true;
	if(controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)){
		if(!blueAllaince){
			controller.set_text(1,1,"BLUE TEAM");
			blueAllaince = true;
		}
		else{
			controller.set_text(1,1,"RED TEAM");
			blueAllaince = false;
		}
	}
	
	

	
		    

        lcd::print(2, "sensor: %d",rotation_sensor.get_position()/100.0);
			controller.set_text(1,1,"RED TEAM");
			blueAllaince = false;
		}
	}
	
	

	
		    

        lcd::print(2, "sensor: %d",rotation_sensor.get_position()/100.0);
	}
}
}