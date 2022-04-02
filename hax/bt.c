#include <math.h>

#include "../cfg.h"
#include "../mem.h"
#include "../sdk/convar.h"
#include "../sdk/cvar.h"
#include "../sdk/engine.h"
#include "../sdk/entlist.h"
#include "../sdk/ent.h"
#include "../sdk/netchan.h"
#include "../util.h"

#include "bt.h"

static ConVar *updaterate, *maxupdaterate, *interp, *interpratio, *mininterpratio, *maxinterpratio, *maxunlag;
cvector_vector_type(Record) bt_records[65] = {NULL};

static float
getlerp(void)
{
	uintptr_t netchan = engine_getnetchan();
	if (!netchan)
		return 0.0f;

	float ratio = CLAMP(convar_getfloat(interpratio), convar_getfloat(mininterpratio), convar_getfloat(maxinterpratio));
	return MAX(convar_getfloat(interp), (ratio / (maxupdaterate ? convar_getfloat(maxupdaterate) : convar_getfloat(updaterate))));
}

int
bt_isvalid(float simtime)
{
	uintptr_t netchan = engine_getnetchan();
	if (!netchan)
		return 0;

	int deadtime = (int)(globalvars_getservertime(NULL) - convar_getfloat(maxunlag));
	if (simtime < deadtime)
		return 0;

	float delta = CLAMP(netchan_getlatency(netchan, 0) + netchan_getlatency(netchan, 1) + getlerp(), 0.0f, convar_getfloat(maxunlag)) - (globalvars_getservertime(NULL) - simtime);
	return fabs(delta) <= 0.2f;
}

void
bt_update(FrameStage stage)
{
	if (!cfg->bt.enabled)
		return;

	if (stage != FS_RENDER_START)
		return;

	if (!engine_isingame())
		return;

	uintptr_t localplayer = entlist_getentity(engine_getlocalplayer());
	if (!localplayer || !ent_isalive(localplayer))
		return;

	int maxclients = engine_getmaxclients();
	for (int i = 1; i <= maxclients; i++) {
		uintptr_t ent = entlist_getentity(i);
		if (!ent)
			continue;

		if (!mem->isotherenemy(ent, localplayer))
			continue;

		if (!ent_isalive(ent) || ent_isdormant(ent) || *ent_getimmunity(ent))
			continue;

		if (!cvector_empty(bt_records[i]) && cvector_end(bt_records[i])->simtime == *ent_getsimtime(ent))
			continue;

		Record record = {
			.origin  = *ent_getabsorigin(ent),
			.simtime = *ent_getsimtime(ent)
		};
		ent_setupbones(ent, record.matrix, 256, 256, 0.0f);

		cvector_push_back(bt_records[i], record);

		while (cvector_size(bt_records[i]) > 3 && cvector_size(bt_records[i]) > TIMETOTICKS((float)cfg->bt.limit / 1000.0f))
			cvector_erase(bt_records[i], 0);

		for (int j = 0; j < cvector_size(bt_records[i]); j++)
			if (!bt_isvalid(bt_records[i][j].simtime))
				cvector_erase(bt_records[i], j);
	}
}

void
bt_run(UserCmd *cmd)
{
	float bestfov;
	uintptr_t besttarget = 0;
	int bestidx = 0;

	if (!cfg->bt.enabled)
		return;

	if (!(cmd->buttons & IN_ATTACK))
		return;

	uintptr_t localplayer = entlist_getentity(engine_getlocalplayer());
	if (!localplayer || !ent_isalive(localplayer))
		return;

	Vector eyepos     = ent_geteyepos(localplayer);
	Vector aimpunch   = ent_getaimpunch(localplayer);
	Vector viewangles = vec_add(cmd->viewangles, aimpunch);

	bestfov = 255.0f;

	int maxclients = engine_getmaxclients();
	for (int i = 1; i <= maxclients; i++) {
		uintptr_t ent = entlist_getentity(i);
		if (!ent)
			continue;

		if (!mem->isotherenemy(ent, localplayer))
			continue;

		if (!ent_isalive(ent) || ent_isdormant(ent) || *ent_getimmunity(ent))
			continue;

		Vector headpos = ent_getbonepos(ent, 8);
		Vector ang     = vec_calcang(eyepos, headpos, viewangles);
		float  fov     = hypot(ang.x, ang.y);

		if (fov < bestfov) {
			bestfov    = fov;
			besttarget = ent;
			bestidx    = i;
		}
	}

	if (!besttarget)
		return;

	if (cvector_empty(bt_records[bestidx]))
		return;

	bestfov = 255.0f;
	int bestrecord = 0;

	for (int i = 0; i < cvector_size(bt_records[bestidx]); i++) {
		Record *record = &bt_records[bestidx][i];

		if (!bt_isvalid(record->simtime))
			continue;

		Vector headpos = mat_origin(record->matrix[8]);
		Vector ang = vec_calcang(eyepos, headpos, viewangles);
		float  fov = hypot(ang.x, ang.y);

		if (fov < bestfov) {
			bestfov    = fov;
			bestrecord = i;
		}
	}

	if (!bestrecord)
		return;

	Record *record = &bt_records[bestidx][bestrecord];

	mem->setabsorigin(besttarget, &record->origin);
	cmd->tickcount = TIMETOTICKS(record->simtime + getlerp());
}

void
bt_drawgui(struct nk_context *ctx)
{
	if (nk_tree_push(ctx, NK_TREE_TAB, "Backtrack", NK_MINIMIZED)) {
		nk_checkbox_label(ctx, "Enabled", &cfg->bt.enabled);
		nk_property_int(ctx, "Time limit [ms]", 0, &cfg->bt.limit, 200, 1, 1);

		nk_tree_pop(ctx);
	}
}

void
bt_loadcfg(cJSON *json)
{
	cJSON* btjson = cJSON_GetObjectItem(json, "Backtrack");

	cJSON* enabled = cJSON_GetObjectItem(btjson, "Enabled");
	if (cJSON_IsBool(enabled))
		cfg->bt.enabled = enabled->valueint;
	cJSON* limit = cJSON_GetObjectItem(btjson, "Time limit");
	if (cJSON_IsNumber(limit))
		cfg->bt.limit = limit->valueint;
}

void
bt_savecfg(cJSON *json)
{
	cJSON* btjson = cJSON_CreateObject();

	cJSON_AddBoolToObject(btjson, "Enabled", cfg->bt.enabled);
	cJSON_AddNumberToObject(btjson, "Time limit", cfg->bt.limit);

	cJSON_AddItemToObject(json, "Backtrack", btjson);
}

void
bt_init(void)
{
	updaterate     = cvar_find("cl_updaterate");
	maxupdaterate  = cvar_find("sv_maxupdaterate");
	interp         = cvar_find("cl_interp");
	interpratio    = cvar_find("cl_interp_ratio");
	mininterpratio = cvar_find("sv_client_min_interp_ratio");
	maxinterpratio = cvar_find("sv_client_max_interp_ratio");
	maxunlag       = cvar_find("sv_maxunlag");
}

void
bt_clean(void)
{
	int i;

	for (i = 0; i < 65; i++)
		cvector_free(bt_records[i]);
}
