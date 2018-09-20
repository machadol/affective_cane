  #include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

#include "ArPy_Remote.h"
#include "dht.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

MPU6050 mpu;

/*DHT Defs*/
dht DHT;

#define DHT22_PIN 11


/* =========================================================================
   NOTE: In addition to connection 3.3v, GND, SDA, and SCL, this sketch
   depends on the MPU-6050's INT pin being connected to the Arduino's
   external interrupt #0 pin. On the Arduino Uno and Mega 2560, this is
   digital I/O pin 2.
 * ========================================================================= */

/* =========================================================================
   NOTE: Arduino v1.0.1 with the Leonardo board generates a compile error
   when using Serial.write(buf, len). The Teapot output uses this method.
   The solution requires a modification to the Arduino USBAPI.h file, which
   is fortunately simple, but annoying. This will be fixed in the next IDE
   release. For more info, see these links:

   http://arduino.cc/forum/index.php/topic,109987.0.html
   http://code.google.com/p/arduino/issues/detail?id=958
 * ========================================================================= */

// uncomment "OUTPUT_READABLE_QUATERNION" if you want to see the actual
// quaternion components in a [w, x, y, z] format (not best for parsing
// on a remote host such as Processing or something though)
#define OUTPUT_READABLE_QUATERNION

#define INTERRUPT_PIN 2  // use pin 2 on Arduino Uno & most boards
#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)
bool blinkState = false;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector  
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

// packet structure for InvenSense teapot demo
uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };

unsigned long StartTime;
unsigned long samplesCount1=0;
unsigned long samplesCount2=0;
unsigned long ADCTimer1 = 0 ;
unsigned long LowRangeTimer = 0;
ProtocolEncoder PE = ProtocolEncoder();
float ACCArray[4]; // Floats of the Quartenions of the ACC
float AMBArray[2]; // Floats of the Ambient (Humidity and Temp)

int GSR = 0;
int Pulse = 0;
int Touch = 0;

// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}



// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================


void setup_Serial()
{
  // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    Serial.begin(115200);
    while (!Serial); // wait for Leonardo enumeration, others continue immediately
  }

void setup_MPU()
{
  mpu.initialize();
    pinMode(INTERRUPT_PIN, INPUT);

    Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

    devStatus = mpu.dmpInitialize();

    // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(41);
    mpu.setYGyroOffset(10);
    mpu.setZGyroOffset(17);
    mpu.setXAccelOffset(-2395);
    mpu.setYAccelOffset(1638);
    mpu.setZAccelOffset(120); // 1688 factory default for my test chip

    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
        Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }

    // configure LED for output
    pinMode(LED_PIN, OUTPUT);
  
  }

void setup_DHT(){
  
  pinMode(DHT22_PIN, INPUT);
  digitalWrite(DHT22_PIN, LOW);
}
void setup() {
    
    setup_Serial();
    setup_MPU();
    setup_DHT();

    StartTime = millis();
}



// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================

void loop() {
    // if programming failed, don't try to do anything
    if (!dmpReady) return;

    // wait for MPU interrupt or extra packet(s) available
    while (!mpuInterrupt && fifoCount < packetSize) {
        
        unsigned long CurrentTime = millis();   
        unsigned long ElapsedTime = CurrentTime - StartTime;
        unsigned long ElapsedTime2 = CurrentTime - LowRangeTimer;

        //timer mais curto
        if(ElapsedTime2 >= 10)
        {
          LowRangeTimer = millis();

           GSR = analogRead(A1);     // read the input pin
           Pulse = analogRead(A3);     // read the input pin

           // Touch

           Touch = analogRead(A2);
           samplesCount2++; 

           

           PE.SendIntBytes(0x01,GSR,'\n');
           PE.SendIntBytes(0x02,Pulse,'\n');
           PE.SendIntBytes(0x04,Touch,'\n');
           
           LowRangeTimer = millis();

        }

        if(ElapsedTime>= 3000)
        {
//            Serial.print("Time: ");
//            Serial.print(ElapsedTime);
//            Serial.print(" ");
//            StartTime = millis();
//  
//            Serial.print("Sample1: ");
//            Serial.print(samplesCount1);
//            Serial.print("Sample2: ");
//            Serial.println(samplesCount2);
            
            int CHK = DHT.read22(DHT22_PIN);

            switch(CHK){

              case DHTLIB_OK:
                AMBArray[0] = DHT.humidity;
                AMBArray[1] = DHT.temperature;
                break;
              default:
                AMBArray[0] = 0.0;
                AMBArray[1] = 0.0;
                break;

            }
            PE.SendFloatArray(0x05,AMBArray,2,'\n' );
            
            samplesCount1 = 0;
            samplesCount2 = 0;

            StartTime = millis();
            
        }
         
    }

    // reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        // reset so we can continue cleanly
        mpu.resetFIFO();
        Serial.println(F("FIFO overflow!"));

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (mpuIntStatus & 0x02) {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        
        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;

        #ifdef OUTPUT_READABLE_QUATERNION
            // display quaternion values in easy matrix form: w x y z
            mpu.dmpGetQuaternion(&q, fifoBuffer);
//            samplesCount1++;
//            Serial.print("quat\t");
//            Serial.print(q.w);
//            Serial.print("\t");
//            Serial.print(q.x);
//            Serial.print("\t");
//            Serial.print(q.y);
//            Serial.print("\t");
//            Serial.println(q.z);
              ACCArray[0] = q.w;
              ACCArray[1] = q.x;
              ACCArray[2] = q.y;
              ACCArray[3] = q.z;
  
              PE.SendFloatArray(0x03,ACCArray,4,'\n' );
        #endif
        
        // blink LED to indicate activity
        blinkState = !blinkState;
        digitalWrite(LED_PIN, blinkState);
    }
}
