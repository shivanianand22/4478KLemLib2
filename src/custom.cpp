#include "devices.h"
#include "lemlib/chassis/chassis.hpp"
#include "actions.h"

bool startForward = true; //if true robot starts facing field


/*    Long time outs are used in all functions to ensure movement is not timed out. Robot end action early when it reaches the target*/
void setLeftPose(){
  //sets pose of robot to infront of the mogo
  chassis.setPose(-51.9,16 , (startForward ? 90 : 270)); // if startForward  is true robot facing field
}

void setRightPose(){
  //sets pose of robot to infront of the mogo
  chassis.setPose(-51.9,-16 , (startForward ? 90 : 270)); // if startForward  is true robot facing field
}

void leftAllainceStake() {
  //moves to allaince stake
  chassis.moveToPose(-63, 7.25, 225, 10000);
  chassis.waitUntilDone(); //waits until robot reaches target
  target = armTargets[3];//set arm to allaince position
  delay(500);//delay to ring gets scored before next action
}

void rightAllainceStake() {
  //moves to allaince stake
  chassis.moveToPose(-63, -7.25, 315, 10000);
  chassis.waitUntilDone();//waits until robot reaches target
  target = armTargets[3];//set arm to allaince position
  delay(500);//delay to ring gets scored before next action
}

void leftMogo() {
  //moves to mogo
  chassis.moveToPose(-31.16,  23.792, 270, 10000, {.forwards = false});
  chassis.waitUntilDone();//waits until robot reaches target
  grab();//grabs mogo
}

void leftRing() {
  //moves to ring
  chassis.moveToPoint(-23.553, 46.674, 10000);
}

void leftRings() {
  //move to the right of stack of rings
  chassis.moveToPose(-29, 24, 65,10000);
  chassis.moveToPose(-7.66,55,360, 4000, {.lead=.6});//curves to pick up rings
  chassis.swingToHeading(250, DriveSide::LEFT, 1000);//locked turn to avoid crossing line
}

void leftFarMogo() {
  //moves to middle mogo
  chassis.moveToPose(-5, 46, 270, 100000, {.forwards = false});
  chassis.waitUntilDone();//waits until robot reaches target
  grab();//grabs mogo
}

void leftCorner() {\
  //sets goal in corner 
  chassis.moveToPose(-62, 52, 135, 100000, {.forwards = false});
  release();//releases mogo
}

void leftBar(){ 
  //moves to bar
  chassis.moveToPose(-14.5, 10, 180, 10000);
  chassis.waitUntilDone();//waits until robot reaches target
  target = armTargets[2];//raises armto touch bar
}

void RightMogo() {
  //moves to mogo
  chassis.moveToPose(-31.16,  -23.792, 270, 10000, {.forwards = false});
  chassis.waitUntilDone();//waits until robot reaches target
  grab();//grabs mogo
}

void RightRing() {
  //moves to ring
  chassis.moveToPoint(-23.553, 46.674, 10000);
}

void RightRings() {
  //moves to move to left of stack of rings
  chassis.moveToPose(-29, -24, 115, 10000);
  chassis.moveToPose(-7.66,-55,180, 4000, {.lead=.6});//curves to pick up rings
  chassis.swingToHeading(300, DriveSide::RIGHT, 1000);//locked turn to avoid crossing line
}

void RightFarMogo() {
  //moves to middle mogo
  chassis.moveToPose(-5, -46, 270, 100000);
  chassis.waitUntilDone();//waits until robot reaches target
  grab();//grabs mogo
}

void RightCorner() {
  //sets goal in corner
  chassis.moveToPose(-62, -52, 45, 100000, {.forwards = false});
  release();//releases mogo
}

void RightBar() {
  //moves to bar
  chassis.moveToPose(-14.5, -10, 0, 10000);
  chassis.waitUntilDone();//waits until robot reaches target
  target = armTargets[2];//raises arm to touch bar
}

void (*leftAutonFunctions[])() = {leftAllainceStake, leftMogo, leftRing, leftRings, leftFarMogo, leftCorner, leftBar};
void (*rightAutonFunctions[])() = {rightAllainceStake, RightMogo, RightRing, RightRings, RightFarMogo, RightCorner, RightBar};
//function pointer array to the functions for the left and right side of the fields

