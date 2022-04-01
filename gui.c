#include "cfg.h"
#include "hax/bt.h"
#include "hax/misc.h"
#include "sdk.h"

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
			sdk_resetinputstate();
		ctx->style.cursor_visible = open;
	}
}


void
gui_render(struct nk_context *ctx)
{
	int flags = NK_WINDOW_BORDER | NK_WINDOW_TITLE | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE;

	if (nk_begin(ctx, "ah3", nk_rect(50, 50, 230, 250), flags)) {
		bt_drawgui(ctx);
		misc_drawgui(ctx);
		cfg_drawgui(ctx);
	}
	nk_end(ctx);

	nk_window_show(ctx, "ah3", open);
}
