#ifndef MEM_H_
#define MEM_H_

#include "sdk/globalvars.h"
#include "sdk/glowobjmgr.h"

typedef struct {
	uintptr_t pollevent;
	uintptr_t swapwindow;
	void (*debugmsg)(const char *msg, ...);
	int (*isotherenemy)(uintptr_t, uintptr_t);
	uintptr_t *clientmode;
	void (*setclantag)(const char *, const char *);
	char (*linegoesthrusmoke)(Vector, Vector, short);
	GlobalVars *globalvars;
	void (*setabsorigin)(uintptr_t, Vector *);
	uintptr_t insertintotree;
	GlowObjMgr *glowobjmgr;
	uintptr_t demoorhltv;
	uintptr_t money;
	uintptr_t scopedust;
	uintptr_t scopearc;
	void *movedata;
	void *movehelper;
	int *predictrandomseed;
} Mem;

void mem_init(void);
void mem_clean(void);

extern Mem *mem;

#endif /* MEM_H_ */
