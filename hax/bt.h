#ifndef BT_H
#define BT_H

#include <cjson/cJSON.h>

#include "../deps/cvector.h"
#include "../gui.h"
#include "../sdk/framestage.h"
#include "../sdk/mat3x4.h"
#include "../sdk/usercmd.h"

typedef struct {
	Matrix3x4 matrix[256];
	float simtime;
} Record;

typedef struct {
	int enabled;
	int limit;
} BtCfg;

int bt_isvalid(float simtime);
void bt_update(FrameStage stage);
Record *bt_getclosestrecord(UserCmd *cmd);
void bt_run(UserCmd *cmd);
void bt_drawgui(struct nk_context *ctx);
void bt_loadcfg(cJSON *json);
void bt_savecfg(cJSON *json);
void bt_init(void);
void bt_clean(void);

extern cvector_vector_type(Record) bt_records[65];

#endif /* BT_H */
