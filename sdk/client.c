#include "../intf.h"
#include "../util.h"

#include "client.h"

ClientClass *
client_getallclasses(void)
{
	return VFN(ClientClass *(*)(uintptr_t *), VMT(intf->client), 8)(intf->client);
}

int
sdk_dispatchusermsg(int type, int flags, int size, void *data)
{
	return VFN(char (*)(uintptr_t *, int, int, int, void *), VMT(intf->client), 38)(intf->client, type, flags, size, data);
}
