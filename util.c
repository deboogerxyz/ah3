#include <stdlib.h> /* calloc, exit, ... */
#include <string.h> /* strlen */

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

unsigned int
hash(const char *data)
{
	int i;
	unsigned int hashed = 0x811C9DC5;

	for (i = 0; i < strlen(data); i++) {
		hashed ^= data[i];
		hashed *= 0x1000193;
	}
	return hashed;
}
