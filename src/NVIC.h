/*
BS-Stm32 Extended lib
Copyright (C) 2026 BlueStar5468
SPDX-License-Identifier: GPL-3.0-only
*/

#ifndef __NVIC_H__
#define __NVIC_H__

#include "stm32f10x_tim.h"

/// @brief NVIC对象
///该类提供了对stm32f10x NVIC的操作接口
///NVIC中断通道表请参见Nvic::Config::Channels
class Nvic
{
public:
    struct Config
    {
        /// @brief NVIC的全局优先级分组
        typedef enum PriorityGroup
        {
            Group0_4 = NVIC_PriorityGroup_0, // 0位抢占优先级，4位响应优先级
            Group1_3 = NVIC_PriorityGroup_1, // 1位抢占优先级，3位响应优先级
            Group2_2 = NVIC_PriorityGroup_2, // 2位抢占优先级，2位响应优先级
            Group3_1 = NVIC_PriorityGroup_3, // 3位抢占优先级，1位响应优先级
            Group4_0 = NVIC_PriorityGroup_4, // 4位抢占优先级，0位响应优先级
        } PriorityGroup;

        struct Channels
        {
            typedef enum Global
            {
                WWDG_IRQn = WWDG_IRQn, //窗口看门狗中断
                PVD_IRQn = PVD_IRQn, //电压检测中断
                TAMPER_IRQn = TAMPER_IRQn, //篡改事件中断
                RTC_IRQn = RTC_IRQn, //RTC全局中断
                FLASH_IRQn = FLASH_IRQn, //FLASH全局中断
                RCC_IRQn = RCC_IRQn, //RCC全局中断

#ifdef STM32F10X_MD
                RTCAlarm_IRQn = RTCAlarm_IRQn,                  //RTC报警中断
#endif
            } Global;

            typedef enum Exti
            {
                EXTI0_IRQn = EXTI0_IRQn, //外部中断0
                EXTI1_IRQn = EXTI1_IRQn, //外部中断1
                EXTI2_IRQn = EXTI2_IRQn, //外部中断2
                EXTI3_IRQn = EXTI3_IRQn, //外部中断3
                EXTI4_IRQn = EXTI4_IRQn, //外部中断4
#ifdef STM32F10X_MD
                EXTI9_5_IRQn = EXTI9_5_IRQn,                    //外部中断9到5
                EXTI15_10_IRQn = EXTI15_10_IRQn,                //外部中断15到10
#endif
            } Exti;

            typedef enum ADC
            {
#ifdef STM32F10X_MD
                ADC1_2_IRQn = ADC1_2_IRQn,                      //ADC1和ADC2全局中断
#endif
            } ADC;

            typedef enum USB
            {
#ifdef STM32F10X_MD
                USB_HP_CAN1_TX_IRQn = USB_HP_CAN1_TX_IRQn,      //USB高优先级或CAN1 TX中断
                USB_LP_CAN1_RX0_IRQn = USB_LP_CAN1_RX0_IRQn,    //USB低优先级或CAN1 RX0中断
                CAN1_RX1_IRQn = CAN1_RX1_IRQn,                  //CAN1 RX1中断
                CAN1_SCE_IRQn = CAN1_SCE_IRQn,                  //CAN1 SCE中断 
                USBWakeUp_IRQn = USBWakeUp_IRQn                 //USB唤醒中断
#endif 
            } USB;

            typedef enum TIM
            {
#ifdef STM32F10X_MD
                TIM1_BRK_IRQn = TIM1_BRK_IRQn,                  //TIM1死区中断  
                TIM1_UP_IRQn = TIM1_UP_IRQn,                    //TIM1更新中断
                TIM1_TRG_COM_IRQn = TIM1_TRG_COM_IRQn,          //TIM1触发和通信中断
                TIM1_CC_IRQn = TIM1_CC_IRQn,                    //TIM1捕获/比较中断
                TIM2_IRQn = TIM2_IRQn,                          //TIM2全局中断
                TIM3_IRQn = TIM3_IRQn,                          //TIM3全局中断
                TIM4_IRQn = TIM4_IRQn,                          //TIM4全局中断
#endif
            } TIM;

            typedef enum BUS
            {
#ifdef STM32F10X_MD
                I2C1_EV_IRQn = I2C1_EV_IRQn,                    //I2C1事件中断
                I2C1_ER_IRQn = I2C1_ER_IRQn,                    //I2C1错误中断
                I2C2_EV_IRQn = I2C2_EV_IRQn,                    //I2C2事件中断
                I2C2_ER_IRQn = I2C2_ER_IRQn,                    //I2C2错误中断
                SPI1_IRQn = SPI1_IRQn,                          //SPI1全局中断
                SPI2_IRQn = SPI2_IRQn,                          //SPI2全局中断
                USART1_IRQn = USART1_IRQn,                      //USART1全局中断
                USART2_IRQn = USART2_IRQn,                      //USART2全局中断
                USART3_IRQn = USART3_IRQn,                      //USART3全局中断
#endif
            } BUS;
        };

        typedef enum PowerMode
        {
            Normal = 0, //正常模式
            Sleep = NVIC_LP_SEVONPEND, //睡眠模式
            Stop = NVIC_LP_SLEEPDEEP,   //停止模式
            Standby = NVIC_LP_SLEEPONEXIT, //待机模式
        } PowerMode;
    };

    /// @brief NVIC中断配置结构体类型定义
    /// @param NVIC_IRQChannel - 需要配置的中断通道, 位于Nvic::Config::Channels中
    /// @param NVIC_IRQChannelPreemptionPriority - 抢占优先级配置
    /// @param NVIC_IRQChannelSubPriority - 响应优先级配置
    /// @param NVIC_IRQChannelCmd - 是否使能该通道 ENABLE或DISABLE
    typedef NVIC_InitTypeDef NvicConfig;

    /// @brief 设置NVIC的全局优先级分组方法(不建议运行时更改)
    /// @param priorityGroup 目标优先级分组方法
    void SetPriorityGrouping(Config::PriorityGroup priorityGroup);
    /// @brief 获取一个默认的NVIC初始化结构体
    /// @return 默认的NVIC初始化结构体
    NvicConfig GetDefaltConfig();
    /// @brief 以NVIC初始化结构体操作指定的中断通道
    /// @param config NVIC初始化结构体指针, 其中NVIC_IRQChannel成员指定了需要操作的中断通道
    void ChannelControl(NvicConfig* config);
    /// @brief 使用参数直接控制NVIC通道
    /// @param channel 要控制的通道 位于Nvic::Config::Channels中
    /// @param preemptionPriority 抢占优先级配置
    /// @param subPriority 执行优先级配置
    /// @param isEnabled 是否使能该通道
    void ChannelControl(uint8_t channel, uint8_t preemptionPriority, uint8_t subPriority, bool isEnabled);
    /// @brief 控制中断向量表
    /// @param NVIC_VectTab 要设置的中断向量表
    /// @param Offset 目标函数在Rom地址中的偏移(单位为4字节,即一个指针的大小)
    void SetVectorTable(uint32_t NVIC_VectTab, uint32_t Offset);
    /// @brief 设置系统低功耗模式(睡眠模式, 停止模式或待机模式)
    /// @param isLowPower 是否要启动低功耗模式
    void SetPowerMode(Config::PowerMode mode);
};

extern Nvic nvic;
















#endif
