//-------------------------------------
//
//           N-Body Simulation
//
//-------------------------------------
// @ Ali Sezgin        Victor Mangeleer


#ifndef Quad_h
#define Quad_h

#include <stdio.h>

typedef struct Quad_t Quad;

Quad   *quadCreate(double xmid, double ymid, double length);
void    quadFree(Quad *q);
int     quadContains(Quad *q, double x, double y);
double  quadLength(Quad *q);
Quad   *quadNW(Quad *q);
Quad   *quadNE(Quad *q);
Quad   *quadSW(Quad *q);
Quad   *quadSE(Quad *q);
void    quadPrint(Quad *q, FILE *fp);

#endif
