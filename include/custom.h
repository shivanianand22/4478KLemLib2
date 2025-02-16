#ifndef CUSTOM_H
#define CUSTOM_H

#include "lemlib/chassis/chassis.hpp"

// Function declarations
void setLeftPose();
void setRightPose();
void leftAllainceStake();
void rightAllainceStake();
void leftMogo();
void leftRing();
void leftRings();
void leftFarMogo();
void leftCorner();
void leftBar();
void RightMogo();
void RightRing();
void RightRings();
void RightFarMogo();
void RightCorner();
void RightBar();
extern bool startForward;
// Array declarations
extern void (*leftAutonFunctions[])();
extern void (*rightAutonFunctions[])();

#endif // CUSTOM_H