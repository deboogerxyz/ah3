#include "../intf.h"
#include "../util.h"
#include "material.h"

#include "studiorender.h"

void
studiorender_forcedmatoverride(uintptr_t mat, OverrideType type, int i)
{
	VFN(void (*)(uintptr_t *, uintptr_t, OverrideType, int), VMT(intf->studiorender), 33)(intf->studiorender, mat, type, i);
}
