/***************************************************************************
  This is a library for the APDS9960 digital proximity, ambient light, RGB, and gesture sensor

  This sketch puts the sensor in color mode and reads the RGB and clear values.

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
Adafruit_APDS9960 apds;

#ifdef RASPBERRY_PI
#include <stdio.h>
#include <stdlib.h>
#endif // ifdef RASPBERRY_PI

void setup() {
#ifndef RASPBERRY_PI
  Serial.begin(115200);

  if(!apds.begin()){
    Serial.println("failed to initialize device! Please check your wiring.");
  }
  else Serial.println("Device initialized!");
#else // ifndef RASPBERRY_PI
  wiringPiSetup();

  if(!apds.begin()){
    puts("failed to initialize device! Please check your wiring.\n");
    abort();
  }
  else puts("Device initialized!");
#endif // ifndef RASPBERRY_PI

  //enable color sensign mode
  apds.enableColor(true);
}

void loop() {
  //create some variables to store the color data in
  uint16_t r, g, b, c;
  
  //wait for color data to be ready
  while(!apds.colorDataReady()){
    delay(5);
  }

  //get the data and print the different channels
  apds.getColorData(&r, &g, &b, &c);

#ifndef RASPBERRY_PI
  Serial.print("red: ");
  Serial.print(r);
  
  Serial.print(" green: ");
  Serial.print(g);
  
  Serial.print(" blue: ");
  Serial.print(b);
  
  Serial.print(" clear: ");
  Serial.println(c);
  Serial.println();
#else // ifndef RASPBERRY_PI
  printf("red: %d green: %d blue: %d clear: %d\n", r, g, b, c);
#endif // ifndef RASPBERRY_PI

  delay(500);
}

#ifdef RASPBERRY_PI
int main(int argc, char* argv[]) {
  setup();
  while (true) loop();
}
#endif // ifdef RASPBERRY_PI
