#include "../intf.h"

#include "trace.h"

static char
shouldhitentity(TraceFilter *this, uintptr_t ent, int unused)
{
	return ent != this->skip;
}

static int
gettracetype(void)
{
	return 0;
}

static void *filtervmt[2] = {shouldhitentity, gettracetype};

void
trace_initfilter(TraceFilter *filter)
{
	filter->vmt = filtervmt;
}

void
trace_traceray(Ray *ray, unsigned int mask, TraceFilter *filter, Trace *trace)
{
	VFN(void (*)(uintptr_t *, const Ray *, unsigned int, const TraceFilter *, Trace *), VMT(intf->trace), 5)(intf->trace, ray, mask, filter, trace);
}
