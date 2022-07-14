#include <dlfcn.h>  /* dlsym, ... */
#include <stdlib.h> /* exit, calloc, ... */

#include "intf.h"

Intf *intf;

static uintptr_t *
find(const char *libname, const char *name)
{
	uintptr_t *(*create)(const char *, int *);

	*(void **)&create = dlsym(dlopen(libname, RTLD_NOLOAD | RTLD_LAZY), "CreateInterface");
	if (create) {
		uintptr_t *found = create(name, 0);
		if (found)
			return found;
	}

	return NULL;
}

void
intf_init(void)
{
	intf = calloc(1, sizeof(Intf));

	intf->client       = find("csgo/bin/linux64/client_client.so", "VClient018");
	intf->cvar         = find("libvstdlib_client.so", "VEngineCvar007");
	intf->engine       = find("engine_client.so", "VEngineClient014");
	intf->trace        = find("engine_client.so", "EngineTraceClient004");
	intf->entlist      = find("csgo/bin/linux64/client_client.so", "VClientEntityList003");
	intf->gamemovement = find("csgo/bin/linux64/client_client.so", "GameMovement001");
	intf->inputsys     = find("inputsystem_client.so", "InputSystemVersion001");
	intf->matsystem    = find("materialsystem_client.so", "VMaterialSystem080");
	intf->modelrender  = find("engine_client.so", "VEngineModel016");
	intf->prediction   = find("csgo/bin/linux64/client_client.so", "VClientPrediction001");
	intf->surface      = find("vguimatsurface_client.so", "VGUI_Surface031");
	intf->studiorender = find("studiorender_client.so", "VStudioRender026");
}

void
intf_clean(void)
{
	if (intf)
		free(intf);
}
