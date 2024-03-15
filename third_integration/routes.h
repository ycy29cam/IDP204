// A header file containing all possible routes, the final turns before approaching pick-up points and route lengths

// The programme should run such that the final element in the route is solely a turn 

bool previous_colour;
// Declare a boolean variable to store the colour of the previous block (for adjusting the index in the route array)

int SA[2] = {0,2};
int AG[1] = {2};
int AR[2] = {0,1};
int GB[3] = {0,1,2};
int RB[4] = {0,0,0,2};
int BG[3] = {2,0,0};
int BR[4] = {1,0,0,0};
int GC[3] = {0,0,0};
int RC[2] = {0,0};
int CG[3] = {0,0,0};
int CR[2] = {0,0};
int GD[3] = {0,1,0};
int RD[2] = {0,2};
int DG[3] = {0,2,0};
int DR[2] = {1,0};
int GS[3] = {1,0,1};
int RS[2] = {2,2};

int* routes[17] = {SA, AG, AR, GB, RB, BG, BR, GC, RC, CG, CR, GD, RD, DG, DR, GS, RS};
// An array of pointers to all routes
int final_turns[17] = {1, 0, 0, 2, 1, 0, 0, 1, 2, 0, 0, 1, 2, 0, 0, 0, 0};
// An array of turns to be made before pick-up points
int route_lengths[17] = {2, 1, 2, 3, 4, 3, 4, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2};
// An array of route lengths, so each route can be iterated

void checkRoute(int colour_present){
  // Adjust the position in the routes array based on the colour of the previous block
  // The AGV can get to point B via routes[4] or routes[5]
  // However, in order for BR (B to red) to be routeCounter += 1 for both scenarios, routeCounter has to be the same
    if (!colour_present){
        routeCounter += 1;
    }
    else {
      routeCounter = routeCounter;
    }
}