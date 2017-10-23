import processing.serial.*;
import shapes3d.utils.*;
import processing.opengl.*;
import java.io.*;

//Global variables
Serial myPort;
float rotX = 0.0;
float rotY = 0.0;
String serialPort="COM9";
float yaw = 0.0;
float roll = 0.0;
float pitch = 0.0;
boolean hasNewData = false;
float[] quat = {1.0,0.0,0.0,0.0};

void setup() {
  size(600, 600,P3D);
   smooth();
   // serial port object instanciation
   myPort=new Serial (this,serialPort, 115200 );


} // setup()

///////////////////////////////// //And initialize the boxes like this:

void draw() {
 //serial port read cycle
  while (myPort.available()>0)
  {
    String inData=myPort.readStringUntil('\n');

    if (inData != null)
    {
      String[] pkg=inData.split ('\t');
       if (pkg!=null)
       {
          if(pkg[0].equals("ypr"))
          {
              // read package with yaw roll anda pitch

              yaw   = pkg[1];
              roll  = pkg[2];
              pitch = pkg[3];

              hasNewData = true;
          }
          if(pkg[0].equals("quat"))
           {
            //read package with quartenions

             quat[0] = Float.parseFloat(phase1[1]); // w
             quat[1] = Float.parseFloat(phase1[2]); // x
             quat[2] = Float.parseFloat(phase1[3]); // y
             quat[3] = Float.parseFloat(phase1[4]); // z


              //Update YawRollPitch


              //
              hasNewData = true;
           }
          if(pkg[0].equals("fsr"))
           {

              //read package with fsr
              hasNewData = true;
           }
          if(pkg[0].equals("dht"))
           {
              //read package with dht
              hasNewData = true;
           }


       }


  }

  // animation
  background(255);
  translate(300,500,-500);//initial position
  rotateX(radians(70));// initial rotation
  box(600,500,50);


  if(!hasNewData)
  {
  //Sensor box position and sensor
   translate(0,0,512-mouseY);
    rotateX(radians(rotX));
    rotateY(radians(rotY));
    rotX +=1.0;
    rotY += 1.0;

  } else
  {
    rotateX(roll);
    rotateY(pitch);
    rotateZ(yaw);
  }


   box(100);

   //XAxis
   line(-150,0,0,0,0,0);
   line(0,0,0,150,0,0);

   //ZAxis
   line(0,0,150,0,0,0);
   line(0,0,-150,0,0,0);

   //ZAxis
   line(0,-150,0,0,0,0);
   line(0,150,0,0,0,0);


  //300,300,100 vista superior
   //visualização no eixo x


} // draw()

// ===============================================================
