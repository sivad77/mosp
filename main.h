
/*****************************************************************************
** MONASH UNIVERSITY, School of Computer Science and Software Engineering
** Student Declaration for CSE3301 Submission. I Joshua Davis, ID:19424094
** declare that this submission is my own work and has not been copied from any
** other source without attribution. I acknowledge that severe penalties exist
** for ant copying of code without attribution, including a mark of 0 for this
** assignment
******************************************************************************/

/* Solving the MOSP by Joshua Davis
** Monash Uni - CSE3301/2006/Sem2 - ID: 19424094
** mosp.h: command line datastructure */
#ifndef MAIN_H
#define MAIN_H

#include "mosp.h"
#include "matrix/matrix.h"
#include "list/list.h"
#include "set/set.h"
	
typedef struct {
	char *f;			/* filename */
	FILE *h;			/* file handle */
	
	int flags;
	int upbound;
	int lowbound;
	int maxIterations;
} Opts;

void verbose(Mosp *m);
void run(Mosp *m, Sched (algorithm)(Segment seg),char *name);

Opts	*options(int argc, char **argv, int *flags);
Mosp *populate(Opts *o);
#endif
