 
// This sketch code is based on the RPLIDAR driver library provided by RoboPeak
#include <RPLidar.h>
#include <math.h>
HardwareSerial Serial21(2); //for esp32

int width = 400;
//int length1 = 1000;
int alart = 300;
int emergency = 200;
float angle;
float distance;

// You need to create an driver instance 
RPLidar lidar;

#define RPLIDAR_MOTOR 3 // The PWM pin for control the speed of RPLIDAR's motor.
                        // This pin should connected with the RPLIDAR's MOTOCTRL signal 
                       
                        
void setup() {
  // bind the RPLIDAR driver to the arduino hardware serial
  lidar.begin(Serial21);
  Serial.begin(115200);
  
  // set pin modes
  pinMode(RPLIDAR_MOTOR, OUTPUT);
}
float angleAtMinDist = 0;

void loop() {
  if (IS_OK(lidar.waitPoint())) {
     distance = lidar.getCurrentPoint().distance; //distance value in mm unit
    angle    = lidar.getCurrentPoint().angle; //anglue value in degree
  //  bool  startBit = lidar.getCurrentPoint().startBit; //whether this point is belong to a new scan
  //  byte  quality  = lidar.getCurrentPoint().quality; //quality of the current measurement
 if (lidar.getCurrentPoint().startBit) {
        // a new scan, display the previous data...
        // minDistance = 100000;
        angleAtMinDist = 0;
      }
      else{    
   //    Serial.println(distance);

   if(distance <= 400 && distance > 0)
   {
//    Serial.print("distance :");
//    Serial.println(distance);
//    Serial.print("angle :");
//    Serial.println(angle);
 
     if(angle >= 270 && angle <= 360)
     {
      
      Serial.println("inn");
      
      float current_angle;
      if(angle >= 270 && angle <= 360)
     current_angle = angle - 270;
     if(angle >= 0 && angle <= 90)
      current_angle = angle + 90;

     
     Serial.println(current_angle);
         
     float xvalue = distance*cos(current_angle);
     Serial.println(xvalue);
     float yvalue = distance*cos(angle);
      Serial.println(yvalue);

     
     if(xvalue <= (width/2) || xvalue >= (-(width/2)))
     {
      if(yvalue <= emergency)
      {
        Serial.print("emergency : ");
        Serial.println(emergency);

      }
      if(yvalue <= alart)
      {
        Serial.print("alart : ");
        Serial.println(alart);
      }
      
      
     }
     }
   }
          
    
    
  } 
  }else {
  //  analogWrite(RPLIDAR_MOTOR, 0); //stop the rplidar motor
    
    // try to detect RPLIDAR... 
    rplidar_response_device_info_t info;
    if (IS_OK(lidar.getDeviceInfo(info, 100))) {
       // detected...
       lidar.startScan();
       
       // start motor rotating at max allowed speed
    //   analogWrite(RPLIDAR_MOTOR, 255);
       delay(1000);
    }
  }
}
