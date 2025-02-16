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
    chassis.setPose(-51.9, -16.0, 90);
    chassis.moveToPoint(-54.5,  -0.2, 1900, {.forwards = false});
	chassis.waitUntilDone();
	chassis.setPose(-54.5,  -0.2, chassis.getPose().theta);
    chassis.moveToPose(-60.8,  -0.2, 90, 1650, {.forwards = false});
    chassis.waitUntilDone();
    delay(100);
    mIntake.move(-220);
    chassis.setPose(-63.5, -0.2, 90);
    delay(900); //score alliance stake
    chassis.moveToPoint(-47.938, -15.951, 1000);
    chassis.moveToPose(-27.5, -24.999, -55, 2800, {.forwards = false}); //get to mogo
    chassis.waitUntilDone();
    delay(75);
    grab();
    delay(75);
	chassis.setPose(-28.5,  -23.999, chassis.getPose().theta);
    chassis.moveToPoint(-19.5, -61.914, 2800);
    chassis.moveToPose(-18, -1.98, 20, 3000); //touch bar
}

void blueLeft(){ //mogo side
    chassis.setPose(-51.9, 16.0, 90);
    chassis.moveToPoint(-54.5,  0.2, 1200, {.forwards = false});
    chassis.moveToPose(-63.5,  0.2, 90, 1500, {.forwards = false});
    chassis.waitUntilDone();
    delay(100);
    mIntake.move(-220);
    delay(1100); //score alliance stake
    chassis.setPose(-63.5,  0.2, chassis.getPose().theta);
    chassis.moveToPoint(-47.938, 15.951, 1000);
    chassis.moveToPose(-28.5, 23.999, -145, 2000, {.forwards = false}); //get to mogo
    chassis.waitUntilDone();
    delay(75);
    grab();
    delay(75);
    chassis.setPose(-28.5,  23.999, chassis.getPose().theta);
    chassis.moveToPoint(-26.0, 57, 1900);//pickup ring to put on Mogo
    chassis.moveToPoint(-21.0, 52.114, 1000, {.forwards=false});//quickly backup to avoid 2nd ring
    chassis.moveToPose(-17, 7, 180,3000); //touch bar
}


void redLeft(){ //ring side done
    chassis.setPose(-52.167, 23.792, 270);
    mIntake.move(127); 
    // while(1){
    //     delay(20);
    //     lcd::print(3, "x: %.4f, y: %.4f, theta: %.4f", chassis.getPose().x, chassis.getPose().y, chassis.getPose().theta);
    // }
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
    chassis.setPose(-51.9, -16.0, 90);
    chassis.moveToPoint(-54.5,  -0.2, 1900, {.forwards = false});
	chassis.waitUntilDone();
	chassis.setPose(-54.5,  -0.2, chassis.getPose().theta);
    chassis.moveToPose(-59.5,  -0.2, 90, 1500, {.forwards = false});
    chassis.waitUntilDone();
    delay(100);
    mIntake.move(-220);
    chassis.setPose(-63.5, -0.2, 90);
    delay(900); //score alliance stake
    chassis.moveToPoint(-47.938, -15.951, 1000);
    chassis.moveToPose(-28.5, -23.999, -55, 2200, {.forwards = false}); //get to mogo
    chassis.waitUntilDone();
    delay(75);
    grab();
    delay(75);
	chassis.setPose(-28.5,  -23.999, chassis.getPose().theta);
    chassis.moveToPoint(-19.5, -61.914, 2800);//pickup ring to put on Mogo
    chassis.moveToPose(-12.561, -60.9, 90, 1000); //prepare to rush
    chassis.moveToPoint(-6, -60.9, 1400); //rush center rings
    chassis.moveToPose(-18, -1.98, 20, 3000); //touch bar
   
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
    /* old "20pt" skills
    chassis.setPose(-53, 0, 90);
    mIntake.move(127);
    chassis.moveToPose(-40.375, 13.262, 45, 1000);
    chassis.moveToPose(-76.7, 66.2, 320, 2500); //score 1st goal
    chassis.moveToPoint(17.138, 37.25, 2000);
    chassis.moveToPoint(56.564, 18,  1300); //get behind 2nd goal
    chassis.moveToPoint((67.5), 65, 2500);
    chassis.moveToPoint(59, 57, 2500, {.forwards = false});
    chassis.moveToPoint(49, 4, 4000);
    chassis.moveToPose(88.864, -59.77, 85, 4000); //score 3rd goal
    chassis.moveToPoint(56, -47, 2500, {.forwards = false});
    chassis.moveToPose(-19.793, -30.913, 320, 4000);
    chassis.moveToPose(-43, -15, 200, 4000);
    chassis.moveToPoint(-64.088, -74.139,2000);//score 4th goal
    chassis.moveToPoint(-64, -53, 2500, {.forwards = false});
    chassis.moveToPoint(-22, -22, 1000);
    mIntake.brake();*/
}
void redRush(){}
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
void blue4Ring(){}
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
    while(loadLB){delay(20);}
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