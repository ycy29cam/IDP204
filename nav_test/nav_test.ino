#include <Adafruit_MotorShield.h>
#include "movements.h"
#include "routes.h"
#include "block_handle.h"

int routeCounter = 0; // Index of the current route, in the 2D array of all possible routes
int red = 7; // Red LED
int green = 8; // Green LED
int button = 13; // Button
int routeNumber = 17; // Total number of routes

void setup() {
  Serial.begin(9600);

  pinMode(LOut, INPUT);
  pinMode(LIn, INPUT);
  pinMode(RIn, INPUT);
  pinMode(ROut, INPUT);
  pinMode(InfraredSensorPin,INPUT);
  pinMode(button, INPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  // Declare all inputs and outputs

  // Magical code (but why?)
  // https://forum.arduino.cc/t/my-void-setup-is-repeating/669468/7 (potential reason?)
  if (!AFMS.begin()) {
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
}

void loop() {

  Serial.print("Begin");

  while(routeCounter < routeNumber){
    drive_route(routes[routeCounter], route_lengths[routeCounter]);
    if (routeCounter >= 4){checkRoute(true); }
    // approach the block and pick it up
    approach_block(final_turns[routeCounter]);
    leave(routeCounter, true);
  }
}