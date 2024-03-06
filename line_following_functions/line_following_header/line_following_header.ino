#include <Adafruit_MotorShield.h>
#include "movements.h"
// Movement functions included as a header file

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_DCMotor *left = AFMS.getMotor(1);
Adafruit_DCMotor *right = AFMS.getMotor(2);
// D9 and D10 are used for the motor shield

int LOut = 1, LIn = 2, RIn = 3, ROut = 4;
// Declare pins used by the 4 line sensors
int lineStates[4];
// Declare a list to store the 4 states of 4 separate line sensors

int speed = 200;
// Declare the default speed of the motors
int nintyDegrees = 700;
// Declare the time required for the robot to turn ninty degrees using the turnLeft or turnRight functions

void setup(){
  Serial.begin(9600);

  pinMode(LOut, INPUT);
  pinMode(LIn, INPUT);
  pinMode(RIn, INPUT);
  pinMode(ROut, INPUT);

  // Magical code (but why?)
  // https://forum.arduino.cc/t/my-void-setup-is-repeating/669468/7 (potential reason?)
  if (!AFMS.begin()) {
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

  Serial.println("Set up done executing");

  left->setSpeed(speed);
  right->setSpeed(speed);
  left->run(BACKWARD);
  right->run(BACKWARD);
  delay(1000);

  left->setSpeed(0);
  right->setSpeed(0);
  left->run(RELEASE);
  right->run(RELEASE);
}

void start_to_A(){
  // Maneuver the robot to travel from the starting point to the first pick up point
  readLine();
  while(lineStates[2] == 0){
    Serial.println("Moving forward to locate the first line");
    forward(speed);
    readLine();
  }

  // Might need to travel forward more, depending on the sensor placement
  stop();
  delay(250);

  turnLeft();
  delay(nintyDegrees);
  stop();
  delay(250);

  readLine();
  while(lineStates[3] == 0){
    Serial.println("Moving forward to locate the first right turn");
    adjust(lineStates);
    readLine();
  }

  // Might need to travel forward more, depending on the sensor placement
  stop();
  delay(250);

  turnRight();
  delay(nintyDegrees);
  stop();
  delay(250);
}

void loop(){
  // Test level 1: whether the robot stays on the marked white line, carry out adjustments and calibrations so the adjust() function keeps the robot on the line
  //readLine();
  //adjust(lineStates);

  //Test level 2: whether the robot stays on the line and stop when a turn is detected
  /*
  readLine();
  while(lineStates[3] == 0){
    // Keep adjusting and moving forward until a line is detected on the right
    adjust(lineStates);
    readLine();
  }

  stop();
  */

  // Test level 3: whether all calibrations contribute to the consistent navigation of the robot from the starting point to the first block
  /*
  start_to_A(); 
  */
}


