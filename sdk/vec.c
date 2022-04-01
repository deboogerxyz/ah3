#include <math.h>

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
vec_toang(Vector v)
{
	Vector a = {
		.x = atan2(-v.z, hypot(v.x, v.y)) * (180.0f / M_PI),
		.y = atan2(v.y, v.x) * (180.0f / M_PI),
		.z = 0.0f
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

