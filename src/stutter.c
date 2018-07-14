/* 
 * Implementation of Algorithm 1 in the thesis. 
 * 
 */
#include <stdio.h>
#include <stdlib.h>

#include "fsa.h"

/* Returns true if x1 <= x2 */
size_t isSmaller (Point x1, Point x2, int n) {
	for (int i=0; i<n; i++) {
		if (x1.x[i] > x2.x[i])
		  return 0;
	}
	return 1;
}

void getNewIntersect (Point y1, Point y2,
											Point x1, Point x2,
											Point *xi1, Point *xi2, int n) {
	for (int i=0; i<n; i++) {
		xi1->x[i] = max(x1.x[i],y1.x[i]);
		xi2->x[i] = min(x2.x[i],y2.x[i]);
	}
}

/* The input state has a self-loop. This function returns true if the
 * self-loop is spurious
 */
size_t stuttering (FSA* fsa, Grid* Q, int state) {
	/* initialization */
	State q = fsa->q[state];
	int iter = 0;
	int n = Q->dims;
	Point x1, x2;
	Point xi1, xi2;
	for (int i=0; i<n; i++) {
		x1.x[i] = xi1.x[i] = Q->xi[i][q.iota[i]];
		x2.x[i] = xi2.x[i] = Q->xi[i][q.iota[i]+1];
	}
	
	while (iter < MAX_ITER) {
		/* compute image box of decomposition function */
		Point y1 = f (xi1, xi2);
		Point y2 = f (xi2, xi1);
		if (isSmaller (y1, x2, n) && isSmaller (x1, y2, n) ) {
			getNewIntersect (y1,y2, x1,x2, &xi1,&xi2, n);
			iter++;
		}
		else {
			return 1;
		}
	}
	return 0;
}
