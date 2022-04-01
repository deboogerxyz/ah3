#include "mat3x4.h"

Vector
mat_origin(Matrix3x4 m)
{
	Vector v = {
		.x = m[0][3],
		.y = m[1][3],
		.z = m[2][3]
	};

	return v;
}
