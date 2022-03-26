#include <dlfcn.h>  /* dl_iterate_phdr, dlopen, dlsym, ... */
#include <link.h>   /* dl_phdr_info */

#include "cvector.h"
#include "intf.h"
#include "mem.h"
#include "util.h"

typedef struct {
	const char *name;
	uintptr_t addr;
	size_t size;
} Lib;

static cvector_vector_type(Lib) libs = NULL;

Mem *mem;

static int
callback(struct dl_phdr_info *info, size_t size, void *addr)
{
	Lib lib;

	lib.name = info->dlpi_name;
	lib.addr = (uintptr_t)(info->dlpi_addr + info->dlpi_phdr[0].p_vaddr);
	lib.size = info->dlpi_phdr[0].p_memsz;

	cvector_push_back(libs, lib);

	return 0;
}

static Lib *
getlibinfo(const char *name)
{
	Lib *it;

	if (cvector_empty(libs)) {
		cvector_grow(libs, 150);
		dl_iterate_phdr(callback, NULL);
	}

	for (it = cvector_begin(libs); it != cvector_end(libs); it++) {
		if (!strstr(it->name, name))
			continue;
		return it;
	}

	return NULL;
}

static uintptr_t
find(const char *libname, const char *ptrn)
{
	Lib *lib;
	const char *start, *end,
	           *first, *second;

	lib = getlibinfo(libname);
	if (!lib || !lib->addr || !lib->size)
		return 0;

	start = (const char *)lib->addr;
	end   = start + lib->size;
	first = start;
	second = ptrn;

	while (first < end && *second)
		if (*first == *second || *second == '?') {
			first++;
			second++;
		} else {
			first = ++start;
			second = ptrn;
		}

	if (!*second)
		return (uintptr_t)start;

	return 0;
}

static uintptr_t
reltoabs(uintptr_t addr)
{
	return (uintptr_t)(addr + 4 + *(int *)addr);
}

void
mem_init(void)
{
	void *tier0;

	mem = calloc(1, sizeof(Mem));

	tier0 = dlopen("libtier0_client.so", RTLD_NOLOAD | RTLD_NOW);
	*(void **)&mem->debugmsg = dlsym(tier0, "Msg");
	dlclose(tier0);

	*(void **)&mem->isotherenemy = (void *)reltoabs(find(
		            "csgo/bin/linux64/client_client.so",
	                    "\xE8????\x84\xC0\x44\x89\xE2") + 1);
	*(void **)&mem->clientmode = *(void **)reltoabs(
	                    reltoabs(VMT(intf->client)[10] + 12) + 4);
	*(void **)&mem->gvars = *(void **)reltoabs(VMT(intf->client)[11] + 16);
	*(void **)&mem->setabsorigin = (void *)reltoabs(find(
	                    "csgo/bin/linux64/client_client.so",
	                    "\xE8????\x49\x8B\x07\x31\xF6") + 1);
}

void
mem_clean(void)
{
	cvector_free(libs);

	if (mem)
		free(mem);
}
