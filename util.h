#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

#define VMT(X) (*((uintptr_t **)(X)))
#define VFN(type, vmt, i) ((type)(vmt)[i])
#define MAX(A, B) ((A) > (B) ? (A) : (B))
#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define CLAMP(X, A, B) (MIN((B), MAX((X), (A))))

unsigned int hash(const char *data);

#endif /* UTIL_H */
