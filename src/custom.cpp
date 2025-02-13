#include "devices.h"
bool startForward = true;
void setLeftPose(){
  chassis.setPose(-50.636,23.701 , (startForward ? 90 : 270)); // if startForward  is true robot facing field
}
void setRightPose(){
  chassis.setPose(-50.636,-23.701 , (startForward ? 90 : 270));
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
