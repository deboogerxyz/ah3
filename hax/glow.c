#include <cjson/cJSON.h>
#include <stdint.h>

#include "../cfg.h"
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

	for (int i = 0; i < mem->glowobjmgr->objs.size; i++) {
		GlowObjDef *glowobj = (GlowObjDef *)mem->glowobjmgr->objs.memory + i;
		uintptr_t ent = glowobj->ent;
		if (!ent || glowobj->nextfreeslot != -2)
			continue;

		switch (ent_getclientclass(ent)->classid) {
		case CLASSID_CSPLAYER:
			if (ent == localplayer)
				break;

			if (!ent_isalive(ent) || ent_isdormant(ent) || *ent_getimmunity(ent))
				break;

			if (mem->isotherenemy(ent, localplayer)) {
				if (!cfg->glow[0].enabled)
					break;

				if (cfg->glow[0].healthbased) {
					glowobj->color = colora_health(*ent_gethealth(ent));
					glowobj->color.a = cfg->glow[0].color.a;
				} else {
					glowobj->color = cfg->glow[0].color;
				}
			} else {
				if (!cfg->glow[1].enabled)
					break;

				if (cfg->glow[1].healthbased) {
					glowobj->color = colora_health(*ent_gethealth(ent));
					glowobj->color.a = cfg->glow[1].color.a;
				} else {
					glowobj->color = cfg->glow[1].color;
				}
			}

			glowobj->alphamax = 0.75f;
			glowobj->occluded = 1;
			glowobj->style = 0;

			break;
		case CLASSID_C4:
			if (!cfg->glow[2].enabled)
				break;

			glowobj->color = cfg->glow[2].color;
			glowobj->alphamax = 0.75f;
			glowobj->occluded = 1;
			glowobj->style = 0;

			break;
		case CLASSID_PLANTEDC4:
			if (!cfg->glow[3].enabled)
				break;

			glowobj->color = cfg->glow[3].color;
			glowobj->alphamax = 0.75f;
			glowobj->occluded = 1;
			glowobj->style = 0;

			break;
		case CLASSID_BASECSGRENADEPROJECTILE: /* FALLTHROUGH */
		case CLASSID_BREACHCHARGEPROJECTILE:
		case CLASSID_BUMPMINEPROJECTILE:
		case CLASSID_DECOYPROJECTILE:
		case CLASSID_MOLOTOVPROJECTILE:
		case CLASSID_SENSORGRENADEPROJECTILE:
		case CLASSID_SMOKEGRENADEPROJECTILE:
		case CLASSID_SNOWBALLPROJECTILE:
			if (!cfg->glow[4].enabled)
				break;

			if (!glowobjmgr_hasglow(ent))
				glowobjmgr_register(ent);

			glowobj->color = cfg->glow[4].color;
			glowobj->alphamax = 0.75f;
			glowobj->occluded = 1;
			glowobj->style = 0;

			break;
		default:
			if (!cfg->glow[5].enabled)
				break;

			if (!ent_isweapon(ent))
				break;

			glowobj->color = cfg->glow[5].color;
			glowobj->alphamax = 0.75f;
			glowobj->occluded = 1;
			glowobj->style = 0;

			break;
		}
	}
}

void
glow_drawgui(struct nk_context *ctx)
{
	const char *categories[] = {"Enemies", "Teammates", "Dropped C4", "Planted C4", "Projectiles", "Dropped weapons"};

	if (nk_tree_push(ctx, NK_TREE_TAB, "Glow", NK_MINIMIZED)) {
		for (int i = 0; i < LEN(cfg->glow); i++)
			if (nk_tree_push_id(ctx, NK_TREE_NODE, categories[i], NK_MINIMIZED, i)) {
				nk_checkbox_label(ctx, "Enabled", &cfg->glow[i].enabled);
				if (i < 2)
					nk_checkbox_label(ctx, "Health based", &cfg->glow[i].healthbased);
				gui_colorpicker(ctx, (struct nk_colorf *)&cfg->glow[i].color);

				nk_tree_pop(ctx);
			}

		nk_tree_pop(ctx);
	}
}

void
glow_loadcfg(cJSON *json)
{
	int i = 0;
	cJSON* glowjson;

	cJSON_ArrayForEach(glowjson, cJSON_GetObjectItem(json, "Glow")) {
		cJSON* enabled = cJSON_GetObjectItem(glowjson, "Enabled");
		if (cJSON_IsBool(enabled))
			cfg->glow[i].enabled = enabled->valueint;
		if (i < 2) {
			cJSON* healthbased = cJSON_GetObjectItem(glowjson, "Health based");
			if (cJSON_IsBool(healthbased))
				cfg->glow[i].healthbased = healthbased->valueint;
		}

		int j = 0;
		cJSON *colorjson;

		cJSON_ArrayForEach(colorjson, cJSON_GetObjectItem(glowjson, "Color")) {
			if (cJSON_IsNumber(colorjson))
				*(&cfg->glow[i].color.r + j) = (float)colorjson->valuedouble;

			j++;
		}

		i++;
	}
}

void
glow_savecfg(cJSON *json)
{
	cJSON* glowjson = cJSON_AddArrayToObject(json, "Glow");

	for (int i = 0; i < LEN(cfg->glow); i++) {
		cJSON* glow = cJSON_CreateObject();

		cJSON_AddBoolToObject(glow, "Enabled", cfg->glow[i].enabled);
		if (i < 2)
			cJSON_AddBoolToObject(glow, "Health based", cfg->glow[i].healthbased);

		cJSON* colorjson = cJSON_AddArrayToObject(glow, "Color");

		for (int j = 0; j < 4; j++)
			cJSON_AddItemToArray(colorjson, cJSON_CreateNumber(*(&cfg->glow[i].color.r + j)));

		cJSON_AddItemToArray(glowjson, glow);
	}
}
