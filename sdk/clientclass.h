#ifndef CLIENTCLASS_H_
#define CLIENTCLASS_H_

#include "classid.h"
#include "recv.h"

typedef struct ClientClass ClientClass;
struct ClientClass {
	PAD(16);
	char *name;
	RecvTable *table;
	ClientClass *next;
	ClassId classid;
};

#endif /* CLIENTCLASS_H_ */
