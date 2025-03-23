#include <Arduino.h>
#include "bsp/power/power.h"

Power power;

void setup() {
  Serial.begin(115200);
  
  power.init();
}

// the loop function runs over and over again forever
void loop() {
  // power test
  #if 0
  Serial.printf("volt: %.2f (%d%%)\n", power.readBatVoltage(), power.readBatPercentage());
  power.keepAlive();
  delay(10000);
  #endif
}