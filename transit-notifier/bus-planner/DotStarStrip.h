/*********************************************
 ** DotStar strip controller - DotStarStrip **
 ********************************************/

// ensure this library description is only included once
#ifndef DotStarStrip_h
#define DotStarStrip_h

#include <Adafruit_DotStar.h>

#include <SPI.h>

class DotStarStrip {
  public:
    DotStarStrip();

    void setup(uint8_t length, uint8_t pinData, uint8_t pinClk);
    void show(); // override with specific stripe show command
    void write(uint8_t ndx, unsigned int color); // override with specific stripe set pixel command

  protected:
    Adafruit_DotStar* strip = NULL;
};

#endif

