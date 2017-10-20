#include "DHT.h"

#define DHTPIN 2  

//#define DHTTYPE DHT11   // DHT 11 
#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600); 
  Serial.println("DHTxx test!");

  dht.begin();
}

void loop() {
  delay(2000);

  float umidade     = dht.readHumidity();
  float temperatura = dht.readTemperature();

  if (isnan(umidade) || isnan(temperatura)) {
    Serial.println("Falha de leitura DHT!");
    return;
  }

  //float hi = dht.computeHeatIndex(f, h);

  Serial.print("Umidade: "); 
  Serial.print(umidade);
  Serial.print(" %\t");
  Serial.print("Temperatura: "); 
  Serial.print(temperatura);
  Serial.print(" °C ");
  Serial.print("\n");
}
