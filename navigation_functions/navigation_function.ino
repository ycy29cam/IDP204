#include <Adafruit_MotorShield.h>

// Assuming SA, AG, AR, etc. are defined elsewhere in your code
int routes[] = {SA, AG, AR, AB, GB, RB, BG, BR, BC, GC, RC, CG, CR, CD, GD, RD, DG, DR, DS, GS, RS};

int route_counter = 0;

void loop() {
    // drive to the first checkpoint
    drive_route(routes[0]);

    // check for block
    bool block_present = block_detect();

    // run delivery of block if present
    if (block_present) {
        // detect the block's color
        bool colour_present = colour_detect();

        // pick up the block and return to the track
        pick_up_block();

        // adjust route depending on the platform required
        if (colour_present == black) {
            route_counter += 1;
        } else if (colour_present == red) {
            route_counter += 2;
        }

        // deliver block to the platform and proceed to the next location
        drive_route(routes[route_counter]);
        drop_off_block();
        route_counter += 3;
        drive_route(routes[route_counter]);
    }

    // if no block detected, proceed to the next station
    else if (!block_present) {
        route_counter += 3;
        drive_route(routes[route_counter]);
        route_counter += 2;
    }
}
