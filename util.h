#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

#define VMT(X) (*((uintptr_t **)(X)))
#define VFN(type, vmt, i) ((type)(vmt)[i])

#define MAX(A, B) ((A) > (B) ? (A) : (B))
#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define CLAMP(X, A, B) (MIN((B), MAX((X), (A))))

#define CAT(A, B) A##B
#define PADNAME(X) CAT(pad, X)
#define PAD(X) char PADNAME(__LINE__)[X]

unsigned int hash(const char *data);

#endif /* UTIL_H */
