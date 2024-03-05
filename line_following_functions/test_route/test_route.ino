#include <Adafruit_MotorShield.h>
#include "movements.h"

long startLine;

void setup() {
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

void loop() { 
  Serial.println("Main loop running");
  delay(5000);

  readLine();
  while(lineStates[0] == 0 && lineStates[3] == 0){
    forward(speed);
    readLine();
  }
  stop();
  delay(250);
  
  forward(speed);
  delay(1000);

  readLine();
  while(lineStates[0] == 0){
    adjust(lineStates);
    readLine();
  }

  stop();
  delay(250);

  turn(2);

  readLine();
  while(lineStates[3] == 0){
    adjust(lineStates);
    readLine();
  }

  stop();
  delay(250);

  //turn(1);

  forward(speed);
  delay(500);
  turnRight();
  delay(700);
  readLine();
  while(lineStates[1] == 0){
    turnRight();
    readLine();
  }

  stop();
  delay(250);
  startLine = millis();

  while((millis() - startLine) < 1000){
  adjust(lineStates);
  readLine();
  }

  stop();
  delay(2000);

  readLine();
  while(lineStates[0] == 0){
    backward(speed);
    readLine();
  }

  stop();
  delay(250);

  turn(2);

  readLine();
  while(lineStates[0] == 0){
    adjust(lineStates);
    readLine();
  }

  stop();
  delay(250);

  turn(2);

  readLine();
  while(lineStates[0] == 0 || lineStates[1] == 0 || lineStates[2] == 0 || lineStates[3] == 0){
    adjust(lineStates);
    readLine();
  }

  stop();
  delay(2000);
}
