#include <Arduino.h>
#include "bsp/power/power.h"
#include <TFT_eSPI.h>
#include <lvgl.h>
#include <demos/lv_demos.h>


#include "CST816T.h"

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

// 触摸
#define I2C_SDA 5
#define I2C_SCL 4
#define RST_N_PIN -1
#define INT_N_PIN 36
CST816T cst816t(I2C_SDA, I2C_SCL, RST_N_PIN, INT_N_PIN); 

// 打点函数
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

/*Read the touchpad*/
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
    TouchInfos tp;
    tp = cst816t.GetTouchInfo();
    // bool touched = (tp.touching == 1&&tp.isValid == 1);
    bool touched = tp.touching;

    if (!touched)
    {
        data->state = LV_INDEV_STATE_REL;
    }
    else
    {
        data->state = LV_INDEV_STATE_PR;

        /*Set the coordinates*/
        data->point.x = tp.x;
        data->point.y = tp.y;

        Serial.printf("touchX = %d, touchY = %d\n", tp.x, tp.y);
    }
}

void lvgl_handler(void *pvParameters)
{
	while(1)
	{
        lv_timer_handler(); /* let the GUI do its work */
        delay(5);
	}
}

// 页面
void first_screen(){
    // 对象
    //lv_obj_t *obj = lv_obj_create(lv_scr_act());
    //lv_obj_set_pos(obj, 100, 100);
    //v_obj_set_size(obj, 50,50);

    // label
    lv_obj_t * label = lv_label_create(lv_scr_act());
    lv_obj_set_size(label, 100, 50);
    lv_obj_set_align(label, LV_ALIGN_TOP_MID);
    lv_label_set_text(label, "hello");
    
}


// tft 初始化
void tft_init(){
    tft.init();
    //tft.fillScreen(TFT_LIGHTGREY);
    // 旋转
    // tft.setRotation(1);
}

void setup() {
    Serial.begin(115200);
    delay(500);
    Serial.printf("setup init");

    power.init();

    // tft
    tft_init();
    // 触摸
    cst816t.begin();
    // lvgl
    lv_init();
    
    lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * screenHeight );
    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init( &disp_drv );
    
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    disp_drv.sw_rotate = 1;   // 配置：允许旋转
    //disp_drv.rotated = LV_DISP_ROT_90;   // add for rotation
    lv_disp_drv_register( &disp_drv );

    // 旋转
    lv_disp_set_rotation(NULL, LV_DISP_ROT_90);

     /*Initialize the (dummy) input device driver*/
    
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);

    //lv_demo_benchmark();          // OK
    lv_demo_widgets();
    // 绘制界面
    //first_screen();

    xTaskCreate(lvgl_handler, "lvgl_handler", 4096, NULL, 2, NULL);
    Serial.println( "Setup done" );

}

// the loop function runs over and over again forever
void loop() {

    // power test
    #if 1
    Serial.printf("volt: %.2f (%d%%)\n", power.readBatVoltage(), power.readBatPercentage());
    power.keepAlive();
    delay(1000);
    #endif
}