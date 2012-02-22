
/*****************************************************************************
** MONASH UNIVERSITY, School of Computer Science and Software Engineering
** Student Declaration for CSE3301 Submission. I Joshua Davis, ID:19424094
** declare that this submission is my own work and has not been copied from any
** other source without attribution. I acknowledge that severe penalties exist
** for ant copying of code without attribution, including a mark of 0 for this
** assignment
******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "matrix.h"

void matrixPrint(int **m,int i, int j) {
	int k,l;
	printf("   ");
	for (k=1;k<=j;k++) printf("%3d",k); puts(""); 
	for(k=1;k<=i;k++) {
		printf("%3d",k);
		for (l=1;l<=j;l++)
			printf("%3d",m[k][l]);
		puts("");
	}
}

int **m_alloc(int a, int b) {
	int i,**m;

	m = (int**)malloc(sizeof(int*)*a);
	assert(m);
	for(i=0; i<a; i++) {
		m[i] = (int*)malloc(sizeof(int)*b);
		assert(m[i]);
	}
	return m;
}

void m_dump(int **m) {

}


