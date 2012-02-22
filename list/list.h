
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
** list.h: universal list prototypes */
#ifndef LIST_H
#define LIST_H

typedef struct _List List;

/* comments are found in aList.c */

/* alloc/free */
List						*listInit			(unsigned int s);
void						 listKill			(List *l);

/* (add/del)[ing] */
List						*listLast			(List *l, int d);
void						listInsert		(List *l, int d, int (*compare)(int, int));  
void						listInsertPos	(List *l, int value, unsigned int pos);
void						listPosAdd		(List *l, int p, int data);
List						*listDelete		(List *l, int d);  

/* ops */
int							listFront			(List *l);
unsigned int		listSize			(const List *l);
int							listPos				(const List *l, unsigned int p);
void						listMerge			(List *a, const List *b);

/* aux */
void						listPrint			(const List *l);
List						*listCopy			(const List *l);
#endif
