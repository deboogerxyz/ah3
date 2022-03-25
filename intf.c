#include <dlfcn.h> /* dlsym, ... */
#include <stdint.h>
#include <stdlib.h> /* exit, calloc, ... */

#include "intf.h"
#include "util.h"

Intf *intf;

static uintptr_t *
find(const char *mod, const char *name)
{
	uintptr_t *(*create)(const char *, int *);
	uintptr_t *found;

	*(void **)&create = dlsym(dlopen(mod, RTLD_NOLOAD | RTLD_LAZY),
	                          "CreateInterface");
	if (create) {
		found = create(name, 0);
		if (found)
			return found;
	}
	fatal("Failed to find %s interface!", name);

	return NULL; /* NOT REACHED */
}

void
intf_init(void)
{
	intf = ecalloc(1, sizeof(Intf));

	intf->client = find("csgo/bin/linux64/client_client.so", "VClient018");
}
