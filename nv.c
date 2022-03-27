#include <ctype.h>  /* isdigit */
#include <stdio.h>  /* sprintf */
#include <stddef.h> /* size_t */

#include "cvector.h"
#include "sdk.h"
#include "util.h"

typedef struct {
	RecvProxy *addr;
	RecvProxy old;
} Proxy;

typedef struct {
	unsigned int hash;
	size_t offset;
} NetVar;

static Proxy spottedproxy;
static cvector_vector_type(NetVar) netvars = NULL;

static void
spotted(RecvProxyData *data, void *arg2, void *arg3)
{
	data->val.i = 1;

	spottedproxy.old(data, arg2, arg3);
}

void
dump(const char *class, RecvTable *table, size_t offset)
{
	for (int i = 0; i < table->count; i++) {
		RecvProp *prop = &table->props[i];
		if (!prop || isdigit(prop->name[0]))
			continue;

		if (hash(prop->name) == hash("baseclass"))
			continue;

		if (prop->type == 6 && prop->table && prop->table->name[0] == 'D')
			dump(class, prop->table, prop->offset + offset);

		char name[256];
		sprintf(name, "%s->%s", class, prop->name);

		NetVar nv = {
			.hash = hash(name),
			.offset = prop->offset + offset
		};

		cvector_push_back(netvars, nv);

		if (nv.hash == hash("CBaseEntity->m_bSpotted")) {
			spottedproxy.old = prop->proxy;
			spottedproxy.addr = &prop->proxy;
			prop->proxy = spotted;
		}
	}
}

void
nv_init(void)
{
	cvector_grow(netvars, 1800);

	for (ClientClass *class = sdk_getallclasses(); class; class = class->next)
		if (class->table)
			dump(class->name, class->table, 0);
}

unsigned int
nv_get(const char *name)
{
	for (NetVar *it = cvector_begin(netvars); it != cvector_end(netvars); it++)
		if (it->hash == hash(name))
			return it->offset;
	return 0;
}

void
nv_clean(void)
{
	*spottedproxy.addr = spottedproxy.old;

	cvector_free(netvars);
}
