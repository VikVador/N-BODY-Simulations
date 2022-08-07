//-------------------------------------
//
//           N-Body Simulation
//
//-------------------------------------
// @ Ali Sezgin        Victor Mangeleer

#include "gui.h"
#include <stdio.h>


//------------
// Fonctions :
//------------
// GUI Title
void title(void)
{
	printf("\n\n    ~+															 \n");
	printf("                 *       +											 \n");
	printf("           '                  |										 \n");
	printf("       ()    .-.,=----=.    - o -         -----------------			 \n");
	printf("             '=/_       |    |		  N-Body Simulation			     \n");
	printf("          *   |  '=._    |                -----------------			 \n");
	printf("               |     `=./`,         								 \n");
	printf("            .   '=.__.=' `='       Ali Sezgin     Victor Mangeleer   \n");
	printf("   +                         +										 \n");
	printf("        O      *        '       .									 \n\n");
}

// GUI Simulation
void simulation(char *method, char *filename)
{
	printf("\n-------------\n Simulation :\n-------------\n\n");
	printf("Method : %s \n\n", method);
	printf("File   : %s \n", filename);
}


// GUI Computing 
void computing(void)
{
	printf("\n--------------\n Computation :\n--------------\n");
}


// GUI Results
void results(void)
{
	printf("\n----------\n Results :\n----------\n");
}

// GUI Usage
void usage(void) 
{
	printf("\n-------\nUSAGE :\n-------\n\n");
	printf("<executable> <datatype> <method> <maxt> <deltat> <output>\n\n");
	printf("where,\n\n");
	printf("- <datatype> : 'random <n>' to generate a random galaxy of n bodies or 'file <filename>' to load a galaxy from a file.\n");
	printf("- <method>   : simulation methods either naive or bht\n");
	printf("- <maxt>     : the total duration, in real time, of the simulation\n");
	printf("- <deltat>   : the delta of time (use 0.1)\n");
	printf("- <output>   : none (to estimate computing times) or gif\n");
}

