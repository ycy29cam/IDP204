#include <Adafruit_MotorShield.h>
#include "movements.h"

int routeCounter = 0; // Index of the current route, in the 2D array of all possible routes
int red = 7; // Red LED
int green = 8; // Green LED
int button = 13; // Button
int routeNumber = 17; // Total number of routes

int SA[2] = {0,2};
int AG[1] = {2};
int AR[2] = {0,1};
int GB[2] = {0,1,2};
int RB[4] = {0,0,0,2};
int BG[2] = {2,0};
int BR[3] = {1,3,0};
int GC[3] = {0,0,0};
int RC[2] = {0,0};
int CG[3] = {0,4,0};
int CR[2] = {3,0};
int GD[3] = {0,1,0};
int RD[2] = {0,2};
int DG[3] = {0,2,0};
int DR[2] = {1,0};
int GS[4] = {1,0,1,0};
int RS[3] = {2,2,0};

int* routes[17] = {SA, AG, AR, GB, RB, BG, BR, GC, RC, CG, CR, GD, RD, DG, DR, GS, RS};
int route_lengths[17] = {2, 1, 2, 2, 4, 2, 3, 3, 2, 3, 2, 3, 2, 3, 2, 4, 3};

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
  // Declare all inputs and outputs

  // Magical code (but why?)
  // https://forum.arduino.cc/t/my-void-setup-is-repeating/669468/7 (potential reason?)
  if (!AFMS.begin()) {
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
}

void drive_route(int* journey, int number_of_junctions) {
   
    int journey_count = 0;  
    while (journey_count < number_of_junctions) {

        // Move forward consistently
        readLine();
        while(lineStates[0] == 0 && lineStates[3] == 0){
            adjust(lineStates);
            readLine();
        }

        stop();
        delay(250);

        if (journey[journey_count] == 0) {
                // robot will continue to move forwards
            forward(250);
        }
        else if (journey[journey_count] == 1) { 
            turn(1);
        }
        else if (journey[journey_count] == 2) {
            turn(2);
        }

        stop();
        delay(250);
        journey_count++;
    }
}

void loop() {
  Serial.print("Begin");
  while(routeCounter < routeNumber){
    // While the current route index is in range of the 2D array
    drive_route(routes[routeCounter], route_lengths[routeCounter]);
    // Drive the robot according to the route, to a pick up point, from either the start or the drop off points
    routeCounter += 2;
  }
}