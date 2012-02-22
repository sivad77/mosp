
/*****************************************************************************
** MONASH UNIVERSITY, School of Computer Science and Software Engineering
** Student Declaration for CSE3301 Submission. I Joshua Davis, ID:19424094
** declare that this submission is my own work and has not been copied from any
** other source without attribution. I acknowledge that severe penalties exist
** for ant copying of code without attribution, including a mark of 0 for this
** assignment
******************************************************************************/

#ifndef FLAGS_H
#define FLAGS_H

enum {
	VERBOSE = 1,
	SUBSUMPTION = 2,
	PARTITION = 4,
	EXHREC = 8,
	BB = 16,
	EXHIT = 32,
	OPENING = 64,
	STACKS = 128,
	BINARYCHOP = 256,
	BACKWARDS = 512,
	STEPWISE = 1024,
	NO = BINARYCHOP | STEPWISE ,
	STEPS = 2048,
	STATS = 4096 
};

#endif
