#include "turing.h"

#define MAX_INPUT_SIZE 150
#define INPUT_FILENAME "date.in"
#define OUTPUT_FILENAME "date.out"

#define FILE_ERR 1
#define MEMORY_ERR 2

int main (void) {

	//open input file
	FILE *in = fopen(INPUT_FILENAME, "r");
	if (in == NULL) {
		return FILE_ERR;
	}

	//get memory for input string
	char *w = (char *) malloc(150 * sizeof(char));
	if (w == NULL) {
		fclose(in);
		return MEMORY_ERR;
	}

	//read input
	fgets(w, MAX_INPUT_SIZE, in);

	//read Turing Machine
	TuringMachine *tm = readTuringMachine(in);
	if (tm == NULL) {
		fclose(in);
		free(w);
		return MEMORY_ERR;
	}

	//open output file
	FILE *out = fopen(OUTPUT_FILENAME, "w");
	if (out == NULL) {
		fclose(in);
		free(w);
		freeTuringMachine(&tm, 0);
		return FILE_ERR;
	}

	//try running Turing Machine
	if (runTuringMachine(w, tm) == 0) {
		fprintf(out, "%s", w);
	} else {
		fprintf(out, "Eroare!");
	}

	//clear everything
	freeTuringMachine(&tm, 0);
	free(w);
	fclose(in);
	fclose(out);

	return 0;
}