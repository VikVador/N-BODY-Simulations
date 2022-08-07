//-------------------------------------
//
//           N-Body Simulation
//
//-------------------------------------
// @ Ali Sezgin        Victor Mangeleer

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "quad.h"

//-------------
// Prototypes :
//-------------

static void terminate(char *m);

//------------------
// Notre structure :
//------------------

struct Quad_t 
{
  double xmid;
  double ymid;
  double length;
};

//------------
// Fonctions :
//------------

Quad *quadCreate(double xmid, double ymid, double length) 
{    
    Quad *q = malloc(sizeof(Quad));
    if(q == NULL)
    {
      terminate("ERREUR (quadCreate) - L'allocation ne s'est pas faite correctement");
    }

    q -> xmid = xmid;
    q -> ymid = ymid;
    q -> length = length;

    return q;
}

//------

void quadFree(Quad *q) 
{
  free(q); 
}

//------

int quadContains(Quad *q, double x, double y) 
{
  double xmid = q -> xmid;
  double ymid = q -> ymid;
  double radius = (q -> length)/2;
  
  if (x < (xmid - radius) || x > (xmid + radius) || y < (ymid - radius) || y > (ymid + radius))
  {
    return 0;
  }

  return 1;
}

//------

double quadLength(Quad *q) 
{ 
  return q -> length;
}

//------

Quad *quadNW(Quad *q) 
{
  double xmid = q -> xmid;
  double ymid = q -> ymid;
  double radius = (q -> length)/2;

  Quad *q_NW = quadCreate(xmid - radius/2, ymid + radius/2, radius);

  return q_NW;
}

//------

Quad *quadNE(Quad *q)
{
  double xmid = q -> xmid;
  double ymid = q -> ymid;
  double radius = (q -> length)/2;

  Quad *q_NE = quadCreate(xmid + radius/2, ymid + radius/2, radius);

  return q_NE;
}

//------

Quad *quadSW(Quad *q) 
{
  double xmid = q -> xmid;
  double ymid = q -> ymid;
  double radius = (q -> length)/2;

  Quad *q_SW = quadCreate(xmid - radius/2, ymid - radius/2, radius);

  return q_SW;
}

//------

Quad *quadSE(Quad *q) 
{
  double xmid = q -> xmid;
  double ymid = q -> ymid;
  double radius = (q -> length)/2;

  Quad *q_SE = quadCreate(xmid + radius/2, ymid - radius/2, radius);

  return q_SE;
}

//------

void quadPrint(Quad *q, FILE *fp)
{
  fprintf(fp, "[Q (%.2e, %.2e), %.2e]", q -> xmid, q -> ymid, q -> length);
}

//----------------
// Nos fonctions :
//----------------
static void terminate(char *n)
{
  printf("%s\n\n", n);
  exit(-1);
}

// Affiche un message d'erreur
//------------















