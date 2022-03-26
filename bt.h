#ifndef BT_H
#define BT_H

#include "sdk.h"

void bt_update(FrameStage stage);
void bt_run(UserCmd *cmd);
void bt_init(void);
void bt_clean(void);

#endif /* BT_H */
