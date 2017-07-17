#include "Adafruit_APDS9960.h"
Adafruit_APDS9960 adps;

#define PROXIMITY_MODE 1
#define GESTURE_MODE 2
#define COLOR_MODE 3

#define ADPS9960_MODE PROXIMITY_MODE

volatile bool interruptReceived = false;

void ISR_HANDLER()
{
	interruptReceived = true;
}

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(3, INPUT_PULLUP);
  Serial.begin(9600);
  while(!Serial);
  
  if(!adps.begin()){
	  Serial.println("failed to initialize device! Please check your wiring.");
  }
  else Serial.println("Device initialized!");

#if ADPS9960_MODE == GESTURE_MODE || ADPS9960_MODE == PROXIMITY_MODE
  adps.enableProximity(true);
  //adps.setProximityInterruptThreshold(25, 175);
  
  //attachInterrupt(digitalPinToInterrupt(3), ISR_HANDLER, FALLING);
  //adps.enableProximityInterrupt();
#endif

#if ADPS9960_MODE == GESTURE_MODE
  adps.enableGesture(true);
#else if ADPS9960_MODE == COLOR_MODE
  adps.enableColor(true); 
#endif
}

// the loop function runs over and over again forever
void loop() {
	
#if ADPS9960_MODE == GESTURE_MODE
	  uint8_t gesture = adps.readGesture();
	  if(gesture == ADPS9960_DOWN) Serial.println("v");
	  if(gesture == ADPS9960_UP) Serial.println("^");
	  if(gesture == ADPS9960_LEFT) Serial.println("<");
	  if(gesture == ADPS9960_RIGHT) Serial.println(">");
	  
#elif ADPS9960_MODE == PROXIMITY_MODE
		Serial.println(adps.readProximity());
		delay(100);

#elif ADPS9960_MODE == COLOR_MODE
	uint16_t r, g, b, c;
	while(!adps.colorDataReady()){
		delay(5);
	}
	adps.getColorData(&r, &g, &b, &c);
	Serial.print("red: ");
	Serial.print(r);
	
	Serial.print(" green: ");
	Serial.print(g);
	
	Serial.print(" blue: ");
	Serial.print(b);
	
	Serial.print(" clear: ");
	Serial.println(c);
	Serial.println();
	
	delay(500);

#endif
}
