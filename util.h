/* Requires stdint.h */

#define VMT(X) (*((uintptr_t **)(X)))

unsigned int hash(const char *data);
