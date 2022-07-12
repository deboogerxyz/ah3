#ifndef CFG_H_
#define CFG_H_

#include "hax/bt.h"
#include "hax/chams.h"
#include "hax/glow.h"
#include "gui.h"
#include "hax/legitbot.h"
#include "hax/misc.h"
#include "hax/ragebot.h"
#include "hax/visuals.h"

typedef struct {
	RagebotCfg ragebot[6];
	LegitbotCfg legitbot[6];
	BtCfg bt;
	GlowCfg glow[6];
	ChamsCfg chams[2];
	VisualsCfg visuals;
	MiscCfg misc;
} Cfg;

void cfg_load(const char *name);
void cfg_save(const char *name);
void cfg_delete(const char *name);
void cfg_drawgui(struct nk_context *ctx);
void cfg_init(void);
void cfg_clean(void);

extern Cfg *cfg;

#endif /* CFG_H_ */
