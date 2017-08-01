#ifndef PAT_H
#define PAT_H

#define NUM_BLOCKS 3

typedef int colors[NUM_BLOCKS][3];

colors GOOGLE = { 
  {0, 135, 68},
  {214, 45, 32},
  {255, 167, } 
};

colors NIGHT = {
  {100, 10, 0},
  {150, 20, 0},
  {150, 20, 0}
};

void getColors(int, int, int, colors *);
void lamp_off(colors *lamp);
void lava_lamp(int hue, int t, colors *lamp);

#endif
