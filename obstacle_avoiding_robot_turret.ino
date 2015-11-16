



#include <AFMotor.h> //import your motor shield library
#include <NewPing.h>
#include <Servo.h>
#include "Ultrasonic.h"

// pin used to TRIGGER the ultrasonic sensor.
#define TRIGGER_PIN  A1  
// pin used to ECHO the ultrasonic sensor
#define ECHO_PIN     A4
// Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define MAX_DISTANCE 200 

//threshold for obstacles (in cm)
const int dangerThresh = 20; 
//distances on either side
int leftDistance, rightDistance;
Ultrasonic ultrasonic(TRIGGER_PIN,ECHO_PIN);

// set up motors.
AF_DCMotor motor1(1,MOTOR12_64KHZ); 
AF_DCMotor motor2(2, MOTOR12_8KHZ);
Servo panMotor;
// NewPing setup of pins and maximum distance.
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 
 
void setup() {
  Serial.begin(9600);
  //set the speed of the motors, between 0-255
  //105 seems best, too fast, sensor can not keep up, maybe a different delay could help that
  motor1.setSpeed(105); 
  motor2.setSpeed (105); 
  // turn on servo
  panMotor.attach(9); 
  panMotor.write(90);
}
 
void loop()
{
    int distanceFwd = ultrasonic.Ranging(CM);
    Serial.print("distanceFwd : ");
    Serial.println(distanceFwd );
    //if path is clear
    if (distanceFwd == 0 || distanceFwd>dangerThresh) 
    {
      motor1.run(FORWARD);
      motor2.run(FORWARD); //move forward
    }
    else //if path is blocked
    {
      Serial.println("blocked!");
      motor1.run(RELEASE);
      motor2.run(RELEASE);
      
      panMotor.write(0);
      delay(500);
      
      rightDistance = ultrasonic.Ranging(CM); //scan to the right
      Serial.print("rightDistance : ");
      Serial.println( rightDistance );
      delay(500);
      
      panMotor.write(180);
      delay(700);
      
      leftDistance = ultrasonic.Ranging(CM); //scan to the left
      Serial.print("leftDistance : ");
      Serial.print( leftDistance );
      delay(500);

      //return the turret servo center
      panMotor.write(90); 
      
      delay(100);
      
      compareDistance();
    
    }

}
  void compareDistance()
  {
    //if left is less obstructed
    if (leftDistance>rightDistance) 
    {
      motor1.run(BACKWARD);
      motor2.run(FORWARD);
      delay(500);
    }
     //if right is less obstructed
    else if (rightDistance>leftDistance)
    {
      motor1.run(FORWARD);
      motor2.run(BACKWARD);
      delay(500);
    
    }
    //if they are equally obstructed
    else 
    {
      motor1.run(FORWARD);
      motor2.run(BACKWARD);
      delay(1000);
    }
 
 
}
