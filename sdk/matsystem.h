#ifndef MATSYSTEM_H_
#define MATSYSTEM_H_

#include <stdint.h>

uintptr_t matsystem_findmaterial(const char *matname, const char *texturegroupname, char complain, const char *complainprefix);

#endif /* MATSYSTEM_H_ */
