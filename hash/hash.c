
/*****************************************************************************
** MONASH UNIVERSITY, School of Computer Science and Software Engineering
** Student Declaration for CSE3301 Submission. I Joshua Davis, ID:19424094
** declare that this submission is my own work and has not been copied from any
** other source without attribution. I acknowledge that severe penalties exist
** for ant copying of code without attribution, including a mark of 0 for this
** assignment
******************************************************************************/

#include <stdlib.h>
#include <assert.h>
#include "hash.h"

/* Based on Shane Saunders, but all routines changed 
** and a foreach implemented : source may be found via google search alg.tar.tz
** and his name*/


/*
** returns a pointer to a hash table with 'size' many buckets
** -pre-conditions: hashFn and CompFn indeed works as desired
** -post-conditions: a hash table
** -performance: O(n) thanks to calloc
*/
HashTbl *hashTblInit(int size,
										 int (*hashFn)(const void *d, int n),
										 int (*compFn)(const void *a, const void *b)
										 ) {
	HashTbl *tbl;

	tbl = malloc(sizeof(HashTbl));
	assert(tbl); /* we got hash table */
	tbl->array = calloc(size,sizeof(HashItem*));
	assert(tbl->array); /* we got buckets */
	tbl->size = size;

	/* functions we use to make sure its generic */
	tbl->hashFn = hashFn;
	tbl->compareFn = compFn; 
	return tbl;
}


/* FIXME: memory leak :S
** -pre-conditions: tbl is initised
** -post-conditions: tbl is empty
** -performance: O(n + size of hash) apx
*/
void hashTblKill(HashTbl *tbl) {
	int i, n;
	HashItem **array, *item, *temp;

	n = tbl->size;
	array = tbl->array;
	for(i=0; i<n; i++) 
		if((item = array[i])) 
			while(item) {
				temp = item;
				item = item->next;
				free(temp);
			}
	
	free(array);
	free(tbl);
}

/*
** inserted and item into the hash table
** -pre-conditions: tbl is intialised and item is valid
** -post-conditions: item is in hash table
** -performance: O(1) apx
*/
void *hashTblInsert(HashTbl *tbl, void *item) {
	int pos;
	HashItem *new, *cur,/**prev,*/ **array;
	//HashItem **update;

	array = tbl->array;
	
	/* find where to put into hash table */
	pos = tbl->hashFn(item,tbl->size);
	
	/* see if that item is in the list: handles duplicates	
	if(array[pos]) {
		cur = array[pos];
		while (cur) {
			if(tbl->compareFn(cur->item, item))
				return cur->item;
			prev = cur;
			cur = cur->next;
    }
		update = &prev->next;
	}
	else 
		update = &array[pos];

	new = malloc(sizeof(HashItem));
	assert(new); 
	new->item = item;
	new->next = NULL;
	*update = new;
	*/

	/* does not handle dups, optmised for mosp */
	new = malloc(sizeof(HashItem));
	assert(new); /* got memory */
	new->item = item;
	cur = array[pos];
	new->next = cur;
	array[pos] = new;
	
	return NULL;
}

/* sees if an item is in a hash
** -pre-conditions: tbl is intialised and item is valid
** -post-conditions: none
** -performance: O(1) apx
*/
void *hashTblFind(const HashTbl *tbl, void *item) {
	int pos;
	HashItem *cur,**array;

	array = tbl->array;
	pos = tbl->hashFn(item, tbl->size);

	if(array[pos]) {
		cur = array[pos];
		while (cur) {
			if(tbl->compareFn(cur->item, item))
				return cur->item;
			cur = cur->next;
		}
	}
	return NULL;
}

/* deletes item from hash table
** -pre-conditions: tbl is initialsed, and item is valid
** -post-conditions: item is not in table
** -performance: O(1) apx
*/
void *hashTblDelete(HashTbl *tbl, void *item) {
	int pos;
	HashItem *cur, *next, *prev, **array;
	HashItem *ret;

	array = tbl->array;

	pos = tbl->hashFn(item, tbl->size);
	next = array[pos];

	if(array[pos]) {
		cur = array[pos];
		prev = cur;
		
		while (cur) {
			if(tbl->compareFn(cur->item, item)) { /*item found */
				ret = cur->item;
				if (prev == array[pos])  // at head
					array[pos] = cur->next;
				else 
					prev->next = cur->next; //skip cur
				/* free something */
				free(cur);
				return ret;
			}
			prev = cur;			
			cur = cur->next;
		}
	}
	
	return NULL;
}

/* iterates over every item in a hash table, applying func to it: i used to to
** implement and easy way to delete all items from one hash table from another,
** refer to algorithms.c near stepwise algorithm
** -pre-conditions: tbl is initialed and func does what you want
** -post-conditions: whatever func does
** -performance: O(n + number of buckets) apx
*/
void hashTblEach(HashTbl *tbl, void  (*func)(void *item, void *data),
										 void *data) {
	int i;
	HashItem *cur, **array;
	array = tbl->array;
	
	for (i=0;i<tbl->size;i++) {
		if (array[i]) {		
			cur = array[i];
			for(;cur;cur = cur->next) 
				func(cur->item,data);
		}
	}
	
	return;
}
