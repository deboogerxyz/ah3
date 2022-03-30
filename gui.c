#include "sdk.h"

#include "gui.h"

static int open = 1;

int
gui_isopen(void)
{
	(void)open;
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

/* 
 * HACK: Prevents Nuklear from garbage collecting
 *       windows when all are closed
*/

void
gui_dummy(struct nk_context *ctx)
{
	nk_begin(ctx, "dummy", nk_rect(0, 0, 0, 0), 0);
	nk_end(ctx);
}

void
gui_render(struct nk_context *ctx)
{
	int flags = NK_WINDOW_BORDER | NK_WINDOW_TITLE | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE;

	if (nk_begin(ctx, "ah3", nk_rect(50, 50, 230, 250), flags)) {
		nk_layout_row_dynamic(ctx, 30, 2);
		nk_button_label(ctx, "Hello, world!");
	}
	nk_end(ctx);

	nk_window_show_if(ctx, "ah3", NK_SHOWN, open);
	nk_window_show_if(ctx, "ah3", NK_HIDDEN, !open);
}
