#include <stdint.h>

#include "intf.h"
#include "sdk.h"
#include "util.h"

ClientClass *
getallclasses(void)
{
	return VFN(ClientClass *(*)(uintptr_t *),
	           VMT(intf->client), 8, (intf->client));
}
