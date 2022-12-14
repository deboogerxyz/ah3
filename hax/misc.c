#include <cjson/cJSON.h>

#include "../cfg.h"
#include "../sdk/cvar.h"
#include "../sdk/convar.h"
#include "../sdk/engine.h"
#include "../sdk/ent.h"
#include "../sdk/entlist.h"
#include "../mem.h"
#include "../sdk/netchan.h"

#include "misc.h"

void
misc_antiafk(UserCmd *cmd)
{
	if (cfg->misc.antiafk && cmd->commandnumber % 2)
		cmd->buttons |= 1 << 27;
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

	if (!(*ent_getflags(localplayer) & 1) && *ent_getmovetype(localplayer) != MT_LADDER && !wasonground)
		cmd->buttons &= ~IN_JUMP;

	wasonground = *ent_getflags(localplayer) & 1;
}

void
misc_clantagchanger(void)
{
	static char *lasttag = "";
	const char *tag = "";

	if (cfg->misc.clantagchanger)
		tag = "alienhook.xyz ";

	uintptr_t netchan = engine_getnetchan();
	if (!netchan)
		return;

	float time = mem->globalvars->currenttime - netchan_getlatency(netchan, 0);

	cvector_vector_type(char) curtag = NULL;

	for (int i = 0; i < strlen(tag); i++) {
		int letter = i + (int)(time * 2);
		cvector_push_back(curtag, tag[letter % strlen(tag)]);
	}

	cvector_push_back(curtag, '\0');

	if (strcmp(curtag, lasttag) != 0) {
		mem->setclantag(curtag, curtag);

		lasttag = curtag;
	}
}

void
misc_fastduck(UserCmd *cmd)
{
	if (cfg->misc.fastduck)
		cmd->buttons |= IN_BULLRUSH;
}

void
misc_fixmovement(UserCmd *cmd, float yaw)
{
	float oldyaw = yaw + (yaw < 0.0f ? 360.0f : 0.0f);
	float newyaw = cmd->viewangles.y + (cmd->viewangles.y < 0.0f ? 360.0f : 0.0f);
	float delta  = newyaw < oldyaw ? fabsf(newyaw - oldyaw) : 360.0f - fabsf(newyaw - oldyaw);

	delta = 360.0f - delta;

	const float forward = cmd->forwardmove;
	const float side    = cmd->sidemove;

	cmd->forwardmove = cos(DEGTORAD(delta)) * forward + cos(DEGTORAD(delta + 90.0f)) * side;
	cmd->sidemove    = sin(DEGTORAD(delta)) * forward + sin(DEGTORAD(delta + 90.0f)) * side;
}

void
misc_slidewalk(UserCmd *cmd)
{
	uintptr_t localplayer = entlist_getentity(engine_getlocalplayer());
	if (!localplayer)
		return;

	if (cfg->misc.slidewalk && *ent_getmovetype(localplayer) != MT_LADDER)
		cmd->buttons ^= IN_FORWARD | IN_BACK | IN_MOVELEFT | IN_MOVERIGHT;
}

void
misc_faststop(UserCmd *cmd)
{
	if (!cfg->misc.faststop)
		return;

	if (cmd->buttons & (IN_MOVELEFT | IN_MOVERIGHT | IN_FORWARD | IN_BACK))
		return;

	if (cmd->buttons & IN_JUMP)
		return;

	uintptr_t localplayer = entlist_getentity(engine_getlocalplayer());
	if (!localplayer)
		return;

	if (!(*ent_getflags(localplayer) & 1))
		return;

	if (!ent_isalive(localplayer))
		return;

	float speed = vec_len2d(*ent_getvelocity(localplayer));
	if (speed < 15)
		return;

	float dir = cmd->viewangles.y - vec_toang2d(*ent_getvelocity(localplayer));
	Vector negdir = vec_fromang2d(dir);

	ConVar *forwardspeed = cvar_find("cl_forwardspeed");
	ConVar *sidespeed = cvar_find("cl_sidespeed");

	cmd->forwardmove = negdir.x * -convar_getfloat(forwardspeed);
	cmd->sidemove = negdir.y * -convar_getfloat(sidespeed);
}

static float
angledeltarad(float a, float b)
{
	float delta;

	delta = isfinite(a - b) ? remainder(a - b, 360) : 0;

	if (a > b) {
		if (delta >= M_PI)
			delta -= M_PI * 2;
	} else {
		if (delta <= -M_PI)
			delta += M_PI * 2;
	}

	return delta;
}

void
misc_autostrafer(UserCmd *cmd)
{
	if (!cfg->misc.autostrafer)
		return;

	if (!(cmd->buttons & IN_JUMP))
		return;

	uintptr_t localplayer = entlist_getentity(engine_getlocalplayer());
	if (!localplayer)
		return;

	if (!ent_isalive(localplayer))
		return;

	float speed = vec_len2d(*ent_getvelocity(localplayer));

	if (speed < 15)
		return;

	ConVar *wishspeed = cvar_find("sv_air_max_wishspeed");
	ConVar *airaccel = cvar_find("sv_airaccelerate");
	ConVar *maxspeed = cvar_find("sv_maxspeed");

	float bestdelta;

	float term = convar_getfloat(wishspeed) / convar_getfloat(airaccel) / convar_getfloat(maxspeed) * 100.0f / speed;
	if (term < 1 && term > -1)
		bestdelta = acosf(term);
	else
		return;

	float yaw = DEGTORAD(cmd->viewangles.y);
	float veldir = atan2f(ent_getvelocity(localplayer)->y, ent_getvelocity(localplayer)->x) - yaw;
	float wishang = atan2f(-cmd->sidemove, cmd->forwardmove);
	float delta = angledeltarad(veldir, wishang);

	float movedir = delta < 0 ? veldir + bestdelta : veldir - bestdelta;

	ConVar *forwardspeed = cvar_find("cl_forwardspeed");
	ConVar *sidespeed = cvar_find("cl_sidespeed");

	cmd->forwardmove = cosf(movedir) * convar_getfloat(forwardspeed);
	cmd->sidemove = -sinf(movedir) * convar_getfloat(sidespeed);
}


void
misc_drawgui(struct nk_context *ctx)
{
	if (nk_tree_push(ctx, NK_TREE_TAB, "Misc", NK_MINIMIZED)) {
		nk_checkbox_label(ctx, "Anti AFK kick", &cfg->misc.antiafk);
		nk_checkbox_label(ctx, "Bunny hop", &cfg->misc.bhop);
		nk_checkbox_label(ctx, "Clantag changer", &cfg->misc.clantagchanger);
		nk_checkbox_label(ctx, "Fast duck", &cfg->misc.fastduck);
		nk_checkbox_label(ctx, "Slidewalk", &cfg->misc.slidewalk);
		nk_checkbox_label(ctx, "Fast stop", &cfg->misc.faststop);
		nk_checkbox_label(ctx, "Autostrafer", &cfg->misc.autostrafer);

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
	cJSON* clantagchanger = cJSON_GetObjectItem(miscjson, "Clantag changer");
	if (cJSON_IsBool(clantagchanger))
		cfg->misc.clantagchanger = clantagchanger->valueint;
	cJSON* fastduck = cJSON_GetObjectItem(miscjson, "Fast duck");
	if (cJSON_IsBool(fastduck))
		cfg->misc.fastduck = fastduck->valueint;
	cJSON* slidewalk = cJSON_GetObjectItem(miscjson, "Slidewalk");
	if (cJSON_IsBool(slidewalk))
		cfg->misc.slidewalk = slidewalk->valueint;
	cJSON* faststop = cJSON_GetObjectItem(miscjson, "Fast stop");
	if (cJSON_IsBool(faststop))
		cfg->misc.faststop = faststop->valueint;
	cJSON* autostrafer = cJSON_GetObjectItem(miscjson, "Autostrafer");
	if (cJSON_IsBool(autostrafer))
		cfg->misc.autostrafer = autostrafer->valueint;
}

void
misc_savecfg(cJSON *json)
{
	cJSON* miscjson = cJSON_CreateObject();

	cJSON_AddBoolToObject(miscjson, "Anti AFK kick", cfg->misc.antiafk);
	cJSON_AddBoolToObject(miscjson, "Bunny hop", cfg->misc.bhop);
	cJSON_AddBoolToObject(miscjson, "Clantag changer", cfg->misc.clantagchanger);
	cJSON_AddBoolToObject(miscjson, "Fast duck", cfg->misc.fastduck);
	cJSON_AddBoolToObject(miscjson, "Slidewalk", cfg->misc.slidewalk);
	cJSON_AddBoolToObject(miscjson, "Fast stop", cfg->misc.faststop);
	cJSON_AddBoolToObject(miscjson, "Autostrafer", cfg->misc.autostrafer);

	cJSON_AddItemToObject(json, "Misc", miscjson);
}
