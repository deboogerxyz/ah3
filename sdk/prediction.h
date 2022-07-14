#ifndef PREDICTION_H_
#define PREDICTION_H_

#include <stdint.h>

#include "usercmd.h"

void prediction_setup(uintptr_t ent, UserCmd *cmd, void *movehelper, void *movedata);
void prediction_finish(uintptr_t ent, UserCmd *cmd, void *movedata);

#endif /* PREDICTION_H_ */
