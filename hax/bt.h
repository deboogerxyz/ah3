#ifndef BT_H
#define BT_H

#include <cjson/cJSON.h>

#include "../gui.h"
#include "../sdk/framestage.h"
#include "../sdk/usercmd.h"

typedef struct {
	int enabled;
	int limit;
} BtCfg;

void bt_update(FrameStage stage);
void bt_run(UserCmd *cmd);
void bt_drawgui(struct nk_context *ctx);
void bt_loadcfg(cJSON *json);
void bt_savecfg(cJSON *json);
void bt_init(void);
void bt_clean(void);

#endif /* BT_H */
