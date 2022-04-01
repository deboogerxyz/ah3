#include "engine.h"
#include "ent.h"
#include "entlist.h"
#include "../mem.h"

#include "globalvars.h"

float
globalvars_getservertime(UserCmd *cmd)
{
	static int tick;
	static UserCmd *lastcmd;

	if (cmd) {
		uintptr_t localplayer = entlist_getentity(engine_getlocalplayer());
		if (localplayer && (!lastcmd || lastcmd->hasbeenpredicted))
			tick = *ent_gettickbase(localplayer);
		else
			tick++;
		lastcmd = cmd;
	}

	return tick * mem->globalvars->intervalpertick;
}
