#include "../intf.h"
#include "../util.h"

#include "gamemovement.h"

void
gamemovement_process(uintptr_t ent, void *movedata)
{
	VFN(void (*)(uintptr_t *, uintptr_t, void *), VMT(intf->gamemovement), 2)(intf->gamemovement, ent, movedata);
}

void
gamemovement_starttrackpredicterrors(uintptr_t ent)
{
	VFN(void (*)(uintptr_t *, uintptr_t), VMT(intf->gamemovement), 4)(intf->gamemovement, ent);
}

void
gamemovement_finishtrackpredicterrors(uintptr_t ent)
{
	VFN(void (*)(uintptr_t *, uintptr_t), VMT(intf->gamemovement), 5)(intf->gamemovement, ent);
}
