#include "Arduino.h"
#include "Wire.h"
#include "DFRobot_VL53L0X.h"
DFRobot_VL53L0X sensor;
#include <Adafruit_MotorShield.h>
#include <Servo.h>
#include "movements.h"
#include "routes.h"
#include "block_handle.h"

int button = 13; // Button
int routeNumber = 17; // Total number of routes

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

void loop() {
  // put your main code here, to run repeatedly:
  backward(speed);
  delay(2100); // To be hard-coded
  stop();
  readLine();
  turnRight();
  delay(800);
  readLine();
  while(lineStates[2] == 0){
    turnRight();
    readLine();
  }
  stop();
  delay(250);
  readLine();
  while(lineStates[0] == 0 && lineStates[3] == 0){
    adjust(lineStates);
    readLine();
  }
  stop();
  delay(5000);
}
