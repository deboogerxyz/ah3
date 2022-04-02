#include <cjson/cJSON.h>

#include "../cfg.h"
#include "../sdk/client.h"
#include "../sdk/cvar.h"
#include "../sdk/engine.h"
#include "../sdk/ent.h"
#include "../sdk/entlist.h"

#include "visuals.h"

void
visuals_disablepostprocessing(FrameStage stage)
{
	ConVar *var = cvar_find("mat_postprocess_enable");
	convar_setint(var, stage == FS_RENDER_START && cfg->visuals.disablepostprocessing ? 0 : 1);
}

void
visuals_disableshadows(void)
{
	ConVar *var = cvar_find("cl_csm_enabled");
	convar_setint(var, !cfg->visuals.disableshadows);
}

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
visuals_grenadeprediction(FrameStage stage)
{
	ConVar *var = cvar_find("cl_grenadepreview");
	convar_setint(var, cfg->visuals.grenadeprediction);
}

void
visuals_remove3dsky(FrameStage stage)
{
	ConVar *var = cvar_find("r_3dsky");
	convar_setint(var, stage == FS_RENDER_START && !cfg->visuals.remove3dsky);
}

void
visuals_revealranks(UserCmd *cmd)
{
	if (cfg->visuals.revealranks && cmd->buttons & IN_SCORE)
		client_dispatchusermsg(50, 0, 0, NULL);
}

void
visuals_drawgui(struct nk_context *ctx)
{
	if (nk_tree_push(ctx, NK_TREE_TAB, "Visuals", NK_MINIMIZED)) {
		nk_checkbox_label(ctx, "Disable post-processing", &cfg->visuals.disablepostprocessing);
		nk_checkbox_label(ctx, "Disable shadows", &cfg->visuals.disableshadows);
		nk_checkbox_label(ctx, "Force crosshair", &cfg->visuals.forcecrosshair);
		nk_checkbox_label(ctx, "Grenade prediction", &cfg->visuals.grenadeprediction);
		nk_checkbox_label(ctx, "Override FOV", &cfg->visuals.overridefov);
		if (cfg->visuals.overridefov)
			nk_property_float(ctx, "FOV", 0.0f, &cfg->visuals.fov, 180.0f, 0.2f, 0.2f);
		nk_checkbox_label(ctx, "Remove 3D sky", &cfg->visuals.remove3dsky);
		nk_checkbox_label(ctx, "Reveal money", &cfg->visuals.revealmoney);
		nk_checkbox_label(ctx, "Reveal radar", &cfg->visuals.revealradar);
		nk_checkbox_label(ctx, "Reveal ranks", &cfg->visuals.revealranks);

		nk_tree_pop(ctx);
	}
}

void
visuals_loadcfg(cJSON *json)
{
	cJSON* visualsjson = cJSON_GetObjectItem(json, "Visuals");

	cJSON* disablepostprocessing = cJSON_GetObjectItem(visualsjson, "Disable post-processing");
	if (cJSON_IsBool(disablepostprocessing))
		cfg->visuals.disablepostprocessing = disablepostprocessing->valueint;
	cJSON* disableshadows = cJSON_GetObjectItem(visualsjson, "Disable shadows");
	if (cJSON_IsBool(disableshadows))
		cfg->visuals.disableshadows = disableshadows->valueint;
	cJSON* forcecrosshair = cJSON_GetObjectItem(visualsjson, "Force crosshair");
	if (cJSON_IsBool(forcecrosshair))
		cfg->visuals.forcecrosshair = forcecrosshair->valueint;
	cJSON* grenadeprediction = cJSON_GetObjectItem(visualsjson, "Grenade prediction");
	if (cJSON_IsBool(grenadeprediction))
		cfg->visuals.grenadeprediction = grenadeprediction->valueint;
	cJSON* overridefov = cJSON_GetObjectItem(visualsjson, "Override FOV");
	if (cJSON_IsBool(overridefov))
		cfg->visuals.overridefov = overridefov->valueint;
	cJSON* fov = cJSON_GetObjectItem(visualsjson, "FOV");
	if (cJSON_IsNumber(fov))
		cfg->visuals.fov = (float)fov->valuedouble;
	cJSON* remove3dsky = cJSON_GetObjectItem(visualsjson, "Remove 3D sky");
	if (cJSON_IsBool(remove3dsky))
		cfg->visuals.remove3dsky = remove3dsky->valueint;
	cJSON* revealmoney = cJSON_GetObjectItem(visualsjson, "Reveal money");
	if (cJSON_IsBool(revealmoney))
		cfg->visuals.revealmoney = revealmoney->valueint;
	cJSON* revealradar = cJSON_GetObjectItem(visualsjson, "Reveal radar");
	if (cJSON_IsBool(revealradar))
		cfg->visuals.revealradar = revealradar->valueint;
	cJSON* revealranks = cJSON_GetObjectItem(visualsjson, "Reveal ranks");
	if (cJSON_IsBool(revealranks))
		cfg->visuals.revealranks = revealranks->valueint;
}

void
visuals_savecfg(cJSON *json)
{
	cJSON* visualsjson = cJSON_CreateObject();

	cJSON_AddBoolToObject(visualsjson, "Disable post-processing", cfg->visuals.disableshadows);
	cJSON_AddBoolToObject(visualsjson, "Disable shadows", cfg->visuals.disableshadows);
	cJSON_AddBoolToObject(visualsjson, "Force crosshair", cfg->visuals.forcecrosshair);
	cJSON_AddBoolToObject(visualsjson, "Grenade prediction", cfg->visuals.grenadeprediction);
	cJSON_AddBoolToObject(visualsjson, "Override FOV", cfg->visuals.overridefov);
	cJSON_AddNumberToObject(visualsjson, "FOV", cfg->visuals.fov);
	cJSON_AddBoolToObject(visualsjson, "Remove 3D sky", cfg->visuals.remove3dsky);
	cJSON_AddBoolToObject(visualsjson, "Reveal money", cfg->visuals.revealmoney);
	cJSON_AddBoolToObject(visualsjson, "Reveal radar", cfg->visuals.revealradar);
	cJSON_AddBoolToObject(visualsjson, "Reveal ranks", cfg->visuals.revealranks);

	cJSON_AddItemToObject(json, "Visuals", visualsjson);
}
