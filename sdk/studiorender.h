#ifndef STUDIORENDER_H_
#define STUDIORENDER_H_

#include <stdint.h>

typedef enum {
	OVERRIDETYPE_NORMAL,
	OVERRIDETYPE_BUILDSHADOWS,
	OVERRIDETYPE_DEPTHWRITE,
	OVERRIDETYPE_CUSTOMMATERIAL,
	OVERRIDETYPE_SSAODEPTHWRITE
} OverrideType;

void studiorender_forcedmatoverride(uintptr_t mat, OverrideType type, int i);

#endif /*  STUDIORENDER_H_ */
