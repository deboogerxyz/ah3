#include <stdlib.h> /* malloc, ... */
#include <string.h> /* memcpy */

#include "bt.h"
#include "hk.h"
#include "intf.h"
#include "mem.h"
#include "sdk.h"
#include "util.h"

typedef struct {
	uintptr_t addr;
	uintptr_t *old;
	uintptr_t *new;
	size_t size;
} Vmt;

static Vmt client, clientmode;

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
	vmt->addr = addr;
	vmt->old  = VMT(addr);
	vmt->size = getvmtsize(vmt->old) + 2;

	uintptr_t *new = malloc(vmt->size * sizeof(uintptr_t));
	if (!new)
		return;

	vmt->new = new;
	memcpy(vmt->new, vmt->old - 2, vmt->size * sizeof(uintptr_t));
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

static char
createmove(void *this, float inputsampletime, UserCmd *cmd)
{
	char result;

	result = VFN(char (*)(void *, float, UserCmd *), clientmode.old, 25)(mem->clientmode, inputsampletime, cmd);

	if (!cmd->cmdnumber)
		return result;
	
	sdk_getservertime(cmd);

	bt_run(cmd);

	return 0;
}

static void
framestagenotify(void *this, FrameStage stage)
{
	static int once;

	if (!once) {
		bt_init();
		once = 1;
	}

	bt_update(stage);

	VFN(void (*)(uintptr_t *, FrameStage), client.old, 37)(intf->client, stage);
}

void
hk_init(void)
{
	hookvmt((uintptr_t)intf->client, &client);
	hookfn(&client, 37, &framestagenotify);

	hookvmt((uintptr_t)mem->clientmode, &clientmode);
	hookfn(&clientmode, 25, &createmove);
}

void
hk_clean(void)
{
	unhookvmt(&client);
	unhookvmt(&clientmode);
}
