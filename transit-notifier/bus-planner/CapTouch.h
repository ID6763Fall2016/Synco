/*********************************************
 ** Capacitive touch sensor x 16 - CapTouch **
 ********************************************/

// ensure this library description is only included once
#ifndef CapTouch_h
#define CapTouch_h

/*************************************************** 
  This is a library for the CAP1188 I2C/SPI 8-chan Capacitive Sensor

  Designed specifically to work with the CAP1188 sensor from Adafruit
  ----> https://www.adafruit.com/products/1602

  These sensors use I2C/SPI to communicate, 2+ pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/
 
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_CAP1188.h>

// Reset Pin is used for I2C or SPI
#define CAP1188_RESET  4

#define ADDR_REG_1 0x28
#define ADDR_REG_2 0x29

class CapTouch {
  public:
    CapTouch();

    bool* readArray();
    uint16_t read();
    void setup();

  protected:
    Adafruit_CAP1188* register1 = NULL;
    Adafruit_CAP1188* register2 = NULL;
};

#endif
