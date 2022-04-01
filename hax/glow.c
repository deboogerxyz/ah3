#include <stdint.h>

#include "../sdk/engine.h"
#include "../sdk/ent.h"
#include "../sdk/entlist.h"
#include "../mem.h"

#include "glow.h"

void
glow_render(void)
{
	if (!engine_isingame())
		return;

	uintptr_t localplayer = entlist_getentity(engine_getlocalplayer());
	if (!localplayer)
		return;

	/* TODO: Register glow object for non-player entities here */

	for (int i = 0; i < mem->glowobjmgr->objs.size; i++) {
		GlowObjDef *glowobj = (GlowObjDef *)mem->glowobjmgr->objs.memory + i;
		uintptr_t ent = glowobj->ent;
		if (!ent || glowobj->nextfreeslot != -2)
			continue;

		switch (ent_getclientclass(ent)->classid) {
		case CLASSID_CSPLAYER:
			if (!mem->isotherenemy(ent, localplayer))
				break;

			if (!ent_isalive(ent) || ent_isdormant(ent))
				break;

			glowobj->alpha = 0.65f;
			Color white = {1.0f, 1.0f, 1.0f};
			glowobj->color = white;
			glowobj->occluded = 1;
			glowobj->style = 0;
			glowobj->alphamax = 0.75f;

			break;
		case CLASSID_C4: /* FALLTHROUGH */
		case CLASSID_PLANTEDC4:
			glowobj->alpha = 0.85f;
			glowobj->color.r = 1.0f;
			glowobj->color.g = 1.0f;
			glowobj->color.b = 0.25f;
			glowobj->occluded = 1;
			glowobj->style = 0;
			glowobj->alphamax = 0.75f;

			break;
		case CLASSID_BASECSGRENADEPROJECTILE: /* FALLTHROUGH */
		case CLASSID_BREACHCHARGEPROJECTILE:
		case CLASSID_BUMPMINEPROJECTILE:
		case CLASSID_DECOYPROJECTILE:
		case CLASSID_MOLOTOVPROJECTILE:
		case CLASSID_SENSORGRENADEPROJECTILE:
		case CLASSID_SMOKEGRENADEPROJECTILE:
		case CLASSID_SNOWBALLPROJECTILE:
			if (!glowobjmgr_hasglow(ent))
				glowobjmgr_register(ent);

			glowobj->alpha = 1.0f;
			glowobj->color.r = 1.0f;
			glowobj->color.g = 0.0f;
			glowobj->color.b = 0.0f;
			glowobj->occluded = 1;
			glowobj->style = 0;
			glowobj->alphamax = 0.75f;

			break;
		default:
			if (!ent_isweapon(ent))
				break;

			glowobj->alpha = 0.65f;
			glowobj->color.r = 0.25f;
			glowobj->color.g = 0.85f;
			glowobj->color.b = 1.0f;
			glowobj->occluded = 1;
			glowobj->style = 0;
			glowobj->alphamax = 0.75f;

			break;
		}
	}
}
