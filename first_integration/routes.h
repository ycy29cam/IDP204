// this is a header file to contain the routes in the form 0, 1, 2 for the 21 routes listed in navigation_function
// can potenitally move to storing some more global variables in here

// routes assume that the junction turned into on the last pick up is not sensed, but the junction 
// before the platform will be sensed after the 180 degree turn
// the programme should run such that the final element in the route is solely a turn 
// and no forward motion will ocur thereafter

// N.B. 3 and 4 represent arc turns if the corners are treated as junctions
// if they are just line followed then all 3s and 4s can be removed


int SA[2] = {0,2};
int AG[1] = {2};
int AR[2] = {0,1};
int GB[3] = {0,1,2};
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
int final_turns[] = {2, 0, 0, 1, 2, 0, 0, 1, 1, 0, 0, 1, 2, 0, 0, 0, 0};
int route_lengths[17] = {2, 1, 2, 3, 4, 2, 3, 3, 2, 3, 2, 3, 2, 3, 2, 4, 3};

void checkRoute(int colour_present){
    if (!colour_present){
        routeCounter += 1;
    }
}