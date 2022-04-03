#include "../intf.h"
#include "../util.h"

#include "entlist.h"

uintptr_t
entlist_getentity(int i)
{
	return VFN(uintptr_t (*)(uintptr_t *, int), VMT(intf->entlist), 3)(intf->entlist, i);
}

uintptr_t
entlist_getentityfromhandle(int *handle)
{
	return VFN(uintptr_t (*)(uintptr_t *, int *), VMT(intf->entlist), 4)(intf->entlist, handle);
}

int
entlist_gethighestidx(void)
{
	return VFN(int (*)(uintptr_t *), VMT(intf->entlist), 6)(intf->entlist);
}
