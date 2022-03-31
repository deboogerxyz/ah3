#include "cfg.h"
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
	int flags = NK_WINDOW_BORDER | NK_WINDOW_TITLE | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE | NK_WINDOW_MINIMIZABLE;

	if (nk_begin(ctx, "ah3", nk_rect(50, 50, 230, 250), flags)) {
		if (nk_tree_push(ctx, NK_TREE_TAB, "Backtrack", NK_MINIMIZED)) {
			nk_checkbox_label(ctx, "Enabled", &cfg->bt.enabled);
			nk_property_int(ctx, "Time limit [ms]", 0, &cfg->bt.limit, 200, 1, 1);

			nk_tree_pop(ctx);
		}
		if (nk_tree_push(ctx, NK_TREE_TAB, "Config", NK_MINIMIZED)) {
			static char buf[256];

			nk_edit_string_zero_terminated(ctx, NK_EDIT_FIELD, buf, sizeof(buf), nk_filter_ascii);

			if (nk_button_label(ctx, "Load"))
				cfg_load(buf);

			if (nk_button_label(ctx, "Save"))
				cfg_save(buf);

			if (nk_button_label(ctx, "Delete"))
				cfg_delete(buf);

			nk_tree_pop(ctx);
		}
	}
	nk_end(ctx);

	nk_window_show(ctx, "ah3", open);
}
