#ifndef VEC_H_
#define VEC_H_

typedef struct {
	float x, y, z;
} Vector;

Vector vec_add(Vector a, Vector b);
Vector vec_sub(Vector a, Vector b);
Vector vec_mul(Vector v, float f);
Vector vec_div(Vector v, float f);
Vector vec_toang(Vector v);
float vec_toang2d(Vector v);
Vector vec_fromang(Vector v);
Vector vec_fromang2d(float ang);
Vector vec_norm(Vector v);
Vector vec_calcang(Vector start, Vector end, Vector ang);
int vec_isnull(Vector v);
Vector vec_crossprod(Vector a, Vector b);
float vec_len(Vector v);
float vec_len2d(Vector v);
float vec_dist(Vector a, Vector b);

#endif /* VEC_H_ */
