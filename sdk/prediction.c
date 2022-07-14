#include "../intf.h"
#include "usercmd.h"
#include "../util.h"

#include "prediction.h"

void
prediction_setup(uintptr_t ent, UserCmd *cmd, void *movehelper, void *movedata)
{
	VFN(void (*)(uintptr_t *, uintptr_t, UserCmd *, void *, void *), VMT(intf->prediction), 21)(intf->prediction, ent, cmd, movehelper, movedata);
}

void
prediction_finish(uintptr_t ent, UserCmd *cmd, void *movedata)
{
	VFN(void (*)(uintptr_t *, uintptr_t, UserCmd *, void *), VMT(intf->prediction), 22)(intf->prediction, ent, cmd, movedata);
}
