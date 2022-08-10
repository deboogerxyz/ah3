#include <dlfcn.h>  /* dl_iterate_phdr, dlopen, dlsym, ... */
#include <link.h>   /* dl_phdr_info */

#include "deps/cvector.h"
#include "intf.h"
#include "util.h"

#include "mem.h"

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
	if (cvector_empty(libs)) {
		cvector_grow(libs, 150);
		dl_iterate_phdr(callback, NULL);
	}

	for (Lib *it = cvector_begin(libs); it != cvector_end(libs); it++) {
		if (!strstr(it->name, name))
			continue;
		return it;
	}

	return NULL;
}

static uintptr_t
find(const char *libname, const char *ptrn)
{
	Lib *lib = getlibinfo(libname);
	if (!lib || !lib->addr || !lib->size)
		return 0;

	const char *start  = (const char *)lib->addr;
	const char *end    = start + lib->size;
	const char *first  = start;
	const char *second = ptrn;

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
	mem = calloc(1, sizeof(Mem));

	void *tier0 = dlopen("libtier0_client.so", RTLD_NOLOAD | RTLD_NOW);
	*(void **)&mem->debugmsg = dlsym(tier0, "Msg");
	dlclose(tier0);

	void *sdl = dlopen("libSDL2-2.0.so.0", RTLD_LAZY | RTLD_NOLOAD);
	mem->pollevent = reltoabs((uintptr_t)dlsym(sdl, "SDL_PollEvent") + 2);
	mem->swapwindow = reltoabs((uintptr_t)dlsym(sdl, "SDL_GL_SwapWindow") + 2);
	dlclose(sdl);

	*(void **)&mem->isotherenemy      = (void *)reltoabs(find("csgo/bin/linux64/client_client.so", "\xE8????\x84\xC0\x44\x89\xE2") + 1);
	*(void **)&mem->clientmode        = *(void **)reltoabs(reltoabs(VMT(intf->client)[10] + 12) + 4);
	*(void **)&mem->setclantag        = (void *)reltoabs(find("engine_client.so", "\xE8????\xE9????\x66\x0F\x1F\x44??\x48\x8B\x7D\xB0") + 1);
	*(void **)&mem->linegoesthrusmoke = (void *)find("csgo/bin/linux64/client_client.so", "\x55\x48\x89\xE5\x41\x56\x41\x55\x41\x54\x53\x48\x83\xEC\x30\x66\x0F\xD6\x45\xD0");
	*(void **)&mem->globalvars        = *(void **)reltoabs(VMT(intf->client)[11] + 16);
	*(void **)&mem->setabsorigin      = (void *)reltoabs(find("csgo/bin/linux64/client_client.so", "\xE8????\x49\x8B\x07\x31\xF6") + 1);
	mem->insertintotree               = find("csgo/bin/linux64/client_client.so", "\x74\x24\x4C\x8B\x10") + 31;
	*(void **)&mem->glowobjmgr        = (void *)reltoabs(reltoabs(find("csgo/bin/linux64/client_client.so", "\xE8????\x4C\x89\xE7\x8B\x70\x20") + 1) + 12);
	mem->demoorhltv                   = find("csgo/bin/linux64/client_client.so", "\x0F\xB6\x10\x89\xD0") - 16;
	mem->money                        = find("csgo/bin/linux64/client_client.so", "\x84\xC0\x75\x9E\xB8????\xEB\xB9");
	mem->scopedust                    = find("csgo/bin/linux64/client_client.so", "\x8B\x85????\x43\x8D\x14\x2E");
	mem->scopearc                     = find("csgo/bin/linux64/client_client.so", "\x49\x8B\x3C\x24\x8B\xB3????\x48\x8B\x07\xFF\x90????\x49\x8B\x3C\x24\x4C\x89\xEA");
	mem->movedata                     = **(void ***)reltoabs(find("csgo/bin/linux64/client_client.so", "\x4C\x8B\x2D????\x0F\xB6\x93") + 3);
	mem->movehelper                   = **(void ***)reltoabs(find("csgo/bin/linux64/client_client.so", "\x48\x8B\x05????\x44\x89\x85????\x48\x8B\x38") + 3);
	*(void **)&mem->predictrandomseed = *(int **)reltoabs(find("csgo/bin/linux64/client_client.so", "\x41\x8D\x56\xFF\x31\xC9") - 14);
}

void
mem_clean(void)
{
	cvector_free(libs);

	if (mem)
		free(mem);
}
