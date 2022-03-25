#include <dlfcn.h>  /* dlsym, ... */
#include <stdint.h>
#include <stdlib.h> /* exit, calloc, ... */

#include "intf.h"

Intf *intf;

static uintptr_t *
find(const char *libname, const char *name)
{
	uintptr_t *(*create)(const char *, int *);
	uintptr_t *found;

	*(void **)&create = dlsym(dlopen(libname, RTLD_NOLOAD | RTLD_LAZY),
	                          "CreateInterface");
	if (create) {
		found = create(name, 0);
		if (found)
			return found;
	}

	return NULL;
}

void
intf_init(void)
{
	intf = calloc(1, sizeof(Intf));

	intf->client = find("csgo/bin/linux64/client_client.so", "VClient018");
}
