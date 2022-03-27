#include <math.h>

#include "cvector.h"
#include "mem.h"
#include "sdk.h"
#include "util.h"

typedef struct {
	Vector origin;
	float simtime;
	Matrix3x4 matrix[256];
} Record;

static ConVar *updaterate, *maxupdaterate, *interp, *interpratio, *mininterpratio, *maxinterpratio, *maxunlag;
static cvector_vector_type(Record) records[65] = {NULL};

static float
getlerp(void)
{
	uintptr_t netchan = sdk_getnetchan();
	if (!netchan)
		return 0.0f;

	float ratio = CLAMP(convar_getfloat(interpratio), convar_getfloat(mininterpratio), convar_getfloat(maxinterpratio));
	return MAX(convar_getfloat(interp), (ratio / (maxupdaterate ? convar_getfloat(maxupdaterate) : convar_getfloat(updaterate))));
}

static int
isvalid(float simtime)
{
	uintptr_t netchan = sdk_getnetchan();
	if (!netchan)
		return 0;

	int deadtime = (int)(sdk_getservertime(NULL) - convar_getfloat(maxunlag));
	if (simtime < deadtime)
		return 0;

	float delta = CLAMP(sdk_getlatency(netchan, 0) + sdk_getlatency(netchan, 1) + getlerp(), 0.0f, convar_getfloat(maxunlag)) - (sdk_getservertime(NULL) - simtime);
	return fabs(delta) <= 0.2f;
}

void
bt_update(FrameStage stage)
{
	if (stage != FS_RENDER_START)
		return;

	if (!sdk_isingame())
		return;

	uintptr_t localplayer = sdk_getentity(sdk_getlocalplayer());
	if (!localplayer || !ent_isalive(localplayer))
		return;

	int maxclients = sdk_getmaxclients();
	for (int i = 1; i <= maxclients; i++) {
		uintptr_t ent = sdk_getentity(i);
		if (!ent)
			continue;

		if (!mem->isotherenemy(ent, localplayer))
			continue;

		if (!ent_isalive(ent) || ent_isdormant(ent))
			continue;

		if (!cvector_empty(records[i]) && cvector_end(records[i])->simtime == *ent_simtime(ent))
			continue;

		Record record = {
			.origin  = *ent_getabsorigin(ent),
			.simtime = *ent_simtime(ent)
		};
		ent_setupbones(ent, record.matrix, 256, 256, 0.0f);

		cvector_push_back(records[i], record);

		while (cvector_size(records[i]) > 3 && cvector_size(records[i]) > sdk_timetoticks(0.2f))
			cvector_erase(records[i], 0);

		for (int j = 0; j < cvector_size(records[i]); j++) {
			if (!isvalid(records[i][j].simtime))
				cvector_erase(records[i], j);
		}
	}
}

void
bt_run(UserCmd *cmd)
{
	float bestfov;
	uintptr_t besttarget = 0;
	int bestidx = 0;

	if (!(cmd->buttons & IN_ATTACK))
		return;

	uintptr_t localplayer = sdk_getentity(sdk_getlocalplayer());
	if (!localplayer || !ent_isalive(localplayer))
		return;

	Vector eyepos     = ent_geteyepos(localplayer);
	Vector aimpunch   = ent_getaimpunch(localplayer);
	Vector viewangles = vec_add(cmd->viewangles, aimpunch);

	bestfov = 255.0f;

	int maxclients = sdk_getmaxclients();
	for (int i = 1; i <= maxclients; i++) {
		uintptr_t ent = sdk_getentity(i);
		if (!ent)
			continue;

		if (!mem->isotherenemy(ent, localplayer))
			continue;

		if (!ent_isalive(ent) || ent_isdormant(ent))
			continue;

		Vector bonepos = ent_getbonepos(ent, 8);
		Vector ang     = vec_calcang(eyepos, bonepos, viewangles);
		float  fov     = hypot(ang.x, ang.y);

		if (fov < bestfov) {
			bestfov    = fov;
			besttarget = ent;
			bestidx    = i;
		}
	}

	if (cvector_empty(records[bestidx]))
		return;

	bestfov = 255.0f;
	int bestrecord = 0;

	for (int i = 0; i < cvector_size(records[bestidx]); i++) {
		Record *record = &records[bestidx][i];

		if (!isvalid(record->simtime))
			continue;

		int bones[6] = {8, 4, 3, 6, 7, 5}; /* TODO: Use hitboxes instead */
		for (int j = 0; j < 6; j++) {
			Vector bonepos = mat_origin(record->matrix[bones[j]]);
			Vector ang     = vec_calcang(eyepos, bonepos, viewangles);
			float  fov     = hypot(ang.x, ang.y);

			if (fov < bestfov) {
				bestfov    = fov;
				bestrecord = i;
			}
		}
	}

	if (bestrecord && besttarget) {
		Record *record = &records[bestidx][bestrecord];

		mem->setabsorigin(besttarget, &record->origin);
		cmd->tickcount = sdk_timetoticks(record->simtime + getlerp());
	}
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
		cvector_free(records[i]);
}
