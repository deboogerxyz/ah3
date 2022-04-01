#include "convar.h"

float
convar_getfloat(ConVar *var)
{
	return VFN(float (*)(ConVar *), VMT(var), 15)(var);
}

int
convar_getint(ConVar *var)
{
	return VFN(int (*)(ConVar *), VMT(var), 16)(var);
}


void
convar_setstr(ConVar *var, const char *str)
{
	VFN(void (*)(ConVar *, const char *), VMT(var), 17)(var, str);
}

void
convar_setfloat(ConVar *var, float val)
{
	VFN(void (*)(ConVar *, float), VMT(var), 18)(var, val);
}

void
convar_setint(ConVar *var, int val)
{
	VFN(void (*)(ConVar *, int), VMT(var), 19)(var, val);
}
