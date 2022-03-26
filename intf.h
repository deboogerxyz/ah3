#ifndef INTF_H
#define INTF_H

#include <stdint.h>

typedef struct {
	uintptr_t *client;
	uintptr_t *cvar;
	uintptr_t *engine;
	uintptr_t *entlist;
} Intf;

void intf_init(void);
void intf_clean(void);

extern Intf *intf;

#endif /* INTF_H */
