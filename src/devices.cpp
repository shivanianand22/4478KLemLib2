#include "devices.h"
#include "pros/distance.hpp"
using namespace pros;
using namespace lemlib;
Controller controller(E_CONTROLLER_MASTER);
MotorGroup mLefts({-7, -9, -20}); // left motors forward (negitve)
MotorGroup mRights({5, 6, 10}); // right motors reverse (positve)
Motor mIntake(-4, pros::MotorGearset::blue);
MotorGroup mArm({-1, 2});

Imu imu(16);
Distance sDist(19);
Optical sOpt(14);
adi::Port Clamper ('A', E_ADI_DIGITAL_OUT); //even different than PROS + EZTemplate or PROS + OkapiLib
Rotation rotation_sensor(3);
Distance leftDist(4);
Distance rightDist(5);
Distance backDist(6);
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
ControllerSettings lateral_controller(8, // proportional gain (kP)
                                              0, // integral gain (kI)
                                              3, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in inches
                                              500, // small error range timeout, in milliseconds
                                              2, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
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

PID armSetPid(3,0.00 ,0.0,0);
PID armScorePID(2,0,0.1,0);