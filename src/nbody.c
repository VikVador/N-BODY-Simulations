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
#include "body.h"
#include "nbody.h"
#include "bhtree.h"
#include "gifenc.h"

//------------
// Structure :
//------------

struct NBody_t 
{
  int nbodies;
  int nbframes;
  double radius;

  ge_GIF *gif;
  BHTree *tree;
  Body **bodies;
};

//----------------
// Nos fonctions :
//----------------

void nbodySimulateBHT(NBody *nb, double dt) 
{
  // ETAPE 1 : Initialisation du quadrant et de l'arbre
  Quad *quadrant = quadCreate(0, 0, 2 * nb -> radius);
  nb -> tree = bhtreeCreate(quadrant);
  
  // ETAPE 2 : Mise à jour de la position des particules 
  // (Insertion, Reset des forces, Update des forces, Update des positions)
  for (int i = 0 ; i < nb -> nbodies ; i++)
  {
    bhtreeInsert(nb -> tree, nb -> bodies[i]);
  }

  for(int i = 0 ; i < nb -> nbodies ; i++)
  {
    bodyResetForce(nb -> bodies[i]);
  }

  for(int i = 0 ; i < nb -> nbodies ; i++)
  {
    bhtreeUpdateForce(nb -> tree, nb->bodies[i]);
  }

  for(int i = 0 ; i < nb -> nbodies ; i++)
  {
    bodyUpdate(nb->bodies[i], dt);
  }
}

// REMARQUE : nbodyFree se charge d'utiliser bhtreeFree
//------------
// Fonctions :
//------------

void nbodySimulate(NBody *nb, double dt) 
{
  int n = nb->nbodies;
  
  // update the forces
  for (int i = 0; i < n; i++) 
  {
    bodyResetForce(nb->bodies[i]);
    
    for (int j = 0; j < n; j++) 
    {
      if (i != j)
        bodyAddForce(nb->bodies[i], nb->bodies[j]);
    }
  }
  
  // update the bodies
  for (int i = 0; i < n; i++) {
    bodyUpdate(nb->bodies[i], dt);
  }
}

//------------

NBody *nbodyGenerateRandom(int n) 
{
  double radius = 5e08;
  NBody *nbody = malloc(sizeof(NBody));
  Body **bodies = malloc(n * sizeof(Body *));
  nbody->bodies = bodies;
  nbody->nbodies = n;
  nbody->radius = radius;
  nbody->tree = NULL;

  bodies[0] = bodyCreate(10e34, 0.0, 0.0, 0, 0);
  
  for (int i = 1; i < n; i++) {
    double rx = ((double)rand() / (double)RAND_MAX*2*radius-radius);
    double ry = ((double)rand() / (double)RAND_MAX*2*radius-radius);

    // make velocity perpendicular to position wrt (0,0) so that they
    // are in orbit around body at (0,0).
    double vx = -(ry/rx);
    double vy = 1.0;
    double br = 1.0-2.0*(rand()%2);
    double rnv = br*((double)rand() / (double)RAND_MAX*2*radius)/(5*sqrt(vx*vx+vy*vy));
    
    vx = rnv * vx;
    vy = rnv * vy;
    
    bodies[i] = bodyCreate(10.0, rx, ry, vx, vy);
  }
  
  return nbody;
}

//------------

NBody *nbodyLoadFromFile(char *filename) 
{
  FILE *fp = fopen(filename,"r");

  // read number of bodies
  int n;
  fscanf(fp, "%d", &n);

  double radius;
  fscanf(fp, "%lf", &radius);
  
  NBody *nbody = malloc(sizeof(NBody));
  Body **bodies = malloc(n*sizeof(Body *));
  nbody->bodies = bodies;
  nbody->nbodies = n;
  nbody->radius = radius;
  nbody->tree = NULL;
  
  for (int i = 0; i < n; i++) {
    double rx, ry, vx, vy, mass;
    fscanf(fp,"%lf",&rx);
    fscanf(fp,"%lf",&ry);
    fscanf(fp,"%lf",&vx);
    fscanf(fp,"%lf",&vy);
    fscanf(fp,"%lf",&mass);
    bodies[i] = bodyCreate(mass, rx, ry, vx, vy);
  }
  return nbody;  
}

//------------

double nbodyGetRadius(NBody *nb) 
{
  return nb->radius;
}

//------------

void nbodyFree(NBody *nb) 
{
  for (int i = 0; i < nb->nbodies; i++) 
  {
    free(nb->bodies[i]);
  }
  if (nb->tree != NULL)
    bhtreeFree(nb->tree);
  free(nb);
}

//------------
// To generate the GIF file

void nbodyPrepareGIF(NBody *nb, char *filename, int w, int h) 
{
  nb->gif = ge_new_gif(filename, w, h, (uint8_t []) 
  {  /* palette */
      0x00, 0x00, 0x00, /* 0 -> black */
	  0xFF, 0xFF, 0xFF, /* 1 -> white */
	0x00, 0xFF, 0x00, /* 2 -> green */
	0xFF, 0x00, 0x00 /* 3 -> blue */
	},
    2,              /* palette depth == log2(# of colors) */
    0               /* infinite loop */
    );

  for (int j = 0; j < (w * h); j++)
    nb->gif->frame[j] = 0;

  nb->nbframes = 0;
}

//------------

void nbodyCloseGIF(NBody *nb) {
  ge_close_gif(nb->gif);
  nb->gif = NULL;
}

//------------

void nbodyPlotGIF(NBody *nb, int hold) 
{
  ge_GIF *gif = nb->gif;

  if (gif == NULL) {
    fprintf(stderr, "nbodyPrepareGIF has not been run\n");
    return;
  }
  
  // black background
  if (!hold)
    for (int j = 0; j < (gif->w * gif->h); j++)
      gif->frame[j] = 0;
  
  // Plot the bodies
  for (int i = 0; i < nb->nbodies; i++) {
    int posx = (int)((bodyGetrx(nb->bodies[i]) + nb->radius)/(2*nb->radius) * (double)gif->w);
    int posy = gif->h - (int)((bodyGetry(nb->bodies[i]) + nb->radius)/(2*nb->radius) * (double)gif->h);
    if ((posx>0) && (posx<gif->w-1) && (posy>0) && (posy<gif->h-1)) {
      // we don't plot bodies too close to the border
      gif->frame[posy * gif->w + posx] = 1;
      gif->frame[posy * gif->w + posx+1] = 1;
      gif->frame[posy * gif->w + posx-1] = 1;
      gif->frame[(posy-1) * gif->w + posx] = 1;
      gif->frame[(posy+1) * gif->w + posx] = 1;
    }
  }

  ge_add_frame(gif,5);
  nb->nbframes++;
}

//------------
