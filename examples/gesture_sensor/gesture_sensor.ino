#include "Adafruit_APDS9960.h"
Adafruit_APDS9960 apds;

#define PROXIMITY_MODE 1
#define GESTURE_MODE 2
#define COLOR_MODE 3

#define APDS9960_MODE PROXIMITY_MODE

#define IRQ_PIN 3

volatile bool interruptReceived = false;

void ISR_HANDLER()
{
	interruptReceived = true;
  digitalWrite(LED_BUILTIN, HIGH);
}

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(IRQ_PIN, INPUT_PULLUP);
  Serial.begin(9600);
  while(!Serial);
  
  if(!apds.begin()){
	  Serial.println("failed to initialize device! Please check your wiring.");
  }
  else Serial.println("Device initialized!");

#if APDS9960_MODE == GESTURE_MODE 
  apds.enableProximity(true);
  apds.enableGesture(true);
  apds.setProxGain(APDS9960_PGAIN_1X);
  apds.setProxPulse(APDS9960_PPULSELEN_32US, 8);
  apds.setLED(APDS9960_LEDDRIVE_100MA, APDS9960_LEDBOOST_300PCNT);
  apds.enableProximityInterrupt();

#elif APDS9960_MODE == PROXIMITY_MODE
  apds.enableProximity(true);
  apds.setProxGain(APDS9960_PGAIN_4X);
  apds.setProxPulse(APDS9960_PPULSELEN_32US, 8);
  apds.setLED(APDS9960_LEDDRIVE_100MA, APDS9960_LEDBOOST_300PCNT);

  apds.setProximityInterruptThreshold(60, 240, 4);  // low, high and 'persistance'
  attachInterrupt(digitalPinToInterrupt(IRQ_PIN), ISR_HANDLER, FALLING);
  apds.enableProximityInterrupt();

#elif APDS9960_MODE == COLOR_MODE
  apds.enableColor(true); 
#endif
}

// the loop function runs over and over again forever
void loop() {
	
#if APDS9960_MODE == GESTURE_MODE
	 uint8_t gesture = apds.readGesture();
   switch (gesture) {
    case APDS9960_UP: Serial.println("^"); break;
    case APDS9960_DOWN: Serial.println("v"); break;
    case APDS9960_LEFT: Serial.println("<"); break;
    case APDS9960_RIGHT: Serial.println(">"); break;
    //default: Serial.print(".");
   }
	  
#elif APDS9960_MODE == PROXIMITY_MODE
  boolean irq = apds.getProximityInterrupt();
  if (irq) {
    apds.clearInterrupt();
    digitalWrite(LED_BUILTIN, LOW);
  }
  Serial.print(irq); Serial.print(", ");
	Serial.println(apds.readProximity());
	delay(10);

#elif APDS9960_MODE == COLOR_MODE
	uint16_t r, g, b, c;
	while(!apds.colorDataReady()){
		delay(5);
	}
	apds.getColorData(&r, &g, &b, &c);
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
