

// Adafruit IO RGB LED Output Example
// Tutorial Link: https://learn.adafruit.com/adafruit-io-basics-color
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016-2017 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"
#include "patterns.h"

/************************ Example Starts Here *******************************/

#define SPEED 100  // Number of milliseconds for each timestep

colors PINS = { {14, 12, 13},
                {15, 0, 16},
                {2, 5, 4} };

colors cur_lamp;

bool do_lava = 0;

void lava(int);
void setColor(int, int, int);
void setColor(const colors);
void setLamp();

// set up the 'color' feed
AdafruitIO_Feed *color = io.feed("smart-lamp");

void setup() {

  lamp_off(&cur_lamp);
  setColor(GOOGLE);

  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while(! Serial);

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  color->onMessage(handleMessage);

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

  // set analogWrite range for ESP8266
  #ifdef ESP8266
    analogWriteRange(255);
  #endif

}

void loop() {
  io.run();

  if (do_lava) {
    lava(0); // Zero for red
  }
  setLamp();
}

void handleMessage(AdafruitIO_Data *data) {

  Serial.println("Data recieved:");
  Serial.println(data->value());

  String command = data->value();
  command.toLowerCase();

  if (command.startsWith("green")) {
    setColor(0, 255, 0);
  }
  else if (command.startsWith("red")) {
    setColor(255, 0, 0);
  }
  else if (command.startsWith("blue")) {
    setColor(0, 0, 255);
  }
  else if (command.startsWith("weather")) {
    Serial.println("weather not implemented");
    setColor(255, 255, 255);
  }
  else if (command.startsWith("orange")) {
    setColor(255, 118, 0);
  }
  else if (command.startsWith("night")) {
    setColor(NIGHT);
  }
  else if (command.startsWith("@")) {
    flash(300, 5000);
    // TODO then show weather
  }
  else if (command.startsWith("google")) {
    setColor(GOOGLE);
  }
  else {
    setColor(data->toRed(), data->toGreen(), data->toBlue());
  }

  if (command.endsWith("lava")) {
    do_lava = true;
  }
  else {
    do_lava = false;
  }
}

void flash(uint32_t flash_length, uint32_t total_time)
{
  uint32_t start = millis();
  while( (millis() - start) < total_time ) {
    setColor(255, 255, 255);
    setLamp();
    delay(flash_length);
    lamp_off(&cur_lamp);
    setLamp();
    delay(flash_length);
  }
}

// Adjust boxes to create lava effect
void lava(int hue)
{

  uint32_t timestep = millis() / SPEED;

  lava_lamp(hue, timestep, &cur_lamp);
}

// Set all boxes to color
void setColor(int red, int green, int blue)
{
  getColors(red, green, blue, &cur_lamp);
}

void setColor(const colors copy)
{
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      cur_lamp[i][j] = copy[i][j];
    }
  }
}

// Set lamp to boxes
void setLamp()
{
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      analogWrite(PINS[i][j], cur_lamp[i][j]);
    }
  }
}


