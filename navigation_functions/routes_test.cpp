#include <iostream>

int SA[3] = {0,2,1};
int AG[1] = {2};
int AR[2] = {0,1};
int GB[4] = {0,1,2,2};
int RB[6] = {0,0,4,0,2,1};
int BG[4] = {2,4,0,0};
int BR[4] = {1,3,0,0};
int GC[5] = {0,0,3,0,1};
int RC[4] = {0,0,4,2};
int CG[4] = {0,4,0,0};
int CR[3] = {3,0,0};
int GD[4] = {0,1,0,1};
int RD[3] = {0,2,2};
int DG[3] = {0,2,0};
int DR[2] = {1,0};
int GS[4] = {1,0,1,0};
int RS[3] = {2,2,0};

int* routes[17] = {SA, AG, AR, GB, RB, BG, BR, GC, RC, CG, CR, GD, RD, DG, DR, GS, RS};


std::cout << routes[0][1] << std::endl;