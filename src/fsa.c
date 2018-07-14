/* File: fsa.c
 * Author: Erik Voogd
 * Date: 14 July 2018 
 * 
 * Contains functions to construct a finite state automaton given a 
 * gridded partition. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fsa.h"

extern int stut, cand;

Grid* newEmptyGrid (int dims) {
	Grid *Q = malloc (sizeof(Grid));
	Q->dims = dims;
	Q->N = malloc (dims*sizeof(int));
	Q->xi = malloc (dims*sizeof(int*));
	return Q;
}

void scanGrid (Grid* Q) {
	int n;
	float xi;
	for (int i = 0; i < Q->dims; i++) {
		// scan interval partition
		scanf ("%d", &n);
		Q->N[i] = n;
		Q->xi[i] = malloc ( (n+1)*sizeof(float));
		for (int j=0; j<=n; j++) {
			scanf("%f", &xi);
			Q->xi[i][j] = xi;
		}
	}
	return;
}

void cleanGrid (Grid *Q) {
	if (!Q) return;
	if (Q->xi) {
		for (int i = 0; i < Q->dims; i++) {
			if (Q->xi[i]) free (Q->xi[i]);
		}
		free (Q->xi);
	}
	if (Q->N) free (Q->N);
	free (Q);
	return;
}

void fillStatesWithIotas (FSA* fsa, Grid *Q, int *seq, int state) {
	int dims = Q->dims;
	
	/* copy sequence of iota's to the state in fsa */
	for (int i=0; i < dims; i++) {
		fsa->q[state].iota[i] = seq[i];
	}
	/* increment sequence  */
	for (int i=0; i < dims; i++) {
		if (seq[i]+1 < Q->N[i]) { // try incrementing iota for dimension i
			seq[i]++;
			for (int j = i-1; j >= 0; j--) { // reset all previous dimensions
				seq[j] = 0;
			}
			fillStatesWithIotas (fsa, Q, seq, state+1);
		}
	}
	/* for all dimensions: seq[i] = N_i - 1  --> base case */
	return;
}

/* Turns a sequence of dimension indexes for q into its actual state 
 * index
 */
int stateIdx (Grid *Q, int seq[MAX_DIMENSIONS]) {
	int n = Q->dims;
	int idx = 0;
	int factor = 1;
	for (int i=0; i<n; i++) {
		idx += factor * seq[i];
		factor *= Q->N[i];
	}
	return idx;
}

/* Store the boundary points of a state in x1 and x2
 */
void boundaryPoints (FSA* fsa, Grid* Q, int state, Point *x1, Point *x2) {
	int n = Q->dims;
	int i;
	int iota;
	for (i=0; i<n; i++) {
		iota = fsa->q[state].iota[i];
		x1->x[i] = Q->xi[i][iota];
		x2->x[i] = Q->xi[i][iota+1];
	}
}
		 
/* Initialize the fsa with a number of states based on the partition
 */
FSA* initFSA (Grid* Q) {
	FSA* fsa = malloc (sizeof(FSA));
	int *seq = malloc (Q->dims*sizeof(int));
	fsa->num_init = 0;
	
	/* calculate total number of boxes in Q */
	fsa->size=1;
	for (int i=0; i < Q->dims; i++) {
		fsa->size *= Q->N[i];
		seq[i] = 0;
	}
	fsa->q = malloc (fsa->size*sizeof(State));
	for (int i=0; i < fsa->size; i++) {
		fsa->q[i].iota = malloc (Q->dims*sizeof(float));
	}
	
	/* assign iota's to every box, starting with all zeroes */
	fillStatesWithIotas (fsa, Q, seq, 0);
	free (seq);
	
	/* calculate representative central point for each state */
	Point x1, x2;
	for (int state=0; state<fsa->size; state++) {
		boundaryPoints (fsa, Q, state, &x1, &x2);
		for (int i=0; i<Q->dims; i++) {
			fsa->q[state].center.x[i] = (x1.x[i]+x2.x[i])/2.0;
		}
	}
		
	/* initialize the transition map */
	fsa->delta = malloc (fsa->size*sizeof(StateSet));
	for (int i=0; i < fsa->size; i++) {
		fsa->delta[i].size = 0;
	}
	return fsa;
}

void cleanFSA (FSA* fsa) {
	for (int i=0; i < fsa->size; i++) {
		free (fsa->q[i].iota);
		free (fsa->delta[i].transition);
	}
	free (fsa->q);
	free (fsa->delta);
	free (fsa);
	return;
}

/* This function constructs the one-step reachable set of states.
 */
void computeSuccessors (FSA* fsa, Grid* Q, int state, 
													Point y1, Point y2) {
	/* initialization */
	int n = Q->dims;								// state space dimension
	int iota_low[MAX_DIMENSIONS];   // lowest index for each dimension
	int iota_high[MAX_DIMENSIONS];  // highest index for each dimension
	int seq[MAX_DIMENSIONS];				// sequence used for each combination of iota's
	int statesetsize = 1;						// assume image is non-empty
	
	/* for each dimension, compute the highest and lowest index for the 
	 * interval partition of this dimension, store it in iota */
	for (int i=0; i < n; i++) { 
		float minY = min(y1.x[i],y2.x[i]);
		float maxY = max(y1.x[i],y2.x[i]);
		
		iota_low[i] = 0;
		while (iota_low[i] < Q->N[i]-1 && Q->xi[i][iota_low[i]+1] <= minY) 
			iota_low[i]++;
		
		iota_high[i] = Q->N[i];
		do 
			iota_high[i]--; 
		while (iota_high[i] > 0 && Q->xi[i][iota_high[i]] >= maxY );
		
		/* multiply state set size by the number of overlapping intervals */
		statesetsize *= (iota_high[i] - iota_low[i] + 1);
		
		seq[i] = iota_low[i];
	}	
				
	/* create the one step transition state set */
	fsa->delta[state].size = statesetsize;
	fsa->delta[state].transition  = malloc (statesetsize * sizeof (int));
	
	for (int i=0; i < statesetsize; i++) {
		int tran = stateIdx (Q, seq);
		
		if (state == tran) {
			/* self-loop detected */
			int isStuttering = stuttering (fsa,Q,state);
			if (isStuttering) {
				/* self-loop is stuttering, remove it */
				stut++;
				fsa->delta[state].size--;
				statesetsize--;
				i--;
			}
			else {
				fsa->delta[state].transition[i] = tran;
			}
			cand++;
		} 
		
		else {
			fsa->delta[state].transition[i] = tran;
		}
		/* increment sequence  */
		for (int j=0; j < n; j++) {
			if (seq[j] < iota_high[j]) { // try incrementing iota for dimension i
				seq[j]++;
				for (int k = j-1; k >= 0; k--) { // reset all previous dimensions
					seq[k] = iota_low[k];
				}
				j=n; // break loop
			}
		}
	}
	return;
}


