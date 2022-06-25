#include "sdk/mat3x4.h"
#include <GL/glew.h>
#include <stdint.h>
#include <stdlib.h> /* malloc, ... */
#include <string.h> /* memcpy */

#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024

#include "hax/bt.h"
#include "cfg.h"
#include "hax/chams.h"
#include "hax/glow.h"
#include "gui.h"
#include "intf.h"
#include "hax/legitbot.h"
#include "mem.h"
#include "sdk/material.h"
#include "sdk/matsystem.h"
#include "hax/misc.h"
#include "sdk/engine.h"
#include "sdk/ent.h"
#include "sdk/entlist.h"
#include "sdk/studiorender.h"
#include "hax/visuals.h"
#include "sdk/viewsetup.h"
#include "util.h"

#include "hk.h"

typedef struct {
	uintptr_t addr;
	uintptr_t *old;
	uintptr_t *new;
	size_t size;
} Vmt;

static int (*origpollevent)(SDL_Event *);
static void (*origswapwindow)(SDL_Window *);
static Vmt bspquery, client, clientmode, engine, modelrender;

static size_t
getvmtsize(uintptr_t *vmt)
{
	int i = 0;

	while (vmt[i])
		i++;

	return i;
}

static void
hookvmt(uintptr_t addr, Vmt *vmt)
{
	vmt->addr = addr;
	vmt->old  = VMT(addr);
	vmt->size = getvmtsize(vmt->old) + 2;

	uintptr_t *new = malloc(vmt->size * sizeof(uintptr_t));
	if (!new)
		return;

	vmt->new = new;
	memcpy(vmt->new, vmt->old - 2, vmt->size * sizeof(uintptr_t));
	VMT(vmt->addr) = vmt->new + 2;
}

static void
hookfn(Vmt *vmt, int i, uintptr_t addr)
{
	vmt->new[i + 2] = addr;
}

static void
unhookvmt(Vmt *vmt)
{
	if (!vmt->new)
		return;

	VMT(vmt->addr) = vmt->old;
	free(vmt->new);
}

static int
pollevent(SDL_Event *event)
{
	int result = origpollevent(event);

	if (result && nk_sdl_handle_event(event) && gui_isopen())
		event->type = 0;

	return result;
}

static void
swapwindow(SDL_Window *win)
{
	static struct nk_context *ctx;
	static struct nk_font_atlas *atlas;
	static SDL_GLContext origglctx, glctx;

	if (!ctx) {
		origglctx = SDL_GL_GetCurrentContext();
		glctx = SDL_GL_CreateContext(win);

		glewInit();

		ctx = nk_sdl_init(win);
		nk_sdl_font_stash_begin(&atlas);
		nk_sdl_font_stash_end();
		nk_style_load_all_cursors(ctx, atlas->cursors);
	}

	SDL_GL_MakeCurrent(win, glctx);

	gui_handletoggle(ctx);
	gui_render(ctx, win);

	nk_sdl_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY);

	SDL_GL_MakeCurrent(win, origglctx);

	nk_input_begin(ctx);
	origswapwindow(win);
	nk_input_end(ctx);
}

static char
createmove(void *this, float inputsampletime, UserCmd *cmd)
{
	char result = VFN(char (*)(void *, float, UserCmd *), clientmode.old, 25)(mem->clientmode, inputsampletime, cmd);

	if (!cmd->commandnumber)
		return result;

	const Vector currentangles = cmd->viewangles;

	globalvars_getservertime(cmd);

	misc_antiafk(cmd);
	misc_bhop(cmd);
	misc_clantagchanger();
	misc_fastduck(cmd);
	misc_slidewalk(cmd);
	misc_faststop(cmd);
	visuals_revealranks(cmd);
	legitbot_run(cmd);
	bt_run(cmd);

	cmd->viewangles = vec_norm(cmd->viewangles);
	misc_fixmovement(cmd, currentangles.y);

	cmd->viewangles.x = CLAMP(cmd->viewangles.x, -89.f, 89.f);
	cmd->viewangles.y = CLAMP(cmd->viewangles.y, -180.f, 180.f);
	cmd->viewangles.z = 0.f;

	cmd->forwardmove = CLAMP(cmd->forwardmove, -450.f, 450.f);
	cmd->sidemove    = CLAMP(cmd->sidemove, -450.f, 450.f);

	return 0;
}

static void
dopostscreeneffects(void *this, void *param)
{
	glow_render();

	VFN(void (*)(void *, void *), clientmode.old, 45)(mem->clientmode, param);
}

static char
shoulddrawfog(void *this)
{
	return !cfg->visuals.disablefog;
}

static void
overrideview(void *this, ViewSetup *setup)
{
	uintptr_t localplayer = entlist_getentity(engine_getlocalplayer());
	if (localplayer && !*ent_getisscoped(localplayer)) {
		if (cfg->visuals.overridefov)
			setup->fov = cfg->visuals.fov;

		if (cfg->visuals.viewmodel.enabled) {
			Vector x90 = {90, 0, 0};

			Vector forward = vec_fromang(setup->angles);
			Vector up      = vec_fromang(vec_sub(setup->angles, x90));
			Vector side    = vec_crossprod(forward, up);
			Vector offset  = vec_add(vec_add(vec_mul(side, cfg->visuals.viewmodel.x), vec_mul(forward, cfg->visuals.viewmodel.y)), vec_mul(up, cfg->visuals.viewmodel.z));

			uintptr_t viewmodel = entlist_getentityfromhandle(ent_getviewmodel(localplayer));
			uintptr_t weapon    = ent_getactiveweapon(localplayer);

			if (viewmodel && weapon) {
				ClassId class = ent_getclientclass(weapon)->classid;

				if (class != CLASSID_TABLET) {
					Vector angle = vec_add(*ent_getrenderorigin(viewmodel), offset);
					mem->setabsorigin(viewmodel, &angle);
				}
			}
		}
	}


	VFN(void (*)(uintptr_t *, ViewSetup *), clientmode.old, 19)(mem->clientmode, setup);
}

static void
framestagenotify(void *this, FrameStage stage)
{
	static int once;
	if (!once) {
		bt_init();
		once = 1;
	}

	visuals_disablepostprocessing(stage);
	visuals_disableshadows();
	visuals_forcecrosshair(stage);
	visuals_grenadeprediction(stage);
	visuals_oppositehandknife(stage);
	visuals_remove3dsky(stage);
	bt_update(stage);

	VFN(void (*)(uintptr_t *, FrameStage), client.old, 37)(intf->client, stage);
}

static char
isplayingdemo(void *this)
{
	if (cfg->visuals.revealmoney && (uintptr_t)__builtin_return_address(0) == mem->demoorhltv && *(uintptr_t *)((uintptr_t)__builtin_frame_address(0) + 24) == mem->money)
		return 1;

	return VFN(char (*)(void *), engine.old, 82)(intf->engine);
}

static void
drawmodelexecute(void *this, void *ctx, void *state, ModelRenderInfo *info, Matrix3x4 *custombonetoworld)
{
	if (!chams_render(ctx, state, info, custombonetoworld))
		hk_orig_drawmodelexecute(ctx, state, info, custombonetoworld);

	studiorender_forcedmatoverride(0, OVERRIDETYPE_NORMAL, -1);
}

typedef struct {
	uintptr_t renderable;
	PAD(18);
	uint16_t flags;
	uint16_t flags2;
} RenderableInfo;

static int
listleavesinbox(void *this, Vector *mins, Vector *maxs, unsigned short *list, int listmax)
{
	if ((uintptr_t)__builtin_return_address(0) != mem->insertintotree)
		goto skip;

	RenderableInfo *info = *(RenderableInfo **)((uintptr_t)__builtin_frame_address(0) + 0x10 + 0x948);
	if (!info || !info->renderable)
		goto skip;

	uintptr_t ent = VFN(uintptr_t (*)(uintptr_t), VMT(info->renderable - sizeof(uintptr_t)), 8)(info->renderable - sizeof(uintptr_t));
	if (!ent || !ent_isplayer(ent))
		goto skip;

	const float maxcoord = 16384.0f;
	const float mincoord = -maxcoord;

	Vector min = {
		.x = mincoord,
		.y = mincoord,
		.z = mincoord
	};

	Vector max = {
		.x = maxcoord,
		.y = maxcoord,
		.z = maxcoord
	};

	return VFN(int (*)(void *, Vector *, Vector *, unsigned short *list, int), bspquery.old, 6)(bspquery.old, &min, &max, list, listmax);

skip:
	return VFN(int (*)(void *, Vector *, Vector *, unsigned short *list, int), bspquery.old, 6)(bspquery.old, mins, maxs, list, listmax);
}

void
hk_orig_drawmodelexecute(void *ctx, void *state, ModelRenderInfo *info, Matrix3x4 *custombonetoworld)
{
	VFN(void (*)(void *, void *, void *, ModelRenderInfo *, Matrix3x4 *), modelrender.old, 21)(intf->modelrender, ctx, state, info, custombonetoworld);
}

void
hk_init(void)
{
	*(void **)&origpollevent = *(void **)mem->pollevent;
	*(void ***)mem->pollevent = (void *)pollevent;

	*(void **)&origswapwindow = *(void **)mem->swapwindow;
	*(void ***)mem->swapwindow = (void *)swapwindow;

	hookvmt(engine_getbsptreequery(), &bspquery);
	hookfn(&bspquery, 6, &listleavesinbox);

	hookvmt((uintptr_t)intf->client, &client);
	hookfn(&client, 37, &framestagenotify);

	hookvmt((uintptr_t)mem->clientmode, &clientmode);
	hookfn(&clientmode, 18, &shoulddrawfog);
	hookfn(&clientmode, 19, &overrideview);
	hookfn(&clientmode, 25, &createmove);
	hookfn(&clientmode, 45, &dopostscreeneffects);

	hookvmt((uintptr_t)intf->engine, &engine);
	hookfn(&engine, 82, &isplayingdemo);

	hookvmt((uintptr_t)intf->modelrender, &modelrender);
	hookfn(&modelrender, 21, &drawmodelexecute);
}

void
hk_clean(void)
{
	unhookvmt(&modelrender);
	unhookvmt(&engine);
	unhookvmt(&clientmode);
	unhookvmt(&client);
	unhookvmt(&bspquery);

	*(void ***)mem->swapwindow = (void *)origswapwindow;
	*(void ***)mem->pollevent = (void *)origpollevent;
}
