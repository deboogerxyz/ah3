#include "cfg.h"
#include "hax/bt.h"
#include "hax/chams.h"
#include "hax/glow.h"
#include "hax/legitbot.h"
#include "hax/misc.h"
#include "hax/visuals.h"
#include "sdk/inputsys.h"

#include "gui.h"

static int open = 1;

int
gui_isopen(void)
{
	return open;
}

void
gui_handletoggle(struct nk_context *ctx)
{
	struct nk_input *in = &ctx->input;

	if (nk_input_is_key_released(in, NK_KEY_TEXT_START)) {
		open = !open;
		if (!open)
			inputsys_resetinputstate();
		ctx->style.cursor_visible = open;
	}
}

void
gui_colorpicker(struct nk_context *ctx, struct nk_colorf *color)
{
	nk_layout_row_dynamic(ctx, 120, 1);
	*color = nk_color_picker(ctx, *color, NK_RGBA);

	nk_layout_row_dynamic(ctx, 25, 1);
	color->r = nk_propertyf(ctx, "#Red:", 0, color->r, 1.0f, 0.01f, 0.005f);
	color->g = nk_propertyf(ctx, "#Green:", 0, color->g, 1.0f, 0.01f, 0.005f);
	color->b = nk_propertyf(ctx, "#Blue:", 0, color->b, 1.0f, 0.01f, 0.005f);
	color->a = nk_propertyf(ctx, "#Alpha:", 0, color->a, 1.0f, 0.01f, 0.005f);
}

void
gui_render(struct nk_context *ctx, SDL_Window *win)
{
	int flags = NK_WINDOW_BORDER | NK_WINDOW_TITLE | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE;

	int sdlw, sdlh;
	SDL_GetWindowSize(win, &sdlw, &sdlh);

	const float w = 300.0f;
	const float h = 450.0f;

	float x = (float)sdlw / 2 - w / 2;
	float y = (float)sdlh / 2 - h / 2;

	if (nk_begin(ctx, "ah3", nk_rect(x, y, w, h), flags)) {
		ragebot_drawgui(ctx);
		legitbot_drawgui(ctx);
		bt_drawgui(ctx);
		glow_drawgui(ctx);
		chams_drawgui(ctx);
		visuals_drawgui(ctx);
		misc_drawgui(ctx);
		cfg_drawgui(ctx);
	}
	nk_end(ctx);

	nk_window_show(ctx, "ah3", open);
}
