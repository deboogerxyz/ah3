#include <stdint.h>

#include "intf.h"
#include "mem.h"
#include "util.h"

ClientClass *
sdk_getallclasses(void)
{
	return VFN(ClientClass *(*)(uintptr_t *),
	           VMT(intf->client), 8)(intf->client);
}

int
sdk_dispatchusermsg(int type, int flags, int size, void *data)
{
	return VFN(char (*)(uintptr_t *, int, int, int, void *),
	           VMT(intf->client), 38)(intf->client, type,
	                                  flags, size, data);
}

int
sdk_istakingscreenshot(void)
{
	return VFN(char (*)(void *), VMT(intf->engine), 92)(intf->engine);
}

int
sdk_getlocalplayer(void)
{
	return VFN(int (*)(uintptr_t *), VMT(intf->engine), 12)(intf->engine);
}

uintptr_t
sdk_getentity(int i)
{
	return VFN(uintptr_t (*)(uintptr_t *, int),
	           VMT(intf->entlist), 3)(intf->entlist, i);
}

int
ent_setupbones(uintptr_t ent, Matrix3x4 *out, int max, int mask, float curtime)
{
	return VFN(char (*)(uintptr_t, Matrix3x4 *, int, int, float),
	           VMT(ent + sizeof(uintptr_t)), 13)(ent + sizeof(uintptr_t),
	                                             out, max, mask, curtime);
}

int
ent_isdormant(uintptr_t ent)
{
	return VFN(char (*)(uintptr_t), VMT(ent + sizeof(uintptr_t) * 2), 9)
                   (ent + sizeof(uintptr_t) * 2);
}

int
ent_isalive(uintptr_t ent)
{
	return VFN(char (*)(uintptr_t), VMT(ent), 208)(ent);
}

Vector
ent_geteyepos(uintptr_t ent)
{
	return VFN(Vector (*)(uintptr_t), VMT(ent), 348)(ent);
}

Vector
ent_getaimpunch(uintptr_t ent)
{
	return VFN(Vector (*)(uintptr_t), VMT(ent), 409)(ent);
}

uintptr_t
ent_getactiveweapon(uintptr_t ent)
{
	return VFN(uintptr_t (*)(uintptr_t), VMT(ent), 331)(ent);
}

WeaponInfo *
ent_getweaponinfo(uintptr_t ent)
{
	return VFN(WeaponInfo *(*)(uintptr_t), VMT(ent), 529)(ent);
}

void
sdk_setviewangles(Vector *ang)
{
	VFN(void (*)(uintptr_t *, Vector *), VMT(intf->engine), 19)
	    (intf->engine, ang);
}

float
sdk_getservertime(UserCmd *cmd)
{
	static int tick;
	static UserCmd *lastcmd;
	uintptr_t localplayer;

	if (cmd) {
		localplayer = sdk_getentity(sdk_getlocalplayer());
		if (localplayer && (!lastcmd || lastcmd->hasbeenpredicted))
			tick = *ent_tickbase(localplayer);
		else
			tick++;
		lastcmd = cmd;
	}
	return tick * mem->gvars->intervalpertick;
}

NVDEF(tickbase, "CBasePlayer", "m_nTickBase", 0, int)
