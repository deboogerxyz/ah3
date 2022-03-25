/* Requires stdint.h */

#define VMT(X) (*((uintptr_t **)(X)))
#define VFN(type, vmt, i, args) \
        (((type)(vmt)[i])args)

unsigned int hash(const char *data);
