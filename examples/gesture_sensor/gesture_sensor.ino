#include "Adafruit_APDS9960.h"

Adafruit_APDS9960 adps;

void setup() {
  Serial.begin(9600);
  
  if(!adps.begin()){
    Serial.println("failed to initialize device! Please check your wiring.");
  }
  
  adps.enableProximity(true);
  adps.enableGesture(true);
}

void loop() {
    uint8_t gesture = adps.readGesture();
    if(gesture == ADPS9960_DOWN) Serial.println("v");
    if(gesture == ADPS9960_UP) Serial.println("^");
    if(gesture == ADPS9960_LEFT) Serial.println("<");
    if(gesture == ADPS9960_RIGHT) Serial.println(">");
}
