/*
  ArPy_remote.cpp - Library for Protocol Communication Arduino/Python.
  Created by Pedro Inazawa, September, 2018.
*/

#include "Arduino.h"
#include "ArPy_Remote.h"

typedef union {
 float floatingPoint;
 uint8_t binary[4];
} binaryFloat;


ProtocolEncoder::ProtocolEncoder(void)
{}

void ProtocolEncoder::SendIntBytes(byte ID, int num, byte Terminator)
{
  byte buf[2];


    buf[0] = num & 255;
    buf[1] = (num >> 8) & 255;

  
  Serial.write(ID);
  Serial.write(buf,2);
  Serial.write(Terminator);
}

void ProtocolEncoder::SendFloatBytes(byte ID ,float num, byte Terminator)
{
  binaryFloat bf;

  bf.floatingPoint = num;

  Serial.write(ID);
  Serial.write(bf.binary,4);
  Serial.write(Terminator);
  
}

void ProtocolEncoder::SendIntArray(byte ID, int *Array, int len, byte Terminator)
{ 

    int iter = 0, subiter = 0, num = 0;
    byte buf[4];
    
    Serial.write(ID);
    Serial.write(len);
    
    for(iter = 0; iter < len; iter++)
    {

      //In Arduino Uno, an INT has only 16 bits (2 bytes) - This yields a range of -32,768 to 32,767
      num = Array[iter];  
      buf[0] = num & 255;
      buf[1] = (num >> 8) & 255;

          for(subiter = 0; subiter < 2; subiter ++)
              Serial.write(buf[subiter]);
      }

      Serial.write(Terminator);

   
}

void ProtocolEncoder::SendFloatArray(byte ID, float *Array, int len, byte Terminator)
{

    int iter = 0;
    float num = 0.0;
    float *addrFloat = Array;

    binaryFloat bf;

    Serial.write(ID);
    Serial.write(len);

    for(iter = 0; iter < len; iter++)
    {

      num = *addrFloat; 
      bf.floatingPoint = num;
      Serial.write(bf.binary,4);

      addrFloat++;
    }

    Serial.write(Terminator);
}




