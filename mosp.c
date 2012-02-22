
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
** mosp.c: solving algorithms */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include "mosp.h"

int max(int a, int b) {
	return (a > b) ? a : b;
}

/*
** returns number of active customers given what products still need to be
**	scheduled set after, and the product were interested in p is scheduled
**	next 
** -pre-conditions: p is an element of P, and all arguments are defined
** -behavior: using the universal set, determine what has been scheduled and
**		take the result of that and after and union each. This determines what
**		products are active if p is scheduled next, also all in the clients which
**		are only relevent to p.
** -post-conditions: active contains the number of active clients
** -performance: O(N)
*/
int active(int p, Set *after, Segment *m) {
	int active;
	Set afterUnion,beforeUnion,*before;
	
	before = setDifference(m->up,after);									/* ALLO before*/
	setDelete(before,p);
	
	afterUnion = unionSet(after,m);
	beforeUnion = unionSet(before,m);
	
	setIntersectionSelf(&afterUnion,&beforeUnion);				
	active = setCardinal(setUnionSelf(&afterUnion,m->cust[p]));

	setKill(before);
	/* FREE before */
	return active;
}

/*
** returns the the number of open stacks for a given schedule a
** -pre-conditions: a is initialised and contains a shedual, m contains the the
**		relvent bits and peices like number of products etc
** -behaviour: using the sliding window technique iterates over the schedule
**		taking the intersection of the (union of before and after) then with that
**		result to take the intersection with current. This leaves a set
**		containing the active customers at cur.
** -post-conditions: m is not changed, and a is left empty FIXME: free a??
** -performance: O(N)
*/
int maxOpen(List *a, Segment *m) {
	Set before, after;
	int cur,open,max;

	max = 0;
	setZero(&before);
	while(listSize(a)) {
		/* get current from schedule */
		cur = listPos(a,0);
		listDelete(a,cur); /* FIXME: ripple not good */
		
		/* prepare before */
		after = unionList(a,m);
		
		/* count number of open stacks */
		setIntersectionSelf(&after,&before);/*after = result of inter */							
		open = setCardinal(setUnionSelf(&after,m->cust[cur]));
		if (open>max) max = open;
		
		/* prepare 'before' for next iteration */
		setUnionSelf(&before,m->cust[cur]);	
	}
	listKill(a);	
	return max;
}	


/*
** returns a constructed schedule from a hash table
** -pre-conditions: stack contains all information required to generate a
**		schedule, s contains all elements to which that schedule can be made up
**		form, and min is the correct MOS
** -behavior: pretend its traversing a tree, and at each branch test if p could
**		be true
** -post-conditions: a schedule is obtained
** -preformance: O(n^2)
*/
List *stacksSchedule(HashTbl *stack, Set *s, int min, Segment *m) {
	int p;
	Set temp;
	List *sched;
	StackItem item,*ret;
	
	setAssign(&temp,s);
	sched = listInit(m->pn);
	
	for (p=0;(p = setNext(&temp,p)) > 0;) {
		setDelete(&temp,p);
		item.s = &temp;
		ret = ((StackItem*)hashTblFind(stack,&item)); 
		if ((ret && ret->v <= min && active(p,&temp,m) <= min) || setEmpty(&temp)) {
			listLast(sched,p);
			p=0;
		}
		else {
			setInsert(&temp,p);
		}
	}
	return sched;
}

/*
** returns a set, which is the union of using each element of set p as the
**	index for cust[]
** -pre-conditions: m and p is initialised
** -behaviour: loop over each element in p, taking the union of a temp variable
**		and the relevent position in the customer array
** -post-conditions: a contains the union of p, given p is the index for the
**		customer array
** -performance: O(n)
*/
Set unionSet(Set *p, Segment *m) {
	int i;

	Set a; setZero(&a);
	
	for(i=0;(i=setNext(p,i)) > 0;) {
		setUnionSelf(&a,m->cust[i]);
	}
	return a;
}

/*
** returns a set, which is the union of using each element in list p as the
**	index for cust[]
** -pre-conditions: m and p is initialised
** -behaviour: loop over each element in p, taking the union of a temp variable
**		and the relevent position in the customer array
** -post-conditions: a contains the union of p, given p is the index for the
**		customer array
** -performance: O(n)
*/
Set unionList(List *p, Segment *m) {
	int i;
	Set a;
	setZero(&a);
	
	for(i=0;i<listSize(p);++i)
		setUnionSelf(&a,m->cust[listPos(p,i)]);
	
	return a;
}

/* comparison function for qsort in indexMin() */
int isLessIndexMin(const void *a, const void *b) {
	const IndexMin *aa = (const IndexMin*)a;
	const IndexMin *bb = (const IndexMin*)b;

	/* stable */
	return (aa->value > bb->value) - (aa->value < bb->value);
	/* alternatives */
	//return (aa->value) - (bb->value);
	//return (aa->value>bb->value) ? 1 : 0;
}


/*
** a basic heuristic for picking the next p in the while loop of bb and stacks.
**	it works by sorting looking over what still needs to be scheduled, and
**	placing each p in ascending order based on active clints. returns a
**	indexMin data-structure.
** -pre-conditions:
** -behaviour:
** -post-conditions:
** -performance: O(n)
*/
void indexMin(Set *a, Set *t, Segment *m, IndexMin *min) {
	int c,i, maxClients, curActive; /* ret = return value */
	Set after;
	setAssign(&after,a);
	maxClients = PRODUCTS;
	//setPrint(t);puts("");
	
	/* FIXME: appears this could all be done alot better */
	for(c=0,i=0;(c = setNext(t,c)) > 0;i++) { /* where c is element of t */
	//	printf("%d\n",c);
		setDelete(&after,c);
		curActive = active(c,&after,m);	
		setInsert(&after,c);
		min[i].index  = c;
		min[i].value = curActive;
	}
	
	qsort(min,i,sizeof(IndexMin),&isLessIndexMin);
	
	return;
}	

/* determine if the non-opening algorithm is application and return p in which
** is or -1 if not */
int opening(Set *s, Segment *m) {
	Set *open,beforeUnion,afterUnion;
	int c;
	
	/* o(S) = c(P-S) intersect c(S) */
	open = setDifference(m->up,s);
	beforeUnion = unionSet(s,m);
	afterUnion = unionSet(open,m);
		
	setIntersectionSelf(&beforeUnion,&afterUnion);
	
	for(c=0;(c = setNext(s,c)) > 0;)  /* where c is element of s */
		if (setSubset(m->cust[c],&beforeUnion))
			return c;
	
	setKill(open);
	
	return -1;
}

/* to put products in order into segments */
int isLessInt(int a, int b) {
	return (a<b) ? 1 : 0;
}

/* looks for the max |c(p)| where p is in list l */
int lowerBound(List *l, Set **cust) {
	int i, max, clients;
	for (max=i=0;i<listSize(l);i++) {
		clients = setCardinal(cust[listPos(l,i)]);
		if (clients > max)
			max = clients;
	}
	return max;	
}

/* returns a universal customers set */
Set *uc(int s, List *l) {
	int i;
	Set *a = setAlloc();
	
	for (i=1;i<=s;i++) {
		setInsert(a,i);
	}
	
	return a;	
}

/* returns a universal product set, i.e used to generate sets for algorithms */
Set *up(int s, List *l) {
	int i;
	Set *a = setAlloc();
	
	for (i=0;i<s;i++) {
		setInsert(a,listPos(l,i));
	}
	
	return a;	
}

/******************************************************************************
** MOSP data manipulation
******************************************************************************/

/* slots all informaion required to run an algorithm on a segment */
Segment createSegment(int custNum, List *products, Mosp *m) {
	Segment s;															/* Sub-problem */
		
	s.cn = custNum;													/* FIXME: this should change */
	s.pn = listSize(products);							/* number of prods in seg */
	s.p = products;													/* the list of products */
	
	s.u = (m->ub) ? m->ub : custNum;											
	s.l = (m->lb) ? m->lb : lowerBound(products, m->cust);
	s.provenOptimal = m->provenOptimal;
	s.maxIterations = &(m->maxIterations);
	
	
	s.uc = uc(s.cn,products);								/* all products in set */
	s.up = up(s.pn,products);
	s.cust = m->cust;													/* fast look up array */
	s.flags = m->flags;
	s.stats = &(m->stats);
	return s;
}

/* default information for algorithms is -d or -p not seleteced */
void defaultMOSP(Mosp *m) {
	int i;
	
	m->sp = listInit(1);			/* default is no subsumption */					
	
	for (i=1;i<m->pn+1;i++)		/* default is all products are looked at */
		listLast(m->s[0].p,i);
	
	/* add all this to segment 0 */
	m->s[0] = createSegment(m->cn,m->s[0].p,m);
}

/******************************************************************************
** Post-processing
******************************************************************************/
/* put subsumed products back into the schedule */
void unSubsume(List *sched, List *subsumed) {
	List *temp = listCopy(subsumed);
	int eater,food,prod,prodI;
	int eaten;
	
//	listPrint(sched);puts("sched");
//	listPrint(subsumed);puts("saached");
	while(listSize(temp)) {
		eater = listPos(temp,0);
		listDelete(temp,eater);
		food = listPos(temp,0);
		listDelete(temp,food);
		eaten = 1;
		for (prodI=0;prodI < listSize(sched);prodI++) {
			prod = listPos(sched,prodI);
			if (prod==eater) {
				listInsertPos(sched,food,prodI);
				eaten = 0; 
				break;
			}
		}
		if (eaten) { /* we did not find the eater, must wait until its in the
										schedule */
			listLast(temp,eater);
			listLast(temp,food);
		}
	}
	return;
}

/******************************************************************************
** Pre-processing
******************************************************************************/

/* FIXME: trim unused allocated space WITH FUNNY DATA
** creates independent mosps
** preconditions: m contains the matrix and the non-subsumed list of products
** behaviour: while the non sub-subsumed product list exists, take the head
** (also remove it from the list) product and test if it intersects with 
** any subsequent products, if it does take the union with the intersecting
** product and delete that product, then re-test the priorly tested products.
** If all subseqent products fail to intersect we have found a subproblem,
** record it. The repeat that just descibed process.
** posconditions: m now contains the number of sub problems.
** performance: O(n^2)
*/
void partition(Mosp *m) {
	int prodA,prodB,l,s;
	Set *cmp;
	List *segment, *production;
	
	production = (m->s[0].p);				/* production stored in s[0] by deafult */
	
	for (s=0;listSize(production);s++) {					
		segment = listInit(m->pn+1);							
		prodA = listPos(production,0);					
		cmp = setCopy(m->cust[prodA]);				
		listLast(segment,prodA);					
		listDelete(production,prodA);			
		for (l=0;l<listSize(production);l++) {
			prodB = listPos(production,l);
			if (setIntersect(cmp,m->cust[prodB])) {	
				cmp = setUnion(cmp,m->cust[prodB]);	
				
				listDelete(production,prodB);
				listInsert(segment,prodB,isLessInt);
				l=-1;
			}
		}
		/* give segments everything it needs to solve it */
		m->s[s] = createSegment(m->cn,segment,m);
		setKill(cmp);
	}
	listKill(production);
	m->sn = s;	/* number of segments found */

	return;
}

/* looks for subsumed products, and generates a non-subsumed product list
** preconditions: that m exists
** behaviour: loop over the products, look for subsets, if it exists record it
** postconditions: segment[0] contains a list of non subsumed products (ready
** for use of partition is not selected), and m->sp has the supsumed products where each
** odd number subsumes the even
** performance: O(n^2)
*/
void subsumption(Mosp *m) {
	/* i and j are list iterators, and curP and cmpP and current product and
	** compared product respectivly */
	int curP,cmpP,i,j;
	
	m->sp = listInit(m->pn*2);
	
	for (i=0;i<listSize(m->s[0].p);i++) {
		curP = listPos(m->s[0].p,i);
		/* j < listSize - 1 as we need at least one product in the list */ 
		for (j=0;j<listSize(m->s[0].p);j++) { 	
			cmpP = listPos(m->s[0].p,j);
			if (curP!=cmpP && setSubset(m->cust[cmpP],m->cust[curP])) {	
				listLast(m->sp,curP);					/* subsumer */
				listLast(m->sp,cmpP);					/* sumsumed */
				listDelete(m->s[0].p,cmpP);
				if (cmpP < curP) i=0;;
				j=-1; /* as we deleted one, we need to step the counter back */
				
			}
		}
	}
	/* update default segment, as partition may not be flagged */
	m->s[0] = createSegment(m->cn,m->s[0].p,m);
}
