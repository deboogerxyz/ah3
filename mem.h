#ifndef MEM_H
#define MEM_H

#include "sdk.h"

typedef struct {
	void (*debugmsg)(const char *msg, ...);
	GlobalVars *gvars;
} Mem;

void mem_init(void);
void mem_clean(void);

extern Mem *mem;

#endif /* MEM_H */
