#ifndef INTF_H_
#define INTF_H_

#include <stdint.h>

typedef struct {
	uintptr_t *client;
	uintptr_t *cvar;
	uintptr_t *engine;
	uintptr_t *trace;
	uintptr_t *entlist;
	uintptr_t *inputsys;
	uintptr_t *matsystem;
	uintptr_t *modelrender;
	uintptr_t *studiorender;
} Intf;

void intf_init(void);
void intf_clean(void);

extern Intf *intf;

#endif /* INTF_H_ */
