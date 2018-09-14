/*
	ArPy_Remote.hpp
	Created by Pedro Inazawa - September of 2018

*/


#ifndef ARPY_H
#define ARPY_H

#include "Arduino.h"

class ProtocolEncoder
{
  public:
    ProtocolEncoder(void);
    void SendIntArray(byte ID, int *Array, int len, byte Terminator);
    void SendFloatBytes(byte ID ,float num, byte Terminator);
    void SendIntBytes(byte ID, int num, byte Terminator);
    void SendFloatArray(byte ID, float *Array, int len, byte Terminator);
};

#endif
