# Color Alarm Clock

Arduino Alarm clock with LED strip that wakes the kids who don't know the time.
Currently hardcoded to start waking sequence at 5 am and display all green after one hour at 6 am.
Current time is synced over the internet when power on. Uses ESP.deepSleep for most of the waiting.

![GitHub Logo](/AlarmClock.jpg)

## Components:
* Node MCU (esp8266)
* Adafruit neopixel strip, 8 LEDs
* 3xAA 4.5V power
* Plastic case
* Sliding switch

## Code setup
* Update wifi credentials
* Set time to start waking sequence
* Update time zone or waking sequence duration
* Update anything else
