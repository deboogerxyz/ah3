#ifndef WEAPONINFO_H_
#define WEAPONINFO_H_

#include "../util.h"

typedef enum {
	WEAPONTYPE_KNIFE,
	WEAPONTYPE_PISTOL,
	WEAPONTYPE_SMG,
	WEAPONTYPE_RIFLE,
	WEAPONTYPE_SHOTGUN,
	WEAPONTYPE_SNIPER,
	WEAPONTYPE_MACHINEGUN,
	WEAPONTYPE_C4,
	WEAPONTYPE_PLACEHOLDER,
	WEAPONTYPE_GRENADE,
	WEAPONTYPE_UNKNOWN,
	WEAPONTYPE_STACKABLE,
	WEAPONTYPE_FISTS,
	WEAPONTYPE_BREACHCHARGE,
	WEAPONTYPE_BUMPMINE,
	WEAPONTYPE_TABLET,
	WEAPONTYPE_MELEE
} WeaponType;

typedef struct {
	PAD(32);
	int maxclip;
	PAD(204);
	const char* name;
	PAD(72);
	int type;
	PAD(4);
	int price;
	PAD(12);
	float cycletime;
	PAD(12);
	char fullauto;
	PAD(3);
	int damage;
	float headshotmultiplier;
	float armorratio;
	int bullets;
	float penetration;
	PAD(8);
	float range;
	float rangemod;
	PAD(16);
	char silencer;
	PAD(23);
	float maxspeed;
	float maxspeedalt;
	PAD(100);
	float recoilmagnitude;
	float recoilmagnitudealt;
	PAD(16);
	float recoverytimestand;
} WeaponInfo;

#endif /* WEAPONINFO_H_ */
