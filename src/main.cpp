#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "devices.h"
#include "auton.h"
#include "actions.h"
#include "lemlib/chassis/chassis.hpp"
#include "liblvgl/core/lv_disp.h"
#include "liblvgl/core/lv_event.h"
#include "liblvgl/core/lv_obj.h"
#include "liblvgl/core/lv_obj_style.h"
#include "liblvgl/draw/lv_img_buf.h"
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


extern void displayPaths(); //function that displays the path at the end
extern void updatePathLabel(std::string movement); //updates the path user is creating
extern void blue_btn_event_handler(lv_event_t * e); //callback function for blue menu button
extern void red_btn_event_handler(lv_event_t * e); //callback function for red menu button
extern void mogo_side_event_handler(lv_event_t * e); //callback function for mogo menu button
extern void ring_side_event_handler(lv_event_t * e); //callback function for ring menu button
extern void allianceStakeButtonHandler(lv_event_t * e); //callback function for alliance stake button
extern void mogoButtonHandler(lv_event_t * e); //callback function for mogo button
extern void ringsButtonHandler(lv_event_t * e); //callback function for rings button
extern void farMogoButtonHandler(lv_event_t * e); //callback function for far mogo button
extern void ringButtonHandler(lv_event_t * e); //callback function for ring button
extern void cornerButtonHandler(lv_event_t * e); //callback function for corner button
extern void barButtonHandler(lv_event_t * e); //callback function for bar button
extern void startPositionButtonHandler(lv_event_t * e); //callback function for start position button
extern void alignButton(lv_obj_t *alliance_btn,
                        lv_obj_t *mogo_btn, 
                        lv_obj_t *ring_btn, 
                        lv_obj_t *mid_btn, 
                        lv_obj_t *corner_btn, 
                        lv_obj_t *bar_btn); //aligns the buttons on the screen


lv_obj_t * pathLabel;//label object which diplays the path
lv_obj_t *allianceScreen ;  //user picks the color allaince they are on
lv_obj_t *autonTypeScreen ; //user picks between mogo and ring side
lv_obj_t *pathingScreen ;   //user picks the path they want to take

std::vector<void(*)()> autonPath;//vector keeping track of the functions needed to be run
int count=0;//keeps track of # of actions in the path
std::string pathText = "Path:\n"; //string that holds info for what Path the user is creating

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

  // Apply styles to button
  lv_obj_add_style(blue_btn, &blueStyle, LV_STATE_DEFAULT);      // Apply normal blue with not checked
  lv_obj_add_style(blue_btn, &darkBlueStyle, LV_STATE_CHECKED);  // Apply dark blue when checked
  lv_obj_add_event_cb(blue_btn, blue_btn_event_handler, LV_EVENT_CLICKED, NULL); //register clalback function
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

  // Apply styles to button
  lv_obj_add_style(red_btn, &redStyle, LV_STATE_DEFAULT);// Apply normal red with not checked
  lv_obj_add_style(red_btn, &darkRedStyle, LV_STATE_CHECKED);  // Apply dark red when checked
  lv_obj_add_event_cb(red_btn, red_btn_event_handler, LV_EVENT_CLICKED, NULL);
  lv_obj_set_size(red_btn, 120, 240);//set size of red button
  lv_obj_align(red_btn, LV_ALIGN_RIGHT_MID, 0, 0);//allign to right side of screen

  //Create the mogo menu button
  lv_obj_t * mogoSide = lv_imgbtn_create(autonTypeScreen); //vvvsets button to mogo Image
  lv_imgbtn_set_src(mogoSide, LV_IMGBTN_STATE_RELEASED, NULL, &Mogo, NULL);
  lv_obj_align(mogoSide, LV_ALIGN_LEFT_MID, 0, 0);//alligns the button to the left side of the screen
  lv_obj_add_event_cb(mogoSide, mogo_side_event_handler, LV_EVENT_CLICKED, NULL);
  //^^^registers the call back function to the button

  // Create the rings menu button
  lv_obj_t * ringSide = lv_imgbtn_create(autonTypeScreen);  //vvvvsets button to ring Image
  lv_imgbtn_set_src(ringSide, LV_IMGBTN_STATE_RELEASED, NULL, &Rings, NULL);
  lv_obj_align(ringSide, LV_ALIGN_RIGHT_MID, 0, 0);//alligns the button to the right side of the screen
  lv_obj_add_event_cb(ringSide, ring_side_event_handler, LV_EVENT_CLICKED, NULL);
  //^^^registers the call back function to the button
  
}

// Callback function for blue allaince button
void blue_btn_event_handler(lv_event_t * e) {
  lv_obj_t * btn = lv_event_get_target(e);
  blueAuton = true; //sets the allaince to blue
  lv_scr_load_anim(autonTypeScreen, LV_SCR_LOAD_ANIM_OVER_BOTTOM, 500, 0, true); 
} ///^^^ loads next screen and deltes previous one to conserve RAM

// Callback function for red allaince button
void red_btn_event_handler(lv_event_t * e) {
  lv_obj_t * btn = lv_event_get_target(e);
  blueAuton = false;//sets the allaince to red
  lv_scr_load_anim(autonTypeScreen, LV_SCR_LOAD_ANIM_OVER_BOTTOM, 500, 0, true); 
}///^^^ loads next screen and deltes previous one to conserve RAM

// Callback function for mogo button
void mogo_side_event_handler(lv_event_t * e) {
  lv_obj_t * btn = lv_event_get_target(e);
  mogoAuton = true;//sets the path to mogo side
  displayPaths();
} //^^^^^^finalized the path region and calls the function to load the auton screen

// Callback function menu rings button
void ring_side_event_handler(lv_event_t * e) {
  lv_obj_t * btn = lv_event_get_target(e);
  mogoAuton = false;//sets the path to ring side
  displayPaths();
}//^^^^^^finalized the path region and calls the function to load the auton screen


//intializes the screens and sets the background color
void createScreens(){
  allianceScreen = lv_obj_create(NULL); // Create a new screen
  autonTypeScreen = lv_obj_create(NULL); // Create a new screen
  pathingScreen = lv_obj_create(NULL); // Create a new screen
  lv_obj_set_style_bg_color(allianceScreen, lv_color_hex(0x003a57), LV_PART_MAIN); // Dark blue
  lv_obj_set_style_bg_color(autonTypeScreen, lv_color_hex(0x003a57), LV_PART_MAIN); // Dark blue
  lv_obj_set_style_bg_color(pathingScreen, lv_color_hex(0x003a57), LV_PART_MAIN); // Dark blue
  //^^^^sets the backgrund color for the screens
  // Load the initial screen
  lv_scr_load(allianceScreen);

}

void createPathButtons() {
  // Alliance Stake Button
  lv_obj_t * allianceStakeButton = lv_btn_create(pathingScreen); //create allaince stake button
  lv_obj_set_size(allianceStakeButton, 30, 30); //set button size
  lv_obj_set_style_bg_color(allianceStakeButton, lv_color_black(), LV_PART_MAIN);
  //^^make button black^^
  lv_obj_set_style_bg_opa(allianceStakeButton, 60, LV_PART_MAIN); 
  //^^make button barley visible to show it is there^^ 
  lv_obj_add_event_cb(allianceStakeButton, allianceStakeButtonHandler, LV_EVENT_CLICKED, NULL);
  //^^^registers the call back function to the button 

  // Mogo Button
  lv_obj_t * mogoButton = lv_btn_create(pathingScreen);//create mogo button
  lv_obj_set_size(mogoButton, 30, 30);//set button size
  lv_obj_set_style_bg_color(mogoButton, lv_color_black(), LV_PART_MAIN);
  //^^make button black^^
  lv_obj_set_style_bg_opa(mogoButton, 60, LV_PART_MAIN);
  //^^make button barley visible to show it is there^^
  lv_obj_add_event_cb(mogoButton, mogoButtonHandler, LV_EVENT_CLICKED, NULL);
  //^^^registers the call back function to the button

  // Rings or mogo Button
  lv_obj_t * middleButton = lv_btn_create(pathingScreen); //create middle button
  lv_obj_set_size(middleButton, 30, 30);//set button size
  lv_obj_set_style_bg_color(middleButton, lv_color_black(), LV_PART_MAIN);
  //^^make button black^^
  lv_obj_set_style_bg_opa(middleButton, 60, LV_PART_MAIN); 
  //^^make button barley visible to show it is there^^
  /* Call Back for this button is set in alignButton*() becasue it is conditol on the path */

  // Ring Button
  lv_obj_t * ringButton = lv_btn_create(pathingScreen);//create ring button
  lv_obj_set_size(ringButton, 30, 30);//set button size
  lv_obj_set_style_bg_color(ringButton, lv_color_black(), LV_PART_MAIN);
  //^^make button black
  lv_obj_set_style_bg_opa(ringButton, 60, LV_PART_MAIN); 
  //^^make button barley visible to show it is there
  lv_obj_add_event_cb(ringButton, ringButtonHandler, LV_EVENT_CLICKED, NULL);
  //^^^registers the call back function to the button

  // Corner Button
  lv_obj_t * cornerButton = lv_btn_create(pathingScreen);//create corner button
  lv_obj_set_size(cornerButton, 30, 30);//set button size
  lv_obj_set_style_bg_color(cornerButton, lv_color_black(), LV_PART_MAIN);
  //^^make button black
  lv_obj_set_style_bg_opa(cornerButton, 60, LV_PART_MAIN);
  //^^make button barley visible to show it is there
  lv_obj_add_event_cb(cornerButton, cornerButtonHandler, LV_EVENT_CLICKED, NULL);
  //^^ registers the call back function to the button

  // Bar Button
  lv_obj_t * barButton = lv_btn_create(pathingScreen);//create bar button
  lv_obj_set_size(barButton, 30, 30);//set button size
  lv_obj_set_style_bg_color(barButton, lv_color_black(), LV_PART_MAIN);
  //^^make button black
  lv_obj_set_style_bg_opa(barButton, 60, LV_PART_MAIN);
  //^^make button barley visible to show it is there
  lv_obj_add_event_cb(barButton, barButtonHandler, LV_EVENT_CLICKED, NULL);
  //^^^registers the call back function to the button

  alignButton(allianceStakeButton, mogoButton,  ringButton, middleButton, cornerButton, barButton);
  //^^^ alligns the buttons depending on which portion of field auton is running  

  // Start Position Button, to change starting orientation of the robot
  lv_obj_t * startPositionButton = lv_btn_create(pathingScreen);
  lv_obj_set_size(startPositionButton, 100, 50);//set button size
  lv_obj_set_style_bg_color(startPositionButton, lv_color_hex(0x00FF00), LV_PART_MAIN); 
  //^^ set green default ^^

  if(!blueAuton)  lv_obj_align(startPositionButton, LV_ALIGN_BOTTOM_RIGHT, -10, -10);
  else            lv_obj_align(startPositionButton, LV_ALIGN_BOTTOM_LEFT, 10, -10);
  //^^^ alligns the button in either bottom right or left depending on the auton so it doesn't overlap

  lv_obj_add_event_cb(startPositionButton, startPositionButtonHandler, LV_EVENT_CLICKED, NULL);
  //^^ registers the call back function to the button

  lv_obj_t * label = lv_label_create(startPositionButton);//Putting text on the button
  lv_label_set_text(label, "Forwards"); //forwards by deafult
  lv_obj_center(label);//center text in middle of button 
}

//generates the pathing screen, handling all 4 casez
void alignButton(lv_obj_t *alliance_btn,lv_obj_t *mogo_btn, lv_obj_t *ring_btn, lv_obj_t *mid_btn, lv_obj_t *corner_btn, lv_obj_t *bar_btn){

  pathLabel = lv_label_create(pathingScreen); //initalizes pathLabel to show the path

  if(blueAuton && mogoAuton){ //aligns for blueLeft
    leftSide = true;//this is leftSide of field
    lv_obj_align(alliance_btn, LV_ALIGN_BOTTOM_RIGHT, -10, -145);//aligns button
    lv_obj_align(mogo_btn, LV_ALIGN_BOTTOM_RIGHT, -90, -90);
    lv_obj_align(mid_btn, LV_ALIGN_BOTTOM_RIGHT, -140, -45);
    lv_obj_align(ring_btn, LV_ALIGN_BOTTOM_RIGHT, -91, -45);
    lv_obj_align(corner_btn, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
    lv_obj_align(bar_btn, LV_ALIGN_BOTTOM_RIGHT, -110, -105);
    lv_obj_align(pathLabel, LV_ALIGN_TOP_LEFT, 10, 10);
    // ^^^ aligns all necceasry feutres
    lv_obj_add_event_cb(mid_btn, farMogoButtonHandler, LV_EVENT_CLICKED, NULL);
    //^^^registers middle button to Mogo becasue this is mogo side
  }
  else if(blueAuton && !mogoAuton){//aligns for blueRight
    leftSide = false; //this is not leftSide of field
    lv_obj_align(alliance_btn, LV_ALIGN_TOP_RIGHT, -10, 145);
    lv_obj_align(mogo_btn, LV_ALIGN_TOP_RIGHT, -90, 90);
    lv_obj_align(mid_btn, LV_ALIGN_TOP_RIGHT, -135, 40);
    lv_obj_align(ring_btn, LV_ALIGN_TOP_RIGHT, -91, 40);
    lv_obj_align(corner_btn, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_obj_align(bar_btn, LV_ALIGN_TOP_RIGHT, -128, 115);
    lv_obj_align(pathLabel, LV_ALIGN_TOP_LEFT, 10, 10);
    // ^^^ aligns all necceasry feutres
    lv_obj_add_event_cb(mid_btn, ringsButtonHandler, LV_EVENT_CLICKED, NULL);
    //^^^registers middle button to rings becasue this is ring side
  }
  else if(!blueAuton && !mogoAuton){//aligns for redLeft
    leftSide = true; //this is leftSide of field
    lv_obj_align(alliance_btn, LV_ALIGN_TOP_LEFT, 10, 145);
    lv_obj_align(mogo_btn, LV_ALIGN_TOP_LEFT, 90, 90);
    lv_obj_align(mid_btn, LV_ALIGN_TOP_LEFT, 145, 40);
    lv_obj_align(ring_btn, LV_ALIGN_TOP_LEFT, 83, 40);
    lv_obj_align(corner_btn, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_align(bar_btn, LV_ALIGN_TOP_LEFT, 125, 125);
    lv_obj_align(pathLabel, LV_ALIGN_TOP_RIGHT, -10, 10);
    // ^^^ aligns all necceasry feutres
    lv_obj_add_event_cb(mid_btn, ringsButtonHandler, LV_EVENT_CLICKED, NULL);
    //^^^registers middle button to rings becasue this is ring side
  }
  else{//aligns for redRight
    leftSide = false;//this is not leftSide of field
    lv_obj_align(alliance_btn, LV_ALIGN_BOTTOM_LEFT, 10, -145);
    lv_obj_align(mogo_btn, LV_ALIGN_BOTTOM_LEFT, 85, -90);
    lv_obj_align(mid_btn, LV_ALIGN_BOTTOM_LEFT, -50, -51);
    lv_obj_align(ring_btn, LV_ALIGN_BOTTOM_LEFT, 91, -45);
    lv_obj_align(corner_btn, LV_ALIGN_BOTTOM_LEFT, 0, 0);
    lv_obj_align(bar_btn, LV_ALIGN_BOTTOM_LEFT, 120, -120);
    lv_obj_align(pathLabel, LV_ALIGN_TOP_RIGHT, -10, 10);
    // ^^^ aligns all necceasry feutres
    lv_obj_add_event_cb(mid_btn, farMogoButtonHandler, LV_EVENT_CLICKED, NULL);
    //^^^registers middle button to Mogo becasue this is mogo side
  }
}


//6 below functions add the respective function to the path vector when respective button is clicked
void allianceStakeButtonHandler(lv_event_t * e) {
  // vvvv if on leftSide of field run leftSide function if not then rightSide function
  autonPath.push_back(leftSide ? leftAutonFunctions[0] : rightAutonFunctions[0]);
  updatePathLabel("Alliance Stake"); //updates the path label
}

void mogoButtonHandler(lv_event_t * e) {
  // vvvv if on leftSide of field run leftSide function if not then rightSide function
  autonPath.push_back(leftSide ? leftAutonFunctions[1] : rightAutonFunctions[1]);
  updatePathLabel("Mobile Goal");//updates the path label
}

void ringsButtonHandler(lv_event_t * e) {
  // vvvv if on leftSide of field run leftSide function if not then rightSide function
  autonPath.push_back(leftSide ? leftAutonFunctions[3] : rightAutonFunctions[3]);
  updatePathLabel(" Middle Rings");//updates the path label
}

void farMogoButtonHandler(lv_event_t * e) {
  // vvvv if on leftSide of field run leftSide function if not then rightSide function
  autonPath.push_back(leftSide ? leftAutonFunctions[4] : rightAutonFunctions[4]);
  updatePathLabel("Middle Mogo");//updates the path label
}

void ringButtonHandler(lv_event_t * e) {
  // vvvv if on leftSide of field run leftSide function if not then rightSide function
  autonPath.push_back(leftSide ? leftAutonFunctions[2] : rightAutonFunctions[2]);
  updatePathLabel("Ring");//updates the path label
}

void cornerButtonHandler(lv_event_t * e) {
  // vvvv if on leftSide of field run leftSide function if not then rightSide function
  autonPath.push_back(leftSide ? leftAutonFunctions[5] : rightAutonFunctions[5]);
  updatePathLabel("Corner");//updates the path label
}

void barButtonHandler(lv_event_t * e) {
  // vvvv if on leftSide of field run leftSide function if not then rightSide function
  autonPath.push_back(leftSide ? leftAutonFunctions[6] : rightAutonFunctions[6]);
  updatePathLabel("touch bar");//updates the path label
}

//callback functions for startButton which toggles wheter the robot is going forwards or backwards
void startPositionButtonHandler(lv_event_t * e) {
    startForward = !startForward; //toggles value when clicked
    lv_obj_t * btn = lv_event_get_target(e);
    if (startForward) {
        lv_obj_set_style_bg_color(btn, lv_color_hex(0x00FF00), LV_PART_MAIN); // Green for forwards
        lv_label_set_text(lv_obj_get_child(btn, 0), "Forwards"); //print forwards on the button
    } else {
        lv_obj_set_style_bg_color(btn, lv_color_hex(0xFF0000), LV_PART_MAIN); // Red for backwards
        lv_label_set_text(lv_obj_get_child(btn, 0), "Backwards");//print backwards on the button
    }
}

//the functions below load the differnt auton paths

void displayPaths(){ //displays the final pathing screen
  createPathButtons();
  lv_obj_t * img = lv_img_create(pathingScreen); //create an image object on pathing screen
  
  if(blueAuton && mogoAuton){//blue left case
    lv_img_set_src(img, &BlueLeft); //set the image to blue left 
    lv_obj_align(img,  LV_ALIGN_BOTTOM_RIGHT, 0, 0);
    
  }
  else if(blueAuton && !mogoAuton){//blue right case
    lv_img_set_src(img, &BlueRight);//set the image to blue right
    lv_obj_align(img, LV_ALIGN_TOP_RIGHT, 0,0);//align the image to the top right of the screen
  }
  else if(!blueAuton && !mogoAuton){//red left case
    lv_img_set_src(img, &RedLeft);//set the image to red left
    lv_obj_align(img, LV_ALIGN_TOP_LEFT, 0, 0);//align the image to the top left of the screen
  }
  else{//red right case
    lv_img_set_src(img, &RedRight);//set the image to red right
    lv_obj_align(img, LV_ALIGN_BOTTOM_LEFT, 0, 0); //align the image to the bottom left of the screen
  }
  lv_obj_move_background(img);//send image to back so buttons sit over it
  lv_scr_load_anim(pathingScreen, LV_SCR_LOAD_ANIM_OVER_BOTTOM, 500, 0, true);
  // ^^^ load the pathing screen and delete the previous screen to conserve RAM
}

//updates the current path on the brain screen
void updatePathLabel(std::string movement){//movement is the action the user is adding to the path
  count++;//increments the count of actions
  pathText += (std::to_string(count) + "." + movement + " \n"); //adds the movement to the path
  lv_label_set_text(pathLabel, pathText.c_str()); //updates the brain screen
}




/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */

void initialize() {
	lv_init(); //initalize  lvgl
	createScreens(); //create all the screens
	create_buttons();//create all menu buttons
  chassis.calibrate();
  imu.reset();
  Rotation rotation_sensor(10);
	mArm.set_gearing(pros::E_MOTOR_GEARSET_36);
	Clamper.set_value(LOW);
	mArm.set_brake_mode(MotorBrake::hold);
	Task armTask(moveArm, (void*)"PROS", TASK_PRIORITY_DEFAULT,
                TASK_STACK_DEPTH_DEFAULT,"controls arm tasks" );
    rotation_sensor.reset_position();
    rotation_sensor.set_position(0);
    rotation_sensor.reverse();
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




//FRONT OF THE ROBOT IS THE INTAKE



void autonomous() {
	mLefts.tare_position();//get rid of any error in the encoders
	mRights.tare_position();//get rid of any error in the encoders
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
	 
	mIntake.set_brake_mode(MotorBrake::coast);
	rotation_sensor.reset_position();
	rotation_sensor.set_position(0);

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
		}
		else{
			controller.set_text(1,1,"RED TEAM");
			blueAllaince = false;
		}
	}
	
	

	
		    

        lcd::print(2, "sensor: %d",rotation_sensor.get_position()/100.0);
	}
}