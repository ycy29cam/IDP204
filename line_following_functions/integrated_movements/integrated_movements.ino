#include "movements.h"
#include "routes.h"
#include "block_handling.h"

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  while(routeCounter < routeNumber){
    drive_route(routes[routeCounter], route_lengths[routeCounter]);

    int pickup_turn = routes[routeCounter][route_lengths[routeCounter] - 1];

    if (routeCounter >= 4){checkRoute(colour_present); }
    // approach the block and pick it up
    approach_block(pickup_turn); // Direction to be specified

    // detect the block's color
    // black block returns false, red block returns true
    // bool colour_present = colour_detect();

    // tellColour(colour_present); // Light up the corresponding LED

    // exit station: reverse out and make a 90 degree turn with direction decided by station and colour
    // adjust route depending on the platform required
    leave(route_counter, colour_present); // Increment routeCounter by 1 if black, 2 if red

    drive_route(routes[routeCounter], route_lengths[routeCounter]);

    startLine = millis();

    while((millis() - startLine) < 1000){
    adjust(lineStates);
    readLine();
    }

    stop();
    delay(2000);

    routeCounter += 2;
    // dropOffBlock(); // Increment routeCounter by 2 (drop off point to the next pick up point)
  }
}
