#include <math.h>
#include <stdint.h>

#include "intf.h"
#include "mem.h"
#include "util.h"

ClientClass *
sdk_getallclasses(void)
{
	return VFN(ClientClass *(*)(uintptr_t *), VMT(intf->client), 8)(intf->client);
}

int
sdk_dispatchusermsg(int type, int flags, int size, void *data)
{
	return VFN(char (*)(uintptr_t *, int, int, int, void *), VMT(intf->client), 38)(intf->client, type, flags, size, data);
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
	return VFN(uintptr_t (*)(uintptr_t *, int), VMT(intf->entlist), 3)(intf->entlist, i);
}

int
ent_setupbones(uintptr_t ent, Matrix3x4 *out, int max, int mask, float curtime)
{
	return VFN(char (*)(uintptr_t, Matrix3x4 *, int, int, float), VMT(ent + sizeof(uintptr_t)), 13)(ent + sizeof(uintptr_t), out, max, mask, curtime);
}

int
ent_isdormant(uintptr_t ent)
{
	return VFN(char (*)(uintptr_t), VMT(ent + sizeof(uintptr_t) * 2), 9)(ent + sizeof(uintptr_t) * 2);
}

Vector *
ent_getabsorigin(uintptr_t ent)
{
	return VFN(Vector *(*)(uintptr_t), VMT(ent), 12)(ent);
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
	VFN(void (*)(uintptr_t *, Vector *), VMT(intf->engine), 19)(intf->engine, ang);
}

ConVar *
cvar_find(const char *name)
{
	return VFN(ConVar *(*)(uintptr_t *, const char *), VMT(intf->cvar), 15)(intf->cvar, name);
}

float
convar_getfloat(ConVar *var)
{
	return VFN(float (*)(ConVar *), VMT(var), 15)(var);
}

int
sdk_getmaxclients(void)
{
	return VFN(int (*)(uintptr_t *), VMT(intf->engine), 20)(intf->engine);
}

int
sdk_isingame(void)
{
	return VFN(char (*)(uintptr_t *), VMT(intf->engine), 26)(intf->engine);
}

uintptr_t
sdk_getnetchan(void)
{
	return VFN(uintptr_t (*)(uintptr_t *), VMT(intf->engine), 78)(intf->engine);
}

const char *
sdk_getserveraddress(uintptr_t netchan)
{
	return VFN(const char *(*)(uintptr_t), VMT(netchan), 1)(netchan);
}

float
sdk_getlatency(uintptr_t netchan, int flow)
{
	return VFN(float (*)(uintptr_t, int), VMT(netchan), 9)(netchan, flow);
}

void
sdk_resetinputstate(void)
{
	VFN(void (*)(uintptr_t *), VMT(intf->inputsys), 39)(intf->inputsys);
}

float
sdk_getservertime(UserCmd *cmd)
{
	static int tick;
	static UserCmd *lastcmd;

	if (cmd) {
		uintptr_t localplayer = sdk_getentity(sdk_getlocalplayer());
		if (localplayer && (!lastcmd || lastcmd->hasbeenpredicted))
			tick = *ent_tickbase(localplayer);
		else
			tick++;
		lastcmd = cmd;
	}

	return tick * mem->gvars->intervalpertick;
}

Vector
vec_add(Vector a, Vector b)
{
	Vector c = {
		.x = a.x + b.x,
		.y = a.y + b.y,
		.z = a.z + b.z
	};

	return c;
}

Vector
vec_sub(Vector a, Vector b)
{
	Vector c = {
		.x = a.x - b.x,
		.y = a.y - b.y,
		.z = a.z - b.z
	};

	return c;
}

Vector
vec_toang(Vector v)
{
	Vector a = {
		.x = atan2(-v.z, hypot(v.x, v.y)) * (180.0f / M_PI),
		.y = atan2(v.y, v.x) * (180.0f / M_PI),
		.z = 0.0f
	};

	return a;
}

Vector
vec_norm(Vector v)
{
	Vector a = {
		.x = isfinite(v.x) ? remainder(v.x, 360.0f) : 0.0f,
		.y = isfinite(v.y) ? remainder(v.y, 360.0f) : 0.0f,
		.z = 0.0f
	};

	return a;
}

Vector
vec_calcang(Vector startpos, Vector endpos, Vector ang)
{
	Vector a, b;

	a = vec_sub(endpos, startpos);
	b = vec_sub(vec_toang(a), ang);

	return vec_norm(b);
}

Vector
mat_origin(Matrix3x4 m)
{
	Vector v = {
		.x = m[0][3],
		.y = m[1][3],
		.z = m[2][3]
	};

	return v;
}

Vector
ent_getbonepos(uintptr_t ent, int bone)
{
	Matrix3x4 m[256] = {0};

	ent_setupbones(ent, m, 256, 256, 0.0f);

	return mat_origin(m[bone]);
}

int
sdk_timetoticks(float time)
{
	return (int)(0.5f + time / mem->gvars->intervalpertick);
}

NV_IMPL(simtime, "CBaseEntity", "m_flSimulationTime", 0, float)
NV_IMPL(tickbase, "CBasePlayer", "m_nTickBase", 0, int)
