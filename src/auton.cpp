#include "devices.h"
#include "lemlib/chassis/chassis.hpp"
#include "main.h" 
#include "liblvgl/llemu.hpp"
#include "actions.h"
#include "pros/abstract_motor.hpp"
#include "pros/motors.h"
#include "pros/rtos.h"
#include <cstdio>
#include <ctime>
int selection = 1;
bool cornerMogo= false;
double startTime = 0;
/*
 0 = RedRight Mogo
 1 = RedLeft Ring
 2 = BlueLeft Mogo
 3 = BlueRight Ring
 4 = Prog skills
 5 = Red Goal Rush
 6 = Blue Goal Rush
*/
void placeMogoToggle(){
	cornerMogo = !cornerMogo;
    if(cornerMogo){
        pros::lcd::set_text(3, "Baby going into the corner");
    }
    else{
        pros::lcd::set_text(3, "Robot touching bar");
    }
}

void autonSelector(){
    if(selection <= 7){
        selection++;
    } else {
        selection = 0;
    }
    switch(selection){
    case 0:
        pros::lcd::set_text(2, "RedRight(MOGO Side)");
        break;
    case 1:
        pros::lcd::set_text(2, "RedLeft(Ring Side)");
        break;
    case 2:
        pros::lcd::set_text(2, "BlueLeft(MOGO side)");
        break;
    case 3:
        pros::lcd::set_text(2, "BlueRight(Ring Side)");
        break;
    case 4:
        pros::lcd::set_text(2, "Prog Skills");
        break;
    case 5:
        pros::lcd::set_text(2, "Red Goal Rush");
        break;
    case 6:
        pros::lcd::set_text(2, "Blue Goal Rush");
        break;
    case 7:
        pros::lcd::set_text(2, "Red 4 ring");
        break;
    case 8:
        pros::lcd::set_text(2, "Blue 4 ring");
        break;
    }
}
void reverseAutonSelector(){
    if(selection == 0){
        selection = 7;
    }
    else if(selection ==1){
        selection =8;
    }
    else{
        selection -=2;
    }
    autonSelector();
}
void redRight(){ //mogo side
    
}

void blueLeft(){ //mogo side
    
}

// while(1){
    //     delay(20);
    //     lcd::print(3, "x: %.4f, y: %.4f, theta: %.4f", chassis.getPose().x, chassis.getPose().y, chassis.getPose().theta);
    // }
void redLeft(){ //ring side done
    chassis.setPose(-52.167, 23.792, 270);
    chassis.moveToPoint(-28.16,  23.792, 1000, {.forwards = false});//mogo
    chassis.waitUntilDone();
    delay(50);
    grab();
    mIntake.move(127);
    delay(50); 
    chassis.turnToHeading(65, 500);
    chassis.moveToPose(-9.2,55,360, 4000, {.lead=.6,});//rings
    chassis.swingToHeading(190, DriveSide::LEFT, 1000);
    chassis.waitUntilDone();
    chassis.moveToPoint(-28.8, 40, 500);
    loadLB=true;
    chassis.moveToPose(-61.456, 8.013, 225, 2500);
    chassis.waitUntilDone();
    target = armTargets[3];
    delay(500);
    chassis.moveToPoint(-56.456, 13.013, 1000, {.forwards = false});
    chassis.turnToHeading(110, 500);
    chassis.moveToPoint(-20, 14, 3000); //touch bar
    chassis.waitUntilDone();
    target = armTargets[2];
}


void blueRight(){ //ring side
    chassis.setPose(-52.167, -23.792, 270);
    mIntake.move(127); 
    chassis.moveToPoint(-28.16,  -23.792, 1000, {.forwards = false});//mogo
    chassis.waitUntilDone();
    grab();
    chassis.turnToHeading(115, 500);
    chassis.moveToPose(.5,-55,180, 4000, {.lead=.6});//rings
    chassis.swingToHeading(350, DriveSide::RIGHT, 1000);
    chassis.moveToPoint(-20.8, -40, 500); //ring
    chassis.moveToPose(-61.456, -8.013, 225, 2500);
    chassis.waitUntilDone();
    target = armTargets[3];
    delay(500);
    chassis.moveToPoint(-59.456, -10.013, 150);
    chassis.turnToHeading(70, 500);
    chassis.moveToPoint(-20, -14, 3000); //touch bar
    chassis.waitUntilDone();
    target = armTargets[2];
}

void progSkills(){
    target=47;
    mArm.set_brake_mode(MotorBrake::brake);
    chassis.setPose(-61.317, 7.863, 225);
    target = armTargets[3];//scores allaince stake
    mIntake.move(127);
    delay(500);
    chassis.moveToPoint(-54.67, 12.81, 250, {.forwards= false,.minSpeed = 50});//grab mogo
    chassis.waitUntilDone();
    grab();
    chassis.moveToPoint(-34.38, 17.37,  1500, {.minSpeed =80});//grab ring
    chassis.moveToPose(-3, 39, 45, 1000, {.minSpeed = 100, .earlyExitRange = 6}); //avoid the middleBar
    chassis.waitUntilDone();
    loadLB= true; //next ring going into lb
    chassis.moveToPose(23, 43.1, 90,2000);//pick up ring
    chassis.moveToPoint(1, 30, 1000, {.forwards= false});//allign with wall stake
    chassis.turnToHeading(0, 700);//turn towards stake
    chassis.waitUntilDone();
    chassis.moveToPose(chassis.getPose().x, 59,0,3500);//move to stake
    delay(100);
    target= 70; //get ring away from Intake
    delay(20);
    mIntake.move_relative(-50, 100);
    delay(100);
    mIntake.move(127);
    delay(1000);
    target = armTargets[2] + 30;//moves up lb
    delay(100);
    chassis.waitUntilDone();
    chassis.setPose(0,62.7,0); //reset pose after alligning with wall stake
    //checkpoint can be used for testing
    chassis.moveToPoint(0, 47.5, 1000, {.forwards= false});
    target = armTargets[0];
    chassis.turnToHeading(270, 500);
    chassis.moveToPoint(-60., 47.3, 3500, {.maxSpeed = 50});//pick up the three rings
    chassis.turnToPoint(-49.8, 60, 400); 
    chassis.moveToPoint(-49.8, 60, 1000); //pick up last ring
    chassis.turnToHeading(110, 500);
    chassis.moveToPoint(-52, 61.7, 600, {.forwards = false});
    chassis.waitUntilDone();
    release(); //drop 1st goal in the corner


    chassis.moveToPoint(-41.1, 58.7, 300);
    chassis.moveToPose(-55, -13.3,0, 4000,{.forwards=false});//pick up second mogo
    chassis.waitUntilDone();
    grab();
    chassis.moveToPoint(-27, -24, 1000);//pick up ring
    chassis.waitUntilDone();
    chassis.moveToPose(19.7, -47.4,90, 2500); //pick up lb ring
    delay(1000);
    loadLB=true;
    chassis.moveToPoint(-9.5, -40.32, 1500, {.forwards=false});  
    chassis.turnToHeading(180, 500);//line up with the stake
    chassis.waitUntilDone();
    chassis.moveToPoint(chassis.getPose().x, -66, 2000);
    target= 70; //get ring away from Intake
    delay(20);
    mIntake.move_relative(-50, 100);
    delay(100);
    mIntake.move(127);
    delay(1000);
    target = armTargets[2] + 30;//moves up lb
    delay(100);
    chassis.waitUntilDone();
    chassis.setPose(0,-62.7,180); //reset pose after alligning with wall stake
    //checkpoint can be used for testing
    chassis.moveToPoint(0, -48.5, 1000, {.forwards= false});
    chassis.turnToHeading(270, 500);
    target = armTargets[0];
    chassis.moveToPoint(-60, chassis.getPose().y , 2500, {.maxSpeed=70});//pick up the three rings
    chassis.turnToPoint(-51.8, -55, 400); 
    chassis.moveToPoint(-51.8, -55, 1000); //pick up last ring
    chassis.turnToHeading(70, 500);
    chassis.moveToPoint(-52, -59.7, 600, {.forwards = false});
    chassis.waitUntilDone();
    release(); //drop 2nd goal in the corner

    mIntake.set_brake_mode(MotorBrake::coast);
    chassis.moveToPose(-23, -23,45, 1000,{.minSpeed= 100, .earlyExitRange=4});
    chassis.waitUntilDone();
    mIntake.brake();
    chassis.moveToPoint(23.7, 23.7, 3000);
    chassis.waitUntil(50);
    loadLB= true;// hopefully this picks up both rings and puts one in the lb
    chassis.turnToHeading(315, 500);
    chassis.moveToPoint(40.87, 8.1, 1500, {.forwards = false});
    chassis.waitUntilDone();
    grab();
    chassis.moveToPoint(58, 0, 1000);
    chassis.turnToPoint(69, 4, 500);
    chassis.waitUntilDone();
    target= armTargets[3]; //score allaince stake
    delay(500);
    mIntake.move(127);
    chassis.moveToPoint(54, 0, 500, {.forwards= false});
    chassis.turnToPoint(25, -22.5, 500);
    chassis.moveToPoint(25, -22.5, 1000); //pick up ring
    chassis.moveToPose(49.3, -47.7, 90, 2000);//pick up ring
    chassis.turnToPoint(47.7, -58, 500);
    chassis.moveToPoint(47.7, -58, 1000);
    chassis.turnToHeading(280, 500);
    chassis.moveToPoint(55, -58.8, 600, {.forwards= false}); //put mogo in the corner

    chassis.moveToPose(42, -25, 0, 1000, {.minSpeed = 110, .earlyExitRange = 6});
    chassis.moveToPose(56, 12.6, 0, 1000, {.minSpeed = 110, .earlyExitRange = 6});
    chassis.moveToPoint(62, 58, 1000);






    
}
void redRush(){
    controller.rumble(".");
    target = rotation_sensor.get_position()/100.0;
    chassis.setPose(-55.5, -64.1, 90);
    chassis.moveToPose(-17., -57.5, 50, 1000, {.forwards= true, .lead = 0.35,.minSpeed = 100,.earlyExitRange = 8});//get to mogo 
    delay(500);
    target= armTargets[3];
    chassis.waitUntilDone();
    chassis.moveToPoint(-32, -63, 800, {.forwards= false, .minSpeed = 127});//bring mogo back to our side
    delay(200);
    target=armTargets[0];
    mIntake.move(127);
    chassis.waitUntilDone();
    chassis.setPose(chassis.getPose().x+6, chassis.getPose().y, chassis.getPose().theta);
    mIntake.move(127);
    chassis.turnToPoint(-19, -50, 1000);
    chassis.waitUntilDone();
    chassis.moveToPoint(-19, -50, 2000); //pick up ring
    delay(1800);
    mIntake.brake();
    chassis.turnToHeading(164, 1000);
    chassis.moveToPose(-21, -29.5, 180, 2000, {.forwards = false});//get to mogo
    chassis.waitUntilDone();
    grab();
    mIntake.move(127);
    chassis.moveToPose(-18, 1.98, 160, 3000, {.forwards = true}); //touch bar
    target = armTargets[2];
}
void blueRush(){
    target = rotation_sensor.get_position()/100.0;
    chassis.setPose(-55.5, 64.1, 90);
    chassis.moveToPose(-16.6, 58.5, 120, 930, {.forwards= true, .lead = 0.35,.minSpeed = 100,.earlyExitRange = 8});//get to mogo 
    delay(500);
    target= armTargets[3];
    chassis.waitUntilDone();
    chassis.moveToPoint(-32, 63, 800, {.forwards= false, .minSpeed = 127});//bring mogo back to our side
    delay(200);
    target=armTargets[0];
    mIntake.move(127);
    chassis.waitUntilDone();
    chassis.setPose(chassis.getPose().x+6, chassis.getPose().y, chassis.getPose().theta);
    mIntake.move(127);
    chassis.turnToPoint(-19, 50, 1000);
    chassis.waitUntilDone();
    chassis.moveToPoint(-19, 50, 2000); //pick up ring
    delay(1800);
    mIntake.brake();
    chassis.turnToHeading(16, 1000);
    chassis.moveToPose(-21, 34.5, 0, 2000, {.forwards = false});//get to mogo
    chassis.waitUntilDone();
    grab();
    mIntake.move(127);
    
        chassis.turnToHeading(90, 500);
        chassis.moveToPose(-18, 1.98, 160, 3000, {.forwards = true}); //touch bar
        target = armTargets[2];
    

}
void blue4Ring(){
    chassis.setPose(-61.317, -7.863, 315);
    target = armTargets[3];//scores allaince stake
    delay(600);
    chassis.moveToPose(-34.38, -18.37, 270, 3000, {.forwards = false}); //mogo
    chassis.waitUntilDone();
    grab();
    lcd::print(1, "x: %.4f, y: %.4f, thetdda: %.4f", chassis.getPose().x, chassis.getPose().y, chassis.getPose().theta);
    loadLB = true;
    chassis.turnToHeading(250, 600);
    chassis.moveToPoint(-22, -47, 2000);//ring
    chassis.moveToPose(-11.18, -59.11, 205, 2500);//wall stake
    chassis.waitUntilDone();
    while(loadLB && (millis() - startTime)/1000 < 10){delay(20);}
    if(loadLB){loadLB = false;} //if it got timed out end the loading process
    target = armTargets[2];
    delay(1000);
    target = armTargets[0];
    mIntake.move(127);
    chassis.turnToHeading(50, 500);
    chassis.moveToPose(-6.5, -40, 0, 1500, {.lead= .2});
    chassis.moveToPoint(-10.83, -7.79, 1000);
    chassis.waitUntilDone();
    target= armTargets[2];
}
void red4Ring(){//done
    chassis.setPose(-61.317, 7.863, 225);
    target = armTargets[3];//scores allaince stake
    delay(600);
    chassis.moveToPose(-34.38, 17.37, 270, 3000, {.forwards = false});
    chassis.waitUntilDone();
    grab();
    loadLB = true;
    chassis.turnToHeading(290, 600);
    chassis.moveToPoint(-22, 47, 2000);//ring
    chassis.moveToPose(-11.18, 56.11, 55, 2500);//wall stake
    chassis.waitUntilDone();
    while(loadLB && (millis() - startTime)/1000 < 10){delay(20);}
    if(loadLB){loadLB = false;} //if it got timed out end the loading process
    target = armTargets[2];
    delay(1000);
    target = armTargets[0];
    mIntake.move(127);
    chassis.turnToHeading(130, 500);
    chassis.moveToPose(-6.5, 40, 540, 1500, {.lead= .2});
    chassis.moveToPoint(-10.83, 7.79, 1000);
    chassis.waitUntilDone();
    target= armTargets[2];

    
}