# Team Synco - Transit Notifier

Monitors traffic and directions on stairs

[Blog](http://ipdl.gatech.edu/doie2016/projects/5synco-transit-notifier/)

This folder holds the source code of the Stairs Monitor.

## Authors

* Diego Osorio
* Shambhavi Mahajan

## System

* Components
  *	[Adafruit Huzzah ESP8266 Breakout](https://www.adafruit.com/product/2471)
  * 2 x 8-Channel Capacitive Touch Sensor Breakout [Adafruit CAP1188](https://www.adafruit.com/products/1602)
  * 2 x 14 Leds [Adafruit NeoPixel RGB Led strip](https://www.adafruit.com/products/1460)
  * 25 leds [Adafruit RGB Led pixels](https://www.adafruit.com/products/322)

* Input
  * 16 x Capacitive touch channels
  * GTBus Api services

* Output
  * Stop array using RBG led Pixels
  * Waiting time using Neo Pixel strip
  * Bus route display using Neo Pixel strip

## Libraries

The following libraries are needed:

### Arduino libraries

* [Adafruit Huzzah ESP8266 Breakout - ESP8266](https://github.com/esp8266/Arduino)
* [Adafruit RGB Led pixels - WS2801](https://github.com/adafruit/Adafruit-WS2801-Library)
* [Adafruit NeoPixel RGB Led strip - DotStar](https://github.com/adafruit/Adafruit_DotStar)
* [8-Channel Capacitive Touch Sensor Breakout - CAP1188](https://github.com/adafruit/Adafruit_CAP1188_Library)

### tinyxml2

Handle XML loading and parsing

* [Reference](http://www.grinninglizard.com/tinyxml2/)
* [Library](https://github.com/leethomason/tinyxml2)

### Fritzing Adafruit Library

* [Fritzing Adafruit Library](https://github.com/adafruit/Fritzing-Library)
