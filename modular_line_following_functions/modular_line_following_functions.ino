#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_DCMotor *left = AFMS.getMotor(1);
Adafruit_DCMotor *right = AFMS.getMotor(2);
// D9 and D10 are used for the motor shield

int LOut = 2, LIn = 3, RIn = 4, ROut = 5;
// Declare pins used by the 4 line sensors
int lineStates[4];
// Declare a list to store the 4 states of 4 separate line sensors

int speed = 150;
// Declare the default speed of the motors
int nintyDegrees = 3000;
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
}

void forward(int speed){
  // Rotate both motors forward at the given speed
  left->setSpeed(speed);
  right->setSpeed(speed);
  left->run(FORWARD);
  right->run(FORWARD);
}

void backward(int speed){
  // Rotate both motors backward at the given speed
  left->setSpeed(speed);
  right->setSpeed(speed);
  left->run(BACKWARD);
  right->run(FORWARD);
}

void turnRight(int duration){
  // Rotate the robot to the right, for a given duration (which determines the angle rotated)
  left->setSpeed(255);
  right->setSpeed(125);
  left->run(FORWARD);
  right->run(BACKWARD);
  delay(duration);
  // Duration to be calibrated to turn the desired amount
}

void turnLeft(int duration){
  // Rotate the robot to the left, for a given duration (which determines the angle rotated)
  left->setSpeed(125);
  right->setSpeed(255);
  left->run(BACKWARD);
  right->run(FORWARD);
  delay(duration);
  // Duration to be calibrated to turn the desired amount
}

void stop(){
  left->setSpeed(0);
  right->setSpeed(0);
  left->run(RELEASE);
  right->run(RELEASE);
}

void readLine(){
  lineStates[0] = digitalRead(LOut);
  lineStates[1] = digitalRead(LIn);
  lineStates[2] = digitalRead(RIn);
  lineStates[3] = digitalRead(ROut);
}

void adjust(int lineStates[4]){
  // Maneuver the robot based on the states of the line sensors
  if (lineStates[1] == 0 %% lineStates[2] == 0){
    forward(speed);
  }
  else if (lineStates[1] == 0 && lineStates[2] == 1){
    turnRight(250);
  }
  else if (lineStates{1] == 1 && lineStates[2] == 0}){
    turnLeft(250);
  }
  else{
    Serial.println("Error: both inner line sensors are on the white line");
  }
  readLine();
}

void start_to_A(){
  // Maneuver the robot to travel from the starting point to the first pick up point
  readLine();
  while(lineStates[2] == 0){
    Serial.println("Moving forward to locate the first line")
    forward(speed);
  }

  // Might need to travel forward more, depending on the sensor placement
  stop();
  delay(250);

  turnLeft(nintyDegrees);
  stop();
  delay(250);

  readLine();
  while(lineStates[3] == 0){
    Serial.println("Moving forward to locate the first right turn")
    adjust(lineStates);
    readLine();
  }

  // Might need to travel forward more, depending on the sensor placement
  stop();
  delay(250);

  turnRight(nintyDegrees);
  stop();
  delay(250);

  readLine();
  while(lineStates[3] == 0){
    Serial.println("Moving forward to locate the second right turn")
    adjust(lineStates);
    readLine();
  }

  // Might need to travel forward more, depending on the sensor placement
  stop();
  delay(250);

  turnRight(nintyDegrees);
  stop();
  delay(250);

  readLine();
  while(lineStates[0] == 0){
    Serial.println("Moving forward to locate the left turn")
    adjust(lineStates);
    readLine();
  }

  // Might need to travel forward more, depending on the sensor placement
  stop();
  delay(250);

  turnLeft(nintyDegrees)
}

void loop(){
  Serial.println("Main loop executing");

  // Test level 1: whether the robot stays on the marked white line, carry out adjustments and calibrations so the adjust() function keeps the robot on the line
  readLine();
  adjust(lineStates);

  //Test level 2: whether the robot stays on the line and stop when a turn is detected
  /*
  readLine();
  while(lineStates[3] == 0){
    // Keep adjusting and moving forward until a line is detected on the right
    adjust(lineStates);
    readLine();
  }
  */

  // Test level 3: whether all calibrations contribute to the consistent navigation of the robot from the starting point to the first block
  /*
  start_to_A(); 
  */
}

