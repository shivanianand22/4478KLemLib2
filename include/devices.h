#ifndef DEVICES_H
#define DEVICES_H
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "pros/adi.hpp"
#include "pros/motors.hpp"
#include "pros/distance.hpp"
#include "pros/optical.hpp"
#include "pros/rotation.hpp"
using namespace pros;
using namespace lemlib;
// ADI Ports
constexpr char CLAMP_PORT = 'A';

extern MotorGroup mLefts; // left motors forward (negitve)
extern Controller controller;
extern MotorGroup mRights; // right motors reverse (positve)
extern Motor mIntake;
extern MotorGroup mArm;
extern Imu imu;
extern Rotation rotation_sensor;
extern Distance sDist;
extern Optical sOpt;
extern adi::Port Clamper; //even different than PROS + EZTemplate or PROS + OkapiLib
extern Drivetrain drivetrain;
extern OdomSensors sensors;
extern Distance leftDist;
extern Distance rightDist;
extern Distance backDist;
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
extern PID armSetPid;
extern PID armScorePID;

#endif 