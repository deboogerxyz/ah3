#ifndef CONVAR_H_
#define CONVAR_H_

#include "../util.h"

typedef struct ConVar ConVar;
struct ConVar {
	PAD(56);
	ConVar *parent;
	const char *defval;
	const char *str;
};

float convar_getfloat(ConVar *var);
int convar_getint(ConVar *var);
void convar_setstr(ConVar *var, const char *str);
void convar_setfloat(ConVar *var, float val);
void convar_setint(ConVar *var, int val);

#endif /* CONVAR_H_ */
