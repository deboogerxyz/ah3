#include <GL/glew.h>
#include <stdlib.h> /* malloc, ... */
#include <string.h> /* memcpy */

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#include "deps/nuklear.h"
#include "deps/nuklear_sdl_gl3.h"

#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024

#include "hax/bt.h"
#include "intf.h"
#include "mem.h"
#include "sdk.h"
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
static int open = 1;
static Vmt client, clientmode;

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

	if (result && nk_sdl_handle_event(event) && open)
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

	struct nk_input *in = &ctx->input;

	if (nk_input_is_key_released(in, NK_KEY_TEXT_START)) {
		open = !open;
		if (!open)
			sdk_resetinputstate();
		ctx->style.cursor_visible = open;
	}

	int flags = NK_WINDOW_BORDER | NK_WINDOW_TITLE;
	if (nk_begin(ctx, "helo?", nk_rect(50, 50, 230, 250), flags)) {
		nk_layout_row_dynamic(ctx, 30, 2);
		nk_button_label(ctx, "helo word");
	}
	nk_end(ctx);

	flags |= NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE;
	if (nk_begin(ctx, "ah3", nk_rect(50, 50, 230, 250), flags)) {
		nk_layout_row_dynamic(ctx, 30, 2);
		nk_button_label(ctx, "Hello, world!");
	}
	nk_end(ctx);

	nk_window_show(ctx, "ah3", open);

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

	if (!cmd->cmdnumber)
		return result;
	
	sdk_getservertime(cmd);

	bt_run(cmd);

	cmd->viewangles = vec_norm(cmd->viewangles);
	cmd->viewangles.x = CLAMP(cmd->viewangles.x, -89.f, 89.f);
	cmd->viewangles.y = CLAMP(cmd->viewangles.y, -180.f, 180.f);
	cmd->viewangles.z = 0.f;
	cmd->forwardmove = CLAMP(cmd->forwardmove, -450.f, 450.f);
	cmd->sidemove = CLAMP(cmd->sidemove, -450.f, 450.f);

	return 0;
}

static void
framestagenotify(void *this, FrameStage stage)
{
	static int once;

	if (!once) {
		bt_init();
		once = 1;
	}

	bt_update(stage);

	VFN(void (*)(uintptr_t *, FrameStage), client.old, 37)(intf->client, stage);
}

void
hk_init(void)
{
	*(void **)&origpollevent = *(void **)mem->pollevent;
	*(void ***)mem->pollevent = (void *)pollevent;

	*(void **)&origswapwindow = *(void **)mem->swapwindow;
	*(void ***)mem->swapwindow = (void *)swapwindow;

	hookvmt((uintptr_t)intf->client, &client);
	hookfn(&client, 37, &framestagenotify);

	hookvmt((uintptr_t)mem->clientmode, &clientmode);
	hookfn(&clientmode, 25, &createmove);
}

void
hk_clean(void)
{
	unhookvmt(&client);
	unhookvmt(&clientmode);

	*(void ***)mem->pollevent = (void *)origpollevent;
	*(void ***)mem->swapwindow = (void *)origswapwindow;
}
