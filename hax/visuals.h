#ifndef VISUALS_H_
#define VISUALS_H_

#include <cjson/cJSON.h>

#include "../sdk/framestage.h"
#include "../sdk/usercmd.h"
#include "../gui.h"

typedef struct {
	int disablepostprocessing;
	int disableshadows;
	int forcecrosshair;
	int grenadeprediction;
	int overridefov;
	float fov;
	int remove3dsky;
	int revealmoney;
	int revealradar;
	int revealranks;
} VisualsCfg;

void visuals_disablepostprocessing(FrameStage stage);
void visuals_disableshadows(void);
void visuals_forcecrosshair(FrameStage stage);
void visuals_grenadeprediction(FrameStage stage);
void visuals_remove3dsky(FrameStage stage);
void visuals_revealranks(UserCmd *cmd);
void visuals_drawgui(struct nk_context *ctx);
void visuals_loadcfg(cJSON *json);
void visuals_savecfg(cJSON *json);

#endif /* VISUALS_H_ */
