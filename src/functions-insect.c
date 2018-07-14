/* Implements the mixed monotone function F(x) for a system that models
 * insect population dynamics.
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "fsa.h"

float b = 7.88;
float c_ea = 0.011;
float c_el = 0.014; 
float p = 0.839;
float q = 0.5; 
float c_pa = 0.0047;

Point F (Point x) {
	Point F;
	F.x[0] =   x.x[2]*b*exp(-c_el*x.x[0]-c_ea*x.x[2]);
	F.x[1] =   x.x[0]*p;
	F.x[2] =   x.x[1]*exp(-c_pa*x.x[2])
	         + x.x[2]*q; 
	return F;
}

/* Decomposition function of F
 */
Point f (Point x, Point y) {
	Point f;
	f.x[0] =   x.x[2]*b*exp(-c_el*y.x[0]-c_ea*y.x[2]);
	f.x[1] =   x.x[0]*p;
	f.x[2] =   x.x[1]*exp(-c_pa*y.x[2])
	         + x.x[2]*q; 
	return f;
}
