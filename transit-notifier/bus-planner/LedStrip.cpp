/*********************************************
 ** Basic led strip controller - LedStrip   **
 ********************************************/

// include this library's description file
#include "LedStrip.h"

// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances
LedStrip::LedStrip() {
  
}

void LedStrip::setup(uint8_t length, uint8_t pinData, uint8_t pinClock)
{
  strip = new Adafruit_WS2801(length, pinData, pinClock);

  strip->begin();
  strip->show();
}

void LedStrip::write(uint8_t ndx, uint32_t color) {
  strip->setPixelColor(ndx, color);
}

void LedStrip::show() {
  strip->show();
}
