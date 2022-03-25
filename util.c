#include <stdlib.h> /* calloc, exit, ... */

#include "util.h"

void
fatal(const char *fmt, ...)
{
	exit(1); /* TODO: Add error logging */
}

void *
ecalloc(size_t nmemb, size_t size)
{
	void *p;

	if (!(p = calloc(nmemb, size)))
		fatal("Failed to calloc!");
	return p;
}
