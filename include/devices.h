#ifndef DEVICES_H
#define DEVICES_H
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "pros/adi.hpp"
#include "pros/motors.hpp"
#include "pros/distance.hpp"
#include "pros/optical.hpp"


using namespace pros;
using namespace lemlib;


// ADI Ports
constexpr char CLAMP_PORT = 'B';

extern MotorGroup mLefts; // left motors forward (negitve)
extern Controller controller;
extern MotorGroup mRights; // right motors reverse (positve)
extern Motor mIntake;
extern Motor mArm;
extern Imu imu;
extern Distance sDist;
extern Optical sOpt;
extern adi::Port clamp; //even different than PROS + EZTemplate or PROS + OkapiLib
extern Drivetrain drivetrain;
extern OdomSensors sensors;
// lateral PID controller
extern ControllerSettings lateral_controller;
// angular PID controller
extern ControllerSettings angular_controller;
// input curve for throttle input during driver control
extern ExpoDriveCurve throttleCurve;
// input curve for steer input during driver control
extern ExpoDriveCurve steerCurve;
// create the chassis
extern Chassis chassis;
#endif 