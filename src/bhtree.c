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
#include "quad.h"
#include "bhtree.h"

#define theta 0.5


//-------------
// Prototypes :
//-------------

static void terminate(char *n);

static int distanceToRoot(BHTree *tree);

static void bhtreeInsertAux(BHTree *bht, Body *b);

static void bhtreePrintAux(BHTree *tree, char *q_XX, FILE *fp);

//------------------
// Notre structure :
//------------------

struct BHTree_t 
{	
	BHTree *q_NW;
	BHTree *q_NE;
	BHTree *q_SW;
	BHTree *q_SE;
	BHTree *parent;
	Quad *quadrant;
	Body *particule;
};

// REMARQUE : Si parent == NULL, le noeud est la racine.
//------------
// Fonctions :
//------------

BHTree *bhtreeCreate(Quad *q) 
{
  // Hypothèse
  if(q == NULL)
  {
  	terminate("ERREUR (bhtreeCreate) - Un pointeur nul a été passé en argument");
  }

  // Création du noeud
  BHTree *tree = malloc(sizeof(BHTree));
  if(tree == NULL)
  {
  	terminate("ERREUR (bthreeCreate) - L'allocation ne s'est pas faite correctement");
  }

  // Initialisation du noeud
  tree -> quadrant = q;
  tree -> q_NW = NULL;
  tree -> q_NE = NULL;
  tree -> q_SW = NULL;
  tree -> q_SE = NULL;
  tree -> parent = NULL;
  tree -> particule = NULL;

  return tree;
}

//------------

void bhtreeFree(BHTree *bht) 
{
  // Descente jusqu'au noeuds externes
  if(bht -> q_NW != NULL)
  {
  	bhtreeFree(bht -> q_NW);
  	bhtreeFree(bht -> q_NE);
  	bhtreeFree(bht -> q_SW);
  	bhtreeFree(bht -> q_SE);
  }

  // Libération des informations
  quadFree(bht -> quadrant);
  bodyFree(bht -> particule);
  free(bht);
}

//------------

void bhtreeInsert(BHTree *bht, Body *b) 
{	
	// Hypothèse
	if(b == NULL)
	{
		terminate("ERREUR (bhtreeInsert) - Un pointeur nul a été passé en argument");
	}

	// CAS 1 : Le noeud ne contient pas de particule, on lui associe b
	if(bht -> particule == NULL)
	{
		bht -> particule = bodyCopy(b);
		return;
	}

	// CAS 2 : Le noeud est interne, MAJ du CM + insertion de b dans le fils correspondant
	else if(bht -> q_NW != NULL)
	{
		bodyAggregate(bht -> particule, b);
		bhtreeInsertAux(bht, b);
	}

	// CAS 3 : Le noeud est externe et contient une particule b', création des fils + insertion de b et b'
	else
	{
		// Création des fils
		bht -> q_NW = bhtreeCreate(quadNW(bht -> quadrant));
		bht -> q_NE = bhtreeCreate(quadNE(bht -> quadrant));
		bht -> q_SW = bhtreeCreate(quadSW(bht -> quadrant));
		bht -> q_SE = bhtreeCreate(quadSE(bht -> quadrant));

		// Sauvegarde du parent
		bht -> q_NW -> parent = bht;
		bht -> q_NE -> parent = bht;
		bht -> q_SW -> parent = bht;
		bht -> q_SE -> parent = bht;

		// Insertion de b et b'
		bhtreeInsertAux(bht, bht -> particule);
		bhtreeInsert(bht, b);	
	}
}	

//------------

void bhtreeUpdateForce(BHTree *bht, Body *b) 
{
	// Hypothèse
	if(b == NULL)
	{
		terminate("ERREUR (bhtreeUpdateForce) - Un pointeur nul a été passé en argument");
	}

	// REMARQUE : Si b appartient au noeud interne, on ne fait pas de calcul et on descend dans l'arbre
	int bIsContained = quadContains(bht -> quadrant, bodyGetrx(b), bodyGetry(b));

	// CAS 1 : Le noeud est externe, non-vide et contient une particule != b
  	if(bht -> q_NW == NULL && bht -> particule != NULL && !bIsContained)
  	{
  		bodyAddForce(b, bht -> particule);
  		return;
  	}

  	// CAS 2 : Le rapport s/d est calculé
  	if(!bIsContained && bht -> particule != NULL)
  	{
  		double s = quadLength(bht -> quadrant);
  		double d = bodyDistanceTo(bht -> particule, b);

  		if(s/d < theta)
  		{
  			bodyAddForce(b, bht -> particule);
  			return;
  		}
  	}
  	
  	// Si CM n'a pas été, on descend dans l'arbre
 	if(bht -> q_NW != NULL)
 	{
 		bhtreeUpdateForce(bht -> q_NW, b);
  		bhtreeUpdateForce(bht -> q_NE, b);
  		bhtreeUpdateForce(bht -> q_SW, b);
  		bhtreeUpdateForce(bht -> q_SE, b);
 	}
}

//------------

void bhtreePrint(BHTree *bht, FILE *fp) 
{
	// CAS 1 : Affichage de la racine
	if(bht -> parent == NULL)
	{
		fprintf(fp, "Root:");
		quadPrint(bht -> quadrant, fp);
		bodyPrint(bht -> particule, fp);
		fprintf(fp, "\n");
	}

	// CAS 2 : Affichage d'un noeud
	if(bht -> q_NW != NULL)
	{
		bhtreePrintAux(bht -> q_NW, "NW", fp);
		bhtreePrint(bht -> q_NW, fp);
		bhtreePrintAux(bht -> q_NE, "NE", fp);
		bhtreePrint(bht -> q_NE, fp);
		bhtreePrintAux(bht -> q_SW, "SW", fp);
		bhtreePrint(bht -> q_SW, fp);
		bhtreePrintAux(bht -> q_SE, "SE", fp);
		bhtreePrint(bht -> q_SE, fp);
	}

	return;
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

static void bhtreeInsertAux(BHTree *bht, Body *b) 
{
	double x = bodyGetrx(b);
	double y = bodyGetry(b);

	if(quadContains(bht -> q_NW -> quadrant, x, y))
	{
		bhtreeInsert(bht -> q_NW, b);
	}
	else if(quadContains(bht -> q_NE -> quadrant, x, y))
	{
		bhtreeInsert(bht -> q_NE, b);
	}
	else if(quadContains(bht -> q_SW -> quadrant, x, y))
	{
		bhtreeInsert(bht -> q_SW, b);
	}
	else if(quadContains(bht -> q_SE -> quadrant, x, y))
	{
		bhtreeInsert(bht -> q_SE, b);
	}
}

// Insère une paticule dans son quadrant respectif
//------------

static void bhtreePrintAux(BHTree *tree, char *q_XX, FILE *fp)
{
	fprintf(fp,"   ");

	for (int i = 1 ; i < distanceToRoot(tree) ; i++)
	{
		fprintf(fp,"|   ");
	}

	fprintf(fp,"|-->%s:", q_XX);

	quadPrint(tree -> quadrant, fp);

	if(tree -> particule != NULL)
	{
		bodyPrint(tree -> particule, fp);
	}

	fprintf(fp, "\n");
}

// Affiche les informations d'un noeud
//------------

static int distanceToRoot(BHTree *tree)
{
	if(tree -> parent == NULL)
	{
		return 0;
	}

	return 1 + distanceToRoot(tree -> parent);
}

// Calcule la distance d'un noeud dans l'arbre (Utilisé par bhtreePrintAux)
//------------
















