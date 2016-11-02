/*********************************************
 ** DotStar strip controller - DotStarStrip **
 ********************************************/

// include this library's description file
#include "DotStarStrip.h"

// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances
DotStarStrip::DotStarStrip() {
  
}

void DotStarStrip::setup(uint8_t length, uint8_t pinData, uint8_t pinClock)
{
  strip = new Adafruit_DotStar(length, pinData, pinClock, DOTSTAR_BGR);
  delay(100);

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
#endif

  strip->begin();
  strip->show();
  delay(100);
}

void DotStarStrip::write(uint8_t ndx, uint32_t color) {
  strip->setPixelColor(ndx, color);
}

void DotStarStrip::show() {
  strip->show();
}
