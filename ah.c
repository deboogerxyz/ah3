#include <dlfcn.h> /* dlopen */
#include <pthread.h>

#include "hax/bt.h"
#include "cfg.h"
#include "hk.h"
#include "intf.h"
#include "mem.h"
#include "nv.h"

static pthread_t thrd;

static void *
thrd_main(void *vargp)
{
	struct timespec ts = {0, 100000000}; /* 100ms */

	while (!dlopen("bin/linux64/serverbrowser_client.so", RTLD_NOLOAD | RTLD_NOW))
		nanosleep(&ts, &ts); /* Wait for the game to fully load */

	intf_init();
	mem_init();
	hk_init();
	nv_init();
	cfg_init();

	return NULL;
}

static int
__attribute__((constructor))
onload(void)
{
	pthread_create(&thrd, NULL, thrd_main, NULL);
	pthread_detach(thrd);

	return 0;
}

static void
__attribute__((destructor))
onunload(void)
{
	intf_clean();
	hk_clean();
	nv_clean();
	bt_clean();
	mem_clean();
	cfg_clean();
}
