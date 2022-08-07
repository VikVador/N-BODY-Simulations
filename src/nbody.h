//-------------------------------------
//
//           N-Body Simulation
//
//-------------------------------------
// @ Ali Sezgin        Victor Mangeleer

#ifndef Nbody_h
#define Nbody_h

#include "gifenc.h"

typedef struct NBody_t NBody;

void   nbodyFree(NBody *nb);
NBody *nbodyLoadFromFile(char *filename);
NBody *nbodyGenerateRandom(int nb);
void   nbodySimulate(NBody *nb, double dt);
double nbodyGetRadius(NBody *nb);

void   nbodyPrepareGIF(NBody *nb, char *filename, int w, int h);
void   nbodyCloseGIF(NBody *nb);
void   nbodyPlotGIF(NBody *nb, int hold);

void   nbodySimulateBHT(NBody *nb, double dt);

#endif

