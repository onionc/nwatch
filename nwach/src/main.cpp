#include <Arduino.h>
#include "bsp/power/power.h"
#include <TFT_eSPI.h>

Power power;

TFT_eSPI tft = TFT_eSPI(200,200);
#define BL_PIN GPIO_NUM_42  // 替换为实际背光引脚

void display(){
  tft.init();
  tft.fillScreen(TFT_LIGHTGREY);
  //pinMode(BL_PIN, OUTPUT);
  

}

void setup() {
  //Serial.begin(115200);
  //delay(1000);
  //Serial.printf("test port1");
  //pinMode(BL_PIN, OUTPUT);
  //gpio_set_direction(BL_PIN, GPIO_MODE_OUTPUT_OD);

  
  //power.init();
/*
  Serial.printf("test port2");
  if (psramFound()) {
    Serial.println("PSRAM detected!（但硬件没有PSRAM，说明配置错误）");
  } else {
    Serial.println("No PSRAM（正确状态）");
  }*/
  display();
}

// the loop function runs over and over again forever
void loop() {


  // power test
  #if 0
  Serial.printf("volt: %.2f (%d%%)\n", power.readBatVoltage(), power.readBatPercentage());
  power.keepAlive();
  delay(1000);
  #endif
}