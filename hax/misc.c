#include <cjson/cJSON.h>

#include "../cfg.h"
#include "../sdk/engine.h"
#include "../sdk/ent.h"
#include "../sdk/entlist.h"

#include "misc.h"

void
misc_bhop(UserCmd *cmd)
{
	static int wasonground = 0;

	if (!cfg->misc.bhop)
		return;

	uintptr_t localplayer = entlist_getentity(engine_getlocalplayer());
	if (!localplayer)
		return;

	if (!(*ent_getflags(localplayer) & 1) && !wasonground)
		cmd->buttons &= ~IN_JUMP;

	wasonground = *ent_getflags(localplayer) & 1;
}

void
misc_drawgui(struct nk_context *ctx)
{
	if (nk_tree_push(ctx, NK_TREE_TAB, "Misc", NK_MINIMIZED)) {
		nk_checkbox_label(ctx, "Radar hack", &cfg->misc.radar);
		nk_checkbox_label(ctx, "Bunny hop", &cfg->misc.bhop);

		nk_tree_pop(ctx);
	}
}

void
misc_loadcfg(cJSON *json)
{
	cJSON* miscjson = cJSON_GetObjectItem(json, "Misc");

	cJSON* radar = cJSON_GetObjectItem(miscjson, "Radar hack");
	if (cJSON_IsBool(radar))
		cfg->misc.radar = radar->valueint;
	cJSON* bhop = cJSON_GetObjectItem(miscjson, "Bunny hop");
	if (cJSON_IsBool(bhop))
		cfg->misc.bhop = bhop->valueint;
}

void
misc_savecfg(cJSON *json)
{
	cJSON* miscjson = cJSON_CreateObject();

	cJSON_AddBoolToObject(miscjson, "Radar hack", cfg->misc.radar);
	cJSON_AddBoolToObject(miscjson, "Bunny hop", cfg->misc.bhop);

	cJSON_AddItemToObject(json, "Misc", miscjson);
}

