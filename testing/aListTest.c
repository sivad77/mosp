
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

#include "../list/list.h"
#include "../list/aList.h"


int cmpA(int a, int b) {
	return (a<b) ? 1 : 0;
}


int cmpB(int a, int b) {
	return (a<b) ? 0 : 1;
}

int main(int argc, char **argv) {
	List *a,*b,*c,*d;
	
	a = listInit(100);
	b = listInit(50);
	c = listInit(100);
	
	puts("Testing insert");
	puts("a: insert 2 4 6 8 10 30 60 99 [8] ascending");	
	listInsert(a,2,cmpA);
	listInsert(a,4,cmpA);
	listInsert(a,6,cmpA);
	listInsert(a,8,cmpA);
	listInsert(a,10,cmpA);
	listInsert(a,30,cmpA);
	listInsert(a,60,cmpA);
	listInsert(a,99,cmpA);
	printf("a: print  "); listPrint(a); puts("");

	
	puts("b: insert 1 3 5 7 9 11 25 49 [8] decending");
	puts("b: PRINT  1 3 5 7 9 11 25 49 [8] decending");
	listInsert(b,1,cmpB);
	listInsert(b,3,cmpB);
	listInsert(b,5,cmpB);
	listInsert(b,7,cmpB);
	listInsert(b,9,cmpB);
	listInsert(b,11,cmpB);
	listInsert(b,25,cmpB);
	listInsert(b,49,cmpB);
	printf("b: print  "); listPrint(b); puts("");
	
	puts("c: append 70 1 3 5 7 9 [6]");
	listAppend(c,70); // test second array
	listAppend(c,1);
	listAppend(c,3);
	listAppend(c,5);
	listAppend(c,7);
	listAppend(c,9);
	printf("c: print  "); listPrint(c); puts("");

	puts("Testing copy");
	puts("d: copy  70 1 3 5 7 9");
	d = listCopy(c);
	printf("d: print "); listPrint(d); puts("");


	puts("Testing delete");
	puts("d: 70 1 3 5 7 9 delete 3 7 9 [6]");
	puts("d: PRINT 70 1 5 ");
	listDelete(d,3);
	listDelete(d,7);
	listDelete(d,9);
	printf("d: print "); listPrint(d); puts("");

	return 0;

}

