//-------------------------------------
//
//           N-Body Simulation
//
//-------------------------------------
// @ Ali Sezgin        Victor Mangeleer

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "body.h"


//-------------
// Fonctions  :
//-------------

struct Body_t 
{
  double mass;
  double rx;
  double ry;
  double vx;
  double vy;
  double fx;
  double fy;
};

//-----

Body *bodyCreate(double mass, double rx, double ry, double vx, double vy) 
{
  Body *b = malloc(sizeof(Body));
  b->mass = mass;
  b->rx = rx;
  b->ry = ry;
  b->vx = vx;
  b->vy = vy;
  return b;
}

//-----

Body *bodyCopy(Body *b) 
{
  return bodyCreate(b->mass, b->rx, b->ry, b->vx, b->vy);
}

//-----

void bodyFree(Body *b) 
{
  free(b);
}

//-----

double bodyGetmass(Body *b) 
{
  return b->mass;
}

//-----

double bodyGetrx(Body *b) 
{
  return b->rx;
}
//-----

double bodyGetry(Body *b) 
{
  return b->ry;
}

//-----

double bodyGetvx(Body *b) 
{
  return b->vx;
}

//-----

double bodyGetvy(Body *b) 
{
  return b->vy;
}

//-----

void bodyUpdate(Body *b, double dt) 
{
  b->vx += dt * b->fx / b->mass;
  b->vy += dt * b->fy / b->mass;
  b->rx += dt * b->vx;
  b->ry += dt * b->vy;
}

//-----

double bodyDistanceTo(Body *b1, Body *b2) 
{
  double dx = b1->rx - b2->rx;
  double dy = b1->ry - b2->ry;
  return sqrt(dx*dx + dy*dy);
}

//-----

void bodyResetForce(Body *b) 
{
  b->fx = 0.0;
  b->fy = 0.0;
}

//-----

void bodyAddForce(Body *b1, Body *b2) 
{
  double G = 6.67e-11;   // gravational constant
  double EPS = 3E4;      // softening parameter
  double dx = b2->rx - b1->rx;
  double dy = b2->ry - b1->ry;
  double dist = sqrt(dx*dx + dy*dy);
  double F = (G * b1->mass * b2->mass) / (dist*dist + EPS*EPS);
  b1->fx += F * dx / dist;
  b1->fy += F * dy / dist;
}

//-----

void bodyPrint(Body *b, FILE *fp) 
{
  fprintf(fp, "[B (%.2e,%.2e),%.2e]", b->rx, b->ry, b->mass);
}

//-----

void bodyAggregate(Body *b1, Body *b2) 
{
  double masstot = b1->mass + b2->mass;
  double newrx = (b1->rx * b1->mass + b2->rx * b2->mass)/masstot;
  double newry = (b1->ry * b1->mass + b2->ry * b2->mass)/masstot;

  b1->mass = masstot;
  b1->rx = newrx;
  b1->ry = newry;
}

//-----







