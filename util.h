#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

#define VMT(X) (*((uintptr_t **)(X)))
#define VFN(type, vmt, i) ((type)(vmt)[i])

unsigned int hash(const char *data);

#endif /* UTIL_H */
