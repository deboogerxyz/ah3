#include "../intf.h"
#include "../util.h"

#include "inputsys.h"

void
inputsys_resetinputstate(void)
{
	VFN(void (*)(uintptr_t *), VMT(intf->inputsys), 39)(intf->inputsys);
}
