#include <cjson/cJSON.h>
#include <stddef.h>

#include "../cfg.h"
#include "../sdk/engine.h"
#include "../sdk/ent.h"
#include "../sdk/entlist.h"
#include "../hk.h"
#include "../sdk/material.h"
#include "../sdk/matsystem.h"
#include "../mem.h"
#include "../sdk/studiorender.h"

#include "chams.h"

static uintptr_t normal;

static void
initmaterials(void)
{
	normal = matsystem_findmaterial("debug/debugambientcube", NULL, 1, NULL);
}

int
chams_render(void *ctx, void *state, ModelRenderInfo *info, Matrix3x4 *custombonetoworld)
{
	static int once = 0;
	if (!once) {
		initmaterials();
		once = 1;
	}

	if (!info->renderable)
		return 0;

	uintptr_t localplayer = entlist_getentity(engine_getlocalplayer());
	if (!localplayer)
		return 0;

	uintptr_t ent = entlist_getentity(info->entindex);
	if (!ent || !ent_isplayer(ent) || !ent_isalive(ent) || ent_isdormant(ent))
		return 0;

	ChamsCfg config = cfg->chams[!mem->isotherenemy(ent, localplayer)];

	if (!config.visible.enabled && !config.occluded.enabled)
		return 0;

	uintptr_t mat = normal;

	if (config.occluded.enabled) {
		if (!config.visible.enabled)
			hk_orig_drawmodelexecute(ctx, state, info, custombonetoworld);

		material_setvarflag(mat, MAT_IGNOREZ, 1);

		if (config.occluded.healthbased)
			material_colormodulate(mat, color_health(*ent_gethealth(ent)));
		else
			material_colormodulate(mat, color_fromcolora(config.occluded.color));
		material_alphamodulate(mat, config.occluded.color.a);

		studiorender_forcedmatoverride(mat, OVERRIDETYPE_NORMAL, -1);
		hk_orig_drawmodelexecute(ctx, state, info, custombonetoworld);
		studiorender_forcedmatoverride(0, OVERRIDETYPE_NORMAL, -1);
	}

	if (config.visible.enabled) {
		material_setvarflag(mat, MAT_IGNOREZ, 0);

		if (config.visible.healthbased)
			material_colormodulate(mat, color_health(*ent_gethealth(ent)));
		else
			material_colormodulate(mat, color_fromcolora(config.visible.color));
		material_alphamodulate(mat, config.visible.color.a);

		studiorender_forcedmatoverride(mat, OVERRIDETYPE_NORMAL, -1);
		hk_orig_drawmodelexecute(ctx, state, info, custombonetoworld);
	}

	return 1;
}

void
chams_drawgui(struct nk_context *ctx)
{
	const char *categories[] = {"Enemies", "Teammates"};

	if (nk_tree_push(ctx, NK_TREE_TAB, "Chams", NK_MINIMIZED)) {
		for (int i = 0; i < LEN(cfg->chams); i++)
			if (nk_tree_push_id(ctx, NK_TREE_NODE, categories[i], NK_MINIMIZED, i)) {
				if (nk_tree_push_id(ctx, NK_TREE_NODE, "Visible", NK_MINIMIZED, i * 2)) {
					nk_checkbox_label(ctx, "Enabled", &cfg->chams[i].visible.enabled);
					if (i < 2)
						nk_checkbox_label(ctx, "Health based", &cfg->chams[i].visible.healthbased);
					gui_colorpicker(ctx, (struct nk_colorf *)&cfg->chams[i].visible.color);

					nk_tree_pop(ctx);
				}

				if (nk_tree_push_id(ctx, NK_TREE_NODE, "Occluded", NK_MINIMIZED, i * 2 + 1)) {
					nk_checkbox_label(ctx, "Enabled", &cfg->chams[i].occluded.enabled);
					if (i < 2)
						nk_checkbox_label(ctx, "Health based", &cfg->chams[i].occluded.healthbased);
					gui_colorpicker(ctx, (struct nk_colorf *)&cfg->chams[i].occluded.color);

					nk_tree_pop(ctx);
				}

				nk_tree_pop(ctx);
			}

		nk_tree_pop(ctx);
	}
}

void
chams_loadcfg(cJSON *json)
{
	int i = 0;
	cJSON *chamsjson;

	cJSON_ArrayForEach(chamsjson, cJSON_GetObjectItem(json, "Chams")) {
		cJSON *visible = cJSON_GetObjectItem(chamsjson, "Visible");

		{
			cJSON* enabled = cJSON_GetObjectItem(visible, "Enabled");
			if (cJSON_IsBool(enabled))
				cfg->chams[i].visible.enabled = enabled->valueint;
			if (i < 2) {
				cJSON *healthbased = cJSON_GetObjectItem(visible, "Health based");
				if (cJSON_IsBool(healthbased))
					cfg->chams[i].visible.healthbased = healthbased->valueint;
			}

			int j = 0;
			cJSON *colorjson;

			cJSON_ArrayForEach(colorjson, cJSON_GetObjectItem(visible, "Color")) {
				if (cJSON_IsNumber(colorjson))
					*(&cfg->chams[i].visible.color.r + j) = (float)colorjson->valuedouble;

				j++;
			}
		}

		cJSON *occluded = cJSON_GetObjectItem(chamsjson, "Occluded");

		{
			cJSON* enabled = cJSON_GetObjectItem(occluded, "Enabled");
			if (cJSON_IsBool(enabled))
				cfg->chams[i].occluded.enabled = enabled->valueint;
			if (i < 2) {
				cJSON *healthbased = cJSON_GetObjectItem(occluded, "Health based");
				if (cJSON_IsBool(healthbased))
					cfg->chams[i].occluded.healthbased = healthbased->valueint;
			}

			int j = 0;
			cJSON *colorjson;

			cJSON_ArrayForEach(colorjson, cJSON_GetObjectItem(occluded, "Color")) {
				if (cJSON_IsNumber(colorjson))
					*(&cfg->chams[i].occluded.color.r + j) = (float)colorjson->valuedouble;

				j++;
			}
		}

		i++;
	}
}

void
chams_savecfg(cJSON *json)
{
	cJSON *chamsjson = cJSON_AddArrayToObject(json, "Chams");

	for (int i = 0; i < LEN(cfg->chams); i++) {
		cJSON* chams = cJSON_CreateObject();

		cJSON *visible = cJSON_CreateObject();

		{
			cJSON_AddBoolToObject(visible, "Enabled", cfg->chams[i].visible.enabled);
			if (i < 2)
				cJSON_AddBoolToObject(visible, "Health based", cfg->chams[i].visible.healthbased);

			cJSON* colorjson = cJSON_AddArrayToObject(visible, "Color");

			for (int k = 0; k < 4; k++)
				cJSON_AddItemToArray(colorjson, cJSON_CreateNumber(*(&cfg->chams[i].visible.color.r + k)));

			cJSON_AddItemToObject(chams, "Visible", visible);
		}

		cJSON *occluded = cJSON_CreateObject();

		{
			cJSON_AddBoolToObject(occluded, "Enabled", cfg->chams[i].occluded.enabled);
			if (i < 2)
				cJSON_AddBoolToObject(occluded, "Health based", cfg->chams[i].occluded.healthbased);

			cJSON* colorjson = cJSON_AddArrayToObject(occluded, "Color");

			for (int k = 0; k < 4; k++)
				cJSON_AddItemToArray(colorjson, cJSON_CreateNumber(*(&cfg->chams[i].occluded.color.r + k)));

			cJSON_AddItemToObject(chams, "Occluded", occluded);
		}

		cJSON_AddItemToArray(chamsjson, chams);
	}
}
