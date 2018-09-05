int GSR = 32;
int Pulse = 1023;

byte buf[4];

typedef union {
 float floatingPoint;
 byte binary[4];
} binaryFloat;


void setup() {
  
  Serial.begin(9600);
}

void loop() {

    int a[] = {0,1,2,3};


    SendIntArray(0x01, a, 4, '\n');
    


}


/*All data is Big Endian here*/
void SendADCBytes(byte ID, int num, byte Terminator)
{
  byte buf[2];


    buf[0] = num & 255;
    buf[1] = (num >> 8) & 255;

  
  Serial.write(ID);
  Serial.write(buf,2);
  Serial.write(Terminator);
}

/*All data is sent as BIG Endian here*/
void SendFloatBytes(byte ID ,float num, byte Terminator)
{
  binaryFloat bf;

  bf.floatingPoint = num;

  Serial.write(ID);
  Serial.write(bf.binary,4);
  Serial.write(Terminator);
  
}

void SendIntArray(byte ID, int *Array, int len, byte Terminator)
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

