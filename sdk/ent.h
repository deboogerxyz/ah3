#ifndef ENT_H_
#define ENT_H_

#include <stdint.h>

#include "clientclass.h"
#include "mat3x4.h"
#include "../nv.h"
#include "weaponinfo.h"

ClientClass *ent_getclientclass(uintptr_t ent);
int ent_setupbones(uintptr_t ent, Matrix3x4 *out, int max, int mask, float curtime);
int ent_isdormant(uintptr_t ent);
Vector *ent_getabsorigin(uintptr_t ent);
int ent_isalive(uintptr_t ent);
int ent_isweapon(uintptr_t ent);
Vector ent_geteyepos(uintptr_t ent);
Vector ent_getaimpunch(uintptr_t ent);
uintptr_t ent_getactiveweapon(uintptr_t ent);
WeaponInfo *ent_getweaponinfo(uintptr_t ent);
Vector ent_getbonepos(uintptr_t ent, int bone);

NV_DECL(simtime, float);
NV_DECL(flags, int);
NV_DECL(spottedbymask, long);
NV_DECL(tickbase, int);
NV_DECL(isscoped, char);

#endif /* ENT_H_ */
