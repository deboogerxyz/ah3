#ifndef LEGITBOT_H_
#define LEGITBOT_H_

#include <cjson/cJSON.h>

#include "../gui.h"
#include "../sdk/usercmd.h"

typedef struct {
	int enabled;
	int silent;
	int visiblecheck;
	int scopecheck;
	float fov;
	float smooth;
	int bones[6];
} LegitbotCfg;

void legitbot_run(UserCmd *cmd);
void legitbot_drawgui(struct nk_context *ctx);
void legitbot_loadcfg(cJSON *json);
void legitbot_savecfg(cJSON *json);

#endif /* LEGITBOT_H_ */
