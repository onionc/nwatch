
#include "power.h"


void Power::init(){
  gpio_reset_pin(BATM_ADC);
  gpio_reset_pin(BATM_EN);
  gpio_set_direction(BATM_EN, GPIO_MODE_OUTPUT_OD);
  analogReadResolution(12); // 设置adc采集为12位分辨率
}

// 获取分压后的原始电压
uint32_t Power::readBatVoltageRaw(){
    gpio_set_direction(BATM_EN, GPIO_MODE_OUTPUT_OD);
    gpio_set_level(BATM_EN, 0); // 开启测量电压通路，关闭后有助于省电
    delay(10);
    uint32_t ret = analogReadMilliVolts(BATM_ADC);
    gpio_set_level(BATM_EN,1); // 关闭测量通路

    // 恢复引脚默认值
    gpio_reset_pin(BATM_EN);
    gpio_reset_pin(BATM_ADC);
    return ret;
}

// 获取电池电压
float Power::readBatVoltage(){
    // 不理解为什么/2,并且数值正确，可能是校正adc误差
    return ((float)readBatVoltageRaw() * 3 / 2000);
}

// 获取电量百分比
uint8_t Power::readBatPercentage(){
    /* Asume 0~100% to be 3.2~4.2V */
    // uint8_t ret = map((readBatVoltageRaw() * 3 / 2), 3200, 4100, 0, 100);
    // 优化, 避免无效值或负值
    int voltage = (readBatVoltageRaw() * 3) / 2;
    voltage = constrain(voltage, 3200, 4100);  // 限制在 3200mV ~ 4100mV 范围内
    return map(voltage, 3200, 4100, 0, 100);
}

// 保持开机，每20s需要执行一次
void Power::keepAlive(){
    // 一次脉冲，模拟按下

    gpio_reset_pin(PWR_CTRL);
    gpio_set_direction(PWR_CTRL, GPIO_MODE_OUTPUT_OD);

    gpio_set_level(PWR_CTRL, 0);
    delay(100); // 小于50ms无效
    gpio_set_level(PWR_CTRL, 1);
    delay(100);
}