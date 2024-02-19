#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_DCMotor *left = AFMS.getMotor(1);
Adafruit_DCMotor *right = AFMS.getMotor(2);

void setup() {
  Serial.begin(9600);

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

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Main loop executing");
  forward(200);
  delay(1000);
  stop();
  delay(1000);
  backward(200);
  delay(1000);
  stop();
  delay(1000);
  turnRight(3000);
  stop();
  delay(1000);
  turnLeft(3000);
  stop();
  delay(1000);
}
