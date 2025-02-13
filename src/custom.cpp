#include "devices.h"
void setLeftPose(){
  double leftWallDist= leftDist.get()/25.4; //gets dist from wall in inches
  double backWallDist= backDist.get()/25.4;
  chassis.setPose(-backWallDist,70 - leftWallDist , 0);
}
void setRightPose(){
  double rightWallDist= rightDist.get()/25.4;//gets dist from wall in inches
  double backWallDist= backDist.get()/25.4;
  chassis.setPose(-backWallDist,-70 + rightWallDist , 0);
}
void leftAllainceStake() {
     
}

void rightAllainceStake() {

}

void leftMogo() {
     
}

void leftRing() {
     
}

void leftRings() {
     
}

void leftFarMogo() {
     
}

void leftCorner() {
     
}

void leftBar() {
     
}

void RightMogo() {
     
}

void RightRing() {
     
}

void RightRings() {
     
}

void RightFarMogo() {
     
}

void RightCorner() {
     
}

void RightBar() {
     
}

void (*leftAutonFunctions[])() = {leftAllainceStake, leftMogo, leftRing, leftRings, leftFarMogo, leftCorner, leftBar};
void (*rightAutonFunctions[])() = {rightAllainceStake, RightMogo, RightRing, RightRings, RightFarMogo, RightCorner, RightBar};
// ...existing code...
