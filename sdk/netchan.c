#include "../util.h"

#include "netchan.h"

const char *
netchan_getserveraddr(uintptr_t netchan)
{
	return VFN(const char *(*)(uintptr_t), VMT(netchan), 1)(netchan);
}

float
netchan_getlatency(uintptr_t netchan, int flow)
{
	return VFN(float (*)(uintptr_t, int), VMT(netchan), 9)(netchan, flow);
}
