#ifndef GUI_H_
#define GUI_H_

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#include "deps/nuklear.h"
#include "deps/nuklear_sdl_gl3.h"

int gui_isopen(void);
void gui_handletoggle(struct nk_context *ctx);
void gui_render(struct nk_context *ctx);

#endif /* GUI_H_ */
