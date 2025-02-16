#include "devices.h"
#include "lemlib/chassis/chassis.hpp"
#include "actions.h"
bool startForward = true;
void setLeftPose(){
  chassis.setPose(-51.9,16 , (startForward ? 90 : 270)); // if startForward  is true robot facing field
}
void setRightPose(){
  chassis.setPose(-51.9,-16 , (startForward ? 90 : 270));
}
void leftAllainceStake() {
  chassis.moveToPose(-63, 7.25, 225, 10000);
  //^^long timeouts becasue to ensure robot reaches the target. When target ends action ends anyway
  chassis.waitUntilDone();
  target = armTargets[3];//set arm to allaince position
}

void rightAllainceStake() {
  chassis.moveToPose(-63, -7.25, 315, 10000);
  chassis.waitUntilDone();
  target = armTargets[3];
}

void leftMogo() {
  chassis.moveToPose(-31.16,  23.792, 270, 10000, {.forwards = false});
  chassis.waitUntilDone();
  grab();
}

void leftRing() {
  chassis.moveToPoint(-23.553, 46.674, 10000);
}

void leftRings() {
  chassis.moveToPose(-29, 24, 65,10000);
  chassis.moveToPose(-7.66,55,360, 4000, {.lead=.6});//rings
  chassis.swingToHeading(250, DriveSide::LEFT, 1000); 
}

void leftFarMogo() {//goes for middle mogo
  chassis.moveToPose(-5, 46, 270, 100000);
  chassis.waitUntilDone();
  grab();    
}

void leftCorner() {//sets goal in corner to end auton
  chassis.moveToPose(-62, 52, 135, 100000, {.forwards = false});
}

void leftBar() {//touches bar 
  chassis.moveToPose(-14.5, 10, 180, 10000);
  chassis.waitUntilDone();
  target = armTargets[2];
}

void RightMogo() {
  chassis.moveToPose(-31.16,  -23.792, 270, 10000, {.forwards = false});
  chassis.waitUntilDone();
  grab();
}

void RightRing() {
  chassis.moveToPoint(-23.553, 46.674, 10000);
}

void RightRings() {
  chassis.moveToPose(-29, -24, 115, 10000);
  chassis.moveToPose(-7.66,-55,180, 4000, {.lead=.6});//rings
  chassis.swingToHeading(300, DriveSide::RIGHT, 1000);
}

void RightFarMogo() {
  chassis.moveToPose(-5, -46, 270, 100000);
  chassis.waitUntilDone();
  grab();
}

void RightCorner() {
  chassis.moveToPose(-62, -52, 45, 100000, {.forwards = false});
}

void RightBar() {
  chassis.moveToPose(-14.5, -10, 0, 10000);
  chassis.waitUntilDone();
//  target = armTargets[2];
}

void (*leftAutonFunctions[])() = {leftAllainceStake, leftMogo, leftRing, leftRings, leftFarMogo, leftCorner, leftBar};
void (*rightAutonFunctions[])() = {rightAllainceStake, RightMogo, RightRing, RightRings, RightFarMogo, RightCorner, RightBar};
//function pointer array to the functions for the left and right side of the fields

