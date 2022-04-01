#ifndef RECV_H_
#define RECV_H_

#include "../util.h"
#include "vec.h"

typedef struct {
	PAD(8);
	union {
		float f;
		long i;
		char *s;
		void *data;
		Vector v;
		int64_t i64;
	} val;
} RecvProxyData;

typedef void (*RecvProxy)(RecvProxyData *, void *, void *);

typedef struct RecvTable RecvTable;
typedef struct {
	char *name;
	int type;
	PAD(36);
	RecvProxy proxy;
	PAD(8);
	RecvTable *table;
	int offset;
	PAD(16);
} RecvProp;

struct RecvTable {
	RecvProp *props;
	int count;
	PAD(8);
	char *name;
	PAD(2);
};

#endif /* RECV_H_ */
