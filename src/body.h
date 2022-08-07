//-------------------------------------
//
//           N-Body Simulation
//
//-------------------------------------
// @ Ali Sezgin        Victor Mangeleer

#include <stdio.h>

#ifndef Body_h
#define Body_h

typedef struct Body_t Body;

Body  *bodyCreate(double mass, double rx, double ry, double vx, double vy);
Body  *bodyCopy(Body *b);
void   bodyFree(Body *b);

double bodyGetmass(Body *body);
double bodyGetrx(Body *body);
double bodyGetry(Body *body);
double bodyGetvx(Body *body);
double bodyGetvy(Body *body);

void   bodyUpdate(Body *b, double dt);

double bodyDistanceTo(Body *b1, Body *b2);
void   bodyResetForce(Body *b);
void   bodyAddForce(Body *a, Body *b);
void   bodyPrint(Body *b, FILE *fp);

void   bodyAggregate(Body *b1, Body *b2);

#endif
