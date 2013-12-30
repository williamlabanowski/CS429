#include <arpa/inet.h>
#include "pdp.h"

pdpword deflat(FILE* f)
{
	pdpword ondisk;

	fread(&ondisk, sizeof(ondisk), 1, f);
	return ntohs(ondisk);
}
