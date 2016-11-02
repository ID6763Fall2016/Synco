/*********************************************
 ** Capacitive touch sensor x 16 - CapTouch **
 ********************************************/

// include this library's description file
#include "CapTouch.h"

// Constructor /////////////////////////////////////////////////////////////////
// Function that handles the creation and setup of instances
CapTouch::CapTouch() {
  
}

void CapTouch::setup() {
  // Use I2C, no reset pin!
  register1 = new Adafruit_CAP1188();
  register2 = new Adafruit_CAP1188();

  delay(100);

  // Initialize the sensor, if using i2c you can pass in the i2c address
  if (!register1->begin(0x28)) {
    Serial.println("First register not found");
    while (1);
  }

  if (!register2->begin(0x29)) {
    Serial.println("Second register not found");
    while (1);
  }
}

uint16_t CapTouch::read() {
  // Use I2C, no reset pin!
  uint16_t touched = register2->touched();
  touched <<= 8;
  touched |= register1->touched();

  //Serial.println("Touch");
  //Serial.println(register1->touched());
  //Serial.println(register2->touched());
  //Serial.println(touched);
  //Serial.println();
  return touched;
}

bool* CapTouch::readArray() {
  uint16_t touched = read();
  bool readValue[16];
    
  for (uint16_t i=0; i<16; i++) {
    readValue[i] = (touched & (1 << i));
  }

  return readValue;
}

