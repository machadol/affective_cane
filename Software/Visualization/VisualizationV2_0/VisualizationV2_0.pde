/*
Visualization V2.0

Changelog:

24/10 - Initial Version;
      - Added Writer();
      -Insert the colors with temperature
      -put the limeited of the ground
      -make a method of the ground collors
*/
import processing.serial.*;
import shapes3d.utils.*;
import processing.opengl.*;
import java.io.*;
import java.io.FileWriter;

//Global variables
Serial myPort;
float rotX = 0.0;
float rotY = 0.0;
String serialPort="COM12"; // Replace here the serial port of the cane.
float yaw = 0.0;
float roll = 0.0;
float pitch = 0.0;
boolean hasNewData = false;
int timestamp = 0;
int FSRStatus = 0;
int serialPortTries = 2; //Number of tries to open the serial port
float[] quat = {1.0,0.0,0.0,0.0};
boolean serialPortIsOpen = false;
float bodyTemperature = 33.0;
float ambientTemperature = 25.0;
float AmbientTemp=0.0;
int timeSince = 0;
int firstTimer = 0;
 FileWriter output;

void OpenFile()
{
   String filePath;
   int d = day();
   int m = month();
   int y = year();
   int hour = hour();
   int minute = minute();
   
   filePath = String.format("%s\\"+"%d-%d-%d"+"@"+"%d_%d.csv",dataPath(""),d,m,y,hour,minute);
    
    try {
      output = new FileWriter(filePath, true); //the true will append the new data
    }
    catch (IOException e)
   {
      println("Error opening file.");
      e.printStackTrace();
    }
    finally 
    {
       
    }
    
    println("Output is in " + filePath);
    try
    {
    output.write("timestamp;pitch;roll;yaw;FSRStatus\n\t");
    }
    catch(IOException e) {    }
}

void writer(){
try {

  output.write(timestamp + ";");
  output.write(pitch + ";");
  output.write(roll + ";");
  output.write(yaw + ";");
  output.write(FSRStatus + "\n\t");
}
catch (IOException e) {
  println("Error Writing");
  e.printStackTrace();
}
finally {}

}

void tempB(float temp){ // body temperature between 33 a 41
  // color definitions
  color blue = color (0,0,139);
  color green = color (50,205,50);
  color orange = color (255,140,0);
  color yellow =color(255,204,0);
  color red = color (255,0,0);
  //color darkRed = color (178,34,34);
  //color maroon = color (128,0,0);
  color slateBlue = color (106,90,205);
  color Chocolate = color (210 ,164,96);
  color SpringGreen = color (0,255,127);
    // parte de puxar os dados da porta serial


   if (33<temp && temp<=34)
    { fill(blue);
//      box( 100);

    }
   else if(34<temp && temp <=35)
    { fill(green);
    //  box( 100);


    }
   else if(35<temp && temp <=36)
    { fill(orange);
      //box( 100);


    }
   else if(36<temp && temp <=37)
    { fill(yellow);
      //box( 100);


    }
   else if(37<temp && temp <=38)
      { fill(SpringGreen);
        //box( 100);


      }
   else if(38<temp && temp <=39)
      {
        fill(slateBlue);
        //box( 100);


      } else if(38<temp && temp <=39)
      {
        fill(Chocolate);
        //box( 100);

      }
      else if (temp>=40)
      {
          fill(red);
          //box( 100);
      }
      else if (temp<=33)
      {
          fill(255);
          //box( 100);
      }

    box( 100);

}


void tempA(float ambientTemp){
  
    // color definitions
    color plum = color (221,160,221);
    color crimson = color (220,20,60);
    color salmon = color (250,128,114);
    //color yellow =color(255,204,0);
    //color darkRed = color (178,34,34);
    color skyBlue = color (135,206,235);
    color red = color (255,0,0);
    color lightSlateGray = color (119,136,153);
  
     // parte de puxar os dados da porta serial
  
    //arrange of 25 until 40
    
     if (24<ambientTemp && ambientTemp<=26.5)
      { background(skyBlue);
       
      }
     else  if (26.5<ambientTemp && ambientTemp<=29)
      { background(plum);
       
  
      }
     else  if (29<ambientTemp && ambientTemp<=31.5)
      { background(crimson);
       
  
  
          }
        else  if (31.5<ambientTemp && ambientTemp<=34)
      { background(salmon);
       
  
  
      }
    else  if (34<ambientTemp && ambientTemp<=36.5)
      { 
        background(lightSlateGray);
       
      }
        else  if (36.5<ambientTemp && ambientTemp<=40)
      { 
        background(skyBlue);
  
      }
       else  if (40<ambientTemp && ambientTemp<=42.5)
      { 
        background(red);
  
      }
      else if(ambientTemp>42.5)
      {
         background(0); 
      }
      else if(ambientTemp<=24)
      {
         background(255); 
      }
  }




void setup() {
  size(600, 600,P3D);
   smooth();
   // serial port object instanciation

   while(serialPortTries > 0)
   {
      try
      {
        println("Trying to open Serial port...");
        myPort=new Serial (this,serialPort, 115200 );
      }
      catch(Exception e)
      {
        println("Serial Port Error");
        serialPortTries--;
        continue;
      }

      break;
   }

   if(serialPortTries > 0)
   {
      serialPortIsOpen = true;
   }

  firstTimer = millis();
  OpenFile();
} // setup()

///////////////////////////////// //And initialize the boxes like this:

void draw() {
 tempA( ambientTemperature);
 //serial port read cycle
  if(serialPortIsOpen)
  {
    while (myPort.available()>0)
    {
      String inData=myPort.readStringUntil('\n');

      if (inData != null)
      {
        String[] pkg=inData.split ("\t");
         if (pkg!=null)
         {
            if(pkg[0].equals("ypr"))
            {
                // read package with yaw roll anda pitch

                yaw   = Float.parseFloat(pkg[1]);
                roll  = Float.parseFloat(pkg[2]);
                pitch = Float.parseFloat(pkg[3]);

                hasNewData = true;
            }
            if(pkg[0].equals("quat"))
             {
              //read package with quartenions

               quat[0] = Float.parseFloat(pkg[1]); // w
               quat[1] = Float.parseFloat(pkg[2]); // x
               quat[2] = Float.parseFloat(pkg[3]); // y
               quat[3] = Float.parseFloat(pkg[4]); // z


                //Update YawRollPitch


                //
                hasNewData = true;
             }
            if(pkg[0].equals("fsr"))
             {
               timestamp = Integer.parseInt(pkg[1]);
               FSRStatus = Integer.parseInt(pkg[2]);
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


    }

  }

  // animation
  //background(255);
  translate(300,500,-500);//initial position
  rotateX(radians(70));// initial rotation 70
  fill(250);
  box(600,500,50);



    if(!serialPortIsOpen)
    {
     
        //Sensor box position and sensor
       if (mouseY<435){
          translate(0,0,512-mouseY);
          println(mouseY);
         rotateX(radians(rotX));
         rotateY(radians(rotY));
          rotX +=1.0;
          rotY += 1.0;
       }
       else
       {
           translate(0,0,76);
       }
      // box(100);

    //XAxis
    line(-150,0,0,0,0,0);
    line(0,0,0,150,0,0);

    //YAxis
    line(0,0,150,0,0,0);
    line(0,0,-150,0,0,0);

    //ZAxis
    line(0,-150,0,0,0,0);
    line(0,150,0,0,0,0);

     timeSince = millis();
     

    if(timeSince - firstTimer > 500)
  {
    firstTimer = millis();
     bodyTemperature+=1.0;
     ambientTemperature +=1.0;
     
  }


    if(bodyTemperature > 40.0)
    {
    bodyTemperature = 33.0;
    }
    if(ambientTemperature > 40.0)
    {
    ambientTemperature = 25.0;
    }
    
    timestamp = millis(); 
    }
  else
  {

     translate(0,0,512);

    //Decode Quaternion data from Serial Port
    double [] EulerRot = new Rotation(quat[0], quat[1], quat[2], quat[3], false).getAngles(RotationOrder.XYZ);

    //Convert from quat to euler
    roll=  Math.round(100.0*(float)EulerRot[0]);
    pitch =  Math.round(-100.0*(float)EulerRot[1]);
    yaw =  Math.round(100.0*(float)EulerRot[2]);

    //refresh data.
    rotateX(radians(pitch));
    rotateY(radians(roll));
    rotateZ(radians(yaw));

    hasNewData = false;
    //noFill();
    box(100);

    //XAxis
    line(-150,0,0,0,0,0);
    line(0,0,0,150,0,0);

    //YAxis
    line(0,0,150,0,0,0);
    line(0,0,-150,0,0,0);

    //ZAxis
    line(0,-150,0,0,0,0);
    line(0,150,0,0,0,0);

  }

  writer();
  tempB( bodyTemperature);


  }

void stop() 
{
   try {
      output.close();
    } catch (IOException e) {
      println("Error while closing the writer");
    }
} 
// ===============================================================

