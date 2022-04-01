#ifndef GLOBALVARS_H_
#define GLOBALVARS_H_

#include "usercmd.h"
#include "../util.h"

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

float globalvars_getservertime(UserCmd *cmd);

#endif /* GLOBALVARS_H_ */
