#ifndef NV_H
#define NV_H

#include <stdint.h>

#define NV_DECL(name, type) type *ent_##name(uintptr_t ent)
#define NV_IMPL(name, class, var, offset, type)                         \
        type *                                                          \
        ent_##name(uintptr_t ent)                                       \
        {                                                               \
                return (type *)(ent + nv_get(class "->" var) + offset); \
        }

void nv_init(void);
unsigned int nv_get(const char *hash);
void nv_clean(void);

#endif /* NV_H */
