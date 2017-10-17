#include "FSR.h"
#include "Arduino.h"

FSR_manager::FSR_manager(int analogport)
{
	analogRead(analogport);
	analogport = FSR_port;
}

int FSR_manager::kernel_FSR(int analog)
{
	return map(analog,0,1023,0,5);
}

int FSR_manager::FSR_current_value()
{
	return kernel_FSR(analogRead(FSR_port));
}

bool FSR_manager::state_of_button()
{
	if(FSR_current_value() < button_level){
		return true;
	} 
	else{
		return false;
	};
}

bool FSR_manager::FSR_is_pressed()
{
	return state_of_button();
}




