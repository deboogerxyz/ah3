#include "../hax/bt.h"
#include "../cfg.h"
#include "../sdk/engine.h"
#include "../sdk/ent.h"
#include "../sdk/entlist.h"
#include "../sdk/globalvars.h"
#include "../sdk/mat3x4.h"
#include "../mem.h"
#include "../sdk/weaponid.h"
#include "../sdk/weaponinfo.h"

#include "legitbot.h"

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

void
legitbot_run(UserCmd *cmd)
{
	if (!(cmd->buttons & IN_ATTACK))
		return;

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

	LegitbotCfg config = cfg->legitbot[getweaponconfig(weaponid)];
	if (!config.enabled)
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

			if (config.visiblecheck && !ent_cansee(localplayer, ent, ent_getbonepos(ent, 8 - j)))
				continue;

			Vector bonepos = ent_getbonepos(ent, 8 - j);
			Vector ang     = vec_calcang(eyepos, bonepos, viewangles);
			float  fov     = hypot(ang.x, ang.y);

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

			if (config.visiblecheck && !ent_cansee(localplayer, record->ent, mat_origin(record->matrix[8 - k])))
				continue;

			Vector headpos = mat_origin(record->matrix[8 - k]);
			Vector ang = vec_calcang(eyepos, headpos, viewangles);
			float  fov = hypot(ang.x, ang.y);

			if (fov < bestfov) {
				bestfov = fov;
				bestang = ang;
			}
		}
	}

	if (vec_isnull(bestang))
		return;

	if (!config.silent && config.smooth > 1.0f)
		bestang = vec_div(bestang, config.smooth);

	cmd->viewangles = vec_add(cmd->viewangles, bestang);

	if (!config.silent)
		engine_setviewangles(&cmd->viewangles);
}

void
legitbot_drawgui(struct nk_context *ctx)
{
	const char *categories[] = {"Pistols", "Rifles", "AWP", "Scout", "SMGs", "Shotguns"};

	if (nk_tree_push(ctx, NK_TREE_TAB, "Legitbot", NK_MINIMIZED)) {
		for (int i = 0; i < LEN(cfg->legitbot); i++)
			if (nk_tree_push_id(ctx, NK_TREE_NODE, categories[i], NK_MINIMIZED, i)) {
				nk_checkbox_label(ctx, "Enabled", &cfg->legitbot[i].enabled);
				nk_checkbox_label(ctx, "Silent", &cfg->legitbot[i].silent);
				nk_checkbox_label(ctx, "Visible check", &cfg->legitbot[i].visiblecheck);
				if (i > 0 && i < 4)
					nk_checkbox_label(ctx, "Scope check", &cfg->legitbot[i].scopecheck);
				nk_property_float(ctx, "#FOV:", 0.0f, &cfg->legitbot[i].fov, 255.0f, 0.025f, 0.025f);
				if (!cfg->legitbot[i].silent)
					nk_property_float(ctx, "#Smooth:", 1.0f, &cfg->legitbot[i].smooth, 100.0f, 0.1f, 0.1f);

				const char *bones[] = {"Head", "Neck", "Sternum", "Chest", "Stomach", "Pelvis"};

				for (int j = 0; j < LEN(cfg->legitbot[i].bones); j++)
					nk_checkbox_label(ctx, bones[j], &cfg->legitbot[i].bones[j]);

				nk_tree_pop(ctx);
			}

		nk_tree_pop(ctx);
	}
}

void
legitbot_loadcfg(cJSON *json)
{
	int i = 0;
	cJSON* legitbotjson;

	cJSON_ArrayForEach(legitbotjson, cJSON_GetObjectItem(json, "Legitbot")) {
		cJSON* enabled = cJSON_GetObjectItem(legitbotjson, "Enabled");
		if (cJSON_IsBool(enabled))
			cfg->legitbot[i].enabled = enabled->valueint;
		cJSON* silent = cJSON_GetObjectItem(legitbotjson, "Silent");
		if (cJSON_IsBool(silent))
			cfg->legitbot[i].silent = silent->valueint;
		cJSON* visiblecheck = cJSON_GetObjectItem(legitbotjson, "Visible check");
		if (cJSON_IsBool(visiblecheck))
			cfg->legitbot[i].visiblecheck = visiblecheck->valueint;
		if (i > 0 && i < 4) {
			cJSON* scopecheck = cJSON_GetObjectItem(legitbotjson, "Scope check");
			if (cJSON_IsBool(scopecheck))
				cfg->legitbot[i].scopecheck = scopecheck->valueint;
		}
		cJSON* fov = cJSON_GetObjectItem(legitbotjson, "FOV");
		if (cJSON_IsNumber(fov))
			cfg->legitbot[i].fov = (float)fov->valuedouble;
		cJSON* smooth = cJSON_GetObjectItem(legitbotjson, "Smooth");
		if (cJSON_IsNumber(smooth))
			cfg->legitbot[i].smooth = (float)smooth->valuedouble;

		int j = 0;
		cJSON *bonejson;

		cJSON_ArrayForEach(bonejson, cJSON_GetObjectItem(legitbotjson, "Bones")) {
			if (cJSON_IsBool(bonejson))
				cfg->legitbot[i].bones[j] = bonejson->valueint;

			j++;
		}

		i++;
	}
}

void
legitbot_savecfg(cJSON *json)
{
	cJSON* legitbotjson = cJSON_AddArrayToObject(json, "Legitbot");

	for (int i = 0; i < LEN(cfg->glow); i++) {
		cJSON* legitbot = cJSON_CreateObject();

		cJSON_AddBoolToObject(legitbot, "Enabled", cfg->legitbot[i].enabled);
		cJSON_AddBoolToObject(legitbot, "Silent", cfg->legitbot[i].silent);
		cJSON_AddBoolToObject(legitbot, "Visible check", cfg->legitbot[i].visiblecheck);
		if (i > 0 && i < 4)
			cJSON_AddBoolToObject(legitbot, "Scope check", cfg->legitbot[i].scopecheck);
		cJSON_AddNumberToObject(legitbot, "FOV", cfg->legitbot[i].fov);
		cJSON_AddNumberToObject(legitbot, "Smooth", cfg->legitbot[i].smooth);

		cJSON* bones = cJSON_AddArrayToObject(legitbot, "Bones");

		for (int j = 0; j < LEN(cfg->legitbot[i].bones); j++)
			cJSON_AddItemToArray(bones, cJSON_CreateBool(cfg->legitbot[i].bones[j]));


		cJSON_AddItemToArray(legitbotjson, legitbot);
	}
}
