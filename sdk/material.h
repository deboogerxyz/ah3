#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <stdint.h>

#include "color.h"

typedef enum {
	MAT_NODRAW = (1 << 2),
	MAT_IGNOREZ = (1 << 15),
	MAT_WIREFRAME = (1 << 28),
} MaterialVarFlag;

const char *material_getname(uintptr_t material);
void material_alphamodulate(uintptr_t material, float alpha);
void material_colormodulate(uintptr_t material, Color col);
void material_setvarflag(uintptr_t material, MaterialVarFlag flag, char on);

#endif /* MATERIAL_H_ */
