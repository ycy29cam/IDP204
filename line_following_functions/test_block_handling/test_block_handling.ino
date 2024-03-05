//Include all libraries necessary for operation of code
#include "Arduino.h"
#include "Wire.h"
//#include "DFRobot_VL53L0X.h"
#include "block_handle.h"
#include <Servo.h>          
#include <Adafruit_MotorShield.h>

int angle = 19;


//DFRobot_VL53L0X sensor;

void setup() {
  //initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  /**pinMode(LOut, INPUT);
  pinMode(LIn, INPUT);
  pinMode(RIn, INPUT);
  pinMode(ROut, INPUT);
  pinMode(InfraredSensorPin,INPUT);
  pinMode(button, INPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);

   //Magical code (but why?)
  // https://forum.arduino.cc/t/my-void-setup-is-repeating/669468/7 (potential reason?)
  if (!AFMS.begin()) {
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);**/
    myservo.attach(9);      // Servo input at pin 9
  }

 

  /**join i2c bus (address optional for master)
  Wire.begin();
  //Set I2C sub-device address
  sensor.begin(0x50);
  //Set to Back-to-back mode and high precision mode
  sensor.setMode(sensor.eContinuous,sensor.eHigh);
  //Laser rangefinder begins to work
  sensor.start();
}**/

//int run_once = 0; 

void loop() {
/**  while(digitalRead(button) == 0){
    // Wait for the start button to the pressed
    delay(100);
  }**/

  /**if (run_once == 0){
        //raise_block();
        //grab_block();
        int ang = myservo.read();
        Serial.println(ang);
        run_once++; **/
 /** if (Serial.read() != 0){
    angle = Serial.read();
  }
  else{
    angle = angle;
  }
  myservo.write(angle);**/
  int ang = myservo.read();
  Serial.println(ang);
  delay(300);

}
  
  /** //Get the distance
 Serial.print("Distance: ");Serial.println(sensor.getDistance());
 //The delay is added to demonstrate the effect, and if you do not add the delay,
 //it will not affect the measurement accuracy
 delay
}**/



