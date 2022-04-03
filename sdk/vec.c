#include "../util.h"

#include "vec.h"

Vector
vec_add(Vector a, Vector b)
{
	Vector c = {
		.x = a.x + b.x,
		.y = a.y + b.y,
		.z = a.z + b.z
	};

	return c;
}

Vector
vec_sub(Vector a, Vector b)
{
	Vector c = {
		.x = a.x - b.x,
		.y = a.y - b.y,
		.z = a.z - b.z
	};

	return c;
}

Vector
vec_mul(Vector v, float f)
{
	Vector a;

	a.x = v.x * f;
	a.y = v.y * f;
	a.z = v.z * f;
	return a;
}


Vector
vec_div(Vector v, float f)
{
	Vector a;

	a.x = v.x / f;
	a.y = v.y / f;
	a.z = v.z / f;
	return a;
}

Vector
vec_toang(Vector v)
{
	Vector a = {
		.x = RADTODEG(atan2(-v.z, hypot(v.x, v.y))),
		.y = RADTODEG(atan2(v.y, v.x)),
		.z = 0.0f
	};

	return a;
}

Vector
vec_fromang(Vector v)
{
	Vector a = {
		.x = cos(DEGTORAD(v.x)) * cos(DEGTORAD(v.y)),
		.y = cos(DEGTORAD(v.x)) * sin(DEGTORAD(v.y)),
		.z = -sin(DEGTORAD(v.x))
	};

	return a;
}

Vector
vec_norm(Vector v)
{
	Vector a = {
		.x = isfinite(v.x) ? remainder(v.x, 360.0f) : 0.0f,
		.y = isfinite(v.y) ? remainder(v.y, 360.0f) : 0.0f,
		.z = 0.0f
	};

	return a;
}

Vector
vec_calcang(Vector start, Vector end, Vector ang)
{
	Vector a, b;

	a = vec_sub(end, start);
	b = vec_sub(vec_toang(a), ang);

	return vec_norm(b);
}

int
vec_isnull(Vector v)
{
	return v.x == 0 && v.y == 0 && v.z == 0;
}

Vector
vec_crossprod(Vector a, Vector b)
{
	Vector c = {
		.x = a.y * b.z - a.z * b.y,
		.y = a.z * b.x - a.x * b.z,
		.z = a.x * b.y - a.y * b.x
	};

	return c;
}
