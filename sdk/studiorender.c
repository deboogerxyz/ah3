#include <string.h>

#include "../intf.h"
#include "../util.h"
#include "material.h"

#include "studiorender.h"

void
studiorender_forcedmatoverride(uintptr_t mat, OverrideType type, int i)
{
	VFN(void (*)(uintptr_t *, uintptr_t, OverrideType, int), VMT(intf->studiorender), 33)(intf->studiorender, mat, type, i);
}

int
studiorender_isforcedmatoverride(void)
{
	if (!intf->studiorender->matoverride)
		return intf->studiorender->overridetype == OVERRIDETYPE_DEPTHWRITE || intf->studiorender->overridetype == OVERRIDETYPE_SSAODEPTHWRITE;
	return !strncmp("dev/glow", material_getname(intf->studiorender->matoverride), strlen("dev/glow"));
}
