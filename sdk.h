#ifndef SDK_H_
#define SDK_H_

#include <stdint.h>

#include "nv.h"
#include "util.h"

typedef struct {
	float x, y, z;
} Vector;

typedef struct {
	PAD(8);
	union {
		float f;
		long i;
		char* s;
		void* data;
		Vector v;
		int64_t i64;
	} val;
} RecvProxyData;

typedef void (*RecvProxy)(RecvProxyData *, void *, void *);

typedef struct {
	char *name;
	int type;
	PAD(36);
	RecvProxy proxy;
	PAD(8);
	struct RecvTable *table;
	int offset;
	PAD(16);
} RecvProp;

typedef struct RecvTable {
	RecvProp *props;
	int count;
	PAD(8);
	char *name;
	PAD(2);
} RecvTable;

typedef enum {
	CLASSID_BASECSGRENADEPROJECTILE = 9,
	CLASSID_BREACHCHARGEPROJECTILE = 29,
	CLASSID_BUMPMINEPROJECTILE = 33,
	CLASSID_C4,
	CLASSID_CHICKEN = 36,
	CLASSID_CSPLAYER = 40,
	CLASSID_CSPLAYERRESOURCE,
	CLASSID_CSRAGDOLL = 42,
	CLASSID_DEAGLE = 46,
	CLASSID_DECOYPROJECTILE = 48,
	CLASSID_DRONE,
	CLASSID_DRONEGUN,
	CLASSID_DYNAMICPROP = 52,
	CLASSID_ECONENTITY = 53,
	CLASSID_ECONWEARABLE,
	CLASSID_HOSTAGE = 97,
	CLASSID_INFERNO = 100,
	CLASSID_HEALTHSHOT = 104,
	CLASSID_CASH,
	CLASSID_KNIFE = 107,
	CLASSID_KNIFEGG,
	CLASSID_MOLOTOVPROJECTILE = 114,
	CLASSID_AMMOBOX = 125,
	CLASSID_LOOTCRATE,
	CLASSID_RADARJAMMER,
	CLASSID_WEAPONUPGRADE,
	CLASSID_PLANTEDC4,
	CLASSID_PROPDOORROTATING = 143,
	CLASSID_SENSORGRENADEPROJECTILE = 153,
	CLASSID_SMOKEGRENADEPROJECTILE = 157,
	CLASSID_SNOWBALLPILE = 160,
	CLASSID_SNOWBALLPROJECTILE,
	CLASSID_TABLET = 172,
	CLASSID_AUG = 232,
	CLASSID_AWP,
	CLASSID_ELITE = 239,
	CLASSID_FIVESEVEN = 241,
	CLASSID_G3SG1,
	CLASSID_GLOCK = 245,
	CLASSID_P2000,
	CLASSID_P250 = 258,
	CLASSID_SCAR20 = 261,
	CLASSID_SG553 = 265,
	CLASSID_SSG08 = 267,
	CLASSID_TEC9 = 269
} ClassId;

typedef struct ClientClass {
	PAD(16);
	char *name;
	RecvTable *table;
	struct ClientClass *next;
	ClassId classid;
} ClientClass;

typedef float Matrix3x4[3][4];

typedef enum {
	IN_ATTACK = 1 << 0,
	IN_JUMP = 1 << 1,
	IN_DUCK = 1 << 2,
	IN_FORWARD = 1 << 3,
	IN_BACK = 1 << 4,
	IN_USE = 1 << 5,
	IN_MOVELEFT = 1 << 9,
	IN_MOVERIGHT = 1 << 10,
	IN_ATTACK2 = 1 << 11,
	IN_SCORE = 1 << 16,
	IN_BULLRUSH = 1 << 22
} UserCmdButtons;

typedef struct {
	PAD(8);
	int cmdnumber;
	int tickcount;
	Vector viewangles;
	Vector aimdirection;
	float forwardmove;
	float sidemove;
	float upmove;
	UserCmdButtons buttons;
	PAD(13);
	short mousedx;
	short mousedy;
	char hasbeenpredicted;
} UserCmd;

typedef enum {
	WEAPONID_NONE,
	WEAPONID_DEAGLE = 1,
	WEAPONID_ELITE,
	WEAPONID_FIVESEVEN,
	WEAPONID_GLOCK,
	WEAPONID_AK47 = 7,
	WEAPONID_AUG,
	WEAPONID_AWP,
	WEAPONID_FAMAS,
	WEAPONID_G3SG1,
	WEAPONID_GALILAR = 13,
	WEAPONID_M249,
	WEAPONID_M4A1 = 16,
	WEAPONID_MAC10,
	WEAPONID_P90 = 19,
	WEAPONID_ZONEREPULSOR,
	WEAPONID_MP5SD = 23,
	WEAPONID_UMP45,
	WEAPONID_XM1014,
	WEAPONID_BIZON,
	WEAPONID_MAG7,
	WEAPONID_NEGEV,
	WEAPONID_SAWEDOFF,
	WEAPONID_TEC9,
	WEAPONID_TASER,
	WEAPONID_HKP2000,
	WEAPONID_MP7,
	WEAPONID_MP9,
	WEAPONID_NOVA,
	WEAPONID_P250,
	WEAPONID_SHIELD,
	WEAPONID_SCAR20,
	WEAPONID_SG553,
	WEAPONID_SSG08,
	WEAPONID_GOLDENKNIFE,
	WEAPONID_KNIFE,
	WEAPONID_FLASHBANG = 43,
	WEAPONID_HEGRENADE,
	WEAPONID_SMOKEGRENADE,
	WEAPONID_MOLOTOV,
	WEAPONID_DECOY,
	WEAPONID_INCGRENADE,
	WEAPONID_C4,
	WEAPONID_HEALTHSHOT = 57,
	WEAPONID_KNIFET = 59,
	WEAPONID_M4A1_S,
	WEAPONID_USP_S,
	WEAPONID_CZ75A = 63,
	WEAPONID_REVOLVER,
	WEAPONID_TAGRENADE = 68,
	WEAPONID_AXE = 75,
	WEAPONID_HAMMER,
	WEAPONID_SPANNER = 78,
	WEAPONID_GHOSTKNIFE = 80,
	WEAPONID_FIREBOMB,
	WEAPONID_DIVERSION,
	WEAPONID_FRAGGRENADE,
	WEAPONID_SNOWBALL,
	WEAPONID_BUMPMINE
} WeaponId;

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
	float headshotmult;
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

typedef struct {
	float realtime;
	int framecount;
	float absoluteframetime;
	PAD(4);
	float currenttime;
	float frametime;
	int maxclients;
	int tickcount;
	float intervalpertick;
} GlobalVars;

typedef enum {
	FS_UNDEFINED = -1,
	FS_START,
	FS_NET_UPDATE_START,
	FS_NET_UPDATE_POSTDATAUPDATE_START,
	FS_NET_UPDATE_POSTDATAUPDATE_END,
	FS_NET_UPDATE_END,
	FS_RENDER_START,
	FS_RENDER_END
} FrameStage;

typedef struct ConVar {
	PAD(56);
	struct ConVar *parent;
	const char *defval;
	const char *str;
} ConVar;

ClientClass *sdk_getallclasses(void);
int sdk_dispatchusermsg(int type, int flags, int size, void *data);
int sdk_istakingscreenshot(void);
int sdk_getlocalplayer(void);
uintptr_t sdk_getentity(int i);
int ent_setupbones(uintptr_t ent, Matrix3x4 *out, int max, int mask, float curtime);
int ent_isdormant(uintptr_t ent);
Vector *ent_getabsorigin(uintptr_t ent);
int ent_isalive(uintptr_t ent);
Vector ent_geteyepos(uintptr_t ent);
Vector ent_getaimpunch(uintptr_t ent);
uintptr_t ent_getactiveweapon(uintptr_t ent);
WeaponInfo *ent_getweaponinfo(uintptr_t ent);
void sdk_setviewangles(Vector *ang);
ConVar *cvar_find(const char *name);
float convar_getfloat(ConVar *var);
int sdk_getmaxclients(void);
int sdk_isingame(void);
uintptr_t sdk_getnetchan(void);
const char *sdk_getserveraddress(uintptr_t netchan);
float sdk_getlatency(uintptr_t netchan, int flow);
void sdk_resetinputstate(void);
float sdk_getservertime(UserCmd *cmd);
Vector vec_add(Vector a, Vector b);
Vector vec_sub(Vector a, Vector b);
Vector vec_toang(Vector v);
Vector vec_norm(Vector v);
Vector vec_calcang(Vector startpos, Vector endpos, Vector ang);
Vector mat_origin(Matrix3x4 m);
Vector ent_getbonepos(uintptr_t ent, int bone);
int sdk_timetoticks(float time);

NV_DECL(simtime, float);
NV_DECL(flags, int);
NV_DECL(spottedbymask, long);
NV_DECL(tickbase, int);

#endif /* SDK_H_ */
