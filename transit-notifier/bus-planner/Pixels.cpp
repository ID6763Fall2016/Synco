/*********************************************
 ** Basic lights strip controller - Pixels  **
 ********************************************/

// include this library's description file
#include "Pixels.h"

#include <math.h>

// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances
Pixels::Pixels() {
}

void Pixels::write(uint8_t ndx) {
  write(ndx, pixels[ndx]);
}

// Overridable
void Pixels::write(uint8_t ndx, uint32_t color) {
  if (type) {
    dotStarStrip.write(ndx, color);
  } else {
    ledStrip.write(ndx, color);
  }
}

void Pixels::show() {
  if (type) {
    dotStarStrip.show();
  } else {
    ledStrip.show();
  }
 }

void Pixels::setup(uint8_t _length, uint8_t _pinData, uint8_t _pinClock, bool _type) {

  length = _length;
  type = _type;
  animationLength = 0;
  
  pixels = new uint32_t[length];
  animationStack = new uint8_t[length];

  if (type) {
    dotStarStrip.setup(length, _pinData, _pinClock);
  } else {
    ledStrip.setup(length, _pinData, _pinClock);
  }

  fill(0, false);
}

void Pixels::fill(uint32_t color, bool display, uint8_t start, uint8_t end) {
  if (end > length) {
    end = length;
  }

  for (uint8_t i = start; i < end; i++) {
    pixels[i] = color;

    if (display) {
      write(i);
    }
  }

  if (display) {
    show();
  }
}

void Pixels::set(uint8_t ndx, uint32_t color, bool display) {
  if (ndx < length) {
    pixels[ndx] = color;

    if (display) {
      write(ndx);
      show();
    }
  }
}

void Pixels::setAnimation(uint8_t ndx) {
  animationStack[animationLength] = ndx;
  animationLength++;
}

void Pixels::setAnimation(uint8_t ndx, uint32_t color) {
  setAnimation(ndx);

  set(ndx, color, false);
}

void Pixels::fillAnimation(uint8_t ndx, uint8_t start, uint8_t end) {

  if (end > length) {
    end = length;
  }

  stop();

  fill(ndx, false, start, end);

  for (uint8_t i = start; i < end; i++) {
    setAnimation(i);
  }
}

void Pixels::animate(int duration, unsigned long wait) {
  bool isAsync = true;

  ndxDelta = 1;
  ndxTarget = animationLength - 1;
  ndxNext = 0;
  timerWait = wait;

  isAsync = (duration > 0 && ndxTarget > 0);

  if (isAsync) {
    timerDelay = ceil(duration / abs(ndxTarget - ndxNext));
    timerTarget = millis() + timerDelay;
  } else {
    stop();
  }

  showNext(!isAsync);
}

void Pixels::stop() {
  timerDelay = 0;
  timerTarget = 0;
  timerWait = 0;

  ndxTarget = 0;
  ndxNext = 0;
  ndxDelta = 0;

  for (uint8_t i = 0; i < length; i++) {
    animationStack[i] = 0;
  }

  animationLength = 0;
}

void Pixels::reset(bool display) {
  stop();
  fill(0, display);
}

bool Pixels::showNext(bool isSync) {
  bool bStop = false;
  if (ndxNext < animationLength) {
    write(ndxNext, pixels[animationStack[ndxNext]]);
  }
  ndxNext += ndxDelta;

  bStop = ndxNext > animationLength;

  if (bStop) {
    stop();
    show();
  } else {
    if (isSync) {
      showNext(isSync);
    } else {
      show();
    }
  }

  return  (ndxNext < animationLength);
}

void Pixels::loop() {
  unsigned long current;
  if (timerTarget > 0) {
    current = millis();

    if (timerTarget < current) {
      timerTarget = current + timerDelay;

      if (!showNext(false) && timerWait > 0) {
        timerTarget = current + timerWait;
      }
    }
  }

  if (!isReady) {
    isReady = !animating();

    if (isReady) {
      fill(0x000000, true);
    }
  }
}


void Pixels::initialize(int duration, unsigned long wait) {
  uint32_t colorDelta = 0x00000F;
  if (length > 1) colorDelta = 0xFFFFFF / ((uint32_t) length);
  uint32_t color = colorDelta;

  for (uint8_t i = 0; i < length; i++) {
    setAnimation(i, color);
    color += colorDelta;
  }

  animate(duration*1000, wait*1000);
}

bool Pixels::animating() {
  return timerDelay != 0 || timerTarget != 0;
}

bool Pixels::ready() {
  return isReady;
}

