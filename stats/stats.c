
/*****************************************************************************
** MONASH UNIVERSITY, School of Computer Science and Software Engineering
** Student Declaration for CSE3301 Submission. I Joshua Davis, ID:19424094
** declare that this submission is my own work and has not been copied from any
** other source without attribution. I acknowledge that severe penalties exist
** for ant copying of code without attribution, including a mark of 0 for this
** assignment
******************************************************************************/

#include <stdlib.h>
#include "stats.h"

void statsZero(Stats *stats) {
	stats->steps=0;
	stats->no=0;
	stats->yes=0;
	stats->opening=0;
	stats->reusedSets=0;
}

