#include "../intf.h"
#include "../util.h"

#include "cvar.h"

ConVar *
cvar_find(const char *name)
{
	return VFN(ConVar *(*)(uintptr_t *, const char *), VMT(intf->cvar), 15)(intf->cvar, name);
}
