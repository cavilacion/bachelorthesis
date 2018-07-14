/* Author: Erik Voogd
 * Date: 30 June 2018
 *
 * This functions converts the insect population model as a finite 
 * state abstraction to the promela language
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "fsa.h"

void box (int *k, Grid* Q) {
	int n = Q->dims;
	printf ("I_q = [ (");
	for (int i=0; i<n-1; i++) {
		printf ("%1.0f,", Q->xi[i][k[i]]);
	}
	if (n>0) printf ("%1.0f) , (", Q->xi[n-1][k[n-1]]);
	for (int i=0; i<n-1; i++) {
		printf ("%1.0f,", Q->xi[i][k[i]+1]);
	}
	if (n>0) printf ("%1.0f", Q->xi[n-1][k[n-1]+1]);
	printf (") ]");
}

void observe (Point x) {
	printf ("atomic { ");
	printf ("p = %d; ", (x.x[0] <= 10) ? 1:0);
	printf ("q = %d; ", (x.x[2] >= 40) ? 1:0);
	printf ("r = %d; ", (x.x[0] >= 150) ? 1:0);
	printf (" } ");
}

void exportPromela (FSA* fsa, Grid* Q) {
	/* determine initial region */
	Point a,b;
	for (int state = 0; state < fsa->size; state++) {
		boundaryPoints (fsa, Q, state, &a, &b);
		if (a.x[0] <= 100 && 100 <= b.x[0]
		 && a.x[1] <= 100 && 100 <= b.x[1]
		 && a.x[2] <= 100 && 100 <= b.x[2]) {
			 fsa->init[fsa->num_init++]=state;
		}
	}
	
	int i, j;
	printf ("bool p = 0;\nbool q = 0;\nbool r = 0;\n\n");
	printf ("active proctype FSA() {\n");
	printf ("q_init:\n\tif\n");
	for (i=0; i < fsa->num_init; i++) {
		printf ("\t:: (true) -> goto q_%d;\n", fsa->init[i]);
	}
	printf ("\tfi\n");
	for (i=0; i < fsa->size; i++) {
		printf ("q_%d:\t\t/* ", i);
		box (fsa->q[i].iota, Q);
		printf (" */\n\t");
		observe (fsa->q[i].center);
		printf ("\n\tif\n");
		StateSet ss=fsa->delta[i];
		for (j=0; j < ss.size; j++) {
			printf ("\t:: (true) -> ");
			printf ("goto q_%d;\n", ss.transition[j]);
		}
		printf ("\tfi\n");
	}
	printf ("}");
}
