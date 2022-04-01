#ifndef MISC_H_
#define MISC_H_

#include <cjson/cJSON.h>

#include "../gui.h"

typedef struct {
	int radar;
} MiscCfg;

void misc_drawgui(struct nk_context *ctx);
void misc_loadcfg(cJSON *json);
void misc_savecfg(cJSON *json);

#endif /* MISC_H_ */
