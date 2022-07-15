#ifndef ENGINEPREDICTION_H_
#define ENGINEPREDICTION_H_

#include "../sdk/usercmd.h"

void engineprediction_run(UserCmd *cmd);

extern int engineprediction_flags;
extern Vector engineprediction_velocity;

#endif /* ENGINEPREDICTION_H_ */
