#ifndef VISUALS_H_
#define VISUALS_H_

#include <cjson/cJSON.h>

#include "../sdk/framestage.h"
#include "../sdk/usercmd.h"
#include "../gui.h"

typedef struct {
	int enabled;
	float x, y, z;
} ViewmodelCfg;

typedef struct {
	int disablefog;
	int disablepostprocessing;
	int disableshadows;
	int forcecrosshair;
	int grenadeprediction;
	int oppositehandknife;
	int overridefov;
	float fov;
	int overridefarz;
	float farz;
	int remove3dsky;
	int removeaimpunch;
	int removeviewpunch;
	int removescopeoverlay;
	int revealmoney;
	int revealradar;
	int revealranks;
	ViewmodelCfg viewmodel;
} VisualsCfg;

void visuals_disablepostprocessing(FrameStage stage);
void visuals_disableshadows(void);
void visuals_forcecrosshair(FrameStage stage);
void visuals_grenadeprediction(FrameStage stage);
void visuals_oppositehandknife(FrameStage stage);
void visuals_remove3dsky(FrameStage stage);
void visuals_revealranks(UserCmd *cmd);
void visuals_removevisualrecoil(FrameStage stage);
void visuals_drawgui(struct nk_context *ctx);
void visuals_loadcfg(cJSON *json);
void visuals_savecfg(cJSON *json);

#endif /* VISUALS_H_ */
