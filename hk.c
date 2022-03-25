#include <stdint.h>
#include <stdlib.h> /* malloc, ... */
#include <string.h> /* memcpy */

#include "hk.h"
#include "util.h"

typedef struct {
	uintptr_t addr;
	uintptr_t *old;
	uintptr_t *new;
	size_t size;
} Vmt;

static size_t
getvmtsize(uintptr_t *vmt)
{
	int i = 0;

	while (vmt[i])
		i++;

	return i;
}

static void
hookvmt(uintptr_t addr, Vmt *vmt)
{
	uintptr_t *new;

	vmt->addr = addr;
	vmt->old  = VMT(addr);
	vmt->size = getvmtsize(vmt->old) + 2;

	new = malloc(vmt->size * sizeof(uintptr_t));
	if (!new)
		return;

	vmt->new = new;
	memcpy(vmt->new, vmt->old - 2,
	       vmt->size * sizeof(uintptr_t));
	VMT(vmt->addr) = vmt->new + 2;
}

static void
hookfn(Vmt *vmt, int i, uintptr_t addr)
{
	vmt->new[i + 2] = addr;
}

static void
unhookvmt(Vmt *vmt)
{
	if (!vmt->new)
		return;

	VMT(vmt->addr) = vmt->old;
	free(vmt->new);
}

void
hk_init(void)
{

}

void
hk_clean(void)
{

}
