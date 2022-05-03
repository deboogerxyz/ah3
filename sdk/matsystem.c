#include "../intf.h"
#include "../sdk/matsystem.h"
#include "../util.h"

#include "matsystem.h"

uintptr_t
matsystem_findmaterial(const char *matname, const char *texturegroupname, char complain, const char *complainprefix)
{
	return VFN(uintptr_t (*)(uintptr_t *, const char *, const char *, char, const char *), VMT(intf->matsystem), 84)(intf->matsystem, matname, texturegroupname, complain, complainprefix);
}
