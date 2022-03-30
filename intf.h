#ifndef INTF_H_
#define INTF_H_

#include <stdint.h>

typedef struct {
	uintptr_t *client;
	uintptr_t *cvar;
	uintptr_t *engine;
	uintptr_t *entlist;
	uintptr_t *inputsys;
} Intf;

void intf_init(void);
void intf_clean(void);

extern Intf *intf;

#endif /* INTF_H_ */
