#include "patterns.h"
#include <math.h>

#define NUM_BLOCKS 3
#define PI 3.14159265

void getColors(int red, int green, int blue, colors *lamp)
{
    for (int i = 0; i < 3; i++) {
        (*lamp)[i][0] = red;
        (*lamp)[i][1] = green;
        (*lamp)[i][2] = blue;
    }
}


void lamp_off(colors *lamp)
{
    for (int i = 0; i < NUM_BLOCKS; i++) {
        for (int j = 0; j < 3; j++) {
            (*lamp)[i][j] = 0;
        }
    }
}


void lava_lamp(int hue, int t, colors *lamp)
{
    /* This function returns a lamp array
     * for a timestep t and a specified hue.
     */
    int new_step;
    double percent, x, y;
    for (int i = 0; i < NUM_BLOCKS; i++) {
        new_step = t + (i*12);
        percent = (double)new_step / 100.0;
        x = percent * 2.0 * PI;
        y = sin(x);
        (*lamp)[i][hue] = 128 + (int)(127.0 * y);
    }
}

