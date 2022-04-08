#ifndef TRACE_H_
#define TRACE_H_

#include "mat3x4.h"
#include "../util.h"
#include "vec.h"

typedef struct {
	Vector start;
	PAD(4);
	Vector delta;
	PAD(4);
	Vector startoffset;
	PAD(4);
	Vector extents;
	PAD(4);
	Matrix3x4 *worldaxistransform;
	char isray;
	char isswept;
} Ray;

typedef struct {
	void *vmt;
	uintptr_t skip;
} TraceFilter;

typedef struct {
	const char *name;
	short surfaceprops;
	unsigned short flags;
} Surface;

typedef struct {
	Vector start;
	Vector end;
	PAD(20);
	float fraction;
	int contents;
	unsigned short dispflags;
	char allsolid;
	char startsolid;
	PAD(4);
	Surface surface;
	int hitgroup;
	PAD(4);
	uintptr_t ent;
	int hitbox;
} Trace;

void trace_initfilter(TraceFilter *filter);
void trace_traceray(Ray *ray, unsigned int mask, TraceFilter *filter, Trace *trace);

#endif /* TRACE_H_ */
