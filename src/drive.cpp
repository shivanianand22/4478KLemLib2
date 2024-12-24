#include "main.h"
#include "devices.h"


bool pressed= false;//variable pressed ensures we can use the same button on controller  
//to toggle pistons either way: true is open, false is closed
void toggleClamp(){
  			pressed = !pressed;
			if(pressed){											  						  
				Clamper.set_value(HIGH);							  
			}	
			else if(!pressed){
				Clamper.set_value(LOW);
			}
}
void drive(){
    int leftVal = controller.get_analog(ANALOG_LEFT_Y);
		int rightVal = controller.get_analog(ANALOG_RIGHT_Y);
		chassis.tank(leftVal, rightVal);
}