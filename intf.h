/* Requires stdint.h */

typedef struct {
	uintptr_t *client;
} Intf;

void intf_init(void);
void intf_clean(void);

extern Intf *intf;
