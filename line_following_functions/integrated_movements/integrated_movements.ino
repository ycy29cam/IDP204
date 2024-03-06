#include <Adafruit_MotorShield.h>
#include "movements.h"
#include "routes.h"
#include "block_handling.h"

int button = 13; // Button
int routeNumber = 17; // Total number of routes

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(LOut, INPUT);
  pinMode(LIn, INPUT);
  pinMode(RIn, INPUT);
  pinMode(ROut, INPUT);

  pinMode(blue, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);

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
  // put your main code here, to run repeatedly:
  while(routeCounter < routeNumber){
    Serial.println("Currently in loop");
    Serial.print(routes[routeCounter][2]);
    Serial.println(route_lengths[routeCounter]);
    drive_route(routes[routeCounter], route_lengths[routeCounter]);

    if (routeCounter >= 4){checkRoute(true); }
    // approach the block and pick it up
    approach_block(final_turns[routeCounter]); // Direction to be specified

    // detect the block's color
    // black block returns false, red block returns true
    // bool colour_present = colour_detect();

    tellColour(true); // Light up the corresponding LED, currently set to RED

    // exit station: reverse out and make a 90 degree turn with direction decided by station and colour
    // adjust route depending on the platform required
    leave(true); // Increment routeCounter by 1 if black, 2 if red

    drive_route(routes[routeCounter], route_lengths[routeCounter]);

    startLine = millis();

    while((millis() - startLine) < 1000){
    adjust(lineStates);
    readLine();
    }

    stop();
    delay(2000); // This is where the drop off function will go

    backward(speed);
    delay(1000);

    stop();
    delay(250);

    // Assuming all blocks are red
    turnRight();
    delay(500);
    readLine();
    while(lineStates[1] == 0){
      turnRight();
      readLine();
    }

    routeCounter += 2;
    // dropOffBlock(); // Increment routeCounter by 2 (drop off point to the next pick up point)
  }
}
