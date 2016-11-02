/*********************************************
 ** Basic led strip controller - LedStrip   **
 ********************************************/

// ensure this library description is only included once
#ifndef LedStrip_h
#define LedStrip_h

#include <Adafruit_WS2801.h>

#include <SPI.h>

class LedStrip {
  public:
    LedStrip();

    void setup(uint8_t length, uint8_t pinData, uint8_t pinClk);
    void show(); // override with specific stripe show command
    void write(uint8_t ndx, unsigned int color); // override with specific stripe set pixel command

  protected:
    Adafruit_WS2801* strip = NULL;
};

#endif

