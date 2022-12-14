#include <ctype.h>  /* isdigit */
#include <stdio.h>  /* sprintf */
#include <stddef.h> /* size_t */

#include "cfg.h"
#include "deps/cvector.h"
#include "sdk/client.h"
#include "sdk/engine.h"
#include "sdk/ent.h"
#include "sdk/recv.h"
#include "util.h"

#include "nv.h"

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
spotted(RecvProxyData *data, void *ent, void *arg3)
{
	if (cfg->visuals.revealradar) {
		data->val.i = 1;

		static long mask;
		if (!mask) {
			int maxclients = engine_getmaxclients();
			for (int i = 1; i <= maxclients; i++)
				mask |= (1 << i);
		}

		*ent_getspottedbymask((uintptr_t)ent) = mask;
	}

	spottedproxy.old(data, ent, arg3);
}

void
dump(const char *class, RecvTable *table, size_t offset)
{
	static unsigned int baseclass;

	if (!baseclass)
		baseclass = hash("baseclass");

	for (int i = 0; i < table->count; i++) {
		RecvProp *prop = &table->props[i];
		if (!prop || isdigit(prop->name[0]))
			continue;

		if (hash(prop->name) == baseclass)
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

	for (ClientClass *class = client_getallclasses(); class; class = class->next)
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
