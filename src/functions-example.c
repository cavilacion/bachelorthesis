/* Implements the mixed monotone function F(x) = Ax 
 * where x \in R^2 and
 * 
 * A = [ 0.5  0.1  ;  0.1  0.5 ]
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "fsa.h"

Point F (Point x) {
	Point F;
	F.x[0] = 0.5*x.x[0] + 0.1*x.x[1];
	F.x[1] = 0.1*x.x[0] + 0.5*x.x[1];
	return F;
}

/* Decomposition function of f
 */
Point f (Point x, Point y) {
	return F (x);
}
