#include "TimerOne.h"
#include "UltrassonicLib.h"
#include "ADC.h"
#include "DHT.h"
#include <QueueArray.h>

/*IMU headers Defines*/
#include <I2Cdev.h>
#include <MPU6050_6Axis_MotionApps20.h>
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h" 
#endif
#define INTERRUPT_PIN 12  // use pin 2 on Arduino Uno & most boards

MPU6050 mpu;  
bool dmpReady=false;  // set true if DMP init was successful
bool mpuInterrupt = true;
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

Quaternion q;           // [w, x, y, z]         quaternion container
/*********************/

/*GPS headers Defines*/
#include <SoftwareSerial.h> 
#include <TinyGPS.h>    
#define GPS_RX  4
#define GPS_TX  3
#define GPS_Serial_Baud 9600
SoftwareSerial gpsSerial(GPS_RX, GPS_TX);
TinyGPS gps; 
float lat,lon;
/**/

/*DTH headers Defines*/
DHT dht(DHTPIN, DHT22);
/**/

/*App main Defines */
// > https://playground.arduino.cc/Code/QueueArray
QueueArray <int> rxData;
int CounterInnerLoop=0;
int CounterMiddleLoop=0;
int CounterHigherLoop=0;
bool CanRead[3];

// This is the fastest interrupt. 
// Currently is measured in Microssecond
#define READER_LOWER_TIMER 1000  // 1000 us = 1 ms or 100 hz

// This value is based on the accumulated 
// amount of the READER_LOWER_TIMER
#define READER_MIDDLE_TIMER 1000 // 1000 x READER_LOWER_TIMER = 1 second

// This value is based on the accumulated 
// amount of the READER_MIDDLE_TIMER 
#define READER_HIGHER_TIMER 15 // 15 x READER_MIDDLE_TIMER = 15 seconds

//Some other defines, for legibility
#define LOWER_TIMER 0
#define MIDDLE_TIMER 1
#define HIGHER_TIMER 2


//select the data to be analysed.
//M-HEALTH Module 
//#define READ_OXIMETRY 
//#define READ_HANDTEMP 
//#define READ_GSR      

//LOCATION MODULE
//#define READ_DISTANCE_TO_FLOOR 
//#define READ_QUATERNIONS 
#define READ_GPS 

//AMBIENTAL MODULE
//#define READ_AMBTEMP 
//#define READ_AMBLIGHT 
//#define READ_RELUMID


/*****/
void setup(){	

	// Setup the first timers
	Timer1.initialize(READER_LOWER_TIMER); // our interrupts will start each 1 ms (1000 samples/second)
	Timer1.attachInterrupt(interruptManager);

  Serial.begin(115200);
  gpsSerial.begin(GPS_Serial_Baud);
	
	dht.begin();

  Serial.println("StartOK");
}

void loop()
{

	int data;
	float floatData;

	if(CanRead[LOWER_TIMER])
	{
		//Read sequence. 

		#ifdef READ_OXIMETRY
			data = analogRead(OXIMETRY_PORT);
			
			rxData.push(1);
			rxData.push(data);

		#endif

		#ifdef READ_HANDTEMP
			data = analogRead(HANDTEMP_PORT);

			rxData.push(2);
			rxData.push(data);
		#endif

		#ifdef READ_GSR
			//TODO: GERAR UM VALOR NO PROTOCOLO PARA GSR
			
			data = analogRead(HANDTEMP_PORT)
			
			rxData.push(2)
			rxData.push(data)
			
		#endif

		#ifdef READ_QUATERNIONS

			//Run getAccData to stock results in global variable q
			getAccData();

			//push the protocol code of the rxData. 
			rxData.push(9);

		#endif

		#ifdef READ_DISTANCE_TO_FLOOR
			data = getDistanceToFloor();
			
			rxData.push(10)	;

			rxData.push(data)	;

		#endif

		CanRead[LOWER_TIMER] = false;
	}

	if(CanRead[MIDDLE_TIMER])
	{

		#ifdef READ_AMBLIGHT
			data = analogRead(LDR_PORT);

			rxData.push(7);
			rxData.push(data);
			
		#endif

		CanRead[MIDDLE_TIMER] = false;
	}

	if(CanRead[HIGHER_TIMER])
	{

		#ifdef READ_AMBTEMP

			floatData = dht.readTemperature();

			if (isnan(floatData)) {
			    floatData = 0.0;
			}

			//Preserves the last two decimal places of data
			data = (int) floatData*100;

			rxData.push(4);
			rxData.push(data);

		#endif

		#ifdef READ_RELUMID


			floatData = dht.readHumidity();

			if (isnan(floatData)) {
			    floatData = 0.0;
			}

			//Preserves the last two decimal places of data
			data = (int) floatData*100;

			rxData.push(5);
			rxData.push(data);

		#endif

		#ifdef READ_GPS

			// Run getGPSPosition, and all data will be stocked in lat and lon
			getGPSPosition();

			floatData = lat;

			//Preserves the last two decimal places of data
			data = (int) (floatData*100);

			rxData.push(6);
			rxData.push(data); //First is Lat

			floatData = lon;
			//Preserves the last t  wo decimal places of data
			data = (int) (floatData*100);
			rxData.push(data); //Last in lon

		#endif

		CanRead[HIGHER_TIMER] = false;
	}


	//Now print data.
	while (!rxData.isEmpty ())
    Serial.println (rxData.dequeue());

	
}

void interruptManager()
{
	//This enable/disable readings in the manager.
	CounterInnerLoop++;

	CanRead[LOWER_TIMER] = true;

	if(CounterInnerLoop == READER_MIDDLE_TIMER)
	{

		CounterInnerLoop = 0;
		CounterMiddleLoop += 1;
		CanRead[MIDDLE_TIMER] = true;
	}

	if(CounterMiddleLoop == READER_HIGHER_TIMER)
	{

		CounterMiddleLoop = 0;
		CounterHigherLoop += 1;
		CanRead[HIGHER_TIMER] = true;
	}

	if(CounterHigherLoop == READER_HIGHER_TIMER)
	{

		CounterHigherLoop=0;
	}
}

int initialize()
{
  Wire.begin();
  mpu.initialize();

  if(mpu.testConnection() == 0) return -1;

  pinMode(INTERRUPT_PIN, INPUT);

  devStatus = mpu.dmpInitialize();

  // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(-8); //220
    mpu.setYGyroOffset(-25); //76
    mpu.setZGyroOffset(16); //-85
    mpu.setZAccelOffset(2689); // 1688 factory default for my test chip

    if (devStatus == 0) 
    {
      // turn on the DMP, now that it's ready
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        //Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();

    }
    else
    {
      return -1;
    }

}


void getAccData()
{

    uint16_t fifoCount;     // count of all bytes currently in FIFO


    // get current FIFO count
    fifoCount = mpu.getFIFOCount();


    // wait for MPU interrupt or extra packet(s) available
    if(mpuInterrupt && fifoCount >= packetSize)
    {
        // reset interrupt flag and get INT_STATUS byte
        mpuInterrupt = false;
        mpuIntStatus = mpu.getIntStatus();


        // get current FIFO count
        fifoCount = mpu.getFIFOCount();

        // check for overflow (this should never happen unless our code is too inefficient)
        if ((mpuIntStatus & 0x10) || fifoCount == 1024)
            // reset so we can continue cleanly
            mpu.resetFIFO();

        else if (mpuIntStatus & 0x02) 
        {
            // wait for correct available data length, should be a VERY short wait
            while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

            // read a packet from FIFO
            mpu.getFIFOBytes(fifoBuffer, packetSize);
            
            mpu.dmpGetQuaternion(&q, fifoBuffer);
        
        }
    }     

}

void dmpDataReady() {
    mpuInterrupt = true;
}

void getGPSPosition()
{

  bool newData = false;

  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while(gpsSerial.available())
    { // check for gps data 

      char c = gpsSerial.read();
    //   Serial.write(c); //apague o comentario para mostrar os dados crus
      if (gps.encode(c)) // Atribui true para newData caso novos dados sejam recebidos
        newData = true;
    }
  }
  
  if (newData)
  {
  
    Serial.println("Waiting GPS");
    gps.f_get_position(&lat,&lon); // get latitude and longitude 

    if(lat == TinyGPS::GPS_INVALID_F_ANGLE )
    {
      lat = 0; 
    }
    if(lon == TinyGPS::GPS_INVALID_F_ANGLE)
    {
      lon = 0;  
    }
    
  } 

}
