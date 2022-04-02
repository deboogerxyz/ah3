#ifndef GLOWOBJMGR_H_
#define GLOWOBJMGR_H_

#include "color.h"
#include "../util.h"
#include "utlvec.h"

typedef struct {
	int nextfreeslot;
	uintptr_t ent;
	ColorA color;
	PAD(5);
	float alphamax;
	PAD(4);
	char occluded;
	char unoccluded;
	char fullbloom;
	int fullbloomstencil;
	int style;
	int splitscreenslot;
} GlowObjDef;

typedef struct {
	UtlVector objs;
	int firstfreeslot;
} GlowObjMgr;

int glowobjmgr_hasglow(uintptr_t ent);
int glowobjmgr_register(uintptr_t ent);
void glowobjmgr_unregister(uintptr_t ent, int i);

#endif /* GLOWOBJMGR_H_ */
