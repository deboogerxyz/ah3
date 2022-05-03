#include "../util.h"

#include "color.h"

Color
color_fromcolora(ColorA c)
{
	Color a = {
		.r = c.r,
		.g = c.g,
		.b = c.b
	};

	return a;
}

Color
color_health(int health)
{
	health = MIN(health, 100);

	Color col = {
		.r = MIN(2.0f * (100 - health) / 100.0f, 1.f),
		.g = MIN(2.0f * health / 100.0f, 1.f),
		.b = 0.0f
	};

	return col;
}

ColorA
colora_health(int health)
{
	health = MIN(health, 100);

	ColorA col = {
		.r = MIN(2.0f * (100 - health) / 100.0f, 1.f),
		.g = MIN(2.0f * health / 100.0f, 1.f),
		.b = 0.0f,
		.a = 1.0f
	};

	return col;
}
