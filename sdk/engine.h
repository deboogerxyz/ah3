#ifndef ENGINE_H_
#define ENGINE_H_

#include <stdint.h>

#include "vec.h"

int engine_getlocalplayer(void);
void engine_setviewangles(Vector *ang);
int engine_getmaxclients(void);
int engine_isingame(void);
int engine_isconnected(void);
uintptr_t engine_getnetchan(void);
int engine_istakingscreenshot(void);

#endif /* ENGINE_H_ */
