#ifndef UTLVEC_H_
#define UTLVEC_H_

#include <stdint.h>

typedef struct {
	uintptr_t memory;
	int count;
	int growsize;
	int size;
	uintptr_t elements;
} UtlVector;

#endif /* UTLVEC_H_ */
