#include "../util.h"

#include "material.h"

const char *
material_getname(uintptr_t material)
{
	return VFN(const char *(*)(uintptr_t), VMT(material), 0)(material);
}

void
material_alphamodulate(uintptr_t material, float alpha)
{
	VFN(void (*)(uintptr_t, float), VMT(material), 27)(material, alpha);
}

void
material_colormodulate(uintptr_t material, Color col)
{
	VFN(void (*)(uintptr_t, float, float, float), VMT(material), 28)(material, col.r, col.g, col.b);
}

void
material_setvarflag(uintptr_t material, MaterialVarFlag flag, char on)
{
	VFN(void (*)(uintptr_t, MaterialVarFlag, char), VMT(material), 29)(material, flag, on);
}
