/*****************************************************************************
** MONASH UNIVERSITY, School of Computer Science and Software Engineering
** Student Declaration for CSE3301 Submission. I Joshua Davis, ID:19424094
** declare that this submission is my own work and has not been copied from any
** other source without attribution. I acknowledge that severe penalties exist
** for ant copying of code without attribution, including a mark of 0 for this
** assignment
******************************************************************************/

#include <stdio.h>



const int MultiplyDeBruijnBitPosition[32] = {
	  0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8, 
		  31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
};


int main(void) {
	int i;
	for (i=0;i<256;i++) {
		if (!(i%16)) printf("\n");
		printf ("%d, ",MultiplyDeBruijnBitPosition[((i & -i) * 0x077CB531UL) >> 27]);
	}
	return 0;
}
