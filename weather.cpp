#include <ArduinoJson.h>
#include "weather.h"
#include <ESP8266HTTPClient.h>


void getWeatherData(String *days)
{
    HTTPClient http;
 
    // configure traged server and url
    //http.begin("https://192.168.1.12/test.html", "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
    http.begin("http://api.wunderground.com/api/47416f09a0248764/forecast/q/MS/Starkville.json"); //HTTP
  
    // start connection and send HTTP header
    int httpCode = http.GET();
    String payload;
  
    // httpCode will be negative on error
    if(httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);
  
        // file found at server
        if(httpCode == HTTP_CODE_OK) {
            payload = http.getString();
            payload.trim();
        } else {
          Serial.println("HTTP code not OK");
          return;
        }
    } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        return;
    }

    http.end();

    char *buf = const_cast<char*>(payload.c_str());

    //const size_t bufferSize = JSON_ARRAY_SIZE(4) + JSON_ARRAY_SIZE(8) + 2*JSON_OBJECT_SIZE(1) + 35*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + 8*JSON_OBJECT_SIZE(4) + 8*JSON_OBJECT_SIZE(7) + 4*JSON_OBJECT_SIZE(17) + 4*JSON_OBJECT_SIZE(20) + 6130;

    DynamicJsonBuffer jsonBuffer;

    Serial.println("Parsing...");
    Serial.println("--------");
    JsonObject& root = jsonBuffer.parseObject(buf, 20);

    if (!root.success()) {
      Serial.println("Could not read json.");
      return;
    }
    Serial.println("Success.");

    
    JsonArray& forecast = root["forecast"]["simpleforecast"]["forecastday"];

    Serial.println("Fixing to write array");
    for (int i = 0; i < 3; i++) {
      days[i] = forecast[i]["icon"].asString();
    }
}

