#ifndef VIEWSETUP_H_
#define VIEWSETUP_H_

#include "../util.h"

typedef struct {
	PAD(172);
	void *csm;
	float fov;
	PAD(32);
	float farz;
} ViewSetup;

#endif /* VIEWSETUP_H_ */
