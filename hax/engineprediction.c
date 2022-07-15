#include "../sdk/engine.h"
#include "../sdk/ent.h"
#include "../sdk/entlist.h"
#include "../sdk/gamemovement.h"
#include "../sdk/globalvars.h"
#include "../mem.h"
#include "../sdk/movehelper.h"
#include "../sdk/prediction.h"
#include "../sdk/usercmd.h"

#include "engineprediction.h"

int engineprediction_flags = 0;
Vector engineprediction_velocity = {0};

void
engineprediction_run(UserCmd *cmd)
{
	uintptr_t localplayer = entlist_getentity(engine_getlocalplayer());
	if (!localplayer)
		return;

	engineprediction_flags = *ent_getflags(localplayer);
	engineprediction_velocity = *ent_getvelocity(localplayer);

	*mem->predictrandomseed = 0;

	float oldcurtime   = mem->globalvars->currenttime;
	float oldframetime = mem->globalvars->frametime;

	mem->globalvars->currenttime = globalvars_getservertime(cmd);
	mem->globalvars->frametime   = mem->globalvars->intervalpertick;

	gamemovement_starttrackpredicterrors(localplayer);
	movehelper_sethost(localplayer);
	prediction_setup(localplayer, cmd, mem->movehelper, mem->movedata);
	gamemovement_process(localplayer, mem->movedata);
	prediction_finish(localplayer, cmd, mem->movedata);
	gamemovement_finishtrackpredicterrors(localplayer);
	movehelper_sethost(0);

	*mem->predictrandomseed = -1;

	mem->globalvars->currenttime = oldcurtime;
	mem->globalvars->frametime   = oldframetime;

	uintptr_t weapon = ent_getactiveweapon(localplayer);
	if (!weapon)
		return;

	ent_updateaccuracypenalty(weapon);
}
