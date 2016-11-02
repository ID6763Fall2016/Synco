/*********************************************
 ** Basic lights strip controller - Pixels  **
 ********************************************/

// ensure this library description is only included once
#ifndef Pixels_h
#define Pixels_h

#include <SPI.h>
#include <climits>

#include "DotStarStrip.h"
#include "LedStrip.h"

// library interface description
class Pixels
{
  // user-accessible "public" interface
  public:
    Pixels();

    // Synchronize
    void loop();

    // Methods to override
    void setup(uint8_t _length, uint8_t _pinData, uint8_t _pinClk, bool _type);
    void show(); // override with specific stripe show command

    // Methods
    void fill(uint32_t color, bool display, uint8_t start = 0, uint8_t end = UCHAR_MAX);
    void set(uint8_t ndx, uint32_t color, bool display);
    void stop();
    void reset(bool display);
    void initialize(int duration, unsigned long wait = 0);
    bool animating();
    bool ready();

    // Animation
    void animate(int duration, unsigned long wait = 0);
    void setAnimation(uint8_t ndx);
    void setAnimation(uint8_t ndx, uint32_t color);
    void fillAnimation(uint8_t ndx, uint8_t start = 0, uint8_t end = UCHAR_MAX);

  // library-accessible "private" interface
  private:
    // Pins
    bool isReady = false;
    bool type = true;
    uint8_t length = 0;
    uint32_t* pixels = NULL;
    uint8_t* animationStack = NULL;
    uint8_t animationLength = 0;

    // Timer
    unsigned long timerWait = 0;
    unsigned long timerTarget = 0;
    unsigned long timerDelay = 0;

    // Index
    uint8_t ndxNext = 0;
    uint8_t ndxTarget = 0;
    uint8_t ndxDelta = 0;

    // Methods to override
    void write(uint8_t ndx, uint32_t color); // override with specific stripe set pixel command

    // Methods
    bool showNext(bool sync);
    void write(uint8_t ndx); // write helper

    DotStarStrip dotStarStrip = DotStarStrip();
    LedStrip ledStrip = LedStrip();
};

#endif

