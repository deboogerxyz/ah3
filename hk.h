#ifndef HK_H_
#define HK_H_

#include "sdk/modelrender.h"

void hk_orig_drawmodelexecute(void *ctx, void *state, ModelRenderInfo *info, Matrix3x4 *custombonetoworld);

void hk_init(void);
void hk_clean(void);

#endif /* HK_H_ */
