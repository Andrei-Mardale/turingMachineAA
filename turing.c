#include "turing.h"

//alocates memory for turing machine, states are left empty
TuringMachine * newTuringMachine (int stateNo, int fstateNo) {
	TuringMachine *tm = (TuringMachine *) calloc(1, sizeof(TuringMachine));
	
	if (tm == NULL) return NULL;

	tm->stateNo = stateNo;
	tm->fstateNo = fstateNo;

	return tm;
}

//allocate memory for TuringState fields
/*returns:
	0 - everything ok
	1 - could not allocate memory
*/
static int allocTuringState (TuringState *st) {
	st->next = (int *) malloc(ALPHABET_SIZE * sizeof(int));

	if (st->next == NULL) return 1;

	st->out = (char *) malloc(ALPHABET_SIZE * sizeof(char));

	if (st->out == NULL) {
		free(st->next);
		return 1;
	}

	st->move = (char *) malloc(ALPHABET_SIZE * sizeof(char));

	if (st->move == NULL) {
		free(st->next);
		free(st->out);
		return 1;
	}

	return 0;
}

//maps characters to int values => easier referencing
static void translate (char *c) {
	switch (*c) {
			case '0': *c = 0; break;
			case '1': *c = 1; break;
			case '#': *c = 2; break;
		}
}

TuringMachine * readTuringMachine (FILE *in) {
	int stateNo, fstateNo;
	fscanf(in, "%d %d", &stateNo, &fstateNo);

	TuringMachine *tm = newTuringMachine(stateNo, fstateNo);

	if (tm == NULL) return NULL;

	tm->states = (TuringState *) calloc(stateNo - fstateNo, sizeof(TuringState));

	if (tm->states == NULL) {
		free(tm);
		return NULL;
	}

	stateNo -= fstateNo; //faster for instruction

	char buffer[BUFFER_SIZE]; //input buffer

	//read all state transitions
	//if memory allocation fault occurs at any point, free everything and exit
	for (int i = 0; i < stateNo; ++i) {
		//try to allocate
		//if ok(0) then read all 3 lines (whole state)
		//else free everything and exit
		if (allocTuringState(&tm->states[i]) == 0) {
			for (int j = 0; j < ALPHABET_SIZE; ++j) {
				fscanf(in, "%d", &tm->states[i].next[j]);

				fgets(buffer, BUFFER_SIZE, in);

				//if state is not defined then jump to next line
				if (tm->states[i].next[j] == -1) continue;

				tm->states[i].out[j] = buffer[1];
				translate(&tm->states[i].out[j]);

				tm->states[i].move[j] = buffer[3];
			}
		} else {
			freeTuringMachine(&tm, i);
			return NULL;
		}
	}

	return tm;
}

/*applies the following transform:
	'0' -> 0
	'1' -> 1
	'#' -> 2
thus making it easier for the Turing Machine to run
*/
static int encode (char *w) {
	int i = 0;
	while (*w != '\n' && *w != '\0') {
		translate(w);
		++i;
		++w;
	}

	return i;
}

//inverse of decode, prepares it for writing to output
static void decode (char *w, int len) {
	for (int i = 0; i < len; ++i) {
		switch (w[i]) {
			case 0: w[i] = '0'; break;
			case 1: w[i] = '1'; break;
			case 2: w[i] = '#'; break;
		}
	}
}

//runs Turing Machine on input w
/*return codes:
	0 - everything ok
	1 - error
*/
int runTuringMachine (char *w, TuringMachine *tm) {
	int currentState = 0;
	int currentPos = 1; //cursor position; will also be used to check for errors
	int maxPos = encode(w);
	int maxValue = tm->stateNo - tm->fstateNo;

	while (currentState < maxValue && currentPos >= 0 && currentPos < maxPos) {
		//buffer
		int c = w[currentPos];

		//check if transition is defined
		if (tm->states[currentState].next[c] == -1) {
			currentPos = maxPos;
			break;
		}

		//write output
		w[currentPos] = tm->states[currentState].out[c]; 

		//move
		switch(tm->states[currentState].move[c]) {
			case 'L': --currentPos; break;
			case 'R': ++currentPos; break;
		}

		//go to next state
		currentState = tm->states[currentState].next[c];
	}

	decode(w, maxPos);

	if (currentPos == maxPos) return 1;

	return 0;
}

//frees memory
//maxState is when you only want to free states up to maxState (occurs if allocation errors ar encountered)
//usual call: freeTuringMachine(..., 0)
void freeTuringMachine (TuringMachine **tm, int maxState) {
	maxState = (maxState == 0) ? (*tm)->stateNo - (*tm)->fstateNo : maxState;

	for (int i = 0; i < maxState; ++i) {
		free((*tm)->states[i].next);
		free((*tm)->states[i].out);
		free((*tm)->states[i].move);
	}

	free((*tm)->states);
	free(*tm);

	*tm = NULL;
}