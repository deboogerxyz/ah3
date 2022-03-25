#include <pthread.h>
#include <stdint.h> /* Required by inf.h */

#include "hk.h"
#include "intf.h"
#include "mem.h"

static pthread_t thrd;

static void *
thrd_main(void *vargp)
{
	intf_init();
	mem_init();
	hk_init();

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
	mem_onunload();
	hk_onunload();
}
