#include "FSR.h"
#include "Arduino.h"

FSRManager::FSRManager(int analogport)
{
	analogport = port;
	analogRead(analogport);
	
}

int FSRManager::Kernel(int analog)
{
	return map(analog,0,1023,0,5);
}

int FSRManager::CurrentValue()
{
	return Kernel(analogRead(port));
}

bool FSRManager::StateOfButton()
{
	if(CurrentValue() < buttonLevel){
		return true;
	} 
	else{
		return false;
	};
}

bool FSRManager::IsPressed()
{
	return StateOfButton();
}




