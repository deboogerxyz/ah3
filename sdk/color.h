#ifndef COLOR_H_
#define COLOR_H_

typedef struct {
	float r, g, b;
} Color;

typedef struct {
	float r, g, b, a;
} ColorA;

Color color_health(int health);
ColorA colora_health(int health);

#endif /* COLOR_H_ */
