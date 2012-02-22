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

#include "hash/hash.h"
#include "mosp.h"
#define HASHSIZE 5000000


/* quick hack to fix memory problems */
List *result;

/*******************************************************************************
** Exhaustive Iterative
*******************************************************************************/
Sched exhItWrapper(Segment seg) {
	return algExhIt(&seg);
}

/* 
** this algorithm was adapting off code from the net
** generating permutations lexicographic order
** Algorithm due to Dijkstra.
** C Implementation by Glenn C. Rhoads
*/
Sched algExhIt(Segment *m) {
	int i,j,r,s,temp,n,*pi;
  List *sched;
	Sched tempMin,min;
	
	sched = listInit(listSize(m->p));
	min.v = m->u;
	n = listSize(m->p);
	pi = malloc((n+1)* sizeof(int));
	
	pi[0] = 0;
	for (i=0; i < n; i++) pi[i+1] = listPos(m->p,i);
	i = 1;
	while (i) {
		/* copy the schedule into a list */
		for (i=1; i <= n; i++) listLast(sched,pi[i]);
	
		(m->stats->steps)++;
		if (*(m->maxIterations) && m->stats->steps > *(m->maxIterations))
			break;
		
		/* test the schedule */
		tempMin.v = maxOpen(sched,m);
		if (tempMin.v < min.v) {
			min.v = tempMin.v;
			min.l = listInit(m->pn);
			for (i=1; i <= n; i++)
				listLast(min.l, pi[i]);
		}
		
		/* shuffle the numbers */
		i = n-1;
		while (pi[i] > pi[i+1]) i--;
		j = n;
		while (pi[i] > pi[j]) j--;
		temp = pi[i];
		pi[i] = pi[j];
		pi[j] = temp;
		r = n;
		s = i+1;
		while (r > s) {
			temp = pi[r];
			pi[r] = pi[s];
			pi[s] = temp;
			r--; s++;
		}
	}
	
	return min;
}

/*******************************************************************************
** Exhaustive Recursive
*******************************************************************************/
Sched exhRecWrapper(Segment seg) {
	List *sched = listInit(seg.pn);
	result = listCopy(seg.p);
	Sched temp;
	temp.v = algExhRec(setCopy(seg.up),sched,seg.u,&seg);
	temp.l = result;
	return temp;
}


/*
** returns the minimun number of stacks and an optimal schedule
** -pre-conditions: set contains all p in P in the segment, and sched is
** initialised.
** -behaviour: a search tree is generated where at the leaf the
** maxOpen function is called on a schedule, and the result returned and
** cmpared to every other schedule with max open, with the minium returned
** -post-conditions: result contains schedule and the minimun number of stacks
** returned -performance: O(|set|!)
*/
int algExhRec(Set *set, List *sched, int u, Segment *m) {
	int p;
	Set tempSet;
	Set setCopy;
	int min;
	int temp;
	
	/* init values */
	min = u;
	setZero(&tempSet);
	setAssign(&tempSet,set);		

	if (setEmpty(&tempSet)) {
		if ((temp = maxOpen(listCopy(sched),m)) <  maxOpen(listCopy(result),m)) {
			listKill(result);
			result = listCopy(sched);
		}
		listKill(sched);
		return temp;
	}
	
	while((p = setMin(&tempSet)) >= 0) {
		if (*(m->maxIterations) && m->stats->steps > *(m->maxIterations)) {
			*(m->provenOptimal) = 0;
			return min;
		}
		
		setDelete(&tempSet,p);
		(m->stats->steps)++;
		setAssign(&setCopy,set);
		temp = algExhRec(setDelete(&setCopy,p),
											 listLast(listCopy(sched),p),u,m);
		if (temp < min) 
			min = temp;		
	}
	listKill(sched);	
	return min;
}

/*******************************************************************************
** Branch and Bound
*******************************************************************************/
Sched bbWrapper(Segment seg) {
	result = listCopy(seg.p);
	Sched temp;
	List *sched = listInit(seg.pn);
	
	temp.v = algBb(setCopy(seg.up),sched,seg.l,seg.u,&seg);
	temp.l = result;
	
	return temp;
}

/*
** returns the minimun number of stacks and an optimal schedule
** -pre-conditions: set contains all p in P in the segment, and sched is
** initialised
** -behaviour: a search tree is generated where at each branch the decision is
** made if it should be explored for the optimal solution. At the leaf the
** maxOpen function is called on a schedule, and the result returned and
** cmpared only to other leaves which were not pruned. With the minium
** returned, and compared as it goes back up the tree.
** -post-conditions: result contains schedule and the minimun number of stacks
** returned -performance: O(|set|!)
*/
int algBb(Set *set, List *sched, int l, int u, Segment *m) {
	int p;				/* current product */
	int v;				/* minimun going up the tree */
	int act;			/* numnber of active products for p */
	int curMinIt; /* indexMin structure interator */
	int min;			/* current min */
	int temp;			/* used to save horizontal space */
	IndexMin indexMinArray[PRODUCTS];
	Set tempSet;
	Set setCopy;
	
	/* base case */
	if (setEmpty(set)) {
		listKill(result);
		result = sched;
		return 0;
	}
	
	if ((m->flags & OPENING) && (p = opening(set,m)) >0) {
		setAssign(&setCopy,set);
		return algBb(setDelete(&setCopy,p),listLast(sched,p),l,u,m);
		listKill(sched);
	}
	else {	/* we need to explore branch */
		/* initialising variables */
		min = u;
		curMinIt=0;
		setAssign(&tempSet,set);
		indexMin(set,&tempSet,m,indexMinArray);
		while(min > l && !setEmpty(&tempSet)) {
			/* look over whole universe using only p element of t */
			if (*(m->maxIterations) && m->stats->steps > *(m->maxIterations)) {
				*(m->provenOptimal) = 0;
				return min;
			}
			/* selected next best product */
			p = indexMinArray[curMinIt].index;
			setDelete(&tempSet,p);
			act = indexMinArray[curMinIt++].value;
			if (act >= min) break;
			(m->stats->steps)++;
			/* fixme */
			setAssign(&setCopy,set);
			temp = algBb(setDelete(&setCopy,p),listLast(listCopy(sched),p),l,min,m);
			v = max(act,temp);
			if (v < min) {
				min = v;
			}
		}
	}
	listKill(sched);
	return min;
}

/*******************************************************************************
** Stacks
*******************************************************************************/
/* used in the hash table to generate unique hash values (or tries to) */
int hashStackFunc(const void *set, int size) {
	StackItem *ss = (StackItem*)set;
	srand(setMagnitude((Set*)ss->s));
	rand();
	return rand() % size;
}

/* used in the hash table to compare sets */
int lessStackFunc(const void *a, const void *b) {
	StackItem *sa = (StackItem*)a;
	StackItem *sb = (StackItem*)b;
	return setEqual((Set*)sa->s,(Set*)sb->s);
}

/* basic initilisations */
Sched stacksWrapper(Segment seg) {
	Sched sched; HashTbl *stack,*no; int min;
	stack = hashTblInit(HASHSIZE,hashStackFunc,lessStackFunc);
	no = NULL;
	min = algStacks(setCopy(seg.up),stack,no,seg.l,seg.u,&seg);
	sched.l = stacksSchedule(stack,seg.up,min,&seg);
	sched.v = min;

	/* give default solution if no schedule found */
	if (!listSize(sched.l))
		sched.l = seg.p;
	
	hashTblKill(stack);
	return sched;
}

/*
** returns the minimun number of stacks
** -pre-conditions: set contains all p in P in the segment, and the hash is
** initialised, and l and u are valid
** -behaviour: a search tree is generated where at each branch the decision is
** made if it should be explored for the optimal solution. At the leaf the
** maxOpen function is called on a schedule, and the result returned and
** cmpared only to other leaves which were not pruned. With the minium
** returned, and compared as it goes back up the tree.
** Also partial optimal solutions are stored in hash, which are retrieded later
** for speedup.
** -post-conditions: returns minimun number of stacks, and the hash table has
** enough information to generate schedule
** returned -performance: O(2^|set|)
*/
int algStacks(Set *set, HashTbl *stack, HashTbl *no,int l, int u, Segment *m) {
	int p;
	int v;
	int act;
	int curMinIt;
	int min;
	
	IndexMin indexMinArray[PRODUCTS];
	StackItem stackItem, *stackItemPtr;
	Set tempSet;
	
	if (setEmpty(set)) 
		return 0;
	
	/* have a look if we have already computed set */	
	stackItem.s = set;
	if ((stackItemPtr=(StackItem*)hashTblFind(stack,&stackItem))) {
		(m->stats->reusedSets)++;
		return ((StackItem*)stackItemPtr)->v;
	}
	
	if ((m->flags & OPENING) && (p = opening(set,m)) >0) {
		(m->stats->opening)++;
		min = algStacks(setDelete(setCopy(set),p),stack,no,l,u,m);
	}
	else {
		/* itialise variables */
		min = u+1;
		setZero(&tempSet); setAssign(&tempSet,set);
		curMinIt = 0;
		indexMin(set,&tempSet,m,indexMinArray);
		while(min > l && !setEmpty(&tempSet)) {
			if (*(m->maxIterations) && m->stats->steps >= *(m->maxIterations)) {
				*(m->provenOptimal) = 0;
				return min;
			}
			/* look over whole universe using only p element of t */
			p = indexMinArray[curMinIt].index;
			setDelete(&tempSet,p);
		
			act = indexMinArray[curMinIt++].value;
			//		act = active(p,setDelete(setCopy(set),p),m);
			if (act >= min) break;
			(m->stats->steps)++;
			
			v = max(act,algStacks(setDelete(setCopy(set),p),stack,no,l,u,m));
			if (v < min) min = v;	
		}
	}
	
	/* insert item */
	stackItemPtr = malloc(sizeof(StackItem));
	stackItemPtr->s = set;
	stackItemPtr->v = min;
	hashTblInsert(stack,stackItemPtr);
	
	if (m->flags & NO && min > u) {
		(m->stats->no)++;
		stackItemPtr = malloc(sizeof(StackItem));
		stackItemPtr->s = set;
		hashTblInsert(no,stackItemPtr);
	}
	return min;
}

/*******************************************************************************
** Stepwise
*******************************************************************************/

/* this is for the failed set - it mimicks a standard linked list. hence,
** each item slotted into the start of bucket[0] in O(1) time*/
int failHashFunc(const void *set, int size) {
	return 0;
}

/* used with hashTblEach to delelte all items in hash table called with
** hashTblEach, from stack. */
void deleteSets(void *cur, void *stack) {
	Set *set = ((StackItem*)cur)->s;
	StackItem z;
	z.s = set;
	hashTblDelete(stack,&z);
	/* ensure we deleted it */
	assert(!hashTblFind(stack,&z)); 
}

/*
** returns the minimun number of stacks
** -pre-conditions: set contains all p in P in the segment, and hash is
** initialised
** -behaviour: using a star, each successive value from l to u is used until
** successfu.
** -post-conditions: returns the minimun number of stacks and optimal solutions
** returned -performance: O(2^|set|!)
*/
Sched stepwiseWrapper(Segment seg) {
	Sched sched; HashTbl *stack,*no;
	int min=PRODUCTS;
	int try;
	int *proven = seg.provenOptimal;
	
	stack = hashTblInit(HASHSIZE,hashStackFunc,lessStackFunc);	
		
	for (try=seg.l;try<=seg.u;try++) {
		no = hashTblInit(1,failHashFunc,lessStackFunc);
		if (seg.flags & STEPS) printf("try %d ",try);
		min = algStacks(setCopy(seg.up),stack,no,try,try,&seg);
		if (*(seg.maxIterations) && seg.stats->steps >= *(seg.maxIterations)) {
			*(proven) =0;
			sched.v = try;
			sched.l = seg.p;
			return sched;
		}
		if (seg.flags & STEPS) printf("found %d \n",min);
		if (min == try) break;
		hashTblEach(no,&deleteSets,(void*)stack);
	}
		
	sched.l = stacksSchedule(stack,seg.up,min,&seg);
	sched.v = min;
	hashTblKill(stack);
	return sched;
}

/*******************************************************************************
** Backwards
*******************************************************************************/
/*
** returns the minimun number of stacks
** -pre-conditions: set contains all p in P in the segment, and hash is
** initialised
** -behaviour: using a star, each successive value from u to l is used until
** successful.
** -post-conditions: returns the minimun number of stacks and optimal solutions
** returned -performance: O(2^|set|!)
*/
Sched backwardsWrapper(Segment seg) {
	Sched sched;
	HashTbl *stack;
	HashTbl *no;
	int min;
	int gmin;
	int try;
	int *proven = seg.provenOptimal;
	
	min = PRODUCTS;
	no = NULL;
	gmin = seg.u+1;
	try = seg.u;

	for(try = seg.u; try >= seg.l;try--) {
		stack = hashTblInit(HASHSIZE,hashStackFunc,lessStackFunc);
		
		if (seg.flags & STEPS) printf("try %d \n",try);
		min = algStacks(setCopy(seg.up),stack,no,try,try,&seg);
		if (seg.flags & STEPS) printf("found %d \n",min);
		
		if (*(seg.maxIterations) && seg.stats->steps >= *(seg.maxIterations)) {
			*(proven) =0;
			sched.v = try;
			sched.l = seg.p;
			return sched;
		}
		if (min > try || min == seg.l) break;
		try = min;
	}
	
	stack = hashTblInit(HASHSIZE,hashStackFunc,lessStackFunc);
	min = algStacks(setCopy(seg.up),stack,no,min,min,&seg);
	
	sched.l = stacksSchedule(stack,seg.up,min,&seg);
	sched.v = min;
	hashTblKill(stack);
	return sched;
}

/*******************************************************************************
** BinaryChop
*******************************************************************************/
/*
** returns the minimun number of stacks
** -pre-conditions: set contains all p in P in the segment, and hash is
** initialised
** -behaviour: using a star, employing a binary search technique all values are
** tested with [l,u]
** -post-conditions: returns the minimun number of stacks and optimal solutions
** returned -performance: O(2^|set|!)
*/
Sched binaryChopWrapper(Segment seg) {
	Sched sched; HashTbl *stack,*no=NULL;
	int min=PRODUCTS, gmin,try;
	stack = hashTblInit(HASHSIZE,hashStackFunc,lessStackFunc);
	int U = seg.u;
	int L = seg.l;
	int *proven = seg.provenOptimal;
	
	gmin = U+1;
	while(L <= U) {
		no = hashTblInit(1,hashStackFunc,lessStackFunc);
		try = (L + U)/2;
		
		if (seg.flags & STEPS) printf("try %d",try);
		min = algStacks(setCopy(seg.up),stack,no,try,try,&seg);
		if (seg.flags & STEPS) printf("min %d ",min);
		
		if (*(seg.maxIterations) && seg.stats->steps >= *(seg.maxIterations)) {
			sched.v = try;
			sched.l = seg.p;		
			*(proven) =0;
			return sched;
		}
		if (min <= try) {
		if (seg.flags & STEPS) printf("< \n");
			gmin = min;
			U = min -1;
			stack = hashTblInit(HASHSIZE,hashStackFunc,lessStackFunc);
		} else {
			if (seg.flags & STEPS) printf("> \n");
			L = try + 1;
			hashTblEach(no,&deleteSets,(void*)stack);
		}
	}
	
	hashTblEach(no,&deleteSets,(void*)stack);
	min = algStacks(setCopy(seg.up),stack,no,min,min,&seg);
	
	sched.l = stacksSchedule(stack,seg.up,min,&seg);
	sched.v = min;
	return sched;
}

