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

/************************ Example Starts Here *******************************/

// default PWM pins for ESP8266.
// you should change these to match PWM pins on other platforms.
#define RED_PIN   14
#define GREEN_PIN 12
#define BLUE_PIN  13

void setColor(int, int, int);

// set up the 'color' feed
AdafruitIO_Feed *color = io.feed("smart-lamp");

void setup() {

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
}

void handleMessage(AdafruitIO_Data *data) {

  Serial.println("Data recieved:");
  Serial.println(data->value());

  switch (data->value()[0]) {
    case 'g': // green
    case 'G':
        setColor(0, 255, 0);
        break;

    case 'r':  // red
    case 'R':
        setColor(255, 0, 0);
        break;

    case 'b':  // blue
    case 'B':
        setColor(0, 0, 255);
        break;

    case 'w':  // weather
    case 'W':
        Serial.println("weather not implemented");
        setColor(255, 255, 255);
        break;

    case 'o': // orange
    case 'O':
        setColor(255, 118, 0);
        break;

    default:
        // print RGB values and hex value
        Serial.println("Received:");
        Serial.print("  - R: ");
        Serial.println(data->toRed());
        Serial.print("  - G: ");
        Serial.println(data->toGreen());
        Serial.print("  - B: ");
        Serial.println(data->toBlue());
        Serial.print("  - HEX: ");
        Serial.println(data->value());

        setColor(data->toRed(), data->toGreen(), data->toBlue());
        break;
  }
}

void setColor(int red, int green, int blue)
{
  // invert RGB values for common anode LEDs
  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
}