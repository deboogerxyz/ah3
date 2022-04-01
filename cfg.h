#ifndef CFG_H_
#define CFG_H_

#include "hax/bt.h"
#include "gui.h"
#include "hax/misc.h"

typedef struct {
	BtCfg bt;
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
