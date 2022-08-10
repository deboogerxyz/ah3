#include "../mem.h"

#include "glowobjmgr.h"

int
glowobjmgr_hasglow(uintptr_t ent)
{
	for (int i = 0; i < mem->glowobjmgr->objs.size; i++) {
		GlowObjDef *glowobj = (GlowObjDef *)mem->glowobjmgr->objs.memory + i;

		if (glowobj->nextfreeslot == -2 && glowobj->ent == ent)
			return 1;
	}

	return 0;
}

int
glowobjmgr_register(uintptr_t ent)
{
	int i = mem->glowobjmgr->firstfreeslot;
	if (i == -1)
		return i;

	GlowObjDef *glowobj = (GlowObjDef *)mem->glowobjmgr->objs.memory + i;

	mem->glowobjmgr->firstfreeslot = glowobj->nextfreeslot;

	glowobj->ent = ent;
	glowobj->fullbloom = 0;
	glowobj->fullbloomstencil = 0;
	glowobj->splitscreenslot = -1;
	glowobj->nextfreeslot = -2;

	return i;
}

void
glowobjmgr_unregister(uintptr_t ent, int i)
{
	GlowObjDef *glowobj = (GlowObjDef *)mem->glowobjmgr->objs.memory + i;

	glowobj->nextfreeslot = mem->glowobjmgr->firstfreeslot;
	glowobj->ent = ent;
	glowobj->occluded = 0;
	glowobj->unoccluded = 0;

	mem->glowobjmgr->firstfreeslot = i;
}
