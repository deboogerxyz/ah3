#include <cjson/cJSON.h>

#include "../cfg.h"
#include "../sdk/engine.h"
#include "../sdk/ent.h"
#include "../sdk/entlist.h"

#include "misc.h"

void
misc_antiafk(UserCmd *cmd)
{
	if (cfg->misc.antiafk && cmd->commandnumber % 2)
		cmd->buttons |= 1 << 27;
}

void
misc_fastduck(UserCmd *cmd)
{
	if (cfg->misc.fastduck)
		cmd->buttons |= IN_BULLRUSH;
}

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
		nk_checkbox_label(ctx, "Anti AFK kick", &cfg->misc.antiafk);
		nk_checkbox_label(ctx, "Bunny hop", &cfg->misc.bhop);
		nk_checkbox_label(ctx, "Fast duck", &cfg->misc.fastduck);
		nk_checkbox_label(ctx, "Radar hack", &cfg->misc.radar);

		nk_tree_pop(ctx);
	}
}

void
misc_loadcfg(cJSON *json)
{
	cJSON* miscjson = cJSON_GetObjectItem(json, "Misc");

	cJSON* antiafk = cJSON_GetObjectItem(miscjson, "Anti AFK kick");
	if (cJSON_IsBool(antiafk))
		cfg->misc.antiafk = antiafk->valueint;
	cJSON* bhop = cJSON_GetObjectItem(miscjson, "Bunny hop");
	if (cJSON_IsBool(bhop))
		cfg->misc.bhop = bhop->valueint;
	cJSON* fastduck = cJSON_GetObjectItem(miscjson, "Fast duck");
	if (cJSON_IsBool(fastduck))
		cfg->misc.fastduck = fastduck->valueint;
	cJSON* radar = cJSON_GetObjectItem(miscjson, "Radar hack");
	if (cJSON_IsBool(radar))
		cfg->misc.radar = radar->valueint;
}

void
misc_savecfg(cJSON *json)
{
	cJSON* miscjson = cJSON_CreateObject();

	cJSON_AddBoolToObject(miscjson, "Anti AFK kick", cfg->misc.antiafk);
	cJSON_AddBoolToObject(miscjson, "Bunny hop", cfg->misc.bhop);
	cJSON_AddBoolToObject(miscjson, "Fast duck", cfg->misc.fastduck);
	cJSON_AddBoolToObject(miscjson, "Radar hack", cfg->misc.radar);

	cJSON_AddItemToObject(json, "Misc", miscjson);
}
