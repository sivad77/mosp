
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
** aList.h: list ADT implemented with a flat array */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "../list/list.h"
#include "../list/aList.h"
/* based on a list implementation from 2302 - heavly changed */

/*	returns the size of a list
** -pre-conditions: l is initialised
** -behaviour: access data structure
** -post-conditions: no change
** -performance: O(1)
*/
unsigned int listSize(const List *l) {
	return l->s;
}

/* prints the elements in a list in order from 0 to n
** -pre-conditions: l is initialised
** -behaviour: iteration over list
** -post-conditions: no change
** -performance: O(n)
*/
void listPrint(const List *l) {
	int i;
	for (i=0;i<l->s;i++)
		if (l->d[i] >= 0) printf("%d ",l->d[i]);
}

/* returns a pointer to a list of with a capacity of size
** -pre-conditions: size is a positive interger
** -behaviour: allocations
** -post-conditions: 
** -performance: O(1)
*/
List *listInit(unsigned int size) {
  List *l = malloc(sizeof(List));
	assert(l); /* got memory */
 
	l->d = malloc(size*sizeof(int));
	assert(l->d); /* got memory */
	
	/* make empty list */
  l->s = 0;
  l->c = size;
	
	return l;
}

/* frees up memory pointed to by l
** -pre-conditions: l is initialised
** -behaviour: free.
** -post-conditions: l is no longer a list
** -performance: O(1)
*/
void listKill(List *l) {
	free(l->d);
	free(l);
}

/* returns a pointer to a copy of list l
** -pre-conditions: l is intialised
** -behaviour: copy memory from list l to list a
** -post-conditions: a is a list which is a copy of l
** -performance: O(n)
*/
List *listCopy(const List *l) {
	List *a = listInit(l->c);
	
	memcpy(a->d,l->d,sizeof(int)*l->s);
	
	a->s = l->s;
	a->c = l->c;
	return a;
}
	
/* add an item to the end of a list
** -pre-conditions: l is intialised and has room for another element
** -behaviour: add it to the end and increase the size
** -post-conditions: l has more more element
** -performance: O(1)
*/
List *listLast(List *l, int x) {
  /* do we have enough space at the end */
	assert(!(l->s == l->c));
	
	l->d[l->s++] = x;
	return l;
}

/* insert and element at a positon pos in list l
** -pre-conditions: l is initised and has room for another element
** -behaviour: copy all elements from pos up one spot, then insert as pos
** -post-conditions: l contains an element at pos
** -performance: O(n)
*/
void listInsertPos(List *l, int value, unsigned int pos) {
  /* do we have enough space */
	assert(!(l->s == l->c));

	memmove(l->d+pos+1,l->d+pos,sizeof(int)*(l->s-pos));
	l->d[pos] = value;
	l->s++;
}

/* insert an element dependant on copare func, ie, for asscending descending
** order
** -pre-conditions: l is intialised, and compare works to fit and there is room
**		for one more element
** -behaviour: depending on the result of compar, find where the element is to
**		be added, the shuffle down all elements after that position, and insert
**		the item
** -post-conditions: element is inserted in order dectated by comapre
** -performance: O(n)
*/
void listInsert(List *l, int value, int (*compare)(int, int)) {
  int i;

  /* do we have enough space */
  assert(!(l->s == l->c));
  
	/* find spot to insert */
	for (i=0; i<l->s; i++) 
		if ((*compare)(value, l->d[i]) > 0) break;
	
	memmove(l->d + i + 1, l->d + i,sizeof(int)*(l->s - i));
	l->s++;
	l->d[i] = value;
}

/* look for element x in list l and delete it
** -pre-conditions: l is initialised
** -behaviour: iterate until l is found, then shuffle all elements down one and
**		overwrite x
** -post-conditions: x is deleted from l
** -performance: O(n)
*/
List *listDelete(List *l, int x) {
	int i;
	
	/* can we delete an element from an empty list */
  assert(!(l->s == 0));

  for (i=0; i<l->s; i++)
		/* found x, now move all elements after x, towards the front
		** by one element */
		if (l->d[i] == x) {
			memmove(l->d + i, l->d + i + 1,sizeof(int)*(l->c-i-1));
			l->s--;
			break;
		}

	return l;
}

/* move one list to the end of another
** -pre-conditions: both a and b are initialised and a has enough room for b
** -behaviour: move elements across to a
** -post-conditions: a is longer and contains b
** -performance: O(n)
*/
void listMerge(List *a, const List *b) {
	/* is there enough space, to slot b behind a */
	assert((a->s + b->s) < a->c);
	
	memmove(a->d + a->s,b->d,sizeof(int)*(b->s));
	a->s += b->s;
	return;
}

/* return an item at position p
** -pre-conditions: p < less then the size of the list, and l must be
**		initialised
** -behaviour: access array directly
** -post-conditions: no change
** -performance: O(1)
*/
int listPos(const List *l, unsigned int p) {
	assert(p <= l->s);
	return l->d[p];
}
