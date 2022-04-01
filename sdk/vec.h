#ifndef VEC_H_
#define VEC_H_

typedef struct {
	float x, y, z;
} Vector;

Vector vec_add(Vector a, Vector b);
Vector vec_sub(Vector a, Vector b);
Vector vec_toang(Vector v);
Vector vec_norm(Vector v);
Vector vec_calcang(Vector start, Vector end, Vector ang);

#endif /* VEC_H_ */
