#ifndef BT_H
#define BT_H

#include "../gui.h"
#include "../sdk.h"

void bt_update(FrameStage stage);
void bt_run(UserCmd *cmd);
void bt_drawgui(struct nk_context *ctx);
void bt_init(void);
void bt_clean(void);

#endif /* BT_H */
