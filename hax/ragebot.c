#include "../hax/bt.h"
#include "../cfg.h"
#include "../sdk/cvar.h"
#include "../sdk/engine.h"
#include "../sdk/ent.h"
#include "../sdk/entlist.h"
#include "../sdk/globalvars.h"
#include "../sdk/mat3x4.h"
#include "../mem.h"
#include "../sdk/weaponid.h"
#include "../sdk/weaponinfo.h"

#include "ragebot.h"
#include <math.h>

static int 
getweaponconfig(WeaponId id)
{
	switch (id) {
	case WEAPONID_GLOCK: /* FALLTHROUGH */
	case WEAPONID_HKP2000:
	case WEAPONID_USP_S:
	case WEAPONID_ELITE:
	case WEAPONID_P250:
	case WEAPONID_TEC9:
	case WEAPONID_FIVESEVEN:
	case WEAPONID_CZ75A:
	case WEAPONID_DEAGLE:
	case WEAPONID_REVOLVER:
		return 0;
	case WEAPONID_GALILAR: /* FALLTHROUGH */
	case WEAPONID_FAMAS:
	case WEAPONID_AK47:
	case WEAPONID_M4A1:
	case WEAPONID_M4A1_S:
	case WEAPONID_SG553:
	case WEAPONID_AUG:
	case WEAPONID_G3SG1:
	case WEAPONID_SCAR20:
	case WEAPONID_M249:
	case WEAPONID_NEGEV:
		return 1;
	case WEAPONID_AWP:
		return 2;
	case WEAPONID_SSG08:
		return 3;
	case WEAPONID_MAC10: /* FALLTHROUGH */
	case WEAPONID_MP9:
	case WEAPONID_MP7:
	case WEAPONID_MP5SD:
	case WEAPONID_UMP45:
	case WEAPONID_P90:
	case WEAPONID_BIZON:
		return 4;
	case WEAPONID_NOVA: /* FALLTHROUGH */
	case WEAPONID_XM1014:
	case WEAPONID_SAWEDOFF:
	case WEAPONID_MAG7:
		return 5;
	default:
		return -1;
	}
}

static void
autostop(uintptr_t localplayer, WeaponInfo *weaponinfo, UserCmd *cmd)
{
	if (!(*ent_getflags(localplayer) & 1))
		return;

	if (cmd->buttons & IN_JUMP)
		return;

	if (!ent_isalive(localplayer))
		return;

	float maxspeed = (ent_getisscoped(localplayer) ? weaponinfo->maxspeedalt : weaponinfo->maxspeed) / 3.0f;

	float speed = vec_len2d(*ent_getvelocity(localplayer));

	if (speed > maxspeed + 15) {
		float dir = cmd->viewangles.y - vec_toang2d(*ent_getvelocity(localplayer));	

		Vector negdir = vec_fromang2d(dir);

		ConVar *forwardspeed = cvar_find("cl_forwardspeed");
		ConVar *sidespeed = cvar_find("cl_sidespeed");

		cmd->forwardmove = negdir.x * -convar_getfloat(forwardspeed);
		cmd->sidemove = negdir.y * -convar_getfloat(sidespeed);
	} else if (cmd->forwardmove && cmd->sidemove) {
		float maxspeedroot = maxspeed * M_SQRT1_2;

		cmd->forwardmove = cmd->forwardmove < 0 ? -maxspeedroot : maxspeedroot;
		cmd->sidemove = cmd->sidemove < 0 ? -maxspeedroot : maxspeedroot;
	} else if (cmd->forwardmove) {
		cmd->forwardmove = cmd->forwardmove < 0 ? -maxspeed : maxspeed;
	} else if (cmd->sidemove) {
		cmd->sidemove = cmd->sidemove < 0 ? -maxspeed : maxspeed;
	}
}

static float
gethitchance(uintptr_t weapon, float radius, float dist)
{
	float f = radius / (tanf(ent_getspread(weapon) + ent_getinaccuracy(weapon)) * dist);

	return CLAMP(f, 0, 1);
}

void
ragebot_run(UserCmd *cmd)
{
	uintptr_t localplayer = entlist_getentity(engine_getlocalplayer());
	if (!localplayer)
		return;

	if (*ent_getwaitfornoattack(localplayer))
		return;

	if (*ent_getnextattack(localplayer) > globalvars_getservertime(cmd))
		return;

	uintptr_t weapon = ent_getactiveweapon(localplayer);
	if (!weapon)
		return;

	WeaponId weaponid = *ent_getitemindex(weapon);
	if (getweaponconfig(weaponid) < 0)
		return;

	RagebotCfg config = cfg->ragebot[getweaponconfig(weaponid)];
	if (!config.enabled)
		return;

	if (!(cmd->buttons & IN_ATTACK) && !config.autoshoot)
		return;

	if (config.scopecheck && !*ent_getisscoped(localplayer) && ent_getweapontype(weapon) == WEAPONTYPE_SNIPER)
		return;

	WeaponInfo *weaponinfo = ent_getweaponinfo(weapon);
	if (!weaponinfo)
		return;

	if (*ent_getshotsfired(localplayer) > 0 && !weaponinfo->fullauto)
		return;

	Vector eyepos     = ent_geteyepos(localplayer);
	Vector aimpunch   = ent_getaimpunch(localplayer);
	Vector viewangles = vec_add(cmd->viewangles, aimpunch);

	float bestfov  = config.fov;
	Vector bestang = {0};

	int maxclients = engine_getmaxclients();
	for (int i = 1; i <= maxclients; i++) {
		uintptr_t ent = entlist_getentity(i);
		if (!ent)
			continue;

		if (!mem->isotherenemy(ent, localplayer))
			continue;

		if (!ent_isalive(ent) || ent_isdormant(ent) || *ent_getimmunity(ent))
			continue;

		for (int j = 0; j < LEN(config.bones); j++) {
			if (!config.bones[j])
				continue;

			Vector bonepos = ent_getbonepos(ent, 8 - j);
			Vector ang     = vec_calcang(eyepos, bonepos, viewangles);
			float  fov     = hypot(ang.x, ang.y);

			if (fov >= bestfov)
				continue;

			if (config.visiblecheck && !ent_cansee(localplayer, ent, bonepos))
				continue;

			if (config.autostop)
				autostop(localplayer, weaponinfo, cmd);

			float dist = vec_dist(eyepos, bonepos);

			if (gethitchance(weapon, 5, dist) < config.hitchance)
				continue;

			if (fov < bestfov) {
				bestfov = fov;
				bestang = ang;
			}
		}
	}

	Record *record = 0;

	if (cfg->bt.enabled)
		record = bt_getclosestrecord(cmd);

	if (record) {
		for (int k = 0; k < LEN(config.bones); k++) {
			if (!config.bones[k])
				continue;

			Vector bonepos = mat_origin(record->matrix[8 - k]);
			Vector ang     = vec_calcang(eyepos, bonepos, viewangles);
			float  fov     = hypot(ang.x, ang.y);

			if (fov >= bestfov)
				continue;

			if (config.visiblecheck && !ent_cansee(localplayer, record->ent, bonepos))
				continue;

			if (config.autostop)
				autostop(localplayer, weaponinfo, cmd);

			float dist = vec_dist(eyepos, bonepos);

			if (gethitchance(weapon, 5, dist) < config.hitchance)
				continue;

			if (fov < bestfov) {
				bestfov = fov;
				bestang = ang;
			}
		}
	}

	if (vec_isnull(bestang))
		return;

	cmd->viewangles = vec_add(cmd->viewangles, bestang);

	if (!config.silent)
		engine_setviewangles(&cmd->viewangles);

	if (config.autoshoot && *ent_getnextattack(localplayer) <= globalvars_getservertime(cmd))
		cmd->buttons |= IN_ATTACK;
}

void
ragebot_drawgui(struct nk_context *ctx)
{
	const char *categories[] = {"Pistols", "Rifles", "AWP", "Scout", "SMGs", "Shotguns"};

	if (nk_tree_push(ctx, NK_TREE_TAB, "Ragebot", NK_MINIMIZED)) {
		for (int i = 0; i < LEN(cfg->ragebot); i++)
			if (nk_tree_push_id(ctx, NK_TREE_NODE, categories[i], NK_MINIMIZED, i)) {
				nk_checkbox_label(ctx, "Enabled", &cfg->ragebot[i].enabled);
				nk_checkbox_label(ctx, "Silent", &cfg->ragebot[i].silent);
				nk_checkbox_label(ctx, "Visible check", &cfg->ragebot[i].visiblecheck);
				if (i > 0 && i < 4)
					nk_checkbox_label(ctx, "Scope check", &cfg->ragebot[i].scopecheck);
				nk_checkbox_label(ctx, "Auto shoot", &cfg->ragebot[i].autoshoot);
				nk_checkbox_label(ctx, "Auto stop", &cfg->ragebot[i].autostop);
				nk_property_float(ctx, "#FOV:", 0.0f, &cfg->ragebot[i].fov, 255.0f, 0.025f, 0.025f);
				nk_property_float(ctx, "#Hit chance:", 0.0f, &cfg->ragebot[i].hitchance, 1.0f, 0.005f, 0.005f);

				const char *bones[] = {"Head", "Neck", "Sternum", "Chest", "Stomach", "Pelvis"};

				for (int j = 0; j < LEN(cfg->ragebot[i].bones); j++)
					nk_checkbox_label(ctx, bones[j], &cfg->ragebot[i].bones[j]);

				nk_tree_pop(ctx);
			}

		nk_tree_pop(ctx);
	}
}

void
ragebot_loadcfg(cJSON *json)
{
	int i = 0;
	cJSON* ragebotjson;

	cJSON_ArrayForEach(ragebotjson, cJSON_GetObjectItem(json, "Ragebot")) {
		cJSON* enabled = cJSON_GetObjectItem(ragebotjson, "Enabled");
		if (cJSON_IsBool(enabled))
			cfg->ragebot[i].enabled = enabled->valueint;
		cJSON* silent = cJSON_GetObjectItem(ragebotjson, "Silent");
		if (cJSON_IsBool(silent))
			cfg->ragebot[i].silent = silent->valueint;
		cJSON* visiblecheck = cJSON_GetObjectItem(ragebotjson, "Visible check");
		if (cJSON_IsBool(visiblecheck))
			cfg->ragebot[i].visiblecheck = visiblecheck->valueint;
		if (i > 0 && i < 4) {
			cJSON* scopecheck = cJSON_GetObjectItem(ragebotjson, "Scope check");
			if (cJSON_IsBool(scopecheck))
				cfg->ragebot[i].scopecheck = scopecheck->valueint;
		}
		cJSON* autoshoot = cJSON_GetObjectItem(ragebotjson, "Auto shoot");
		if (cJSON_IsBool(autoshoot))
			cfg->ragebot[i].autoshoot = autoshoot->valueint;
		cJSON* autostop = cJSON_GetObjectItem(ragebotjson, "Auto stop");
		if (cJSON_IsBool(autostop))
			cfg->ragebot[i].autostop = autostop->valueint;
		cJSON* fov = cJSON_GetObjectItem(ragebotjson, "FOV");
		if (cJSON_IsNumber(fov))
			cfg->ragebot[i].fov = (float)fov->valuedouble;
		cJSON* hitchance = cJSON_GetObjectItem(ragebotjson, "Hit chance");
		if (cJSON_IsNumber(hitchance))
			cfg->ragebot[i].hitchance = (float)hitchance->valuedouble;

		int j = 0;
		cJSON *bonejson;

		cJSON_ArrayForEach(bonejson, cJSON_GetObjectItem(ragebotjson, "Bones")) {
			if (cJSON_IsBool(bonejson))
				cfg->ragebot[i].bones[j] = bonejson->valueint;

			j++;
		}

		i++;
	}
}

void
ragebot_savecfg(cJSON *json)
{
	cJSON* ragebotjson = cJSON_AddArrayToObject(json, "Ragebot");

	for (int i = 0; i < LEN(cfg->glow); i++) {
		cJSON* ragebot = cJSON_CreateObject();

		cJSON_AddBoolToObject(ragebot, "Enabled", cfg->ragebot[i].enabled);
		cJSON_AddBoolToObject(ragebot, "Silent", cfg->ragebot[i].silent);
		cJSON_AddBoolToObject(ragebot, "Visible check", cfg->ragebot[i].visiblecheck);
		if (i > 0 && i < 4)
			cJSON_AddBoolToObject(ragebot, "Scope check", cfg->ragebot[i].scopecheck);
		cJSON_AddBoolToObject(ragebot, "Auto shoot", cfg->ragebot[i].autoshoot);
		cJSON_AddBoolToObject(ragebot, "Auto stop", cfg->ragebot[i].autostop);
		cJSON_AddNumberToObject(ragebot, "FOV", cfg->ragebot[i].fov);
		cJSON_AddNumberToObject(ragebot, "Hit chance", cfg->ragebot[i].hitchance);

		cJSON* bones = cJSON_AddArrayToObject(ragebot, "Bones");

		for (int j = 0; j < LEN(cfg->ragebot[i].bones); j++)
			cJSON_AddItemToArray(bones, cJSON_CreateBool(cfg->ragebot[i].bones[j]));


		cJSON_AddItemToArray(ragebotjson, ragebot);
	}
}
