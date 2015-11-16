/* 
 *  This uses a combo of NewPhing and AFMotor with the motor shield and the HC-SR04 ultra sonic ping sensor
 *  The basic idea is to constanlty scan for obsticles, if found, turn right until not found.
 *  
 *  based loosely off of 
 *  http://www.instructables.com/id/Arduino-Ultimate-Obstacle-Avoiding-Robot/
 *  Cheers to Roy for his donation
 * 
*/

#include <AFMotor.h> //import your motor shield library
#include <NewPing.h>

// pin used to TRIGGER the ultrasonic sensor.
#define TRIGGER_PIN  A1  
// pin used to ECHO the ultrasonic sensor
#define ECHO_PIN     A4
// Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define MAX_DISTANCE 200 

// set up motors using the commom AFMotr library
AF_DCMotor motor1(1,MOTOR12_64KHZ); 
AF_DCMotor motor2(2,MOTOR12_8KHZ);

//setup teh ping sensor
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

long distance;
unsigned int duration;
 
void setup() {
  //for debugging
  Serial.begin(9600); 
  Serial.println("Robot Engange!");
  //set the speed of the motors, between 0-255. 
  //105 is a safe speed, too fast and the ping sensore has issues 
  motor1.setSpeed(105); 
  motor2.setSpeed (105);  
}
 
void loop() {
  // Send ping, get ping time in microseconds (uS).
   duration = sonar.ping(); 
   // convert the distance to centimeters.
   distance = (duration/US_ROUNDTRIP_CM);
   Serial.print ("Distance From Robot is " );
   Serial.print (distance);
   Serial.print (" CM!");

   /*if there's an obstacle 25 centimers, ahead, do the following: */   
   if (distance < 25) {
      Serial.println ("Close Obstacle detected ahead: " );
      Serial.print(distance);
      Serial.println ("CM");
      motor1.run(FORWARD);  
      motor2.run (BACKWARD);
  } else {
     Serial.println ("No obstacle detected. going forward");
     delay (50);
     motor1.run(FORWARD);
     motor2.run(FORWARD);  
  } 
}
