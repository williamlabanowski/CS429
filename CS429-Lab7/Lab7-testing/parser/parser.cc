#include <stdio.h>
#include <stdint.h>
#include <malloc.h>
#include "pdpstate.h"
#include "pdpaction.h"

void verify(FILE* reff, FILE* gradef)
{
	pdpstate pdp;
	pdpaction refact, gradeact;
	long linen = 1;
	char *linebuf = (char*)malloc(4096);
	char *gradebuf = (char*)malloc(4096);
#if VERBOSE
	fprintf(stderr, "Initial Value:%lu/%lu\n", pdp.matches(), pdp.samples());
#endif
	while (!feof(reff)) {
#if VERBOSE
		fprintf(stderr, "Parsing line %ld\n", linen);
#endif
		size_t n;
		if (linebuf) {
			linebuf[0] = '\0';
		}
		getline(&linebuf, &n, reff);
		refact.parse(linebuf);

		if (gradebuf) {
			gradebuf[0] = '\0';
		}
		getline(&gradebuf, &n, gradef);
		gradeact.parse(gradebuf);

		//fprintf(stderr, "Matching line %ld fpos: %ld\n", ++linen, ftell(reff));
		pdp.compare(refact, gradeact);
		linen++;
	}
	free(linebuf);
	free(gradebuf);
	printf("Grade:%lu/%lu\n", pdp.matches(), pdp.samples());
}

int main(int argc, const char* argv[])
{
	if (argc != 3) {
		fprintf(stderr, "Usage %s <reference journal> <grading journal>\n", argv[0]);
		return 1;
	}
	//FILE *objf = fopen(argv[1], "r");
	FILE *reff = fopen(argv[1], "r");
	FILE *gradef = fopen(argv[2], "r");
	if (!reff || !gradef) {
		fprintf(stderr, "Failed to open %s or %s\n", argv[1], argv[2]);
		return 1;
	}
	verify(reff, gradef);
	fclose(reff);
	fclose(gradef);
	return 0;
}
