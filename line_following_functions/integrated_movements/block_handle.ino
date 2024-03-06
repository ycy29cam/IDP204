//Include all libraries necessary for operation of code
#include "Arduino.h"
#include "Wire.h"
//#include "DFRobot_VL53L0X.h"
#include "block_handle.h"
#include <Servo.h>          
#include <Adafruit_MotorShield.h>

Servo myservo1;              // create servo object to control a servo
Servo myservo2;


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
    myservo1.attach(9);      // Servo input at pin 9
    myservo2.attach(10);

    myservo1.write(110);
    myservo2.write(65);

}

void rotate_arms_to_1(int n){                    //Function to contract the grabber arms
      myservo1.write(n);                             //60 is closed and 110 is open position for the grabber
      delay(15);                         
}

void rotate_arms_to_2(int n){                    //Function to contract the grabber arms   
      myservo2.write(n);                         //18 is the down positions and 65 is the up position for the grabber arms                       
      delay(15);                         
}

void lift_arms(){                           //Function to lift arms to raised position
  int pos = 18;  
  for (pos = 18; pos <= 65; pos += 1) {    
    rotate_arms_to_2(pos);
    delay(50); 
 }
  delay(1000);
}

void lower_arms(){                          //Function to lower arms to bottom position
  int pos = 65;  
  for (pos = 65; pos >= 18; pos -= 1) { 
    rotate_arms_to_2(pos);
    delay(50); 
 }
  delay(1000);
}

void close_arms(){                        //Function to close grabber arms from open position
      rotate_arms_to_1(60);                       
      delay(1000);
}

void open_arms(){                         //Function to open grabber arms from closed position
      rotate_arms_to_1(110);
      delay(1000);
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
  open_arms();
  delay(1000);

  lower_arms();
  delay(1000);

  close_arms();
  delay(1000);

  lift_arms();
  delay(1000);

}
  
  /** //Get the distance
 Serial.print("Distance: ");Serial.println(sensor.getDistance());
 //The delay is added to demonstrate the effect, and if you do not add the delay,
 //it will not affect the measurement accuracy
 delay
}**/


