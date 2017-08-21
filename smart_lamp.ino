#include <Time.h>
#include <TimeLib.h>



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
#include "weather.h"
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>

/************************ Example Starts Here *******************************/

#define SPEED 100  // Number of milliseconds for each timestep

colors PINS = { {14, 12, 13},
                {15, 0, 16},
                {2, 5, 4} };

colors cur_lamp;
String weather[3];

bool do_lava = 0;
bool do_weather = false;
bool do_time = false;

static const char ntpServerName[] = "us.pool.ntp.org";
const int timeZone = -6;


WiFiUDP Udp;
unsigned int localPort = 8888;  // local port to listen for UDP packets

time_t getNtpTime();
void sendNTPpacket(IPAddress &address);

void lava(int);
void setColor(int, int, int);
void setColor(const colors);
void setLamp();

// set up the 'color' feed
AdafruitIO_Feed *color = io.feed("smart-lamp");


void setup() {

  // set analogWrite range for ESP8266
#ifdef ESP8266
  analogWriteRange(255);
#endif

  lamp_off(&cur_lamp);

  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while(! Serial);

  Serial.print("Mac address: ");
  Serial.println(WiFi.macAddress());

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  color->onMessage(handleMessage);

  // wait for a connection
  byte i = 1;
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    
    binary(i%8, &cur_lamp);
    setLamp();
    
    delay(400);
    
    lamp_off(&cur_lamp);
    setLamp();
    delay(100);
    i++;
  }


  // we are connected
  Serial.println();
  Serial.println(io.statusText());

  setColor(0, 255, 0); // Flash green
  setLamp();
  delay(500);
  setColor(GOOGLE);

  Udp.begin(localPort);
  setSyncProvider(getNtpTime);
  setSyncInterval(300);

  // ------- ArduinoOTA -----------
  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
}

void loop() {
  io.run();
  ArduinoOTA.handle();

  if (do_lava) {
    lava(0); // Zero for red
  }
  else if (do_time) {
    byte minutes_ten = minute(now())/10;
    Serial.printf("Minutes: %d\n", minutes_ten);
    binary(minutes_ten, &cur_lamp);
  }
  else if (do_weather) {
    uint32_t t = millis() / SPEED;
    weatherPattern(t, weather, &cur_lamp);
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
    setColor(255, 255, 255);

    getWeatherData(weather);
    do_weather = true;
    do_time = false;
  }
  else if (command.startsWith("orange")) {
    setColor(255, 118, 0);
  }
  else if (command.startsWith("night")) {
    setColor(NIGHT);
  }
  else if (command.startsWith("@")) {
    flash(300, 5000);
    
    // Then show time
    do_time = true;
    do_weather = false;
  }
  else if (command.startsWith("google")) {
    setColor(GOOGLE);
  }
  else if (command.startsWith("colors")) {
    setColor(RGB);
  }
  else if (command.startsWith("off")) {
    setColor(0, 0, 0);
  }
  else if (command.startsWith("time")) {
    do_time = true;
    do_weather = false;
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
  do_weather = false;
  do_time = false;
}

void setColor(const colors copy)
{
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      cur_lamp[i][j] = copy[i][j];
    }
  }
  do_weather = false;
  do_time = false;
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

/*-------- NTP code ----------*/
/* Acquired from example code */

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address

  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmit NTP Request");
  // get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  Serial.print(ntpServerName);
  Serial.print(": ");
  Serial.println(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}


