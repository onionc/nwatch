#include <Arduino.h>
#include "bsp/power/power.h"
#include <TFT_eSPI.h>
#include <lvgl.h>

#include <demos/lv_demos.h>
Power power;

// tft_espi
TFT_eSPI tft = TFT_eSPI();
#define BL_PIN GPIO_NUM_42  // 替换为实际背光引脚


// lvgl
static const uint16_t screenWidth  = 240;
static const uint16_t screenHeight = 280;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ screenWidth * screenHeight ];

//TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */

void my_disp_flush( lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p )
{
    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );

    tft.startWrite();
    tft.setAddrWindow( area->x1, area->y1, w, h );
    tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
    tft.endWrite();

    lv_disp_flush_ready( disp_drv );
}

void display(){
  tft.init();
  //tft.fillScreen(TFT_LIGHTGREY);
  //pinMode(BL_PIN, OUTPUT);
  

}

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.printf("setup init");

  power.init();
  
  

    // lvgl
    lv_init();

    display();
    lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * screenHeight );
    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init( &disp_drv );
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register( &disp_drv );

    lv_demo_benchmark();          // OK

    Serial.println( "Setup done" );


}

// the loop function runs over and over again forever
void loop() {
  lv_timer_handler(); /* let the GUI do its work */
  delay( 5 );

  // power test
  #if 0
  Serial.printf("volt: %.2f (%d%%)\n", power.readBatVoltage(), power.readBatPercentage());
  power.keepAlive();
  delay(1000);
  #endif
}