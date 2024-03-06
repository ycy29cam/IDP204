#include "Arduino.h"
#include "Wire.h"
#include "DFRobot_VL53L0X.h"
DFRobot_VL53L0X sensor;
#include <Adafruit_MotorShield.h>
#include <Servo.h>
#include "movements.h"

Servo myservo1;            
Servo myservo2;

int button = 13; // Button
int routeNumber = 17; // Total number of routes
int colour_sensor = 11;

void setup() {
  Serial.begin(9600);

  // Set up the time of flight distance sensor
  Wire.begin();
  sensor.begin(0x50);
  sensor.setMode(sensor.eContinuous,sensor.eHigh);
  sensor.start();

  pinMode(LOut, INPUT);
  pinMode(LIn, INPUT);
  pinMode(RIn, INPUT);
  pinMode(ROut, INPUT);
  pinMode(colour_sensor,INPUT);
  pinMode(button, INPUT);
  pinMode(blue, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  // Declare all inputs and outputs

  myservo1.attach(9);      // Servo input at pin 9
  myservo2.attach(10);     // Servo input at pin 10

  myservo1.write(110);     // Define initial position
  myservo2.write(65);      // Define initial position

  // Magical code (but why?)
  // https://forum.arduino.cc/t/my-void-setup-is-repeating/669468/7 (potential reason?)
  if (!AFMS.begin()) {
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
}

void rotate_arms_to_1(int n){                    //Function to contract the grabber arms
      myservo1.write(n);                             //60 is closed and 110 is open position for the grabber
      delay(15);                         
}

void rotate_arms_to_2(int n){                    //Function to contract the grabber arms   
      myservo2.write(n);                         //23 is the down positions and 65 is the up position for the grabber arms                       
      delay(15);                         
}

void lift_arms(){                           //Function to lift arms to raised position
  int pos = 27;  
  for (pos = 27; pos <= 65; pos += 1) {    
    rotate_arms_to_2(pos);
    delay(30); 
 }
  delay(1000);
}

void lower_arms(){                          //Function to lower arms to bottom position
  int pos = 65;  
  for (pos = 65; pos >= 27; pos -= 1) { 
    rotate_arms_to_2(pos);
    delay(30); 
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

void loop() {
  while(sensor.getDistance() > 100){
    Serial.print("Distance is: ");
    Serial.println(sensor.getDistance());
    forward(speed);
  }
  stop();

  close_arms();
  lift_arms();

  if (digitalRead(colour_sensor)){
    Serial.println("It's red");
    digitalWrite(red, HIGH);
  }
  else {
    Serial.println("It's black");
    digitalWrite(green, HIGH);
  }

  delay(5000);

  lower_arms();
  open_arms();

  digitalWrite(green, LOW);
  digitalWrite(red, LOW);

  delay(3000);

}
// Not done Not done Not done