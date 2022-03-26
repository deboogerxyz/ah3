#include <math.h>

#include "cvector.h"
#include "mem.h"
#include "sdk.h"
#include "util.h"

typedef struct {
	Vector origin;
	Matrix3x4 matrix[256];
	float simtime;
} Record;

static ConVar *updaterate, *maxupdaterate, *interp, *interpratio,
              *mininterpratio, *maxinterpratio, *maxunlag;
static cvector_vector_type(Record) records[65] = {NULL};

static float
getlerp()
{
	uintptr_t netchan;
	float ratio;

	netchan = sdk_getnetchan();
	if (!netchan)
		return 0.0f;

	/* HACK: Do not compensate for lerp when in local match */
	if (hash(sdk_getserveraddress(netchan)) == hash("loopback"))
		return 0.0f;

	ratio = CLAMP(convar_getfloat(interpratio), convar_getfloat(mininterpratio), convar_getfloat(maxinterpratio));
	return MAX(convar_getfloat(interp), (ratio / (maxupdaterate ? convar_getfloat(maxupdaterate) : convar_getfloat(updaterate))));
}

static int
isvalid(float simtime)
{
	uintptr_t netchan;
	float delta;
	int deadtime;

	netchan = sdk_getnetchan();
	if (!netchan)
		return 0;

	deadtime = (int)(sdk_getservertime(NULL) - convar_getfloat(maxunlag));
	if (simtime < deadtime)
		return 0;

	delta = CLAMP(sdk_getlatency(netchan, 0) + sdk_getlatency(netchan, 1) + getlerp(), 0.0f, convar_getfloat(maxunlag)) - (sdk_getservertime(NULL) - simtime);
	return fabs(delta) <= 0.2f;
}

void
bt_update(FrameStage stage)
{
	int i, j, maxclients;
	uintptr_t localplayer, ent;
	Record record;

	if (stage != FS_RENDER_START)
		return;

	if (!sdk_isingame())
		return;

	localplayer = sdk_getentity(sdk_getlocalplayer());
	if (!localplayer || !ent_isalive(localplayer))
		return;

	maxclients = sdk_getmaxclients();
	for (i = 1; i <= maxclients; i++) {
		if (!(ent = sdk_getentity(i)))
			continue;

		if (!mem->isotherenemy(ent, localplayer))
			continue;

		if (!ent_isalive(ent) || ent_isdormant(ent))
			continue;

		if (!cvector_empty(records[i]) && cvector_end(records[i])->simtime == *ent_simtime(ent))
			continue;

		record.origin  = *ent_getabsorigin(ent);
		ent_setupbones(ent, record.matrix, 256, 256, 0.0f);
		record.simtime = *ent_simtime(ent);

		cvector_push_back(records[i], record);

		while (cvector_size(records[i]) > 3 && cvector_size(records[i]) > sdk_timetoticks(0.2f))
			cvector_erase(records[i], 0);

		for (j = 0; j < cvector_size(records[i]); j++) {
			if (!isvalid(records[i][j].simtime))
				cvector_erase(records[i], j);
		}
	}
}

void
bt_run(UserCmd *cmd)
{
	int i, j, bestidx = 0, bestrecord = 0, maxclients;
	uintptr_t localplayer, ent, besttarget = 0;
	Vector eyepos, aimpunch, viewangles, bonepos, ang;
	float fov, bestfov = 255.0f;
	Record *record;

	if (!(cmd->buttons & IN_ATTACK))
		return;

	localplayer = sdk_getentity(sdk_getlocalplayer());
	if (!localplayer || !ent_isalive(localplayer))
		return;

	eyepos     = ent_geteyepos(localplayer);
	aimpunch   = ent_getaimpunch(localplayer);
	viewangles = vec_add(cmd->viewangles, aimpunch);

	maxclients = sdk_getmaxclients();
	for (i = 1; i <= maxclients; i++) {
		if (!(ent = sdk_getentity(i)))
			continue;

		if (!mem->isotherenemy(ent, localplayer))
			continue;

		if (!ent_isalive(ent) || ent_isdormant(ent))
			continue;

		bonepos = ent_getbonepos(ent, 8);
		ang     = vec_calcang(eyepos, bonepos, viewangles);
		fov     = hypot(ang.x, ang.y);

		if (fov < bestfov) {
			bestfov = fov;
			besttarget = ent;
			bestidx = i;
		}
	}

	if (cvector_empty(records[bestidx]))
		return;

	bestfov = 255.0f;
	for (i = 0; i < cvector_size(records[bestidx]); i++) {
		if (!isvalid(records[bestidx][i].simtime))
			continue;

		int bones[6] = {8, 4, 3, 6, 7, 5}; /* TODO: Use hitboxes instead */
		for (j = 0; j < 6; j++) {
			bonepos = mat_origin(records[bestidx][i].matrix[bones[j]]);
			ang = vec_calcang(eyepos, bonepos, viewangles);
			fov = hypot(ang.x, ang.y);

			if (fov < bestfov) {
				bestfov = fov;
				bestrecord = i;
			}
		}
	}

	if (bestrecord && besttarget) {
		record = &records[bestidx][bestrecord];
		mem->setabsorigin(besttarget, &record->origin);
		cmd->tickcount = sdk_timetoticks(record->simtime + getlerp());
	}
}

void
bt_init(void)
{
	updaterate = cvar_find("cl_updaterate");
	maxupdaterate = cvar_find("sv_maxupdaterate");
	interp = cvar_find("cl_interp");
	interpratio = cvar_find("cl_interp_ratio");
	mininterpratio = cvar_find("sv_client_min_interp_ratio");
	maxinterpratio = cvar_find("sv_client_max_interp_ratio");
	maxunlag = cvar_find("sv_maxunlag");
}

void
bt_clean(void)
{
	int i;

	for (i = 0; i < 65; i++)
		cvector_free(records[i]);
}
