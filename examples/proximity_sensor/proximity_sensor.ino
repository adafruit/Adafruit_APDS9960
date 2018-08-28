/***************************************************************************
  This is a library for the APDS9960 digital proximity, ambient light, RGB, and gesture sensor

  This sketch puts the sensor in proximity mode and enables the interrupt
  to fire when proximity goes over a set value

  Designed specifically to work with the Adafruit APDS9960 breakout
  ----> http://www.adafruit.com/products/3595

  These sensors use I2C to communicate. The device's I2C address is 0x39

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Dean Miller for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/

#include "Adafruit_APDS9960.h"

#ifdef RASPBERRY_PI
#include <stdio.h>
#include <stdlib.h>
#endif // ifndef RASPBERRY_PI

//the pin that the interrupt is attached to
#define INT_PIN 3

//create the APDS9960 object
Adafruit_APDS9960 apds;

void setup() {
#ifndef RASPBERRY_PI
  Serial.begin(115200);
  pinMode(INT_PIN, INPUT_PULLUP);

  if(!apds.begin()){
    Serial.println("failed to initialize device! Please check your wiring.");
  }
  else Serial.println("Device initialized!");
#else  // ifndef RASPBERRY_PI
  wiringPiSetup();
  pinMode(INT_PIN, INPUT);
  pullUpDnControl(INT_PIN, PUD_UP);

  if(!apds.begin()){
    puts("failed to initialize device! Please check your wiring.\n");
    abort();
  } else puts("Device initialized!");
#endif // ifndef RASPBERRY_PI

  //enable proximity mode
  apds.enableProximity(true);

  //set the interrupt threshold to fire when proximity reading goes above 175
  apds.setProximityInterruptThreshold(0, 175);

  //enable the proximity interrupt
  apds.enableProximityInterrupt();
}

void loop() {

  //print the proximity reading when the interrupt pin goes low
  if(!digitalRead(INT_PIN)){
#ifndef RASPBERRY_PI
    Serial.println(apds.readProximity());
#else // ifndef RASPBERRY_PI
    printf("%d\n", (int)apds.readProximity());
#endif // ifndef RASPBERRY_PI

    //clear the interrupt
    apds.clearInterrupt();
  }
}

#ifdef RASPBERRY_PI
int main(int argc, char* argv[]) {
  setup();
  while (true) loop();
}
#endif // ifdef RASPBERRY_PI
