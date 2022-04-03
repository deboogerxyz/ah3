#ifndef ENTLIST_H_
#define ENTLIST_H_

#include <stdint.h>

uintptr_t entlist_getentity(int i);
uintptr_t entlist_getentityfromhandle(int *handle);
int entlist_gethighestidx(void);

#endif /* ENTLIST_H_ */
