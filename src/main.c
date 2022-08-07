//-------------------------------------
//
//           N-Body Simulation
//
//-------------------------------------
// @ Ali Sezgin        Victor Mangeleer

#include "gui.h"
#include "nbody.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char **argv) 
{

  // Terminal GUI
  title();

  // Checking arguments
  if (argc != 7) 
  {
    usage();
    return -1;
  }

  // Initialization
  NBody *nb;
  if (strcmp(argv[1],"random") == 0) 
  {  
    nb = nbodyGenerateRandom(atoi(argv[2]));
  }
  else if (strcmp(argv[1],"file")==0) 
  {
    nb = nbodyLoadFromFile(argv[2]);
  }
  else 
  {
    usage();
    exit(-1);
  }
  double maxt = atof(argv[4]);
  double dt = atof(argv[5]);
  int bht = 0;
  if (strcmp(argv[3],"bht")==0)
    bht = 1;

  int plot = 0;
  if (strcmp(argv[6],"gif") == 0) 
  {
    plot = 1;
    nbodyPrepareGIF(nb, "results/simulation.gif", 500, 500);
    nbodyPlotGIF(nb,0);
  }

  // Terminal GUI
  simulation(argv[3], argv[2]);
  computing();

  // Computing results
  clock_t begin = clock();
  for (double t=0.0; t<maxt; t += dt) 
  {
    fprintf(stdout,".");
    fflush(stdout);
    if (bht)
      nbodySimulateBHT(nb, dt);
    else
      nbodySimulate(nb, dt);
    if (plot)
      nbodyPlotGIF(nb,0);
  }
  fprintf(stdout,"\n");
  fflush(stdout);
  clock_t end = clock();

  // Terminal GUI
  results();

  double time_spent = (double)(end-begin)/ (double)CLOCKS_PER_SEC;
  printf("\nSimulation time: %f [s]\n\n", time_spent);
  if (plot)
    nbodyCloseGIF(nb);
  nbodyFree(nb);
  return 0;
}
