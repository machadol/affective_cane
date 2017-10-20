#include "LDR.h"
#include "Arduino.h"

LDRManager::LDRManager(int analogport)
{
	this->port=analogport;
	
}

int LDRManager::Kernel(int analog)
{
  return map(analog,0,1023,0,5);

}

int LDRManager::CurrentValue()
{
	return Kernel(analogRead(this->port));
}

bool LDRManager::StateOfLight()
{
	if(CurrentValue() < lightLevel){
		return true;
	} 
	else{
		return false;
	};
}

bool LDRManager::LightOn()
{
	return StateOfLight();
}




