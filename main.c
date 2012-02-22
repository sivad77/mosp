/*****************************************************************************
** MONASH UNIVERSITY, School of Computer Science and Software Engineering
** Student Declaration for CSE3301 Submission. I Joshua Davis, ID:19424094
** declare that this submission is my own work and has not been copied from any
** other source without attribution. I acknowledge that severe penalties exist
** for ant copying of code without attribution, including a mark of 0 for this
** assignment
******************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include "main.h"
#include "mosp.h"
#define BUFFER 80
#define PRODUCTS 100
#define CUSTOMERS 100

int main(int argc, char **argv) {
	Mosp *m; Opts *o; /* data variables */
	int flags;
	
	o	= options(argc,argv,&flags);	/* contains command line [o]ptions */
	m = populate(o);								/* contains all [m]osp information */
	defaultMOSP(m);									/* no partitions or substition flags set */
	
	/* how do we pre-process */
	if (m->flags & SUBSUMPTION) subsumption(m);
	if (m->flags & PARTITION) partition(m);	

	/* output */
	printf("Instance: %s",m->n);
	if (m->flags & VERBOSE)	verbose(m);
	
	/* algorithms */
	if (m->flags & EXHREC) run(m,&exhRecWrapper,"Exhaustive recursive");
	if (m->flags & EXHIT) run(m,&exhItWrapper,"Exhaustive iterative");
	if (m->flags & BB) run(m,&bbWrapper,"Branch and bound");
	if (m->flags & STACKS) run(m,&stacksWrapper,"Astar");
	if (m->flags & STEPWISE) run(m,&stepwiseWrapper,"Stepwise");
	if (m->flags & BACKWARDS) run(m,&backwardsWrapper,"Backwards");
	if (m->flags & BINARYCHOP) run(m,&binaryChopWrapper,"Binary Chop");
	
	return 0;
}

void verbose(Mosp *m) {
		int i;
		puts("All output is applied to original matrix, indexed from 1");
		printf("Total customers: %d\n",m->cn);
		printf("Total products: %d\n",m->pn);
		matrixPrint(m->m,m->cn,m->pn);
		printf("Subsumed: ");
		for(i=0;i<listSize(m->sp);i=i+2) 
			printf("[%d -> %d] ",listPos(m->sp,i),listPos(m->sp,i+1));
		puts("");
		printf("Segments %2d \n",m->sn);
		for(i=0;i<m->sn;i++) {
			puts("------------------------");
			printf("Segment %2d ",i);
			printf("[ ");listPrint(m->s[i].p);	printf("]\n");
			printf("Customers: %d\n",m->s[i].cn);
			printf("Products: %d\n",m->s[i].pn);
			printf("Lower: %d\n",m->s[i].l);
			printf("Upper: %d\n",m->s[i].u);
			printf("Universal client: ");
			setPrint(m->s[i].uc);puts("");
			printf("Universal product: ");
			setPrint(m->s[i].up);puts("");
		}

}

void run(Mosp *m, Sched (algorithm)(Segment seg), char *name) {
	printf("Algorithm: %s\n",name);
	int segIt; /*S egment iterator */
	Sched segSched, sched; /* segment schedule, overall schedule */
	
	/* initialise final schedule list */
	sched.l = listInit(PRODUCTS+1);
	/* zero all stats */
	statsZero(&(m->stats));
	sched.v = 0;
	
	/* loop over each partition, building up schedule and retaining maximun
	** minimun */
	for (segIt=0;segIt < m->sn;segIt++) {
		segSched = algorithm(m->s[segIt]);
		if (!*(m->provenOptimal))
			break;
		if (segSched.v > sched.v) sched.v = segSched.v;		/* update min */
		listMerge(sched.l,segSched.l);										/* concat sched */
	}
	/* slot in subsumed products */
	
	if (*(m->provenOptimal))
		unSubsume(sched.l,m->sp);	

	/* print results from algorithm */
	printf("MOS: %d\n",sched.v);
	printf("Schedule: ");	listPrint(sched.l);printf("\n");
	if (m->flags & STATS) {
		printf("Customers: %d\n",m->cn);
		printf("Prodcuts: %d\n",m->pn);
		printf("Proved optimal: %d\n",*(m->provenOptimal));
		printf("Segments: ");
		for (segIt=0;segIt < m->sn;segIt++)
			printf("%d ",listSize(m->s[segIt].p));
		printf("\n");
		printf("Subsumptions: %d\n",listSize(m->sp)/2);
		printf("Steps: %d\n",m->stats.steps);
		printf("Reused sets: %d\n",m->stats.reusedSets);
		printf("Opening: %d\n",m->stats.opening);
		printf("Fail: %d\n",m->stats.no);
		printf("size %d\n",listSize(sched.l));
	}
	/* ensure schedule matched min */
	assert(!*(m->provenOptimal) ||
			(sched.v == maxOpen(listCopy(sched.l),&m->s[0])));
	/* ensure schedule is of correct length */
	assert(!*(m->provenOptimal) || (listSize(sched.l) == m->pn));
}

/* read in the file, set up useful structures etc*/
Mosp *populate(Opts *o) {
	Mosp *m = (Mosp*)malloc(sizeof(Mosp));
	int i,j;
	char b[BUFFER];
	FILE *h;

	/* ensure all data within *m is set to NULL*/
	memset(m, (int) NULL, sizeof(Mosp));
	
	if (!(h = fopen(o->f,"r"))) {
		perror ("populating matrix");
		exit(1);
	}
	
	/* get instance information from a file*/
	fgets(b,BUFFER,h);
	m->n = strdup(b);					/* read in name of instance */
	fgets(b,BUFFER,h);
	if (sscanf(b,"%d %d",&m->cn,&m->pn) != 2) {
		perror("file of incorrect form");
		exit(1);
	}

	/* allocations */
	m->m = m_alloc(m->cn+1,m->pn+1);									/* matrix */
	m->cust = (Set**)malloc(sizeof(Set*)*(m->pn+1));	/* cust */
	m->s = malloc(sizeof(Segment)*m->pn+1);						/* segment array*/
	
	/* give customer array memory */
	for (i=0;i<m->pn+1;i++) {		
		m->cust[i] = setAlloc();
	}
	
	/* default values */
	m->s[0].p = listInit(m->pn+1);					/* default products */
	m->flags = o->flags;
	m->ub = o->upbound;
	m->lb = o->lowbound;
	m->provenOptimal =  malloc(sizeof(int));
	*(m->provenOptimal) = 1; /* default unless we hit maxIterations then 0 */
	m->maxIterations = o->maxIterations;
	
	m->sn=1;																/* default number of segments */
	
	
	/* scan stuff into matrix 
	** FIXME: should i test how many products per line??? */	
	for (i=1;i<=m->cn;i++)
		for(j=1;j<=m->pn;j++) { 
			if (fscanf(h,"%i",&m->m[i][j]) != 1) {
				perror("file of incorrect form");
				exit(1);
			}
			if (!(m->m[i][j] == 0 || m->m[i][j] == 1)) {
				perror("Invalid input file, neither 1 or 0 scanned");
				exit(0);
			}
			/* for fast access to customer sets */
			if (m->m[i][j]) setInsert(m->cust[j],i);
		}
	
	/* done with file */
	fclose(h);

	return m;
}

/* return a structure containing all arguments to the program */
Opts *options(int argc, char **argv, int *flags) {
	int opt;
	Opts *opts = (Opts*)malloc(sizeof(Opts));
	
	/* set all flags to zero */
	memset(opts, (int) NULL, sizeof(Opts));
	
	/* display usage */
	if (argc == 1) {
		fprintf(stderr,"Useage: mosp -v -d <file>\n");
		fprintf(stderr,"  -v verbose\n");
		fprintf(stderr,"  -p find independent sub-problems\n");
		fprintf(stderr,"  -d find redundant products, and consume them\n");
		fprintf(stderr,"  -e solve using recursize exhaustive algorithm\n");
		fprintf(stderr,"  -i <integer> stop after a number of iterations\n");
		fprintf(stderr,"  -l <integer> use this number as the lower bound\n");
		fprintf(stderr,"  -u <integer> use this number as the upper bound\n");
		fprintf(stderr,"  -y solve using binarychop algorithm\n");
		fprintf(stderr,"  -w solve using the backwards algorithm\n");
		fprintf(stderr,"  -t show stats\n");
		fprintf(stderr,"  -s solve using the stepwise algorithm\n");
		fprintf(stderr,"  -f solve using initerative exhaustive algorithm\n");
		fprintf(stderr,"  -b solving using branch and bound\n");
		fprintf(stderr,"  -c use opening products first\n");
		exit(1);
	}
	opts->flags=0;
	opts->lowbound = 0;
	opts->upbound = 0;
	opts->maxIterations = 0;
	
	/* extract flages */
	while ((opt = getopt(argc, argv, "adtzbecsfbpvwyu:l:i:")) != -1) 
		switch(opt) {
			case 'p': opts->flags |= PARTITION; break;
			case 'e': opts->flags |= EXHREC; break;
			case 'd': opts->flags |= SUBSUMPTION; break;
			case 'f': opts->flags |= EXHIT; break;
			case 'c': opts->flags |= OPENING; break;
			case 'a': opts->flags |= STACKS; break;
			case 'b': opts->flags |= BB; break;
			case 's': opts->flags |= STEPWISE; break;
			case 'w': opts->flags |= BACKWARDS; break;
			case 'y': opts->flags |= BINARYCHOP; break;
			case 't': opts->flags |= STATS; break;
			case 'v': opts->flags |= VERBOSE; break;
			case 'z': opts->flags |= STEPS; break;
			case 'u': opts->upbound = atoi(optarg); break;
			case 'l': opts->lowbound = atoi(optarg); break;
			case 'i': opts->maxIterations = atoi(optarg); break;
		}
	/* put that last arg into a file - let populate() see if its valid file*/
	opts->f = strdup(argv[argc-1]);
	
	return opts;
}

