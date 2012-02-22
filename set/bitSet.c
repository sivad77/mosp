
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
** bitSet.c: set ADT using bit-vector implementation */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "set.h"		// universal function prototypes
#include "bitSet.h"

#define MOD 31
#define DIV 5

/*
** return a set on the heap
** -pre-conditions: none
** -behaviour: allocates memory
** -post-conditions: s is allocated
** -performance: O(1)
*/
Set *setAlloc(void) {
	Set *s;
	int i;
	
	s = (Set*)malloc(sizeof(Set));
	assert(s);
	
	for (i=0;i<SIZE;++i) 
		s->b[i] = 0;
	
	return s;
}

/*
** zero all arrays in set s
** -pre-conditions: s points to valid memory
** -behaviour: loops and sets to 0
** -post-conditions: s is zeroed
** -performance: O(1)
*/
void setZero(Set *s) {
	int i;
	
	for (i=0;i<SIZE;++i) 
		s->b[i] = 0;
}

/* 
** free all memory s needs 
** -pre-conditions: s must be initialised
** -post-conditions: s is frred
** -performance: O(1)
*/
void setKill(Set *s) {
	free(s);
}

/*
** insert element into set 
** -pre-conditions: s must be initialised
** -post-conditions: s is unchanged
** -performance: O(1)
*/
inline void setInsert(Set *s, int d) {
	s->b[d>>DIV] |= (INT)1 << (d&MOD);
}

/* 
** delete element from set 
** -pre-conditions: s must be initialised
** -post-conditions: s is unchanged
** -performance: O(1)
*/
inline Set *setDelete (Set *s, int d) {
	s->b[d>>DIV] &= ~((INT)1 << (d&MOD));
	return s;
}

/* 
** return true if element d is in set s 
** -pre-conditions: s must be initialised
** -post-conditions: s is unchanged
** -performance: O(1)
*/
inline int setMember(const Set *s, int d) {
	return (s->b[d>>DIV] >> (d&MOD)) & (INT)1;
}

/*
** return a set containing common elements to both set a and 
**	set b
** -pre-conditions: a and b must be initialised
** -behaviour: bit wise 'and' each bit-vector
** -post-conditions: a and b are unchanged
** -performance: O(1)
*/
inline Set* setIntersection (const Set *a, const Set *b) {
	int i;
	Set *c = setAlloc();
	for (i=0;i<SIZE;++i) c->b[i] = a->b[i] & b->b[i];
	return c;
}

/*
** make self contain common elements to set self and b
** -pre-conditions: self and b must be initialised
** -behaviour: bit wise 'and' each bit-vector
** -post-conditions: b is unchanged, self is the intersection
** -performance: O(1)
*/
inline Set* setIntersectionSelf (Set *self, const Set *b) {	
	int i;
	for (i=0;i<SIZE;++i) self->b[i] &= b->b[i];
	return self;
}

/* 
** return a set containing all elements in sets a and b
** -pre-conditions: a and b must be initialised
** -behaviour: bit wise 'or' each bit-vector
** -post-conditions: a and b are unchanged
** -performance: O(1)
*/
inline Set* setUnion (const Set *a, const Set *b) {	
	int i;
	Set *c =  setAlloc();
	for (i=0;i<SIZE;++i) c->b[i] = a->b[i] | b->b[i];

	return c;
}

/* 
** make self the union of self and b
** -pre-conditions: self and b must be initialised
** -behaviour: bit wise 'or' for each bit-vector
** -post-conditions: b is unchanged, and self is not
** -performance: O(1)
*/
inline Set* setUnionSelf(Set *self, const Set *b) {
	int i;
	for (i=0;i<SIZE;++i) self->b[i] |= b->b[i];
	return self;
}
	
/* 
** return a set with elements not common to a 
** -pre-conditions: a and b must be initialised
** -behaviour: 'not' the bit-field in b, then take the intersection 'and'
** -post-conditions: a and b are unchanged
** -performance: O(1)
*/
inline Set* setDifference(const Set *a, const Set *b) {
	Set *t = setCopy(a); int i;
	for (i=0;i<SIZE;++i) t->b[i] &= ~b->b[i];
	return t;
}

/* 
** make self contain the difference between self and a 
** -pre-conditions: self and b must be initialised
** -behaviour: 'not' the bit-field in b, then take the intersection 'and'
** -post-conditions: b is unchanged, self is changed
** -performance: O(1)
*/
inline Set* setDifferenceSelf (Set *self, const Set *b) {
	int i;
	for (i=0;i<SIZE;++i) self->b[i] &= ~b->b[i];
	return self; 
}

/* 
** return true if a and b are the same
** -pre-conditions: a and b must be initialised
** -behaviour: compare each bit-vector
** -post-conditions: a and b are unchanged
** -performance: O(1)
*/
int setEqual(const Set *a, const Set *b) {
	int i;
	for (i=0;i<SIZE;++i) 
		if (a->b[i] != b->b[i]) return 0;
	return 1;
}

/* 
** return true if a is subset of b 
** -pre-conditions: a and b must be initialised
** -behaviour: take the intersection of a and b, then test if its empty
** -post-conditions: a and b are unchanged
** -performance: O(1)
*/
int setSubset (const Set *a, const Set *b) {
	int i;
	Set *t = setIntersection(a,b);
	i = setEqual(t,a);
	setKill(t);
	return i;
}

/* 
** return true if a and b share a common element  
** -pre-conditions: a and b must be initialised
** -behaviour: take the intersection of a and b, then test if its empty
** -post-conditions: a and b are unchanged
** -performance: O(n)
*/

int setIntersect(const Set *a, const Set *b) {
	int i;
	Set *t = setIntersection(a,b);
	i = !setEmpty(t);
	setKill(t);
	return i;
}

/* return the number of elements in s 
** -pre-conditions: s must be initialised
** -behaviour: count each bit which is on in the bit-vector
** -post-conditions: s is unchanged
** -performance: O(n)
*/
const unsigned char b[] = {
	  0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 
	  1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
	  1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
	  2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
		1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
		3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
		1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
		3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
		3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
		3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
		4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
};

int	setCardinal(const Set *s) { 
	int i,c;
	
	for (i=c=0;i<SIZE;++i) {
		if (s->b[i])

		#ifdef SMALL
			c += b[s->b[i] & 0xff] + b[(s->b[i] >> (INT)8) & 0xff] +
			b[(s->b[i] >> (INT)16) & 0xff] + b[s->b[i] >> (INT)24]; 
		#endif

		#ifdef BIG
			c += b[s->b[i] & 0xff] + b[(s->b[i] >> (INT)8) & 0xff] +
			b[(s->b[i] >> (INT)16) & 0xff] + b[s->b[i] >> (INT)24] + 
			b[(s->b[i] >> (INT)32) & 0xff] + b[(s->b[i] >> (INT)40) & 0xff] +
			b[(s->b[i] >> (INT)48) & 0xff] + b[s->b[i] >> (INT)56];
		#endif
	}
	return c;
}

/*
** return true is s is empty 
** -pre-conditions: s must be initialised
** -behaviour: test the magnitude of each bit-vector array
** -post-conditions: s is unchanged
** -performance: O(1)
*/
int setEmpty(const Set *s) {
	int i;
	for (i=0;i<SIZE;i++)
		if (s->b[i]) return 0;
	return 1;
}

const int MultiplyDeBruijnBitPosition[32] = {
	  0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8, 
		  31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
};

/*
** return the minimun element in s
** -pre-conditions: s must be initialised
** -behaviour: apply something complicated :S
** -post-conditions: s is unchanged
** -performance: O(1)
** reference: bit twidding hacks on google */
int setMin(const Set *s) {
	int i,w;
	for (i=w=0;i<SIZE;i += 32,++w) 
		if (s->b[w])
			return i+MultiplyDeBruijnBitPosition
							 [((s->b[w] & -s->b[w]) * 0x077CB531UL) >> 27];
	return -1;
}

/*
** return a copy of set s
** -pre-conditions: s must be initialised
** -behaviour: copy each bit vector from s into a
** -post-conditions: s is unchanged
** -performance: O(1)
*/
Set *setCopy(const Set *s) {
	int i;
	Set *a = setAlloc();
	for (i=0;i<SIZE;++i) a->b[i] = s->b[i];
	return a;
}

/*
** return a copy of set b
** -pre-conditions: a and bmust be initialised
** -behaviour: copy each bit vector from s into a
** -post-conditions: b is unchanged
** -performance: O(1)
*/
void setAssign(Set *a, const Set *b) {
	int i;
	for (i=0;i<SIZE;++i) a->b[i] = b->b[i];
	return;
}

const int NUM_TRAILING_ZEROS[] = {
    8, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0
};

/* counts the number of trailing 0 from the left in an 32 bit integer */
inline int offset(INT word)
{
    if (word & 0xff) {
        return NUM_TRAILING_ZEROS[word & 0xff];
    }
    else {
        word >>= 8;
        if (word & 0xff) {
            return NUM_TRAILING_ZEROS[word & 0xff] + 8;
        }
        else {
            word >>= 8;
            if (word & 0xff) {
                return NUM_TRAILING_ZEROS[word & 0xff] + 16;
            }
            else {
                word >>= 8;
                return NUM_TRAILING_ZEROS[word & 0xff] + 24;
            }
        }
    }
}



/*
** iterates over set S, starting at from
** -pre-conditions: s must be intialised, and s < size of the set
** -behaviour: using the trailing zero table, and applying the sliding window
**		technique, skips lots of zeros :)
** -post-conditions: s is unchanged
** -performance: O(1) + small constant
** woops, I used someone elses code to help me with this, and forgot to write
** down the reference. I was about to give up, as my generated table above did
** not work with all cases, but I put a sequence of that table into google
** code, and found that someone had this same idea.
*/
inline int setNext(const Set *s, int from) {
		from++;
		INT* bits = s->b;
    int array = from >> 5;	/* array to start in */ 
    int pos = (from & 31);	/* off set in array */

    INT word = s->b[array] >> pos;

    if (word == 0) {
        pos = 0;
        do {
            array++;
            if (array >= SIZE) {
                return -1;
            }
        } while (bits[array] == 0);
        word = bits[array];
    }
		return  ((array<<5) + pos + offset(word));
}

/*
inline int setNext(const Set *s,int i) {
	for (i++;i<SIZE*IBITS;++i)
		if ((s->b[i/IBITS] >> (i%IBITS)) & (INT)1)
			return i;
	return -1;
}
*/

void setPrint(const Set *s) {
	int i;
	for (i=0;i<SIZE*IBITS;++i)
		if(setMember(s,i)) printf("%d ",i);
}

/*
** iterates over the arrays in s, adding the value of each integer, this is
**	useful for the HASH FUNCTIONS
** -pre-conditions: s must be intialised
** -behaviour: 
** -post-conditions: s is unchanged
** -performance: O(1)
*/
int setMagnitude(const Set *s) {
	int i;
	int mag;
	for (mag=i=0;i<SIZE;++i) 
		mag += s->b[i];
	
	return mag;
}
