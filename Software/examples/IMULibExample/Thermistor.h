/**********************************************************************************/
/* Thermistor -- manages a 10k Thermistor in the follwing circuit: 				  */
/*     																			  */
/*		(Ground) ---- (Resister) -------|------- (Thermistor) ---- (+5v)	  */
/*                                      |									  */
/*                                 Analog Pin								  */
/*																				  */
/*			code based from code on Arduino playground found here:				  */
/*			http://www.arduino.cc/playground/ComponentLib/Thermistor2			  */
/*																				  */
/*								max mayfield									  */
/*								mm systems										  */
/*								max.mayfield@hotmail.com						  */
/**********************************************************************************/

#ifndef Thermistor_h
#define Thermistor_h

#include "Arduino.h"
#include "math.h"



class Thermistor {
	public:
		Thermistor(int pin);
		double getTemp();
	private:
		int _pin;
		const int R1 = 10000; //Resistor em série com o thermistor
};

#endif