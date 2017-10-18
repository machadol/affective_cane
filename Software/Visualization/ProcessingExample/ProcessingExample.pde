import processing.serial.*;
import shapes3d.utils.*;
import processing.opengl.*;

Serial myPort;  // The serial port
 String inBuffer;   
 float yaw = 0.0, pitch = 0.0, roll = 0.0;
 float lastyaw = 0.0, lastpitch = 0.0, lastroll = 0.0;
 int count = 1;
 int timeSinceTestStart = 0;
 int testStart = 0;
 boolean firstTry = true;
 boolean firstCalibration = true;
 
 //for Quaternions Calculation
 float[] quat = {1.0,0.0,0.0,0.0};
 float[] eulerRot;
 float M_PI = 3.141516;
 

//For FSR
 int timestamp = 0;
 int FSRStatus = 0;

 //CONTROL PANEL
 boolean tryToCalibrate = true; //Modificar para false se a calibração não for necessaria.
 boolean UseQuartenions = true; // Se for false, usa Euler. Se for true, usa quaternions.
 boolean UseFSR         = true; // Modificar se nao quiser contar com FSR nos calculos.
 int  _CALIBRATION_TIME  = 20000; // Modifica o tempo de calibração.



void setup() {
  // List all the available serial ports:
//  printArray(Serial.list());
  // Open the port you are using at the rate you want:
    size(800,600,P3D);
    smooth();
    translate(400,300,0);

  println("Abrindo porta serial....");
  myPort = new Serial(this, "COM9", 115200  );
  
  
}

void draw() {
  
  while (myPort.available() > 0) 
  {
    //println("PortaSerial");
    if(firstTry)
    {
          testStart = millis();  
          firstTry = false;
    }
    
    inBuffer = myPort.readStringUntil('\n');
    
    
      if(inBuffer != null){
        String[] phase1 = inBuffer.split("\t");
        
        //Arbitra qual visualização escolher.
        if(UseQuartenions == false)
        {
           if(phase1.length == 4 )
          { 
            if(phase1[0].equals("ypr"))
            {
              //printArray(inBuffer);
              yaw = Float.parseFloat(phase1[1]);
              pitch = Float.parseFloat(phase1[2]);
              roll = Float.parseFloat(phase1[3]);
              }
          }
        }
        else
        {
           if(phase1.length == 5)
            {
              if(phase1[0].equals("quat"))
              {
                //printArray(inBuffer);
                quat[0] = Float.parseFloat(phase1[1]); // w
                quat[1] = Float.parseFloat(phase1[2]); // x
                quat[2] = Float.parseFloat(phase1[3]); // y
                quat[3] = Float.parseFloat(phase1[4]); // z
              }
            }
        }
      
      //FSR and Timestamp Processing

      if(phase1.length == 3 && UseFSR)
      {
        if(phase1[0].equals("fsr"))
        {
          timestamp = Integer.parseInt(phase1[1].trim());
          FSRStatus = Integer.parseInt(phase1[2].trim());
        }
      }
    }
  }
      
  if(tryToCalibrate && firstCalibration)
  {
    println("Etapa de calibração - " + (_CALIBRATION_TIME - timeSinceTestStart) + " Yaw - " + lastyaw + " Pitch - " + lastpitch + "Roll - " + lastroll);
    timeSinceTestStart = millis() - testStart;
    if( timeSinceTestStart < _CALIBRATION_TIME)
    {
      if(UseQuartenions)
      {
        double [] EulerRot = new Rotation(quat[0], quat[1], quat[2], quat[3], false).getAngles(RotationOrder.XYZ);
        roll=  Math.round(100.0*(float)EulerRot[0]);
        pitch =  Math.round(-100.0*(float)EulerRot[1]);
        yaw =  Math.round(100.0*(float)EulerRot[2]);
      }
      
      lastyaw += yaw;
      lastpitch += pitch;
      lastroll += roll;
      count++;
    }
    else
    {
      
      lastyaw  = lastyaw/count;
      lastpitch = lastpitch/count;
      lastroll = lastroll/count;
      
      firstCalibration  = false;
      
    }
  }
  else
  {
    
    if(tryToCalibrate && !UseQuartenions)
    {
      //Correcao em funcao dos valores da etapa de calibracao.
      pitch = -pitch + lastpitch;
      roll = roll - lastroll;
      yaw = yaw - lastyaw;
    }

    if(UseQuartenions)
    {
      double [] EulerRot = new Rotation(quat[0], quat[1], quat[2], quat[3], false).getAngles(RotationOrder.XYZ);
       
      roll=  Math.round(100.0*(float)EulerRot[0]);
      pitch =  Math.round(-100.0*(float)EulerRot[1]);
      yaw =  Math.round(100.0*(float)EulerRot[2]);
     
      //printArray(EulerRot);
      //println("YAW = " + yaw + " ROLL = " +roll + " PITCH = " + pitch); 
      
      if(tryToCalibrate)
      { //Tenta deixar o ponto em zero.
        roll = roll - lastroll;
        pitch = -pitch + lastpitch;
        yaw = yaw - lastyaw;
      }
    }
          
          
          //println("yaw = " + yaw);
          //print(" pitch = " + pitch);
          //println(" roll = " + roll);
          
    //println("Etapa de print");
        translate(400,300,-200);
        rotateX(radians(pitch));
        rotateY(radians(yaw));
        rotateZ(radians(roll)); 
        background(255);

        //Muda cor do print quando FSR foi tocado.
        if(!UseFSR)
        {
          fill(255,228,225);
          
        }
        else
        {
          if(FSRStatus == 1)
          {
            fill (255,228,225);
          }
          else
          {
            fill(255,0,0);
          }
        }
          
          box(200);

        //eixo X, y, z
        line(0,300,0,0,-300,0);
        line(300,0,0,-300,0,0);
        line(0,0,300,0,0,-300);
        translate(400,300,-200);
        }
        
  
}

//Outra forma de resolver o problema da angulacao...
//Calculo direto de quartenio para angulo.
/*float[] ToEulerAngle(float w, float i, float j, float k)
{
                     //Yaw Roll Pitch
  float[] output = {0.0,0.0,0.0}; 
  
  float sinr = +2.0 * (w * i + j * k);
  float cosr = +1.0 - 2.0 * (i * i + j * j);
  output[1] = atan2(sinr, cosr); // roll
  
  float sinp = +2.0 * (w * j - k * i);
    if(abs(sinp) >=1)
      output[2] = copySign(M_PI / 2, sinp); // use 90 degrees if out of range
    else
      output[2] = asin(sinp); // pitch

  // yaw (z-axis rotation)
  float siny = +2.0 * (w * k + i * x);
  float cosy = +1.0 - 2.0 * (j * j + k * k);  
  output[3] = atan2(siny, cosy); //yaw

  return output;
  
}
*/
