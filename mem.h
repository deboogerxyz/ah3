#ifndef MEM_H
#define MEM_H

#include "sdk.h"

typedef struct {
	uintptr_t pollevent;
	uintptr_t swapwindow;
	void (*debugmsg)(const char *msg, ...);
	int (*isotherenemy)(uintptr_t, uintptr_t);
	uintptr_t *clientmode;
	GlobalVars *gvars;
	void (*setabsorigin)(uintptr_t, Vector *);
} Mem;

void mem_init(void);
void mem_clean(void);

extern Mem *mem;

#endif /* MEM_H */
