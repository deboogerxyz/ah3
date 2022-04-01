#ifndef VISUALS_H_
#define VISUALS_H_

#include <cjson/cJSON.h>

#include "../gui.h"

typedef struct {
	int overridefov;
	float fov;
} VisualsCfg;

void visuals_drawgui(struct nk_context *ctx);
void visuals_loadcfg(cJSON *json);
void visuals_savecfg(cJSON *json);

#endif /* VISUALS_H_ */
