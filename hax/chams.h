#ifndef CHAMS_H_
#define CHAMS_H_

#include <cjson/cJSON.h>

#include "../sdk/color.h"
#include "../gui.h"
#include "../sdk/modelrender.h"

typedef struct {
	struct {
		int enabled;
		int healthbased;
		ColorA color;
	} visible, occluded;
} ChamsCfg;

int chams_render(void *ctx, void *state, ModelRenderInfo *info, Matrix3x4 *custombonetoworld);
void chams_drawgui(struct nk_context *ctx);
void chams_loadcfg(cJSON *json);
void chams_savecfg(cJSON *json);

#endif /* CHAMS_H_ */
