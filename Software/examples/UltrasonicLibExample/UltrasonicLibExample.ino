#include <Ultrasonic.h>
Ultrasonic ultra(9,8);

long microseg;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  microseg = ultra.timing();
  Serial.print(ultra.MeasuredDistance(microseg));
  Serial.print("\n");
}
