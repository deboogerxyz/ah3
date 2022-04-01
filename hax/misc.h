#ifndef MISC_H_
#define MISC_H_

#include <cjson/cJSON.h>

#include "../gui.h"
#include "../sdk/usercmd.h"

typedef struct {
	int antiafk;
	int bhop;
	int radar;
} MiscCfg;

void misc_antiafk(UserCmd *cmd);
void misc_bhop(UserCmd *cmd);
void misc_drawgui(struct nk_context *ctx);
void misc_loadcfg(cJSON *json);
void misc_savecfg(cJSON *json);

#endif /* MISC_H_ */
