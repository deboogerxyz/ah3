#ifndef VIEWSETUP_H_
#define VIEWSETUP_H_

#include "../util.h"
#include "vec.h"

typedef struct {
	PAD(172);
	void *csm;
	float fov;
	float fovviewmodel;
	Vector origin;
	Vector angles;
	float nearz;
	float farz;
} ViewSetup;

#endif /* VIEWSETUP_H_ */
