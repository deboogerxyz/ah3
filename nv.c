#include <ctype.h>  /* isdigit */
#include <stdio.h>  /* sprintf */
#include <stddef.h> /* size_t */

#include "mem.h"
#include "cvector.h"
#include "nv.h"
#include "sdk.h"
#include "util.h"

typedef struct {
	unsigned int hash;
	size_t offset;
} NetVar;

static cvector_vector_type(NetVar) netvars = NULL;

void
dump(const char *class, RecvTable *table, size_t offset)
{
	int i;
	RecvProp *prop;
	char name[256];
	NetVar nv;

	for (i = 0; i < table->count; i++) {
		prop = &table->props[i];
		if (!prop || isdigit(prop->name[0]))
			continue;

		if (hash(prop->name) == hash("baseclass"))
			continue;

		if (prop->type == 6 && prop->table &&
		    prop->table->name[0] == 'D')
			dump(class, prop->table, prop->offset + offset);

		sprintf(name, "%s->%s", class, prop->name);
		nv.hash = hash(name);
		nv.offset = prop->offset + offset;
		cvector_push_back(netvars, nv);
	}
}

void
nv_init(void)
{
	ClientClass *class;

	cvector_grow(netvars, 1800);

	for (class = getallclasses(); class; class = class->next)
		if (class->table)
			dump(class->name, class->table, 0);
}

unsigned int
nv_get(const char *name)
{
	NetVar *it;

	for (it = cvector_begin(netvars); it != cvector_end(netvars); it++)
		if (it->hash == hash(name))
			return it->offset;
	return 0;
}

void
nv_clean(void)
{
	cvector_free(netvars);
}
