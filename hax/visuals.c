#include <cjson/cJSON.h>

#include "../cfg.h"
#include "../sdk/cvar.h"
#include "../sdk/engine.h"
#include "../sdk/ent.h"
#include "../sdk/entlist.h"

#include "visuals.h"

void
visuals_forcecrosshair(FrameStage stage)
{
	uintptr_t localplayer = entlist_getentity(engine_getlocalplayer());
	if (!localplayer)
		return;

	ConVar *var = cvar_find("weapon_debug_spread_show");
	convar_setint(var, stage == FS_RENDER_START && cfg->visuals.forcecrosshair && !*ent_getisscoped(localplayer) ? 3 : 0);
}

void
visuals_drawgui(struct nk_context *ctx)
{
	if (nk_tree_push(ctx, NK_TREE_TAB, "Visuals", NK_MINIMIZED)) {
		nk_checkbox_label(ctx, "Force crosshair", &cfg->visuals.forcecrosshair);
		nk_checkbox_label(ctx, "Override FOV", &cfg->visuals.overridefov);
		if (cfg->visuals.overridefov)
			nk_property_float(ctx, "FOV", 0.0f, &cfg->visuals.fov, 180.0f, 0.2f, 0.2f);

		nk_tree_pop(ctx);
	}
}

void
visuals_loadcfg(cJSON *json)
{
	cJSON* visualsjson = cJSON_GetObjectItem(json, "Visuals");

	cJSON* forcecrosshair = cJSON_GetObjectItem(visualsjson, "Force crosshair");
	if (cJSON_IsBool(forcecrosshair))
		cfg->visuals.forcecrosshair = forcecrosshair->valueint;
	cJSON* overridefov = cJSON_GetObjectItem(visualsjson, "Override FOV");
	if (cJSON_IsBool(overridefov))
		cfg->visuals.overridefov = overridefov->valueint;
	cJSON* fov = cJSON_GetObjectItem(visualsjson, "FOV");
	if (cJSON_IsNumber(fov))
		cfg->visuals.fov = (float)fov->valuedouble;
}

void
visuals_savecfg(cJSON *json)
{
	cJSON* visualsjson = cJSON_CreateObject();

	cJSON_AddBoolToObject(visualsjson, "Force crosshair", cfg->visuals.forcecrosshair);
	cJSON_AddBoolToObject(visualsjson, "Override FOV", cfg->visuals.overridefov);
	cJSON_AddNumberToObject(visualsjson, "FOV", cfg->visuals.fov);

	cJSON_AddItemToObject(json, "Visuals", visualsjson);
}
