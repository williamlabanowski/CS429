#ifndef PDP_H
#define PDP_H

#include <stdio.h>
#include <stdint.h>

typedef uint16_t pdpword;

#define PDP_MEM_SIZE		4096
#define PDP_OBJG_HDR_SIZE	4
#define PDP_OBJG_HDR		"OBJG"

pdpword deflat(FILE* f);

#endif
