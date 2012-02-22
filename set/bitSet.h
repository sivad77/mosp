
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
** bitSet.h: data structure for set ADT */
#ifndef BSET_H
#define BSET_H
#include <stdint.h>

/* memory macros */

#ifdef BIG
#define INT uint64_t
#define SIZE 2
#endif

#ifdef SMALL
#define INT uint32_t
#define SIZE 4
#endif


#define IBITS (sizeof(INT)*8)
#define IBYTES (sizeof(INT))

/* set ADT data structure */
struct _Set {
	INT b[SIZE];
};


#endif
