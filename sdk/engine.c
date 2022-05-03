#include "../intf.h"
#include "../util.h"

#include "engine.h"

int
engine_getlocalplayer(void)
{
	return VFN(int (*)(uintptr_t *), VMT(intf->engine), 12)(intf->engine);
}

void
engine_setviewangles(Vector *ang)
{
	VFN(void (*)(uintptr_t *, Vector *), VMT(intf->engine), 19)(intf->engine, ang);
}

int
engine_getmaxclients(void)
{
	return VFN(int (*)(uintptr_t *), VMT(intf->engine), 20)(intf->engine);
}

int
engine_isingame(void)
{
	return VFN(char (*)(uintptr_t *), VMT(intf->engine), 26)(intf->engine);
}

int
engine_isconnected(void)
{
	return VFN(char (*)(uintptr_t *), VMT(intf->engine), 27)(intf->engine);
}

uintptr_t
engine_getbsptreequery(void)
{
	return VFN(uintptr_t (*)(uintptr_t *), VMT(intf->engine), 43)(intf->engine);
}

uintptr_t
engine_getnetchan(void)
{
	return VFN(uintptr_t (*)(uintptr_t *), VMT(intf->engine), 78)(intf->engine);
}

int
engine_istakingscreenshot(void)
{
	return VFN(char (*)(void *), VMT(intf->engine), 92)(intf->engine);
}
