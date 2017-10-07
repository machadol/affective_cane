#include <SD.h>
#include<Wire.h>
Sd2Card SDcard;
SdVolume volume;

// Pino do Arduino conectado ao pino CS do modulo
const int chipSelect = 10;  

//Endereco I2C do MPU6050
const int MPU=0x68;  //pino aberto 0X68 , pino ligado em 3,3V 0x69
 
//Variaveis globais
int acelX,acelY,acelZ,temperatura,giroX,giroY,giroZ;

void setup(){
  // Define o pino do push-button como entrada
  Serial.begin(9600);
  Wire.begin();                  //inicia I2C
  Wire.beginTransmission(MPU);  //Inicia transmissão para o endereço do MPU
  Wire.write(0x6B);             
   
  //Inicializa o MPU-6050
  Wire.write(0); 
  Wire.endTransmission(true);
  //Inicia a comunicacao com o modulo SD
  if (!SD.begin(chipSelect)){
    Serial.println("Falha ao acessar o cartao !");
    Serial.println("Verifique o cartao/conexoes e reinicie o Arduino...");
    return;
  }
  Serial.println("Cartao iniciado corretamente !");
  Serial.println();
}

void loop(){

  Wire.beginTransmission(MPU);        //transmite
  Wire.write(0x3B);                  // Endereço 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);      //Finaliza transmissão
  
  Wire.requestFrom(MPU,14,true);  //requisita bytes
   
  //Armazena o valor dos sensores nas variaveis correspondentes
  acelX=Wire.read()<<8|Wire.read(); //0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)     
  acelY=Wire.read()<<8|Wire.read(); //0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  acelZ=Wire.read()<<8|Wire.read(); //0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  giroX=Wire.read()<<8|Wire.read(); //0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  giroY=Wire.read()<<8|Wire.read(); //0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  giroZ=Wire.read()<<8|Wire.read(); //0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  //Envia valores lidos do acelerômetro
  Serial.print("Acel:"); 
  Serial.print("  X:");Serial.print(acelX);
  Serial.print("\tY:");Serial.print(acelY);
  Serial.print("\tZ:");Serial.print(acelZ);
  
  //Envia valores lidos do giroscópio
  Serial.print("\tGiro:"); 
  Serial.print("  X:");Serial.print(giroX);
  Serial.print("\tY:");Serial.print(giroY);
  Serial.print("\tZ:");Serial.print(giroZ);

  
  // Abre o arquivo arquivo.txt do cartao SD
    File dataFile = SD.open("arquivo.txt", FILE_WRITE);
  // Grava os dados no arquivo
    if (dataFile){
      //Envia valores lidos do acelerômetro
      dataFile.print("Acel:"); 
      dataFile.print("  X:");Serial.print(acelX);
      dataFile.print("\tY:");Serial.print(acelY);
      dataFile.print("\tZ:");Serial.print(acelZ);
      
      //Envia valores lidos do giroscópio
      dataFile.print("\tGiro:"); 
      dataFile.print("  X:");Serial.print(giroX);
      dataFile.print("\tY:");Serial.print(giroY);
      dataFile.print("\tZ:");Serial.println(giroZ);
          dataFile.close();
    }  
    else{
      // Mensagem de erro caso ocorra algum problema
      // na abertura do arquivo
      Serial.println("Erro ao abrir arquivo.txt !");
    } 

    delay(500);
}