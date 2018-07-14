/* Author: Erik Voogd
 * Date: 30 June 2018
 *
 * This functions converts the example finite state automaton to
 * the promela language
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
	printf ("a=%d; ", (x.x[0] >= 3 && x.x[0] <= 4 && x.x[1] >= 3 && x.x[1] <= 4)?1:0);
	printf ("b=%d; ", (x.x[0] >= 0 && x.x[0] <= 1 && x.x[1] >= 0 && x.x[1] <= 1)?1:0);
	printf ("d=%d; ", (x.x[0] >= 1 && x.x[0] <= 3 && x.x[1] >= 1 && x.x[1] <= 3)?1:0);
	printf ("e=%d; ", (x.x[0] >= 0 && x.x[0] <= 6 && x.x[1] >= 0 && x.x[1] <= 4)?1:0);
	printf (" } ");
}

void exportPromela (FSA* fsa, Grid* Q) {
	int i, j;
	printf ("bool a = 0;\nbool b = 0;\nbool d = 0;\nbool e = 0;\n\n");
	printf ("active proctype FSA() {\n");
	printf ("q_init:\n\tif\n");
	for (i=0; i < fsa->size; i++) {
		printf ("\t:: (true) -> goto q_%d;\n", i);
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
			printf ("\t:: true -> goto q_%d;\n", ss.transition[j]);
		}
		printf ("\tfi\n");
	}
	printf ("}");
}

