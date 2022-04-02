#include "ent.h"

ClientClass *
ent_getclientclass(uintptr_t ent)
{
	return VFN(ClientClass *(*)(uintptr_t), VMT(ent + sizeof(uintptr_t) * 2), 2)(ent + sizeof(uintptr_t) * 2);
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

int
ent_isweapon(uintptr_t ent)
{
	return VFN(char (*)(uintptr_t), VMT(ent), 218)(ent);
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

Vector
ent_getbonepos(uintptr_t ent, int bone)
{
	Matrix3x4 m[256] = {0};

	ent_setupbones(ent, m, 256, 256, 0.0f);

	return mat_origin(m[bone]);
}

NV_IMPL(movetype, "CBaseEntity", "m_nRenderMode", 1, MoveType)
NV_IMPL(simtime, "CBaseEntity", "m_flSimulationTime", 0, float)
NV_IMPL(health, "CBasePlayer", "m_iHealth", 0, int)
NV_IMPL(flags, "CBasePlayer", "m_fFlags", 0, int)
NV_IMPL(spottedbymask, "CBaseEntity", "m_bSpottedByMask", 0, long)
NV_IMPL(tickbase, "CBasePlayer", "m_nTickBase", 0, int)
NV_IMPL(isscoped, "CCSPlayer", "m_bIsScoped", 0, char)
NV_IMPL(immunity, "CCSPlayer", "m_bGunGameImmunity", 0, char)
NV_IMPL(shotsfired, "CCSPlayer", "m_iShotsFired", 0, int)
NV_IMPL(waitfornoattack, "CCSPlayer", "m_bWaitForNoAttack", 0, char)
NV_IMPL(nextattack, "CBaseCombatCharacter", "m_flNextAttack", 0, float)
NV_IMPL(itemindex, "CBaseAttributableItem", "m_iItemDefinitionIndex", 0, short)
