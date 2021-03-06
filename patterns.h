#ifndef PAT_H
#define PAT_H

#define NUM_BLOCKS 3

typedef byte colors[NUM_BLOCKS][3];

extern const colors GOOGLE;
extern const colors NIGHT;
extern const colors RGB;

//void display_weather(String *, int, colors *);
void getColors(int, int, int, colors *);
void lamp_off(colors *lamp);
void lava_lamp(int hue, int t, colors *lamp);
void binary(byte num, colors *);
void weatherPattern(uint32_t t, String *weather, colors *lamp);


#endif
