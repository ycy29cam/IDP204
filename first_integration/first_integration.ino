#include <Adafruit_MotorShield.h>
#include "movements.h"
#include "routes.h"
#include "block_handle.h"

int routeCounter = 0;
int red = 7;
int green = 8;
int button = 13;
int routeNumber = 17; // Total number of routes

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

  while(routeCounter < routeNumber){
    drive_route(routes[routeCounter], route_lengths[routeCounter]);
    if (routeCounter >= 4){checkRoute(colour_present); }
    // approach the block and pick it up
    approach_block(); // Direction to be specified

    // detect the block's color
    // black block returns false, red block returns true
    bool colour_present = colour_detect();

    tellColour(colour_present); // Light up the corresponding LED

    // exit station: reverse out and make a 90 degree turn with direction decided by station and colour
    // adjust route depending on the platform required
    exit(route_counter, colour_present); // Increment routeCounter by 1 if black, 2 if red

    drive_route(routes[routeCounter], route_lengths[routeCounter]);
    dropOffBlock(); // Increment routeCounter by 2 (drop off point to the next pick up point)
  }
}
// Not done Not done Not done