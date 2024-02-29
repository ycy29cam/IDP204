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
    // approach the block and pick it up
    approach_block(); // Direction to be specified

    // detect the block's color
    bool colour_present = colour_detect();

    tellColour(colour_present); // Light up the corresponding LED

    // exit station: reverse out and make a 90 degree turn with direction decided by station and colour
    exit(route_counter, colour_present);
  
    // adjust route depending on the platform required
    // black block returns false, red block returns true
    
    if (!colour_present) {
        route_counter += 1;
        drive_route(routes[route_counter], route_lengths[route_counter]);
        drop_off_block();
        route_counter += 2;
        drive_route(routes[route_counter], route_lengths[route_counter]);
        route_counter += 1;
    } 
    else if (colour_present) {
        route_counter += 2;
        drive_route(routes[route_counter], route_lengths[route_counter]);
        drop_off_block();
        route_counter += 2;
        drive_route(routes[route_counter], route_lengths[route_counter]);
    }
  
  
  }
}
// Not done Not done Not done