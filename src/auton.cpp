#include "devices.h"
#include "liblvgl/llemu.hpp"
#include "actions.h"
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
	}
}

void redRight(){ //mogo side
	/*
	//IF going for alliance stake
	chassis.setPose(-51.7, -18.0, 90);
	chassis.moveToPoint(-50.5,  0.2, 1500, {.forwards = false});
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
	chassis.moveToPoint(-19.5, -61.914, 3000);//pickup ring to put on Mogo
	mIntake.move(-220);
	chassis.moveToPoint(-20.468, -3.306, 1000); //touch bar
	*/


	//IF not going for alliance stake
	chassis.setPose(-53, -16.0, 90);
	chassis.moveToPoint(-47.938, -15.951, 1000);
	chassis.moveToPose(-28.5, -23.999, -55, 2000, {.forwards = false}); //get to mogo
	chassis.waitUntilDone();
	delay(75);
	grab();
	mIntake.move(-220);
	delay(2000);
	chassis.moveToPoint(-21, -61.914, 3000);//pickup ring to put on Mogo
	chassis.moveToPoint(-20, -2, 2000); //touch bar
	
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
	chassis.moveToPoint(-47.938, 15.951, 1000);
	chassis.moveToPose(-28.5, 23.999, -145, 2000, {.forwards = false}); //get to mogo
	chassis.waitUntilDone();
	delay(75);
	grab();
	delay(75);
	chassis.moveToPoint(-26.0, 53.114, 2400);//pickup ring to put on Mogo
	chassis.moveToPoint(-21.0, 52.114, 1000, {.forwards=false});//quickly backup to avoid 2nd ring
	chassis.moveToPose(-19.561, 51.9, 90, 1000); //prepare to rush
	chassis.moveToPoint(-14.17, 51.9, 1500); //rush center rings
	chassis.moveToPoint(-17.561, -51.9, 1100, {.forwards = false}); //back up
	chassis.waitUntilDone();
	controller.rumble("--");
	chassis.moveToPoint(-18.17, 49,1800); //go for second ring
	chassis.waitUntilDone();
	delay(500);
	

	chassis.moveToPoint(-20.468, -3.306, 1000); //touch bar
}

void blueRight(){ //ring side
	chassis.setPose(-51.7, -18.0, 90);
	chassis.moveToPoint(-50.5,  0.2, 1500, {.forwards = false});
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
	chassis.moveToPoint(-19.5, -61.914, 3000);//pickup ring to put on Mogo
	chassis.moveToPose(-12.561, -60.9, 90, 1000); //prepare to rush
	chassis.moveToPoint(-6, -60.9, 1500); //rush center rings
	//chassis.moveToPose(-19.561, -49.757,90, 1100, {.forwards = false}); //back up
	// chassis.waitUntilDone();
	// controller.rumble("--");
	// chassis.moveToPose(-5.47, -49.221,90,  1000); //go for second ring
	

	//chassis.moveToPoint(-20.468, -3.306, 1000); //touch bar
	chassis.moveToPose(-21.081, -1.98, 20, 3000);
	
}


void progSkills(){
	chassis.setPose(-64.241, -0.705, 90);
	mIntake.move(-280);
	delay(1000);
	chassis.moveToPoint(-58, -0.705, 1000);
	chassis.moveToPose(-46.897, 19.678, 160, 3000, {.forwards = false}); //go to goal
	chassis.waitUntilDone();
	delay(100);
	grab();
	chassis.moveToPoint(-22.593, 28.722, 2000);//pick up first ring
	chassis.moveToPose(-25.912, 49.993, 270, 3000); //go to 2nd ring
	chassis.moveToPose(-40.912, 49.653, 270, 750); //pick up 2nd ring
	chassis.waitUntilDone();
	delay(2000);
	chassis.moveToPoint(-62.972, 49.622, 2000); //3rd & 4th ring
	chassis.moveToPose(-65.257, 64.489, 325, 1000, {.forwards = false});//drop 1st goal into corner
	chassis.waitUntilDone();
	chassis.setPose(-63.052, 60.356, imu.get_heading() + 90);
	release(); 
	// code to finish for good auton skills
	//go towards 2ng Mogo
	chassis.moveToPoint(-55.674, -0.515, 2000, {.forwards = false});
	chassis.moveToPose(-59.864, -19.983, 30, 1000, {.forwards=false});
	chassis.waitUntilDone();
	delay(1000);
	grab();
	delay(100);
	chassis.moveToPoint(-22.593, -24.395, 2000); //1st ring
	chassis.moveToPose(-35.936, -49.326,270,  2000);//2nd ring
	chassis.moveToPoint(-63.351, -49.136, 1000); //3rd & 4th ring
	chassis.moveToPose(-65.057, -63.814,45, 1000, {.forwards = false}); //drop off 2nd mogo in corner
	chassis.waitUntilDone();
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
void blueRush(){}