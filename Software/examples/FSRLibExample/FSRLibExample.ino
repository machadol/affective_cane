#include "FSR.h"

#define LED 13

//Criacão de um Gerente de FSR na porta desejada.
FSRManager FSR(3);


void setup() {
  // Seta o led do arduino como output
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  if(FSR.IsPressed())
  {
  	digitalWrite(LED, HIGH);
  	Serial.print("ON ");
  }
  else
  {
	digitalWrite(LED,LOW);
	Serial.print("OFF ");
  }
  	Serial.print(FSR.CurrentValue());
    Serial.print(" ");
    Serial.println(analogRead(3));

  	delay(100);
}
