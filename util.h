#ifndef UTIL_H_
#define UTIL_H_

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdint.h>

#define VMT(X) (*((uintptr_t **)(X)))
#define VFN(type, vmt, i) ((type)(vmt)[i])

#define LEN(X) (sizeof X / sizeof X[0])

#define MAX(A, B) ((A) > (B) ? (A) : (B))
#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define CLAMP(X, A, B) (MIN((B), MAX((X), (A))))

#define CAT(A, B) A##B
#define PADNAME(X) CAT(pad, X)
#define PAD(X) char PADNAME(__LINE__)[X]

#define TIMETOTICKS(X) ((int)(0.5f + (X) / mem->globalvars->intervalpertick))

#define DEGTORAD(X) ((X) * M_PI / 180.0)
#define RADTODEG(X) ((X) * 180.0 / M_PI)

unsigned int hash(const char *data);

#endif /* UTIL_H_ */
