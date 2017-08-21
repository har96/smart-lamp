
#include <math.h>
#include <Arduino.h>

#define NUM_BLOCKS 3
#define PI 3.14159265

#include "patterns.h"

const colors GOOGLE = { 
  {0, 135, 68},
  {214, 45, 32},
  {255, 167, 0} 
};

const colors NIGHT = {
  {100, 20, 0},
  {150, 10, 0},
  {150, 20, 0}
};

const colors RGB = {
  {255, 0, 0},
  {0, 255, 0},
  {0, 0, 255}
};

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
     * for a timestep t and a specified hue index.
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

void binary(char num, colors *lamp)
{
  if (num > 7) {
    return;
  }

  for (int i = 2; i >= 0; i--) {
    bool bt = num % 2;
    if (bt) {
      (*lamp)[i][0] = (*lamp)[i][1] = (*lamp)[i][2] = 255;
    }
    else {
      (*lamp)[i][0] = (*lamp)[i][1] = (*lamp)[i][2] = 0;
    }

    num /= 2;
  }
}

void weatherPattern(uint32_t t, String *weather, colors *cur_lamp)
{
   for (int i = 0; i < 3; i++) {
      
      String day = weather[i];
      if (day.endsWith("rain")) {
         int blue = (t%3) * 100; // Steps every tenth of a second
         
         (*cur_lamp)[i][0] = 10;
         (*cur_lamp)[i][1] = 10;
         (*cur_lamp)[i][2] = blue;
      }
      else if (day.equals("clear")) {
        (*cur_lamp)[i][0] = 255;
        (*cur_lamp)[i][1] = 160;
        (*cur_lamp)[i][2] = 0;
      }
      else if (day.endsWith("tstorms")) {
        int red_green = (t%3) * 100; // Steps every tenth of a second

        (*cur_lamp)[i][0] = red_green;
        (*cur_lamp)[i][1] = red_green;
        (*cur_lamp)[i][2] = 10;
      }
      else if (day.endsWith("cloudy")) {
        (*cur_lamp)[i][0] = 100;
        (*cur_lamp)[i][1] = 90;
        (*cur_lamp)[i][2] = 120;
      }
      else {
        (*cur_lamp)[i][0] = 255;
        (*cur_lamp)[i][1] = 0;
        (*cur_lamp)[i][2] = 0;
      }
   }
}


