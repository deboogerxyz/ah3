/* Requires stddef.h */

void *ecalloc(size_t nmemb, size_t size);
void fatal(const char *fmt, ...);
unsigned int hash(const char *data);
