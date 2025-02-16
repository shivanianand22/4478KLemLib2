#include "devices.h"
#include "lemlib/chassis/chassis.hpp"
#include "main.h" 
#include "liblvgl/llemu.hpp"
#include "actions.h"
#include "pros/rtos.h"
#include <cstdio>
#include <ctime>
int selection = 6;
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
    mIntake.move(127); 
    chassis.moveToPoint(-31.16,  23.792, 1000, {.forwards = false});//mogo
    chassis.waitUntilDone();
    grab();
    chassis.turnToHeading(65, 500);
    chassis.moveToPose(-7.66,55,360, 4000, {.lead=.6});//rings
    chassis.swingToHeading(190, DriveSide::LEFT, 1000);
    chassis.moveToPoint(-28.8, 40, 500);
    chassis.moveToPoint(-36, 14, 3000); //touch bar
    chassis.waitUntilDone();
    target = armTargets[2];
}


void blueRight(){ //ring side
    chassis.setPose(-52.167, -23.792, 270);
    mIntake.move(127); 
    chassis.moveToPoint(-31.16,  -23.792, 1000, {.forwards = false});//mogo
    chassis.waitUntilDone();
    grab();
    chassis.turnToHeading(115, 500);
    chassis.moveToPose(-7.66,-55,180, 4000, {.lead=.6});//rings
    chassis.swingToHeading(350, DriveSide::RIGHT, 1000);
    chassis.moveToPoint(-28.8, -40, 500);
    chassis.moveToPoint(-36, -14, 3000); //touch bar
    chassis.waitUntilDone();
    target = armTargets[2];
}

void progSkills(){
    controller.set_text(1, 1, "running skills");
    mLefts.tare_position();
    mRights.tare_position();
    chassis.setPose(-64.241, -0.705, 90);
    mIntake.move(-280); //score on alliance stake
    delay(1000);
    chassis.moveToPoint(-60.241, 1.705, 1000);
    chassis.waitUntilDone();
    release();
    chassis.moveToPose(-46.897, 19.678, 160, 3000, {.forwards = false}); //go to goal
    chassis.waitUntilDone();
    delay(100);
    grab();
    mIntake.move(-127);
    chassis.moveToPoint(-22.593, 28.722, 1500);//pick up first ring
    chassis.moveToPose(-20.912, 49.303, 270, 3000); //go to 2nd ring
    chassis.moveToPose(-39.912, 49.953, 270, 2000); //pick up 2nd ring
    chassis.moveToPoint(-58.972, 49.622, 2000); //3rd & 4th ring
    chassis.moveToPose(-58.257, 64.489, 325, 1000, {.forwards = false});//drop 1st goal into corner
    chassis.waitUntilDone();
    controller.clear();
    lcd::print(1, "θ:%f, x: %f, y: %f", chassis.getPose().theta, chassis.getPose().x, chassis.getPose().y);
    release();
    chassis.setPose(-49, 55.000, chassis.getPose().theta);
    delay(100);
    lcd::print(2, "θ: %f, x: %f, y: %f", chassis.getPose().theta, chassis.getPose().x, chassis.getPose().y);
     
    //go towards 2ng Mogo
   // chassis.moveToPoint(-55.674, -0.515, 2000, {.forwards = false});
    chassis.moveToPoint(-32, 41 , 500);
    chassis.moveToPose(-47.5, -16.713, 0, 3500, {.forwards=false});
    chassis.waitUntilDone();
    chassis.setPose(-47.5,-28.713,0);
    delay(500);
    grab(); //grab 2nd mogo
    delay(100);
    chassis.moveToPoint(-16.593, -23.595, 2000); //1st ring
    chassis.moveToPoint(-16.593, -47, 1000);
    chassis.moveToPose(-39.936, -48.9,270,   2000);//2nd ring
    chassis.moveToPose(-60, -48.9,270,  4000); //3rd & 4th ring
    chassis.waitUntilDone();
    chassis.setPose(-62.8, -46.8, 270);
    chassis.moveToPose(-61.957, -63.014,40, 4000, {.forwards = false}); //drop off 2nd mogo in corner
    chassis.waitUntilDone();
    release(); 
    printf("time %i",c::millis()/1000);
    chassis.setPose(-62.5, -62.5, 40);
    delay(200);
    chassis.moveToPoint(6.113, -57.276, 2000);//pick up ring
    chassis.waitUntilDone();
    mIntake.brake();
    chassis.moveToPose(44, -0.5, 180, 5000, {.forwards=false});
    chassis.waitUntilDone();
    chassis.setPose(47, -0.5, 180);
    grab();
    mIntake.move(-127);
    delay(2000); //score ring
    release();
    //start pushing in goals
    chassis.moveToPoint(56.773, 15, 1000);
    chassis.moveToPoint(63.363, 66.554, 2000);
    chassis.moveToPoint(66.271,-16.632, 5000 );
    chassis.moveToPose(67.165, -64.562, -45, 5000);//push in last goal
    release();
    
}
void redRush(){
    target = rotation_sensor.get_position()/100.0;
    chassis.setPose(-55.5, -64.1, 90);
    chassis.moveToPose(-16.6, -58.5, 65, 1800, {.forwards= true, .lead = 0.35,.minSpeed = 100,.earlyExitRange = 8});//get to mogo 
    delay(500);
    target= armTargets[3];
    chassis.waitUntilDone();
    chassis.moveToPoint(-32, -63, 800, {.forwards= false, .minSpeed = 127});//bring mogo back to our side
    delay(200);
    target=armTargets[0];
    mIntake.move(127);
    chassis.waitUntilDone();
    mIntake.move(127);
    chassis.turnToPoint(-19, -50, 1000);
    chassis.waitUntilDone();
    chassis.moveToPoint(-19, -50, 2000); //pick up ring
    delay(1800);
    mIntake.brake();
    chassis.turnToHeading(164, 1000);
    chassis.moveToPose(-21, 34.5, 0, 2000, {.forwards = false});//get to mogo
    chassis.waitUntilDone();
    grab();
    mIntake.move(127);
    chassis.turnToHeading(90, 500);
    chassis.moveToPose(-18, 1.98, 160, 3000, {.forwards = true}); //touch bar
    target = armTargets[2];
}
void blueRush(){
    target = rotation_sensor.get_position()/100.0;
    chassis.setPose(-55.5, 64.1, 90);
    chassis.moveToPose(-16.6, 58.5, 115, 1800, {.forwards= true, .lead = 0.35,.minSpeed = 100,.earlyExitRange = 8});//get to mogo 
    delay(500);
    target= armTargets[3];
    chassis.waitUntilDone();
    chassis.moveToPoint(-32, 63, 800, {.forwards= false, .minSpeed = 127});//bring mogo back to our side
    delay(200);
    target=armTargets[0];
    mIntake.move(127);
    chassis.waitUntilDone();
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
    chassis.setPose(-60.317, -8.863, 315);
    target = armTargets[3];//scores allaince stake
    delay(600);
    chassis.moveToPose(-34.38, -17.37, 270, 3000, {.forwards = false}); //mogo
    chassis.waitUntilDone();
    grab();
    lcd::print(1, "x: %.4f, y: %.4f, thetdda: %.4f", chassis.getPose().x, chassis.getPose().y, chassis.getPose().theta);
    loadLB = true;
    chassis.turnToHeading(250, 600);
    chassis.moveToPoint(-22, -47, 2000);//ring
    chassis.moveToPose(-11.18, -56.11, 133, 2500);//wall stake
    chassis.waitUntilDone();
    while(loadLB && (millis() - startTime)/1000 < 10){delay(20);}
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
    chassis.setPose(-60.317, 8.863, 225);
    target = armTargets[3];//scores allaince stake
    delay(600);
    chassis.moveToPose(-34.38, 17.37, 270, 3000, {.forwards = false});
    chassis.waitUntilDone();
    grab();
    lcd::print(1, "x: %.4f, y: %.4f, thetdda: %.4f", chassis.getPose().x, chassis.getPose().y, chassis.getPose().theta);
    loadLB = true;
    chassis.turnToHeading(290, 600);
    chassis.moveToPoint(-22, 47, 2000);//ring
    chassis.moveToPose(-11.18, 56.11, 47, 2500);//wall stake
    chassis.waitUntilDone();
    while(loadLB && (millis() - startTime)/1000 < 10){delay(20);}
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