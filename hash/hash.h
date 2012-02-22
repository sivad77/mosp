
/*****************************************************************************
** MONASH UNIVERSITY, School of Computer Science and Software Engineering
** Student Declaration for CSE3301 Submission. I Joshua Davis, ID:19424094
** declare that this submission is my own work and has not been copied from any
** other source without attribution. I acknowledge that severe penalties exist
** for ant copying of code without attribution, including a mark of 0 for this
** assignment
******************************************************************************/

#ifndef HASH_H
#define HASH_H

typedef struct _HashItem {
  void *item;
  struct _HashItem *next;
} HashItem;

typedef struct _HashTbl {
  int size;
  HashItem **array;
	int (*hashFn)(const void *d, int n);
  int (*compareFn)(const void *a, const void *b);
} HashTbl;

HashTbl *hashTblInit(int n_buckets,
								  int (*hashFn)(const void *d, int n),
									int (*compFn)(const void *d, const void *item2));				
void	  hashTblKill			(HashTbl *ht);
void	 *hashTblInsert		(HashTbl *ht, void *item);
void	 *hashTblFind			(const HashTbl *ht, void *item);
void	 *hashTblDelete		(HashTbl *ht, void *item);
void	 hashTblEach	(HashTbl *ht, void  (*func)(void *item, void *data),
												 void *data);
#endif

