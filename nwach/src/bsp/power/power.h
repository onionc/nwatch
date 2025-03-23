#pragma once

#include <Arduino.h>

#define PWR_CTRL   GPIO_NUM_17
#define BATM_ADC    GPIO_NUM_8
#define BATM_EN     GPIO_NUM_18

class Power{

public:
    Power(){}
    ~Power(){}
    
    // 初始化
    void init();
    
    // 获取分压后的原始电压
    uint32_t readBatVoltageRaw();
    // 获取电池电压
    float readBatVoltage();
    // 获取电量百分比
    uint8_t readBatPercentage();

    // 保持开机，每20s需要执行一次
    void keepAlive();
};