#ifndef MEM_H_
#define MEM_H_

#include "sdk/globalvars.h"

typedef struct {
	uintptr_t pollevent;
	uintptr_t swapwindow;
	void (*debugmsg)(const char *msg, ...);
	int (*isotherenemy)(uintptr_t, uintptr_t);
	uintptr_t *clientmode;
	GlobalVars *globalvars;
	void (*setabsorigin)(uintptr_t, Vector *);
	uintptr_t demoorhltv;
	uintptr_t money;
} Mem;

void mem_init(void);
void mem_clean(void);

extern Mem *mem;

#endif /* MEM_H_ */
