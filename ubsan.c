#include "mem.h"

#include "ubsan.h"

static void
printloc(SourceLocation loc)
{
	mem->debugmsg("UBSAN: at %s, line %u, column %u\n", loc.filename, loc.line, loc.col);
}

void
__ubsan_handle_load_invalid_value(InvalidValueData *data, void *val)
{
	mem->debugmsg("UBSAN: invalid value of type %s\n", data->type->name);
	printloc(data->loc);
}

void
__ubsan_handle_nonnull_arg(NonnullArgData *data)
{
	mem->debugmsg("UBSAN: null pointer passed as argument %u, which is declared to never be null\n", data->argidx);
	printloc(data->loc);
}

void
__ubsan_handle_nullability_arg(NonnullArgData *data)
{
	mem->debugmsg("UBSAN: null pointer passed as argument %u, which is declared to never be null\n", data->argidx);
	printloc(data->loc);
}

void
__ubsan_handle_nonnull_return_v1(NonnullReturnData *data, SourceLocation *loc)
{
	mem->debugmsg("UBSAN: null pointer return from function declared to never return null\n");
	printloc(*loc);
}

void
__ubsan_handle_nullability_return_v1(NonnullReturnData *data, SourceLocation *loc)
{
	mem->debugmsg("UBSAN: null pointer return from function declared to never return null\n");
	printloc(*loc);
}

void
__ubsan_handle_vla_bound_not_positive(VLABoundData *data, void *val)
{
	mem->debugmsg("UBSAN: VLA bound not positive of type %s\n", data->type->name);
	printloc(data->loc);
}

void
__ubsan_handle_add_overflow(OverflowData *data, void *lval, void *rval)
{
	mem->debugmsg("UBSAN: addition overflow of type %s\n", data->type->name);
	printloc(data->loc);
}

void
__ubsan_handle_sub_overflow(OverflowData *data, void *lval, void *rval)
{
	mem->debugmsg("UBSAN: substraction overflow of type %s\n", data->type->name);
	printloc(data->loc);
}

void
__ubsan_handle_negate_overflow(OverflowData *data, void *val)
{
	mem->debugmsg("UBSAN: negation overflow of type %s\n", data->type->name);
	printloc(data->loc);
}

void
__ubsan_handle_mul_overflow(OverflowData *data, void *lval, void *rval)
{
	mem->debugmsg("UBSAN: multiplication overflow of type %s\n", data->type->name);
	printloc(data->loc);
}

void
__ubsan_handle_shift_out_of_bounds(ShiftOutOfBoundsData *data, void *lval, void *rval)
{
	mem->debugmsg("UBSAN: shift out of bounds, %s shifted by %s\n", data->ltype->name, data->rtype->name);
	printloc(data->loc);
}

void
__ubsan_handle_divrem_overflow(OverflowData *data, void *lval, void *rval)
{
	mem->debugmsg("UBSAN: divrem overflow of type %s\n", data->type->name);
	printloc(data->loc);
}

void
__ubsan_handle_out_of_bounds(OutOfBoundsData *data, void *val)
{
	mem->debugmsg("UBSAN: out of bounds access into array of type %s\n", data->arrtype->name);
	printloc(data->loc);
}

void
__ubsan_handle_type_mismatch_v1(TypeMismatchData *data, void *p)
{
	mem->debugmsg("UBSAN: type mismatch\n");
	printloc(data->loc);
}

void
__ubsan_handle_alignment_assumption(AlignmentAssumptionData *data, void *p, void *alignment, void *offset)
{
	mem->debugmsg("UBSAN: alignment assumption\n");
	printloc(data->loc);
}

void
__ubsan_handle_builtin_unreachable(UnreachableData *data)
{
	mem->debugmsg("UBSAN: execution reached an unreachable program point\n");
	printloc(data->loc);
}

void
__ubsan_handle_missing_return(UnreachableData *data)
{
	mem->debugmsg("UBSAN: execution reached the end of a value-returning function without returning a value\n");
	printloc(data->loc);
}

void
__ubsan_handle_implicit_conversion(ImplicitConversionData *data, void *lval, void *rval)
{
	mem->debugmsg("UBSAN: implicit conversion from type %s to type %s\n", data->fromtype->name, data->totype->name);
	printloc(data->loc);
}

void
__ubsan_handle_invalid_builtin(InvalidValueData data)
{
	mem->debugmsg("UBSAN: passing invalid argument\n");
	printloc(data.loc);
}

void
__ubsan_handle_pointer_overflow(PointerOverflowData *data, void *base, void *result)
{
	mem->debugmsg("UBSAN: pointer overflow\n");
	printloc(data->loc);
}
