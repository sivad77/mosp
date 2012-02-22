
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
** aList.h: list ADT data structure */
#ifndef ALIST_H
#define ALIST_H

/* list ADT data structure */
struct _List{
  int *d;					/* Points to the array of [d]ata */
  int s;					/* Number of elements in data, the current [s]ize */
	int c;					/* The total [c]apacity of the list*/
};
#endif
