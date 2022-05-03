#ifndef MODELRENDER_H_
#define MODELRENDER_H_

#include "mat3x4.h"
#include "../util.h"
#include "vec.h"

typedef struct {
	void *handle;
	char name[260];
	int loadflags;
	int servercount;
	int type;
	int flags;
	Vector mins, maxs;
} Model;

typedef struct {
	Vector origin;
	Vector angles;
	PAD(4);
	void *renderable;
	Model *model;
	Matrix3x4 *modeltoworld;
	Matrix3x4 *lightingoffset;
	Vector *lightingorigin;
	int flags;
	int entindex;
} ModelRenderInfo;

#endif /* MODELRENDER_H_ */
