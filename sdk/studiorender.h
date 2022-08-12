#ifndef STUDIORENDER_H_
#define STUDIORENDER_H_

#include <stdint.h>

#include "../util.h"

typedef enum {
	OVERRIDETYPE_NORMAL,
	OVERRIDETYPE_BUILDSHADOWS,
	OVERRIDETYPE_DEPTHWRITE,
	OVERRIDETYPE_CUSTOMMATERIAL,
	OVERRIDETYPE_SSAODEPTHWRITE
} OverrideType;

typedef struct {
	PAD(600);
	uintptr_t matoverride;
	PAD(24);
	OverrideType overridetype;
} StudioRender;

void studiorender_forcedmatoverride(uintptr_t mat, OverrideType type, int i);
int studiorender_isforcedmatoverride(void);

#endif /*  STUDIORENDER_H_ */
