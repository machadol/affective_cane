#ifndef Ultrasonic_H
#define Ultrasonic_H


class Ultrasonic
{
	public:
		Ultrasonic(int trigger, int echo);
		float MeasuredDistance(long microSeg);
		long timing();
	private:
		int tPin;
		int ePin;
		long microSeg;
		float cmDistance;
		float cmDivisor;
};
#endif