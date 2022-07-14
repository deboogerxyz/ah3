#include "../mem.h"
#include "../util.h"

#include "movehelper.h"

void
movehelper_sethost(uintptr_t ent)
{
	VFN(void (*)(uintptr_t *, uintptr_t), VMT(mem->movehelper), 1)(mem->movehelper, ent);
}
