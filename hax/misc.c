#include <cjson/cJSON.h>

#include "../cfg.h"

#include "misc.h"

void
misc_drawgui(struct nk_context *ctx)
{
	if (nk_tree_push(ctx, NK_TREE_TAB, "Misc", NK_MINIMIZED)) {
		nk_checkbox_label(ctx, "Radar hack", &cfg->misc.radar);

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
}

void
misc_savecfg(cJSON *json)
{
	cJSON* miscjson = cJSON_CreateObject();

	cJSON_AddBoolToObject(miscjson, "Radar hack", cfg->misc.radar);

	cJSON_AddItemToObject(json, "Misc", miscjson);
}

