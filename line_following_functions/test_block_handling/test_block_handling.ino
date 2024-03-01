#include "block_handle.h"

void setup() {
  Serial.being(9600);

  pinMode(LOut, INPUT);
  pinMode(LIn, INPUT);
  pinMode(RIn, INPUT);
  pinMode(ROut, INPUT);
  pinMode(InfraredSensorPin,INPUT);
  pinMode(button, INPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);

  // Magical code (but why?)
  // https://forum.arduino.cc/t/my-void-setup-is-repeating/669468/7 (potential reason?)
  if (!AFMS.begin()) {
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
}

void loop() {
  while(digitalRead(button) == 0){
    // Wait for the start button to the pressed
    delay(100);
  }

  Serial.print("Begin");
    grab_block();
}
