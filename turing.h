#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _TURING_
#define _TURING_

#define ALPHABET_SIZE 3
#define BUFFER_SIZE 10

//this struct defines how the Turing Machine acts in a state
//vectors have dimension equal to alphabet size (here 3)
typedef struct {
	int *next;
	char *out;
	char *move;
} TuringState;

typedef struct {
	int stateNo; //number of states
	int fstateNo; //number of final states
	TuringState *states;
} TuringMachine;

//alocates memory for Turing Machine, states are left empty
TuringMachine * newTuringMachine (int stateNo, int fstateNo);

//reads Turing Machine from input file
TuringMachine * readTuringMachine (FILE *in);

//runs Turing Machine on input w
/*return codes:
	0 - everything ok
	1 - error
*/
int runTuringMachine (char *w, TuringMachine *tm);

//frees memory
//maxState is when you only want to free states up to maxState (occurs if allocation errors ar encountered)
//usual call: freeTuringMachine(..., 0)
void freeTuringMachine (TuringMachine **tm, int maxState);

#endif