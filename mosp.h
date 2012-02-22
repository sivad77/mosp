
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
** mosp.h: instance data structures */
#ifndef MOSP_H
#define MOSP_H

#include "matrix/matrix.h"
#include "list/list.h"
#include "set/set.h"
#include "set/bitSet.h"
#include "flags.h"
#include "stats/stats.h"
#include "hash/hash.h"
#define PRODUCTS 100

/* contains everything an algorithm needs to complete */
typedef struct {
	int u;					/* contains the [u]pper bound */
	int l;					/* contains the [l]ower bound */
	int pn;					/* [p]roduct [n]umber */
	int cn;					/* [c]ustomer [n]umber */
	int *maxIterations;
	int *provenOptimal;
	int *t;
	
	List *p;				/* contains the [p]roduction */
	Set *up;	/* contains all elements in production */
	Set *uc;
	Set **cust;			/* points to m->cust */
	Stats *stats;
	
	int flags;			/* launch time flags */
} Segment;

/* contains the program state */
typedef struct {
	char *n;				/* instance name */
	int cn;					/* customer no. */
	int pn;					/* product no. */
	int lb;					/* lower bound */
	int ub;					/* upper bound */
	int maxIterations;
	int *provenOptimal;
	int t;
	
	List *sp;				/* subsumed product list */
	Set **cust;			/* customer array N.B c(p) for fast look up */

	Segment *s;			/* contains all all MOSP's partitions and info */
	int sn;					/* contains the number of partitions */

	int flags;

	Stats stats;
	
	int **m;				/* matrix */
} Mosp;

/* used in stacks algorithm by the hash table */
typedef struct _StackItem {
	Set *s;					/* set: used as key*/
	int v;					/* set value */
	int yn;					/* success fail */
} StackItem;

/* each algorithm returns this */
typedef struct _Sched {
	List *l;				/* schedule which yielded above */
	int	v;					/* min open stacks */
} Sched;

/* used by indexMin heuristic */
typedef struct _IndexMin {
	int index;
	int value;
} IndexMin;

/* MOSP algorithms */
int			algExhRec					(Set *p, List *s, int u, Segment *m);
Sched		algExhIt					(Segment *m);
int			algBb							(Set	*p, List *s, int l, int u, Segment *m);
int			algStacks					(Set	*p, HashTbl *stacks, HashTbl *no,int l, int u,
													 Segment *m);
/* MOSP algorithm wrappers */
Sched		exhRecWrapper			(Segment seg);
Sched		bbWrapper					(Segment seg);
Sched		exhItWrapper			(Segment seg);
Sched		stacksWrapper			(Segment seg);
Sched		binaryChopWrapper	(Segment seg);
Sched		stepwiseWrapper		(Segment seg);
Sched		backwardsWrapper	(Segment seg);



/* pre-precessing */
void		subsumption		(Mosp *m);
void		partition			(Mosp *m);
Segment createSegment	(int, List *products, Mosp *m);
void		defaultMOSP		(Mosp *m);

/* post-processing */
void unSubsume(List *, List *);
List *stacksSchedule(HashTbl *stack, Set *s, int min, Segment *m);

/* utils */
int				active				(int c, Set *a, Segment *m);
void			indexMin			(Set *a, Set *t, Segment *m, IndexMin *min);
Set				unionList			(List *p, Segment *m);
int			  opening				(Set *s, Segment *m);
int				max						(int a, int b);
Set				unionSet			(Set *p, Segment *m) ;
int				maxOpen				(List *s, Segment *m);
#endif
