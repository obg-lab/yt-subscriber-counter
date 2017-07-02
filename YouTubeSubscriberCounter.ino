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
#include <Adafruit_GFX.h>          // https://github.com/adafruit/Adafruit-GFX-Library
#include "Adafruit_LEDBackpack.h"  // https://github.com/adafruit/Adafruit_LED_Backpack
#include <YoutubeApi.h>            // https://github.com/witnessmenow/arduino-youtube-api
#include <ArduinoJson.h>           // https://github.com/bblanchon/ArduinoJson


#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

//------- Replace the following! ------
char ssid[] = "your wifi network name"; // your network SSID (name)
char password[] = "yourwifipassword";   // your network password

// google API key
// create yours: https://support.google.com/cloud/answer/6158862?hl=en
#define API_KEY "yourAPIkeyhere"

// youtube channel ID
// find yours: https://support.google.com/youtube/answer/3250431?hl=en
#define CHANNEL_ID "yourchannelIDhere"

Adafruit_7segment matrix = Adafruit_7segment();

WiFiClientSecure client;
YoutubeApi api(API_KEY, client);

unsigned long api_mtbs = 1000; //mean time between api requests
unsigned long api_lasttime;   //last time api request has been done

long subs = 0;

void setup() {

  Serial.begin(115200);
  matrix.begin(0x70);
  
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

  if (millis() > api_lasttime + api_mtbs)  {
    if(api.getChannelStatistics(CHANNEL_ID))
    {
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
      //Serial.print("hiddenSubscriberCount: ");
      //Serial.println(api.channelStats.hiddenSubscriberCount);
      Serial.println("------------------------");
      
      matrix.print(api.channelStats.subscriberCount, DEC);
      matrix.writeDisplay();
    }
    api_lasttime = millis();
  }
}
