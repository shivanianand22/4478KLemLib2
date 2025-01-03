#include "devices.h"
#include "lemlib/chassis/chassis.hpp"
#include "main.h"
#include "liblvgl/llemu.hpp"
#include "actions.h"
#include "pros/rtos.h"
#include <cstdio>
#include <ctime>

int selection = 4;
/*
 0 = RedRight Mogo
 1 = RedLeft Ring
 2 = BlueLeft Mogo
 3 = BlueRight Ring
 4 = Prog skills
 5 = Red Goal Rush
 6 = Blue Goal Rush
*/
void autonSelector(){
	if(selection <= 4){
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
	}
}
void redRight(){ //mogo side
    chassis.setPose(-51.7, -18.0, 90);
    chassis.moveToPoint(-50.5,  0.2, 1500, {.forwards = false});
	chassis.waitUntilDone();
	chassis.setPose(-50.5,  0.2, chassis.getPose().theta);
    chassis.moveToPose(-57.4,  0.2, 90, 1500, {.forwards = false});


    chassis.waitUntilDone();
    delay(100);
    mIntake.move(-220);
    chassis.setPose(-63.5, 0.2, 90);
    delay(900); //score alliance stake
    chassis.moveToPoint(-47.938, -15.951, 1000);
    chassis.moveToPose(-28.5, -23.999, -55, 2000, {.forwards = false}); //get to mogo
    chassis.waitUntilDone();
    delay(75);
    grab();
    delay(75);
	chassis.setPose(-28.5,  -23.999, chassis.getPose().theta);
    chassis.moveToPoint(-19.5, -61.914, 3000);//pickup ring to put on Mogo
    chassis.moveToPose(-18, -1.98, 20, 3000); //touch bar
}

void blueLeft(){ //mogo side
    chassis.setPose(-51.9, 16.0, 90);
    chassis.moveToPoint(-54.5,  0.2, 1500, {.forwards = false});
    chassis.moveToPose(-63.5,  0.2, 90, 1500, {.forwards = false});
    chassis.waitUntilDone();
    
    delay(100);
    mIntake.move(-220);
    delay(1100); //score alliance stake
    chassis.moveToPoint(-47.938, 15.951, 1000);
    chassis.moveToPose(-28.5, 23.999, -145, 2000, {.forwards = false}); //get to mogo
    chassis.waitUntilDone();
    delay(75);
    grab();
    delay(75);
	chassis.setPose(-28.5,  23.999, chassis.getPose().theta);
    chassis.moveToPoint(-26.0, 53.114, 2400);//pickup ring to put on Mogo
    chassis.moveToPoint(-24, -2, 3000); //touch bar
}


void redLeft(){ //ring side
    chassis.setPose(-51.9, 16.0, 90);
    chassis.moveToPoint(-54.5,  0.2, 1500, {.forwards = false});
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
    chassis.moveToPoint(-26.0, 57, 2000);//pickup ring to put on Mogo
    chassis.moveToPoint(-21.0, 52.114, 1000, {.forwards=false});//quickly backup to avoid 2nd ring
    chassis.moveToPose(-19.561, 51.9, 90, 1000); //prepare to rush
    chassis.moveToPoint(-14.17, 57, 1500); //rush center rings
    delay(500);
   /*
    chassis.moveToPoint(-17.561, -51.9, 1000, {.forwards = false}); //back up
    chassis.waitUntilDone();
    controller.rumble("--");
    
    chassis.moveToPoint(-18.17, 49,1700); //go for second ring
    chassis.waitUntilDone();
    delay(500);
    */

    chassis.moveToPoint(-20.468, -6, 1100); //touch bar
}


void blueRight(){ //ring side
    chassis.setPose(-51.7, -18.0, 90);
    chassis.moveToPoint(-50.5,  0.2, 1500, {.forwards = false});
	chassis.waitUntilDone();
	chassis.setPose(-50.5,  0.2, chassis.getPose().theta);
    chassis.moveToPose(-57.4,  0.2, 90, 1500, {.forwards = false});


    chassis.waitUntilDone();
    delay(100);
    mIntake.move(-220);
    chassis.setPose(-63.5, 0.2, 90);
    delay(900); //score alliance stake
    chassis.moveToPoint(-47.938, -15.951, 1000);
    chassis.moveToPose(-28.5, -23.999, -55, 2000, {.forwards = false}); //get to mogo
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
    grab();
    controller.set_text(1, 1, "running skills");
    mLefts.tare_position();
    mRights.tare_position();
    chassis.setPose(-64.241, -0.705, 90);
    chassis.moveToPoint(-68.241, -0.705, 5000,{.forwards = false} );
    mIntake.move(-280);
    chassis.waitUntilDone();
    delay(500);
    mIntake.move(127);
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
    delay(3500);//REMOVE THIS. THIS IS SO I CAN MOVE MOGO TO RIGHT POSITION
    lcd::print(2, "θ: %f, x: %f, y: %f", chassis.getPose().theta, chassis.getPose().x, chassis.getPose().y);
     //start of auton that just pushes goal
     /*
    mLefts.tare_position();
    mRights.tare_position();
    chassis.setPose(-62.9, 60.675, chassis.getPose().theta);
    delay(50);
    chassis.moveToPoint(17.138, 45.25, 2000,{.forwards = true});    
    chassis.moveToPoint(50.564, 18,  2500); //get behind 2nd goal
    chassis.moveToPoint((67.5), 65, 2500);
    chassis.moveToPoint(61, 57, 2500, {.forwards = false});
    chassis.moveToPoint(59, 4, 4000);
    chassis.moveToPose(90.864, -59.77, 85, 3900); //score 3rd goal
    chassis.moveToPoint(56, -47, 2500, {.forwards = false});
    chassis.moveToPose(-19.793, -30.913, 320, 4000);
    chassis.moveToPose(-39, -12, 200, 4000);
    chassis.moveToPoint(-64.088, -74.139,2000);//score 4th goal
    chassis.moveToPoint(-64, -53, 2500, {.forwards = false});
    chassis.moveToPoint(-22, -22, 1000);
    mIntake.brake();*/

    //start to go to second goal
    
    
    //code to finish for good auton skills
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
    chassis.moveToPoint(8.113, -57.276, 4000);
    chassis.moveToPose(0, -64.164, 180, 3000);
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
void blueRush(){}