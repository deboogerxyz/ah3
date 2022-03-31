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
