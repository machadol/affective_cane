#include "UltrassonicLib.h"


UltraSonicDistanceSensor distanceSensor(triggerPin, echoPin);

int getDistanceToFloor()
{
	
	double cmMsec;

	cmMsec = distanceSensor.measureDistanceCm();

	return (int) cmMsec*100;
}
