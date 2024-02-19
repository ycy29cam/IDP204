#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *left = AFMS.getMotor(1);
Adafruit_DCMotor *right = AFMS.getMotor(2);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void forward(int speed){
  left->setSpped(speed);
  right->setSpeed(speed);
  left->run(FORWARD);
  right->run(FORWARD);
}

void backward(int speed){
  left->setSpped(speed);
  right->setSpeed(speed);
  left->run(BACKWARD);
  right->run(BACKWARD);
}

void turnRight(int duration){
  left->setSpeed(255);
  right->setSpeed(125);
  left->run(FORWARD);
  right->run(BACKWARD);
  delay(duration);
  // Duration to be calibrated to turn the desired amount
}

void turnLeft(int duration){
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

}
