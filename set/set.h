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
** set.h: universal set ADT prototypes */
#ifndef SET_H
#define SET_H
#define __USE_GNU 1

typedef struct _Set Set;

/* alloc/free/zero */
Set	  * setAlloc							(void);							
void	 setKill								(Set *s);
void   setZero								(Set *a);

/* (add/del)[ing] */
void		setInsert							(Set *s, int d);
Set*		setDelete							(Set *s, int d);

/* ops */

Set		*setUnion								(const Set *a, const Set *b);		
Set		*setIntersection				(const Set *a, const Set *b);		
Set		*setDifference					(const Set *a, const Set *b);
Set	  *setUnionSelf						(Set *self, const Set *b);		/* stop freeing */
Set	  *setIntersectionSelf		(Set *self, const Set *b);		
Set	  *setDifferenceSelf			(Set *self, const Set *b);
Set *setSingleton(int e,int s);

/* tests */
int		 setCardinal						(const Set *s);
int		 setMember							(const Set *s, int d);
int		 setSubset							(const Set *a, const Set *b);
int		 setIntersect						(const Set *a, const Set *b);

/* aux */

Set		*setCopy1								(Set *b,const Set *a);
int		 setMin									(const Set *a);
inline int		setNext									(const Set *s,int p);
int		setEmpty								(const Set *s);
Set		*setCopy								(const Set *a);
void	 setAssign						(Set *a, const Set *b);
void	 setPrint								(const Set *s);
int	setMagnitude						(const Set *s);
int	setEqual								(const Set *a, const Set *b);

#endif
