/*
BS-Stm32 Extended lib
Copyright (C) 2026 BlueStar5468
SPDX-License-Identifier: GPL-3.0-only
*/
#ifndef __TIM_H__
#define __TIM_H__

#include "stm32f10x_tim.h"
//stm32f10x的计时器
//该类提供了对stm32f10x计时器的操作接口
//计时器速查表:
//高级计时器(APB2):TIM1, TIM8
//通用计时器(APB1):TIM2, TIM3, TIM4, TIM5
//基本计时器(APB1):TIM6, TIM7
class Tim
{
public:
    /// @brief 计时器时基单元的配置结构体(请以指针形式使用)
    /// @param TIM_Prescaler - 计时器预分频器, 范围为0x0000到0xFFFF
    /// @param TIM_CounterMode - 计数器模式
    /// @param TIM_Period - 自动重装载值, 范围为0,达到此值即触发更新.
    /// @param TIM_ClockDivision - 时钟分频(用于滤波器采样时钟,基础频率为72MHz)
    /// @param TIM_RepetitionCounter - 重复计数器值, 范围为0x00到0xFF(只适用于高级计时器TIM1和TIM8)
    /// @note 计时器周期可由 ((重装值 + 1)/72MHz) * (预分频器值 + 1) 计算得出
    typedef TIM_TimeBaseInitTypeDef TimeBaseInitConfig;
    /// @brief 计时器的使能状态枚举
    /// @param ENABLE - 使能计时器
    /// @param DISABLE - 禁用计时器
    typedef FunctionalState State;

    struct Configs
    {
        /*
        中断模式表
        */
        typedef enum ITMode
        {
            Update = TIM_IT_Update, //更新中断
            CC1 = TIM_IT_CC1, //捕获/比较1中断
            CC2 = TIM_IT_CC2, //捕获/比较2中断
            CC3 = TIM_IT_CC3, //捕获/比较3中断
            CC4 = TIM_IT_CC4, //捕获/比较4中断
            COM = TIM_IT_COM, //通信中断
            Trigger = TIM_IT_Trigger, //触发中断
            Break = TIM_IT_Break, //死区中断
        } ITMode;

        /*
        时钟模式表
        */
        typedef enum TIMClockMode
        {
            InternalClock,      //内部时钟
            ExternalClockMode1, //外部时钟模式1
            ExternalClockMode2, //外部时钟模式2
            OtherTIM,           //其他定时器作为时钟输入(级联模式)
            TIxClock,           //TIx作为时钟输入
        } TIMClockMode;

        //内部计时器触发器表
        typedef enum InternalTrigger
        {
            IT0 = TIM_TS_ITR0, //内部触发器0
            IT1 = TIM_TS_ITR1, //内部触发器1
            IT2 = TIM_TS_ITR2, //内部触发器2
            IT3 = TIM_TS_ITR3, //内部触发器3
        } InternalTrigger;

        //TIx通道表
        typedef enum TIxChannel
        {
            TI1Edge = TIM_TIxExternalCLK1Source_TI1ED, //TI1通道边缘检测
            TI1 = TIM_TIxExternalCLK1Source_TI1,       //TI1通道
            TI2 = TIM_TIxExternalCLK1Source_TI2,       //TI2通道
        } TIxChannel;

        //外部引脚极性
        typedef enum Polarity
        {
            Rising = TIM_ICPolarity_Rising,   //上升沿触发
            Falling = TIM_ICPolarity_Falling, //下降沿触发
        } Polarity;


        //外部预分频器
        typedef enum ExternalPrescaler
        {
            OFF = TIM_ExtTRGPSC_OFF, //不分频
            Div2 = TIM_ExtTRGPSC_DIV2, //分频2
            Div4 = TIM_ExtTRGPSC_DIV4, //分频4
            Div8 = TIM_ExtTRGPSC_DIV8, //分频8
        } ExternalPrescaler;

        //计数器模式
        typedef enum CounterMode
        {
            Up = TIM_CounterMode_Up, //向上计数
            Down = TIM_CounterMode_Down, //向下计数
            CenterAligned1 = TIM_CounterMode_CenterAligned1, //中心对齐模式1
            CenterAligned2 = TIM_CounterMode_CenterAligned2, //中心对齐模式2
            CenterAligned3 = TIM_CounterMode_CenterAligned3, //中心对齐模式3
        } CounterMode;

        //PSC更新设置
        typedef enum PSCReloadMode
        {
            Wait = TIM_PSCReloadMode_Update,            //计数器完成当前周期后更新PSC
            Immediate = TIM_PSCReloadMode_Immediate,    //PSC立即更新,并重置计数器
        } PSCReloadMode;

        //计时器标志位
        typedef enum Flags
        {
            UpdateFlag = TIM_FLAG_Update,   //更新标志
            CC1Flag = TIM_FLAG_CC1,         //捕获/比较1标志
            CC2Flag = TIM_FLAG_CC2,         //捕获/比较2标志
            CC3Flag = TIM_FLAG_CC3,         //捕获/比较3标志
            CC4Flag = TIM_FLAG_CC4,         //捕获/比较4标志
            COMFlag = TIM_FLAG_COM,         //通信标志
            TriggerFlag = TIM_FLAG_Trigger, //触发标志
            BreakFlag = TIM_FLAG_Break,     //死区标志
        } Flags;

        //滤波采样器分频
        typedef enum ClockDivision
        {
            ClockDiv1 = TIM_CKD_DIV1, //不分频
            ClockDiv2 = TIM_CKD_DIV2, //分频2
            ClockDiv4 = TIM_CKD_DIV4, //分频4
        } ClockDivision;

    };

    //对象硬件地址
    TIM_TypeDef* self;

    /// @brief 计时器对象的构造函数
    Tim(TIM_TypeDef* timDef) : self(timDef) {}

    void DeInit();
    void TimeBaseInit(TimeBaseInitConfig* config);
    TimeBaseInitConfig GetDefaultTimeBaseInitConfig();
    void Switch(State state);
    void InterruptSwitch(State state, Configs::ITMode it = Configs::ITMode::Update);
    void SetClockMode(Configs::TIMClockMode mode, Configs::InternalTrigger trigger = Configs::InternalTrigger::IT0, Configs::ExternalPrescaler extPrescaler = Configs::ExternalPrescaler::OFF, Configs::TIxChannel tixChannel = Configs::TIxChannel::TI1, Configs::Polarity polarity = Configs::Polarity::Rising, uint16_t tixFilter = 0);
    void SetPrescaler(uint16_t prescaler, Configs::PSCReloadMode reloadMode = Configs::PSCReloadMode::Wait);
    void SetCounterMode(Configs::CounterMode mode);
    void ARReloadConfig(State state);
    void SetCounter(uint16_t counter);
    void SetAutoReload(uint16_t autoReload);
    uint16_t GetCounter();
    uint16_t GetPrescaler();
    uint16_t GetAutoReload();

    //标志位相关函数

    FlagStatus GetFlagStatus(Configs::Flags flag);
    void ClearFlag(Configs::Flags flag);
    ITStatus GetITStatus(Configs::ITMode it);
    void ClearITPendingBit(Configs::ITMode it);

    //时钟控制

    void PeripheralClockControl(State state);

    //傻瓜配置函数

    /// @brief 我是懒狗(bushi 这个结构体里的函数可以让你用最简单的方式配置计时器!
    struct Foolish
    {
        void Start(Tim tim, uint16_t seconds);
    };
};

/*
定时器对象表
*/

extern Tim TIM_1; //APB2 高级定时器
extern Tim TIM_2; //APB1 通用定时器
extern Tim TIM_3; //APB1 通用定时器
extern Tim TIM_4; //APB1 通用定时器
extern Tim TIM_5; //APB1 通用定时器
extern Tim TIM_6; //APB1 基本定时器
extern Tim TIM_7; //APB1 基本定时器
extern Tim TIM_8; //APB2 高级定时器

#endif
