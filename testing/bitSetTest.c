
/*****************************************************************************
** MONASH UNIVERSITY, School of Computer Science and Software Engineering
** Student Declaration for CSE3301 Submission. I Joshua Davis, ID:19424094
** declare that this submission is my own work and has not been copied from any
** other source without attribution. I acknowledge that severe penalties exist
** for ant copying of code without attribution, including a mark of 0 for this
** assignment
******************************************************************************/

/* Solving the MOSP by Joshua Davis
** Monash Uni - CSE3301/2006/Sem2 - ID: 19424094 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "../set/set.h"
#include "../set/bitSet.h"

#define ELEMENTS 100
#define TESTS 99999

Set *randomSet(void);
void visualTests(void);
void equalityTests(void);
void specialCaseTests(void);
void relationalTests(void);
void algebraicTests(void);
void testTime(void);

int main(int argc, char **argv) {
	visualTests();
	equalityTests();
	specialCaseTests();
	relationalTests();
	algebraicTests();
	return 0;
}

Set *randomSet(void) {
	Set *s;
	int n = 50;
	int i,r;
	
	s = setAlloc();

	for (i = 0,r = rand() % ELEMENTS; i < n; i++,r = rand() % ELEMENTS) 
		if (r != 5 && r != 60 && r != 70 &&r != 31) { //controls
			setInsert(s,r);
		}

	return s;
}

void equalityTests(void) {
	int i;
	Set *a,b,c;
	
	for (i=0;i<TESTS;i++) {
		a = randomSet();

		/* testing set copy */
		setAssign(&b,a);
		setAssign(&c,&b);
		assert(setEqual(a,&b));
		assert(setEqual(&b,a));
		assert(setEqual(a,&c));

		/* testing setInsert and setEqual and setCopy */
		setInsert(a,5);
		assert(!setEqual(a,&b));
		assert(!setEqual(&b,a));
		
		/* testing delete */
		setDelete(a,5);
		assert(setEqual(a,&b));
		assert(setEqual(&b,a));

		/* testing cardinal */
		assert(setCardinal(&b) == setCardinal(&c));
		
		setKill(a);
	//	setKill(b);
	//	setKill(c);
	}
}

typedef Set *(*setFun)(Set*, const Set*);
void checkCase(setFun fun, Set* s1, Set* s2, Set* expect) {
	Set *res;
	res = setCopy(s1);
	(*fun)(res, s2);
	assert(setEqual(res, expect));
}

void specialCaseTests(void) {
	Set empty;
	Set universal;
	int i;
	Set *s;
	Set *r;
	Set *w;

	setZero(&empty);
	setZero(&universal);

	for (i=0; i<ELEMENTS; i++)
	  setInsert(&universal,i);
	
	checkCase(setUnionSelf, &universal, &universal, &universal);	
	checkCase(setUnionSelf, &universal, &empty, &universal);
	checkCase(setUnionSelf, &empty, &universal, &universal);
	checkCase(setUnionSelf, &empty, &empty, &empty);
	
	checkCase(setIntersectionSelf, &universal, &universal, &universal);
	checkCase(setIntersectionSelf, &universal, &empty, &empty);
	checkCase(setIntersectionSelf, &empty, &universal, &empty);
	checkCase(setIntersectionSelf, &empty, &empty, &empty);
	
	checkCase(setDifferenceSelf, &universal, &universal, &empty);
	checkCase(setDifferenceSelf, &empty, &empty, &empty);

	s = setAlloc();
	assert(setEmpty(s));
	for (i=30; i<80; i++) 
		setInsert(s, i);
	assert(setCardinal(s) == 50);

	for (i=30; i<80; i++) 
	  assert(setMember(s,i));
	
	for (i=30; i<80; i++) {
	  setDelete(s,i);
	  setDelete(s,i);
	  assert(setCardinal(s) == 79 - i); 
	}
	assert(setEmpty(s));

	for (i=0; i<ELEMENTS;i++) {
	  w = randomSet();
		r = setAlloc();
		if (setEmpty(w)) continue;

		assert(setSubset(&empty, w));
	  assert(!setSubset(w, &empty));
	  assert(setSubset(w, &universal));
	  assert(!setSubset(&universal, w));
	  assert(!setSubset(&universal, &empty));
		checkCase(&setIntersectionSelf, &empty, w, &empty);
		checkCase(&setIntersectionSelf, w, &empty, &empty);
		checkCase(&setIntersectionSelf, &universal, w, w);
		checkCase(&setIntersectionSelf, w, &universal, w);
		checkCase(&setUnionSelf, &universal, w, &universal);
		checkCase(&setUnionSelf, w, &universal, &universal);
		checkCase(&setDifferenceSelf, w, &empty, w);
		
		r = setCopy(&universal);
		setDifferenceSelf(r, w);												// r = u - w;
		checkCase(&setDifferenceSelf, &universal, r, w); // (u - (u - w) == w)
		checkCase(&setUnionSelf, w, r, &universal);			// s + (u - w) == u
		checkCase(&setUnionSelf, r, w, &universal);			// (u - w) + w == u
		setKill(w);
		setKill(r);
	}
	printf("The special case tests have been passed\n"); 
}


void algebraicTests(void) {
	Set empty;
	Set universal;
	int i;
  Set *s=NULL;
  Set *t=NULL;
  Set *u=NULL;
  Set *v=NULL;
  Set *w=NULL;

	setZero(&empty);
	setZero(&universal);

	for (i=0; i<ELEMENTS; i++) {
	  setInsert(&universal, i);
	}

	for (i=0; i<TESTS; i++) {
		
		u = randomSet();
	  v = randomSet();
	  w = randomSet();
	  
		/* u + v == v + u */
		s = setCopy(u);
		setUnionSelf(s, v);
		t = setCopy(v);
		setUnionSelf(t, u);
		assert(setEqual(s, t));

		/* u + (v + w) == (u + v) + w */	  
		t = setCopy(v);
		setUnionSelf(t, w);
		s = setCopy(u);
		setUnionSelf(s, t);
		t = setCopy(u);
		setUnionSelf(t, v);
		setUnionSelf(t, w);
		assert(setEqual(s, t));

		/* u * v == v * u */
		s = setCopy(u);
		setIntersectionSelf(s, v);
		t = setCopy(v);
		setIntersectionSelf(t, u);
		assert(setEqual(s, t));

		/* u * (v * w) == (u * v) * w */	  
		t = setCopy(v);
		setIntersectionSelf(t, w);
		s = setCopy(u);
		setIntersectionSelf(s, t);
		t = setCopy(u);
		setIntersectionSelf(t, v);
		setIntersectionSelf(t, w);
		assert(setEqual(s, t));

		/* u - v == u - (u * v) */
		s = setCopy(u);
		setIntersectionSelf(s, v);
		t = setCopy(u);
		setDifference(t, s);
		s = setCopy(u);
		setDifference(s, v);
		assert(setEqual(s, t));

		/* additional tests, not implemented 
	  assert(w * (u - v) == w * u - w * v);
	  assert(u * (v + w) == (u * v) + (u * w));
	  assert(universal - (u * v) == (universal - u) + (universal - v));
	  assert(universal - (u + v) == (universal - u) * (universal - v)); 
	  */
		
		setKill(u);
		setKill(v);
		setKill(w);
		setKill(s);
		setKill(t);
	}
	//setKill(empty);
	//setKill(universal);
	printf("The algebraic tests have been passed\n"); 
}

void relationalTests() {
  Set *s=NULL;
  Set *t=NULL;
  int i;

  s = setAlloc();
  s = setAlloc();

	for (i=0; i<TESTS; i++) {
	  s = randomSet();
	  t = setCopy(s);
		
	  assert(setSubset(s, t));
	  assert(setSubset(t, s));
	  assert(setEqual(s, t));
	  assert(setEqual(t, s));
	  
		setInsert(s,5);
		setInsert(s,70);
	  assert(setSubset(t, s));
	  assert(!setSubset(s, t));
		setKill(s);
		setKill(t);
	}
	printf("The relational tests have been passed\n"); 

}

void visualTests(void) {
	int i;
	
	puts("*Start Visual Test");
	puts("**Testing allocation");
	printf("Using %d bit integers\n",IBITS);
	
	
	Set *a = setAlloc();
	
	assert(setEmpty(a));
	
	printf("a: asked for 100 bits, got %d, with %d arrays at %d bits each\n",
				 ELEMENTS*IBITS,ELEMENTS,IBITS);
	
	puts("**Testing insert");
	puts("aI: 1 2 4 8 16 ... N [7]");	
	for (i=1;i<ELEMENTS;i*=2) setInsert(a,i);
	printf("aP: "); setPrint(a);printf("[%d]\n",setCardinal(a));puts("");

	Set *b;
	b = setAlloc();
	for (i=90;i<=99;i++) setInsert(b,i);
	
	puts("aI: 90 91 92 93 94 95 96 97 98 99 [10]");	
	printf("aP: "); setPrint(b);printf("[%d]\n",setCardinal(b));puts("");
	/*
	printf("min %d\n",setMin(b));
	setDelete(b,setMin(b));
	setInsert(b,68);
*/

	/* testing setMin */
	/*
	printf("min %d\n",setMin(b));
	setDelete(b,setMin(b));
	setInsert(b,44);
	printf("min %d\n",setMin(b));
	setDelete(b,setMin(b));
	setInsert(b,16);
	printf("min %d\n",setMin(b));
	setDelete(b,setMin(b));
	setInsert(b,0);
	printf("min %d\n",setMin(b));
	setDelete(b,setMin(b));
	setInsert(b,31);
	printf("min %d\n",setMin(b));
	*/
/*
	Set aa;
	setZero(&aa);
	int x;

	for (x=1;x*2<100;x++) {
		printf("%d ",x*2);
		setInsert(&aa,x*2);
	}

	puts("\n");setPrint(&aa);puts("\n");

	puts("lol");
	for (x=0;((x=setNext(&aa,x)) != -1);) {
		//x = setNext(&aa,1);	
		printf("%d ",x);
	}
	*/
	return;
	
}	

