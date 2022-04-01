#ifndef NETCHAN_H_
#define NETCHAN_H_

#include <stdint.h>

const char *netchan_getserveraddr(uintptr_t netchan);
float netchan_getlatency(uintptr_t netchan, int flow);

#endif /* NETCHAN_H_ */
