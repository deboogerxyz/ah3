#ifndef GLOW_H_
#define GLOW_H_

#include <cjson/cJSON.h>

#include "../sdk/color.h"
#include "../gui.h"

typedef struct {
	int enabled;
	int healthbased;
	ColorA color;
} GlowCfg;

void glow_render(void);
void glow_drawgui(struct nk_context *ctx);
void glow_loadcfg(cJSON *json);
void glow_savecfg(cJSON *json);

#endif /* GLOW_H_ */
