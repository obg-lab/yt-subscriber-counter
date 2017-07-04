// YouTube Subscriber Counter
// by Becky Stern 2017

// based on library sample code by:
//   Giacarlo Bacchio (Gianbacchio on Github)
//   Brian Lough (witnessmenow on Github)
//   Adafruit (adafruit on github)

// Displays your current subscriber count on a seven-segment display
// This version supports up to 9999 subscribers

// requires the following libraries, search in Library Manager or download from github):
#include <Wire.h>                  // installed by default
#include <YoutubeApi.h>            // https://github.com/witnessmenow/arduino-youtube-api
#include <ArduinoJson.h>           // https://github.com/bblanchon/ArduinoJson
#include <stdlib.h>

#include "LedControl.h"

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#include "config.h"

#define DISPLAYDIGITS 8

// EasyESP or NodeMCU Pin D8 to DIN, D7 to Clk, D6 to LOAD, no.of devices is 1
LedControl lc=LedControl(D8,D7,D6,1);

WiFiClientSecure client;
YoutubeApi api(API_KEY, client);

unsigned long api_mtbs = 1000; //mean time between api requests
unsigned long api_lasttime;   //last time api request has been done

long subs = 0;

void setup() {

  Serial.begin(9600);

  // Initialize the MAX7219 device
  lc.shutdown(0,false);   // Enable display
  lc.setIntensity(0,15);  // Set brightness level (0 is min, 15 is max)
  lc.clearDisplay(0);     // Clear display register

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);


}

void loop() {
  char buffer[10];

  if (millis() > api_lasttime + api_mtbs)  {

    if(api.getChannelStatistics(CHANNEL_ID)) {

      Serial.println("---------Stats---------");
      Serial.print("Subscriber Count: ");
      Serial.println(api.channelStats.subscriberCount);
      Serial.print("View Count: ");
      Serial.println(api.channelStats.viewCount);
      Serial.print("Comment Count: ");
      Serial.println(api.channelStats.commentCount);
      Serial.print("Video Count: ");
      Serial.println(api.channelStats.videoCount);
      // Probably not needed :)
      Serial.print("hiddenSubscriberCount: ");
      Serial.println(api.channelStats.hiddenSubscriberCount);
      Serial.println("------------------------");

      ConvertNumberToString(api.channelStats.subscriberCount, 0, buffer);
      delay(2000);
    }
    api_lasttime = millis();
  }
}

/*****
  https://forum.arduino.cc/index.php?topic=225326.0
  This function converts a number to a string representation and then displays
  it on the LED array. THe output is right-justified on the display.

  Parameter List:
    float val                  the number to be converted
    unsigned char precision    the number of digits after the decimal point
    char *buffer               a character array. It is assumed to be large
                               enough to hold the number represented as an
                               ASCII number plus the null.
  Return value:
    void
*****/
void ConvertNumberToString(float val, unsigned char precision, char *buffer)
{
  int i;
  int start;

  dtostrf(val, DISPLAYDIGITS, precision, buffer);
  start = DISPLAYDIGITS - precision - 1;

  for (i = 0; i < DISPLAYDIGITS; i++) {
    if (buffer[i] != ' ') {              // If we have a digit character
      if (buffer[i + 1] == '.') {        // Need a decimal point?
        lc.setDigit(0, start--, buffer[i] - '0', true);   // Yes
        i++;
      } else {
        lc.setDigit(0, start--, buffer[i] - '0', false);  // No
      }
    } else {
      start--;
    }
  }
}
