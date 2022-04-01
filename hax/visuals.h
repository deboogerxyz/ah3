#ifndef VISUALS_H_
#define VISUALS_H_

#include <cjson/cJSON.h>

#include "../sdk/framestage.h"
#include "../gui.h"

typedef struct {
	int forcecrosshair;
	int overridefov;
	float fov;
} VisualsCfg;

void visuals_forcecrosshair(FrameStage stage);
void visuals_drawgui(struct nk_context *ctx);
void visuals_loadcfg(cJSON *json);
void visuals_savecfg(cJSON *json);

#endif /* VISUALS_H_ */
