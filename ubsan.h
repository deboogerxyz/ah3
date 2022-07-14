#ifndef UBSAN_H_
#define UBSAN_H_

#include <stdint.h>

typedef struct {
	const char *filename;
	uint32_t line;
	uint32_t col;
} SourceLocation;

typedef struct {
	uint16_t kind;
	uint16_t info;
	char name[1];
} TypeDescriptor;

typedef struct {
	SourceLocation loc;
	TypeDescriptor *type;
} InvalidValueData;

typedef struct {
	SourceLocation loc;
	SourceLocation attrloc;
	int argidx;
} NonnullArgData;

typedef struct {
	SourceLocation attrloc;
} NonnullReturnData;

typedef struct {
	SourceLocation loc;
	TypeDescriptor *type;
} OverflowData;

typedef struct {
	SourceLocation loc;
	TypeDescriptor *type;
} VLABoundData;

typedef struct {
	SourceLocation loc;
	TypeDescriptor *ltype;
	TypeDescriptor *rtype;
} ShiftOutOfBoundsData;

typedef struct {
	SourceLocation loc;
	TypeDescriptor *arrtype;
	TypeDescriptor *idxtype;
} OutOfBoundsData;

typedef struct {
	SourceLocation loc;
	TypeDescriptor *type;
	uint8_t logalignment;
	uint8_t typecheckkind;
} TypeMismatchData;

typedef struct {
	SourceLocation loc;
	SourceLocation assumptionloc;
	TypeDescriptor *type;
} AlignmentAssumptionData;

typedef struct {
	SourceLocation loc;
} UnreachableData;

typedef struct {
	SourceLocation loc;
	TypeDescriptor *fromtype;
	TypeDescriptor *totype;
	unsigned char kind;
} ImplicitConversionData;

typedef struct {
	SourceLocation loc;
	unsigned char kind;
} InvalidBuiltinData;

typedef struct {
	SourceLocation loc;
} PointerOverflowData;

typedef struct {
	SourceLocation loc;
	TypeDescriptor *fromtype;
	TypeDescriptor *totype;
} FloatCastOverflowData;

#endif /* UBSAN_H_ */
