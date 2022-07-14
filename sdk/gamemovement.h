#ifndef GAMEMOVEMENT_H_
#define GAMEMOVEMENT_H_

#include <stdint.h>

void gamemovement_process(uintptr_t ent, void *movedata);
void gamemovement_starttrackpredicterrors(uintptr_t ent);
void gamemovement_finishtrackpredicterrors(uintptr_t ent);

#endif /* GAMEMOVEMENT_H_ */
