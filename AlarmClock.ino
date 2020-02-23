#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <ESP8266mDNS.h>
#include <TimeLib.h>
#include <NtpClientLib.h>

// Led strip
#define LED_PIN 14
#define LED_COUNT 8
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);

// Timing
int8_t timeZone = 1; // we are in time zone 1
int timeToStartHours = 5; // 5am
int timeToStartMinutes = 0;
unsigned long wakeUpDuration = 1 * 60 * 60 * 1000; // hours * minutes * seconds * milis

void wait(unsigned long period)
{
    int time_now = millis();
    while(millis() < (time_now + period)){
        delay(1);
    }
}

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void setup() 
{
  // NeoPixel strip - setup plus turn off
  strip.begin();          
  strip.setBrightness(1); // 1-255
  for(int i = 0; i < LED_COUNT; i++)
  {
    strip.setPixelColor(i, strip.Color(0,   0,   0));
  }
  strip.show();

  // Blink built-in LED 2x
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW); 
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW); 
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);

  // Connect to wifi
  Serial.begin(9600);
  Serial.println("Connecting...");
  WiFiManager wifiManager;
  wifiManager.autoConnect("BudikAP");
 
  // Get time
  NTP.begin("pool.ntp.org", timeZone, true);
  delay(2000);
  Serial.print("Current date and time: ");
  Serial.println(NTP.getTimeDateString());

  // Current hour and minutes
  int hours = getValue(NTP.getTimeStr(), ':', 0).toInt();
  int minutes = getValue(NTP.getTimeStr(), ':', 1).toInt();

  // Calculate remaining time
  unsigned long timeToStart = 0;
  if(hours > timeToStartHours)
  {
    timeToStart = (24 - hours + timeToStartHours) * 60 * 60 * 1000;
  }
  else
  {
    timeToStart = (timeToStartHours - hours) * 60 * 60 * 1000;
  }

  if(minutes > timeToStartMinutes)
  {
    timeToStart += (60 - minutes + timeToStartMinutes) * 60 * 1000;
  }
  else
  {
    timeToStart += (timeToStartMinutes - minutes) * 60 * 1000;
  }
  
  Serial.print("Time to start: ");
  Serial.print((23 - hours + timeToStartHours));
  Serial.print("h ");
  Serial.print(60 - minutes + timeToStartMinutes);
  Serial.println("m");

  // Blink LEDs
  for(int i = 0; i < LED_COUNT; i++)
  {
    strip.setPixelColor(i, strip.Color(0,   255,   0));
  }
  strip.show();
  
  delay(1000);
  
  for(int i = 0; i < LED_COUNT; i++)
  {
    strip.setPixelColor(i, strip.Color(0,   0,   0));
  }
  strip.show();

  // Start waiting
  wait(timeToStart);

  for(int round = 0; round < 4; round++)
  {
    for(int step = 0; step < LED_COUNT; step++)
    {
      for(int i = 0; i < LED_COUNT; i++)
      {
        if(step < i)
        {
          strip.setPixelColor(i, strip.Color(0,   0,   0));
        }
        else if (round == 0)
        {
          strip.setPixelColor(i, strip.Color(0,   0,   255));
        }
        else if (round == 1)
        {
          strip.setPixelColor(i, strip.Color(255,   0,   255));
        }
        else if (round == 2)
        {
          strip.setPixelColor(i, strip.Color(255,   140,   0));
        }
        else if (round == 3)
        {
          strip.setPixelColor(i, strip.Color(255,   0,   0));
        }
      }
      strip.show();
      wait(wakeUpDuration / (4 * LED_COUNT));
    }
  }

  for(int i = 0; i < LED_COUNT; i++)
  {
    strip.setPixelColor(i, strip.Color(0,   255,   0));
  }
  strip.show();
}

void loop()
{

}
