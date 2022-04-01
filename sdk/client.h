#ifndef CLIENT_H_
#define CLIENT_H_

#include "clientclass.h"
#include "recv.h"
#include "../util.h"

ClientClass *client_getallclasses(void);
int sdk_dispatchusermsg(int type, int flags, int size, void *data);

#endif /* CLIENT_H_ */
