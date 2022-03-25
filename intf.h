/* Requires stdint.h */

typedef struct {
	uintptr_t *client;
} Intf;

void intf_init(void);

extern Intf *intf;
