#include <dlfcn.h> /* dlopen, dlsym, ... */
#include <stddef.h> /* Required by util.h */

#include "util.h"
#include "mem.h"

Mem *mem;

void
mem_init(void)
{
	void *tier0;

	mem = ecalloc(1, sizeof(Mem));

	tier0 = dlopen("libtier0_client.so", RTLD_NOLOAD | RTLD_NOW);
	*(void **)&mem->debugmsg = dlsym(tier0, "Msg");
	dlclose(tier0);
}
