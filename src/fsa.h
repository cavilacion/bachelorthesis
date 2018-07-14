#ifndef _FSA_H
#define _FSA_H

#define min(a,b) (a)<(b)?(a):(b)

#define max(a,b) (a)>(b)?(a):(b)

#define MAX_DIMENSIONS 32

#define MAX_ITER 500

/* A point in the state domain of the real dynamical system
 */
typedef struct Point {
	float x[MAX_DIMENSIONS];
} Point;

/* Structure for the gridded partition of the real state domain
 */
typedef struct Grid { 
  int dims;  // number of dimensions: should be 2 for now
  int *N;
  float **xi;
} Grid;

/* A state for the finite state abstraction is identified by an array 
 * of iotas, which are indices for the xi's in the grid.
 * Size of array is equal to the number of dimensions (2 for now)
 */
typedef struct State {
	int *iota;
	Point center; // representative real center point of state
} State;

/* For the transition function delta, we use a dynamic list of pointers
 * to states
 */
typedef struct StateSet {
	int size;
	int* transition;
} StateSet;

/* An FSA contains a collection of states and a transition map delta,
 * represented here as a list of statesets
 */
typedef struct FSA {
	int size;
	int num_init;
	int init[100];
	State* q;
	StateSet* delta;
} FSA;

Grid* newEmptyGrid (int dims);
void scanGrid (Grid* Q);
void cleanGrid (Grid *Q);
int stateIdx (Grid *Q, int seq[MAX_DIMENSIONS]);
void boundaryPoints (FSA* fsa, Grid* Q, int state, Point *x1, Point *x2);

FSA* initFSA (Grid* Q);
void makeFSA (FSA *fsa, Grid *Q);
void computeSuccessors (FSA* fsa, Grid* Q, int state, 
													Point y1, Point y2);
void cleanFSA (FSA* fsa);

void printBox (Grid* Q, Point x1, Point x2) ;

/* dynamical system functions */
Point F (Point x);
Point f (Point x, Point y);

/* stuttering algorithm */
size_t stuttering (FSA* fsa, Grid* Q, int state);

/* promela */
void exportPromela (FSA* fsa, Grid* Q);

#endif
