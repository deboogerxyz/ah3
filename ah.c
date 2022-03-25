#include <pthread.h>
#include <stdint.h>

#include "intf.h"

static pthread_t thrd;

static void *
thrd_main(void *vargp)
{
	intf_init();

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
