#ifndef RAGEBOT_H_
#define RAGEBOT_H_

#include <cjson/cJSON.h>

#include "../gui.h"
#include "../sdk/usercmd.h"

typedef struct {
	int enabled;
	int silent;
	int visiblecheck;
	int scopecheck;
	int autoshoot;
	float fov;
	float hitchance;
	int bones[6];
} RagebotCfg;

void ragebot_run(UserCmd *cmd);
void ragebot_drawgui(struct nk_context *ctx);
void ragebot_loadcfg(cJSON *json);
void ragebot_savecfg(cJSON *json);

#endif /* RAGEBOT_H_ */
