#ifndef __NVIC_H__
#define __NVIC_H__

#include "stm32f10x_tim.h"

class Nvic
{
public:
    /// @brief NVIC的全局优先级分组
    typedef enum PriorityGroup
    {
        Group0 = NVIC_PriorityGroup_0, // 0位抢占优先级，4位响应优先级
        Group1 = NVIC_PriorityGroup_1, // 1位抢占优先级，3位响应优先级
        Group2 = NVIC_PriorityGroup_2, // 2位抢占优先级，2位响应优先级
        Group3 = NVIC_PriorityGroup_3, // 3位抢占优先级，1位响应优先级
        Group4 = NVIC_PriorityGroup_4, // 4位抢占优先级，0位响应优先级
    } PriorityGroup;

    typedef enum Channels
    {
        WWDG_IRQn = WWDG_IRQn, //窗口看门狗中断
        PVD_IRQn = PVD_IRQn, //电压检测中断
        TAMPER_IRQn = TAMPER_IRQn, //篡改事件中断
        RTC_IRQn = RTC_IRQn, //RTC全局中断
        FLASH_IRQn = FLASH_IRQn, //FLASH全局中断
        RCC_IRQn = RCC_IRQn, //RCC全局中断
        EXTI0_IRQn = EXTI0_IRQn, //外部中断0
        EXTI1_IRQn = EXTI1_IRQn, //外部中断1
        EXTI2_IRQn = EXTI2_IRQn, //外部中断2
        EXTI3_IRQn = EXTI3_IRQn, //外部中断3
        EXTI4_IRQn = EXTI4_IRQn, //外部中断4
        //Stm32_MD
        ADC1_2_IRQn = ADC1_2_IRQn,                      //ADC1和ADC2全局中断
        USB_HP_CAN1_TX_IRQn = USB_HP_CAN1_TX_IRQn,      //USB高优先级或CAN1 TX中断
        USB_LP_CAN1_RX0_IRQn = USB_LP_CAN1_RX0_IRQn,    //USB低优先级或CAN1 RX0中断
        CAN1_RX1_IRQn = CAN1_RX1_IRQn,                  //CAN1 RX1中断
        CAN1_SCE_IRQn = CAN1_SCE_IRQn,                  //CAN1 SCE中断     
        EXTI9_5_IRQn = EXTI9_5_IRQn,                    //外部中断9到5
        TIM1_BRK_IRQn = TIM1_BRK_IRQn,                  //TIM1死区中断  
        TIM1_UP_IRQn = TIM1_UP_IRQn,                    //TIM1更新中断
        TIM1_TRG_COM_IRQn = TIM1_TRG_COM_IRQn,          //TIM1触发和通信中断
        TIM1_CC_IRQn = TIM1_CC_IRQn,                    //TIM1捕获/比较中断
        TIM2_IRQn = TIM2_IRQn,                          //TIM2全局中断
        TIM3_IRQn = TIM3_IRQn,                          //TIM3全局中断
        TIM4_IRQn = TIM4_IRQn,                          //TIM4全局中断
        I2C1_EV_IRQn = I2C1_EV_IRQn,                    //I2C1事件中断
        I2C1_ER_IRQn = I2C1_ER_IRQn,                    //I2C1错误中断
        I2C2_EV_IRQn = I2C2_EV_IRQn,                    //I2C2事件中断
        I2C2_ER_IRQn = I2C2_ER_IRQn,                    //I2C2错误中断
        SPI1_IRQn = SPI1_IRQn,                          //SPI1全局中断
        SPI2_IRQn = SPI2_IRQn,                          //SPI2全局中断
        USART1_IRQn = USART1_IRQn,                      //USART1全局中断
        USART2_IRQn = USART2_IRQn,                      //USART2全局中断
        USART3_IRQn = USART3_IRQn,                      //USART3全局中断
        EXTI15_10_IRQn = EXTI15_10_IRQn,                //外部中断15到10
        RTCAlarm_IRQn = RTCAlarm_IRQn,                  //RTC报警中断
        USBWakeUp_IRQn = USBWakeUp_IRQn                 //USB唤醒中断
    } Channels;

    /// @brief 设置NVIC的全局优先级分组方法(不建议运行时更改)
    /// @param priorityGroup 目标优先级分组方法
    void SetPriorityGrouping(PriorityGroup priorityGroup);

};

class NvicChannel
{


};
















#endif