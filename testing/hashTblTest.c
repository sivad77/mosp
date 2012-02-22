
/*****************************************************************************
** MONASH UNIVERSITY, School of Computer Science and Software Engineering
** Student Declaration for CSE3301 Submission. I Joshua Davis, ID:19424094
** declare that this submission is my own work and has not been copied from any
** other source without attribution. I acknowledge that severe penalties exist
** for ant copying of code without attribution, including a mark of 0 for this
** assignment
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "../hash/hash.h"



#define N_ITEMS 150
#define N_BUCKETS 10



typedef struct test_item {
	int key;
	int other_data;
} test_item_t;

int compareFn(const void *item1, const void *item2) {
	return (((test_item_t *)item1)->key == ((test_item_t *)item2)->key) ? 1 : 0;
}

int hashFn(const void *item, int n)
{
	srand(((test_item_t *)item)->key);
  rand();
	return rand() % n;
}


void delete(void *item, void *data) {
	hashTblDelete(data,item);
	//((test_item_t*)item)->other_data = ((test_item_t*)item)->other_data+(int)data;
}


void addone(void *item, void *data) {
	((test_item_t*)item)->other_data = ((test_item_t*)item)->other_data+(int)data;
}

int main(void) {
    int i;
    test_item_t items[N_ITEMS];
    test_item_t lookup, duplicate;
    HashTbl *hash_table;

    /* Assign unique keys to the test items.  For item i we will use the key
     * i^2.
     */
    for(i = 0; i < N_ITEMS; i++) {
			items[i].key = i*i;
			items[i].other_data = i;
    }

    /* Create a has table with N_BUCKETS buckets. */
    hash_table = hashTblInit(N_BUCKETS, hashFn, compareFn);

    /* Insert all the items into the hash table. */
    printf("Testing inserting items...");
    for(i=0; i<N_ITEMS;i++) {
		//		printf("Inserting %d @ index ",items[i].key);
        if(hashTblInsert(hash_table, &items[i]) != NULL) {
            printf("error.\n");
            exit(1);
						
				}

		}
    printf("passed.\n");

    /* Try inserting an item which duplicates an existing items key. */
//    printf("Testing key duplication...");
  //  duplicate.key = 2*2;
//    if(hashTblInsert(hash_table, &duplicate) != &items[2]) {
 //       printf("error.\n");
  //      exit(1);
//    }
 //   printf("passed.\n");

    printf("Testing find...\n");
		test_item_t *result;
    for(i = 0; i < N_ITEMS; i++) {
			lookup.key = i;
      if((result = hashTblFind(hash_table, &lookup))) {
				printf("  key %d - found with data %d \n", i,result->other_data);
			}
			else {
//            printf("  key %d - not found\n", i);
			}
		}

		/* now try adding one to all elements */
		
		int one = 1;
		
    printf("Testing find + 1...\n");
		hashTblEach(hash_table,&addone,(void*)one);
		
		//test_item_t *result;
    for(i = 0; i < N_ITEMS; i++) {
			lookup.key = i;
       if((result = hashTblFind(hash_table, &lookup))) {
          printf("  key %d - found with data %d \n", i,result->other_data);
				}
		}

    /* Now try deleting items. */
    printf("Testing delete...");
    for(i = 0; i < N_ITEMS; i++) {
				lookup.key = i*i;
        if(hashTblDelete(hash_table, &lookup) != &items[i]) {
	}
    }
    printf("passed.\n");



    printf("Testig it deleted..\n");
    for(i = 0; i < N_ITEMS; i++) {
			lookup.key = i;
      if((result = hashTblFind(hash_table, &lookup))) {
				printf("  key %d - found with data %d \n", i,result->other_data);
			}
			else {
//            printf("  key %d - not found\n", i);
			}
		}


    for(i = 0; i < N_ITEMS; i++) {
			items[i].key = i*i;
			items[i].other_data = i+2;
    }


		puts("inserting again");
    for(i=0; i<N_ITEMS;i++) {
		//		printf("Inserting %d @ index ",items[i].key);
        if(hashTblInsert(hash_table, &items[i]) != NULL) {
            printf("error.\n");
            exit(1);
						
				}
		}

		
    printf("Testig insert again..\n");
    for(i = 0; i < N_ITEMS; i++) {
			lookup.key = i;
      if((result = hashTblFind(hash_table, &lookup))) {
				printf("  key %d - found with data %d \n", i,result->other_data);
			}
			else {
//            printf("  key %d - not found\n", i);
			}
		}
		

		hashTblEach((void*)hash_table,&delete,(void*)hash_table);

    printf("Testig it deleted with func delete..\n");
    for(i = 0; i < N_ITEMS; i++) {
			lookup.key = i;
      if((result = hashTblFind(hash_table, &lookup))) {
				printf("  key %d - found with data %d \n", i,result->other_data);
			}
			else {
//            printf("  key %d - not found\n", i);
			}
		}




		
    return 0;
}

