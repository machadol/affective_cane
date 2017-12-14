#include "Ultrasonic.h"
#include "Arduino.h"

Ultrasonic::Ultrasonic(int trigger, int echo)
{
	pinMode(trigger, OUTPUT);
	pinMode(echo, INPUT);
	tPin = trigger;
	ePin = echo;
	cmDivisor = 29.412;
}

// float tempo; // para armazenar o tempo de ida e volta do sinal em microsegundos
// float distancia_cm; // para armazenar a distância em centímetros

long Ultrasonic::timing()
{	
    digitalWrite(tPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(tPin, LOW);
	return pulseIn(ePin, HIGH);
}

float Ultrasonic::MeasuredDistance(long microSeg)
{
	return microSeg / cmDivisor / 2;
}



// void setup() 
// {
// Serial.begin(9600);
//  	// configura pino trigger como saída
//  	pinMode(trigger,OUTPUT);
//  	// deixa pino em LOW
//  	digitalWrite(trigger,LOW);
//  	delayMicroseconds(10);
//  	// configura pino ECHO como entrada
//  	pinMode(echo,INPUT);
// }

// void loop() 
// {
//  	// disparar pulso ultrassônico
// 	digitalWrite(trigger, HIGH);
// 	delayMicroseconds(10);
//  	digitalWrite(trigger, LOW);
	
// 	// medir tempo de ida e volta do pulso ultrassônico
//  	tempo = pulseIn(echo, HIGH);

//  	// calcular as distâncias em centímetros e polegadas
//  	distancia_cm = tempo / 29.412 / 2;
 
//  	Serial.print("Distancia: ");
//  	Serial.print(distancia_cm);
//  	Serial.print(" cm");
//  	Serial.print("\n");
// 	// aguardar um pouquinho antes de começar tudo de novo
//  	delayMicroseconds(200);
// }


