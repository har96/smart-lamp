#include "patterns.h"

#define NUM_BLOCKS 3

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
    lamp_off(lamp);

    for (int i = 0; i < 4; i++) {
        (*lamp)[i][hue] = ((t + (i*10)) % 255);
    }
}

