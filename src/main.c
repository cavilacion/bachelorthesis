/* Author: Erik Voogd
 * Date: 20 April 2018
 *
 * This program translates a discrete-time dynamical system
 * into a finite state abstraction. The system is of the form
 * 
 * x[t+1] = F (x[t])
 * 
 * where x is a state with n (Q->dims) variables. 
 * 
 * The non-deterministic finite state abstraction that is computed 
 * consists of the partition set Q along with a transition function 
 * delta that only depends on the current state.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "fsa.h" 

int cand;
int stut;

#define DEBUG 0

void printBox (Grid* Q, Point x1, Point x2) {
	int n = Q->dims;
	int i;
	printf ("(");
	for (i=0; i<n-1; i++) printf ("%.3f,", x1.x[i]);
	printf ("%.3f) - (", x1.x[i]);
	for (i=0; i<n-1; i++) printf ("%.3f,", x2.x[i]);
	printf ("%.3f)", x2.x[i]);
}

/* Function computes image of each part of partition and creates the 
 * finite state abstraction
 */
void makeFSA (FSA *fsa, Grid *Q) {
	for (int state=0; state < fsa->size; state++) { 
		
		/* Extract  a_q  and  b_q  from fsa and Q */
		Point a, b;
	  boundaryPoints (fsa, Q, state, &a, &b);
	  
		/* Compute image box of decomposition function */					
		Point y1 = f (a, b);
		Point y2 = f (b, a);
		computeSuccessors (fsa, Q, state, y1, y2);
	}
	return;
}

int nextState (FSA* fsa, Grid* Q, int state) {
	StateSet delta = fsa->delta[state];
	int t = rand()%delta.size;
	return delta.transition[t];
}

int main (int argc, char *argv[]) {
	srand(time(NULL));
	cand=0;
	stut=0;
	int dims;
	scanf ("%d", &dims);
	
	if (dims > MAX_DIMENSIONS) {
		printf ("Input error: the state space cannot have more than %d dimensions.\n", MAX_DIMENSIONS);
		return 0;
	}
	
	/* Create grid */
	Grid* Q = newEmptyGrid (dims);
	scanGrid (Q);
	
	/* Create finite state abstraction */
	FSA* fsa = initFSA (Q);
	makeFSA (fsa, Q);
	
	printf ("/* Found %d candidate stutter states\n", cand);
	printf (" * and removed %d actual spurious self-loops.\n */\n", stut);
	
	exportPromela (fsa, Q);
	
	cleanFSA (fsa);
  cleanGrid (Q);
  
  return 0;
}
